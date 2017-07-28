/* -*- mode: c; tab-width: 4 -*- */
/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2015-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * This patch is loaded to implement the FLASH_ERASE command.  The
 * flags sent with the command indicate what section is to be erased.
 * The sections include mission firmware (first 126KiB on Denali,
 *  first 128KiB on others), SDB (2 KiB only on Denali), iota (last
 * 64 KiB of the main 192 KiB), or the FIB (strongly unrecommended).
 * The valid combinations for Denali are:
 *     Flags  |  FIB  |  Iota  |  Mission  |  SDB  |
 *       1    |       |        |     X     |       |
 *       2    |       |    X   |           |       |
 *       3    |       |    X   |     X     |       |
 *       8    |       |        |           |   X   |
 *       9    |       |        |     X     |   X   |
 *      10    |       |    X   |           |   X   |
 *      11    |       |    X   |     X     |   X   |
 *      15    |   X   |    X   |     X     |   X   |
 *
 * The valid combinations for others are:
 *     Flags  |  FIB  |  Iota  |  Mission  |
 *       1    |       |        |     X     |
 *       2    |       |    X   |           |
 *       3    |       |    X   |     X     |
 *       7    |   X   |    X   |     X     |
 */


#include "basetypes.h"
#include "bootldr.h"
#include "fwcmd.h"
#include "hwdef.h"
#include "nvm_prog.h"
#include "patchopt.h"
#include "reg.h"
#include "hwreg.h"

#include "cmdmgr.h"

#include "flashtest.h"

#define PATCH_ASSERT(x)

typedef enum __attribute__ ((__packed__)) {
    PATCH_SECSTATE_OPEN = 0,
    PATCH_SECSTATE_ENG = 1,
    PATCH_SECSTATE_PROD = 2,
    PATCH_SECSTATE_LOCKED = 3,
#if defined(PATCHOPT_SEC_FUSE)
    PATCH_SECSTATE_PROG = 4,
    PATCH_SECSTATE_FA   = 5
#endif      /* defined(PATCHOPT_SEC_FUSE) */
} patch_secstate_t;

typedef struct _patch_state_s {
    patch_secstate_t             secstate;
    uint32_t                     pagebuf[HWDEF_MEMMAP_FLASH_PAGE_SIZE
                                         / sizeof(uint32_t)];
} patch_state_t;

/*
 * Read a 32-bit value from a FIB location.
 */
#define PATCH_FIB_READ32(ptr)                                               \
    (*((const uint32_t *) (ptr)))
#define PATCH_FIB_ISVALID(x)  (((x) >> 30) == 1 || ((x) >> 30) == 2)
/* Strip off the "validity" bits at the top */
#define PATCH_FIB_VAL(x)       ((x) & 0x3fffffff)

/*
 * An extents (a region in the flash).
 */
typedef struct patch_extents_s {
    uint32_t         startpage;
    uint32_t         npages;
} patch_extents_t;

/*
 * nvm_prog.h contains NVM_MAIN_ALLOCATED_PAGES, NVM_INFO_ALLOCATED_PAGES,
 *  and NVM_IOTA_ALLOCATED_PAGES.
 * Sadly, nvm_prog.h is not authoritative for these values: bXXXXhwdef.h is.
 * The problem arises on Denali (b1210), where there's also a SDB page.
 * So we have the following definitions.
 */
#define PATCH_MISSION_STARTPAGE                                             \
    ((HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE - HWDEF_MEMMAP_SROM_FLASH_BASE)  \
     / HWDEF_MEMMAP_FLASH_PAGE_SIZE)
#define PATCH_MISSION_NPAGES                                                \
    ROUNDUPDIV(HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE,                        \
               HWDEF_MEMMAP_FLASH_PAGE_SIZE)
#define PATCH_IOTA_STARTPAGE                                                \

#define PATCH_IOTA_NPAGES                                                   \
    ROUNDUPDIV(HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE,                           \
               HWDEF_MEMMAP_FLASH_PAGE_SIZE)

/*
 * This next assertion makes sure that the FLASH_ERASE command
 *  header is a multiple of sizeof(uint32_t) bytes long.  This makes
 *  it possible to directly address the DWORDs that follow it.
 */
BOOTLDR_ASSERT_CT(patch_start_cmd_flash_erase_is_x4,
                  (sizeof(fwcmd_cmd_flash_erase_t) % sizeof(uint32_t)) == 0);

/* -------------------------------------------------------------------------- */
/* Static prototypes */

static int patch_cmd_handler_flash_test(void *ctxp, const uint8_t *cmdbufp,
                                         unsigned int cmdlen, uint8_t *replyp);
static patch_secstate_t patch_get_secstate(void);
static bool_t patch_is_erase_allowed(patch_secstate_t secstate);
#if defined(PATCHOPT_SEC_FUSE)
static bool_t patch_is_hayes_a0(void);
#endif

static int mission_fw_erase(void);
static int iota_area_erase(void);
static int main_flash_erase(void);
static int all_flash_erase(void);
#if defined(PATCHOPT_HAS_SDB)
static int sdb_erase(void);
#endif
static int progword32(uint32_t addr, uint32_t val);
static bool_t patch_pageisblank(unsigned int pagenum);
static int eraseword32(uint32_t addr);
static patch_extents_t patch_iota_extentsfind(void);
static patch_extents_t patch_mission_extentsfind(void);
static int patch_extents_erase(patch_extents_t extents);
static int patch_page_erase(uint32_t pagenum);


/* -------------------------------------------------------------------------- */
/* Static uninitialized data  */

static patch_state_t patch_state __attribute__ ((__section__ (".bss")));


/* -------------------------------------------------------------------------- */
/* Static initialized data  */

static cmdmgr_cmdentry_t patch_cmdtable[] = {
    { VCSFW_CMD_FLASH_TEST, &patch_cmd_handler_flash_test }
};

/* -------------------------------------------------------------------------- */
/* Global functions */

int
patch_start(void *ctxp, uint8_t *replyp)
{
    /*
     * Make sure we're being run on the correct hardware.
     *  This patch cares about this because it needs to determine
     *  the security state.  Hayes implements the security
     *  state using the FUSE registers, Denali implements
     *  the security state using FIB words.
     */
    if (((const bootldr_ver_roinfo_t *)
         BOOTLDR_MEMMAP_VER_ROINFO_BASE)->product != PATCHOPT_PBL_PRODUCT) {
        return VCSFW_STATUS_ERR_BOOTLDR_PATCH_PRODUCT;
    }

    patch_state.secstate = patch_get_secstate();

    return cmdmgr(ctxp, &(patch_cmdtable[0]), NELEM(patch_cmdtable));
}

/* -------------------------------------------------------------------------- */
/* Static functions */

/*
 * Handler for VCSFW_CMD_FLASH_ERASE.
 */
static int
patch_cmd_handler_flash_test(void *ctxp, const uint8_t *cmdbufp,
                              unsigned int cmdlen, uint8_t *replyp)
{
    int                              status;
    uint32_t                         flags;
    const uint32_t                  *wordp;
	long                             i;
	
	uint16_t *rp = ( uint16_t * ) replyp + 2;

	*(((uint16_t *) replyp) + 1 ) = VCSFW_STATUS_OKAY;
	
    status = VCSFW_STATUS_OKAY;

    /* make sure we can access the NVM (flash) controller) */
    if (nvm_unlock() != NVM_SUCCESS) 
	{
        status = VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE;
    }

#if 1	// flash erase
	if( status == NVM_SUCCESS )
	{
		if( main_flash_erase( ) != 0 ) 
		{
	        status = VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE;
	    }
	}

	// write 0x55
	if( status == NVM_SUCCESS )
	{
		// Main flash(96K) = MissionFW(64K) + IOTA(32K)
		for( i = 0; i < 96 * 1024; i += 4 )
		{
			status = progword32( HWDEF_MEMMAP_SROM_FLASH_BASE + i , 0x55555555 );		
			if( status != NVM_SUCCESS )
			{
				status = FLASH_WRITE_55_ERROR;
				break;
			}
		}
	}

	// check if success
	if( status == NVM_SUCCESS )
	{
		for( i = 0; i < 96 * 1024 / 4; i++ )
		{
			if( *(( unsigned long * )HWDEF_MEMMAP_SROM_FLASH_BASE + i ) != 0x55555555 )
			{
				status = FLASH_CHECK_55_ERROR;
				i *= 4;
				break;
			}		
		}
	}

	// erase for another write
	if( status == NVM_SUCCESS )
	{
		if( main_flash_erase( ) != 0 ) 
		{
			status = VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE;
		}
	}

	// write 0xAA
	if( status == NVM_SUCCESS )
	{
		// Main flash(96K) = MissionFW(64K) + IOTA(32K)
		for( i = 0; i < 96 * 1024; i += 4 )
		{
			status = progword32( HWDEF_MEMMAP_SROM_FLASH_BASE + i , 0x0AAAAAAAA );		
			if( status != NVM_SUCCESS )
			{
				status = FLASH_WRITE_AA_ERROR;
				break;
			}
		}
	}

	// check if success
	if( status == NVM_SUCCESS )
	{
		for( i = 0; i < 96 * 1024 / 4; i++ )
		{
			if( *(( unsigned long * )HWDEF_MEMMAP_SROM_FLASH_BASE + i ) != 0x0AAAAAAAA )
			{
				status = FLASH_CHECK_AA_ERROR;
				i *= 4;
				break;
			}		
		}
	}

	// recover to erase state
	if( status == NVM_SUCCESS )
	{
		if( main_flash_erase( ) != 0 ) 
		{
	        status = VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE;
	    }
	}

	// calculate system address when error occur
	if( status == NVM_SUCCESS || status == VCSFW_STATUS_ERR_FLASH_ERASE_FAILURE )
	{
		i = 0;
	}
	else
	{
		i += HWDEF_MEMMAP_SROM_FLASH_BASE;
	}

	// 1st. 2 bytes error code
	*rp++ = status;

	// 2nd. 4 bytes address of error occured
	*rp++ = i;
	*rp++ = i >> 16;
#if 1
cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
			 sizeof(uint16_t) + 6 );

#else// only for debug
	for( i = 0; i < 50 * 1024 / 2; i++ )
	{
		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_SROM_FLASH_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE + i );
//		*rp++ = *(( unsigned short * )HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE + i );

	}
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
			 sizeof(uint16_t) + 50 * 1024 );	
#endif

#endif

// only for debug
#if 0	// FIB, IOTA and Mission firmware flash read,

#if 0 //read FIB, 12KB start from 0x50820000
	BOOTLDR_MEMCPY( rp, HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE, 
						HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE );
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) + HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE  );
#else 
#if 0 // read IOTA, 32KB start from 0x50810000
	BOOTLDR_MEMCPY( rp, HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE, 
						HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE );
	cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
                 sizeof(uint16_t) +  HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE );
#else// read Mission firmware, 64KB start from 0x50800000
BOOTLDR_MEMCPY( rp, HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE, 
					HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE - 32000 );
cmdmgr_reply( ctxp, (uint8_t *)(((uint16_t *) replyp)+1),
			 sizeof(uint16_t) +  HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE - 32000 );
#endif
#endif
		
#endif

    nvm_lock();

    return -1;
}


// The following routines all assume the NVM has been unlocked by higher-level code
static int
mission_fw_erase(void)
{
    int rc;

    rc = patch_extents_erase(patch_mission_extentsfind());

    /*
     * Erase the MISSIONVALID word.  This one is tricky, as
     *  the MISSIONVALID word appears in the high end of the
     *  IOTA data.  So eraseword preserves the rest of the page.
     */
    if (rc == 0) {
        eraseword32(HWDEF_MEMMAP_MISSIONVALID);
    }


    return rc;
}

static int
iota_area_erase(void)
{
    int rc;
    unsigned int        mvalid_word;

    rc = 0;
    mvalid_word = *((unsigned int *)(HWDEF_MEMMAP_MISSIONVALID));
    rc = patch_extents_erase(patch_iota_extentsfind());

    /* if the mission_valid word was written with the magic number, restore it */
    if (mvalid_word == HWDEF_MEMMAP_MISSIONVALID_PATTERN) {
        progword32(HWDEF_MEMMAP_MISSIONVALID, HWDEF_MEMMAP_MISSIONVALID_PATTERN);
    }
    return rc;
}

static int
main_flash_erase(void)
{
    int rc;

    rc = 0;

    if (nvm_mass_erase(0) != NVM_SUCCESS) {
        rc = -1;
    }
    return rc;
}

static int
all_flash_erase(void)
{
    int rc;

    rc = 0;
    if (nvm_mass_erase(1) != NVM_SUCCESS) {
        rc = -1;
    }
    return rc;
}

#if defined(PATCHOPT_HAS_SDB)
static int
sdb_erase(void)
{
    int     p;

    p = ((HWDEF_MEMMAP_SROM_FLASH_SDB_BASE - HWDEF_MEMMAP_SROM_FLASH_BASE)
         / HWDEF_MEMMAP_FLASH_PAGE_SIZE);

    if (!patch_pageisblank(p) && nvm_page_erase(p) != NVM_SUCCESS) {
        return -1;
    }
    else {
        return 0;
    }
}
#endif      /* defined(PATCHOPT_HAS_SDB) */

static int
progword32(uint32_t addr, uint32_t val)
{
    bool_t                  isfib;
    nvm_flash_word_t        fw;
    int                     retval;

    isfib = BOUNDED(addr, HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE,
                    HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE);

    /* Read what's there already */
    addr -= HWDEF_MEMMAP_SROM_FLASH_BASE;
    if (isfib) {
        retval = nvm_info_word_read(addr / sizeof(nvm_flash_word_t), &fw);
    }
    else {
        retval = nvm_word_read(addr / sizeof(nvm_flash_word_t), &fw);
    }

    if (retval != NVM_SUCCESS) {
        return retval;
    }

    if ((addr & 0x4) != 0) {        /* odd word address */
        fw.msw = val;
    }
    else {
        fw.lsw = val;
    }

    if (isfib) {
        retval = nvm_info_word_write(addr / sizeof(nvm_flash_word_t), &fw);
    }
    else {
        retval = nvm_word_write(addr / sizeof(nvm_flash_word_t), &fw);
    }

    return retval;
}

static patch_secstate_t
patch_get_secstate(void)
{
    patch_secstate_t secstate = PATCH_SECSTATE_LOCKED;
    uint32_t sec_word;

#if defined(PATCHOPT_SEC_FUSE)
    /*
     * The fuse model of security.
     */

    sec_word = REG_READ32(FUSE_FUSE_STATUS);
    sec_word &= FUSE_FUSE_STATUS_FUSE_SEC_WORD;
    sec_word >>= FUSE_FUSE_STATUS_FUSE_SEC_WORD_B;

    switch (sec_word) {
    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_PROG:
        secstate = PATCH_SECSTATE_PROG;
        break;

    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_OPEN:
        secstate = PATCH_SECSTATE_OPEN;
        break;

    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_ENG:
        secstate = PATCH_SECSTATE_ENG;
        break;

    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_PROD:
        secstate = PATCH_SECSTATE_PROD;
        break;

    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_FA:
        secstate = PATCH_SECSTATE_FA;
        break;

    case FUSE_FUSE_STATUS_FUSE_SEC_WORD_SECSTATE_LOCKED:
    default:
        secstate = PATCH_SECSTATE_LOCKED;
        break;
    }

#else
    /*
     * We default to a flash model of security.
     */
    uint32_t soft_sec_word;

    sec_word = PATCH_FIB_READ32(HWDEF_FIB_SECW);
    switch(sec_word) {

    case HWDEF_FIB_SECW_MAGIC_OPEN:
        secstate = PATCH_SECSTATE_OPEN;
        break;

    case HWDEF_FIB_SECW_MAGIC_SECURE:
        //Well, what KIND of a secure part are we?
        soft_sec_word = PATCH_FIB_READ32(HWDEF_FIB_SOFTSECW);
        if (soft_sec_word == 0xffffffff) {
            secstate = PATCH_SECSTATE_ENG;
        }
        else {
            secstate = PATCH_SECSTATE_PROD;
        }
        break;

    default:
        secstate = PATCH_SECSTATE_LOCKED;
        break;
    }
#endif

    return(secstate);
}

static bool_t
patch_is_erase_allowed(patch_secstate_t secstate)
{
#if defined(PATCHOPT_SEC_FUSE)
    return (secstate == PATCH_SECSTATE_FA
            || secstate == PATCH_SECSTATE_OPEN
            || (secstate == PATCH_SECSTATE_PROG && patch_is_hayes_a0()));
#else
    return secstate == PATCH_SECSTATE_OPEN;
#endif
}

#if defined(PATCHOPT_SEC_FUSE)
static bool_t
patch_is_hayes_a0(void)
{
    return REG_READ32(SCM_MASKID_LO) == 0 && REG_READ32(SCM_MASKID_HI) == 0;
}
#endif      /* defined(PATCHOPT_SEC_FUSE) */

/*
 * Given a page number in the main flash, figure out if it's blank.
 *  If so, then return TRUE.  Otherwise, return FALSE.
 */

static bool_t
patch_pageisblank(unsigned int pagenum)
{
    const uint32_t      *datap;
    unsigned int         nwords;

    datap = (const uint32_t *) (HWDEF_MEMMAP_SROM_FLASH_BASE
                                + (pagenum * HWDEF_MEMMAP_FLASH_PAGE_SIZE));
    /* Invalidate the cache in the page of interest */
    BOOTLDR_CACHE_DFLUSH((void *) datap, HWDEF_MEMMAP_FLASH_PAGE_SIZE,
                         BOOTLDR_CACHE_FLUSH_INV);

    nwords = HWDEF_MEMMAP_FLASH_PAGE_SIZE / sizeof(uint32_t);

    while (nwords != 0) {
        if (*datap++ != 0xffffffff) {
            return FALSE;
        }

        nwords--;
    }

    return TRUE;
}

/*
 * Erase a single word by page erasing the entire page and rewriting
 *  everything but the single DWORD.
 */
static int
eraseword32(uint32_t addr)
{
    nvm_flash_word_t        *fwp;
    unsigned int             offset, pagenum, pageoff, nwords;
    const uint32_t          *pagestartp;
    bool_t                   isfib;

    PATCH_ASSERT((addr % sizeof(uint32_t)) == 0);
    PATCH_ASSERT(BOUNDED(addr, HWDEF_MEMMAP_SROM_FLASH_BASE,
                         HWDEF_MEMMAP_SROM_FLASH_SIZE)
                 || BOUNDED(addr, HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE,
                            HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE));

    isfib = BOUNDED(addr, HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE,
                    HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_SIZE);

    /*
     * First, check to see if it's already blank.
     */
    BOOTLDR_CACHE_DFLUSH((void *) addr, sizeof(uint32_t),
                         BOOTLDR_CACHE_FLUSH_INV);
    if (*((const uint32_t *) addr) == 0xffffffff) {
        return 0;
    }
    offset = addr - HWDEF_MEMMAP_SROM_FLASH_BASE;
    pagenum = (offset / HWDEF_MEMMAP_FLASH_PAGE_SIZE);
    pageoff = (offset % HWDEF_MEMMAP_FLASH_PAGE_SIZE);
    pagestartp = (const uint32_t *)
        (HWDEF_MEMMAP_SROM_FLASH_BASE
         + (pagenum * HWDEF_MEMMAP_FLASH_PAGE_SIZE));

    /*
     * Fetch a fresh copy of the whole thing into our
     *  page backup area.
     */
    BOOTLDR_CACHE_DFLUSH((void *) pagestartp,
                         HWDEF_MEMMAP_FLASH_PAGE_SIZE,
                         BOOTLDR_CACHE_FLUSH_INV);
    BOOTLDR_MEMCPY(&(patch_state.pagebuf[0]), pagestartp,
                   HWDEF_MEMMAP_FLASH_PAGE_SIZE);

    /*
     * Unprogram our address of interest.
     */
    patch_state.pagebuf[pageoff / sizeof(uint32_t)] = 0xffffffff;

    /*
     * Now erase the page.  Note that we don't care if it fails
     *  because we're going to have to try to reprogram the page
     *  either way.  It's too risky that we might lose data otherwise.
     */
    (void) patch_page_erase(pagenum);

    /*
     * And rewrite the page with the original content, minus
     *  the one word we ffffffffed out.
     */
    fwp = (nvm_flash_word_t *) &(patch_state.pagebuf[0]);
    nwords = HWDEF_MEMMAP_FLASH_PAGE_SIZE / sizeof(*fwp);
    addr = ((uint32_t) pagestartp) - HWDEF_MEMMAP_SROM_FLASH_BASE;
    addr /= sizeof(nvm_flash_word_t);

    while (nwords != 0) {
        /*
         * Again, we do this without looking at success/fail because
         *  we need to try to get the data back into the page.
         */
        if (isfib) {
            (void) nvm_info_word_write(addr, fwp);
        }
        else {
            (void) nvm_word_write(addr, fwp);
        }
        
        fwp++;
        addr++;
        nwords--;
    }

    /*
     * And invalidate the cache again so we get to see our newly
     *  updated page.
     */

    BOOTLDR_CACHE_DFLUSH((void *) pagestartp,
                         HWDEF_MEMMAP_FLASH_PAGE_SIZE,
                         BOOTLDR_CACHE_FLUSH_INV);

    return NVM_SUCCESS;
}

/*
 * Find the beginnings and ends of the iota chain based
 *  on the partioning defined in the FIB.
 */
static patch_extents_t
patch_iota_extentsfind(void)
{
#if defined(PATCHOPT_FLASH_SOFTPART)
    uint32_t         iotaoff, iotasize;
#endif
    patch_extents_t  extents;

    extents.startpage = ((HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE
                          - HWDEF_MEMMAP_SROM_FLASH_BASE)
                         / HWDEF_MEMMAP_FLASH_PAGE_SIZE);
    extents.npages = ROUNDUPDIV(HWDEF_MEMMAP_SROM_FLASH_IOTA_SIZE,
                                HWDEF_MEMMAP_FLASH_PAGE_SIZE);

#if defined(PATCHOPT_FLASH_SOFTPART)
    /*
     * Read the FIB locations.  If either is not programmed,
     *  or contains an invalid value, BOTH are ignored.
     */

    iotaoff = PATCH_FIB_READ32(HWDEF_FIB_IOTAOFF);
    iotasize = PATCH_FIB_READ32(HWDEF_FIB_IOTASIZE);
    if (PATCH_FIB_ISVALID(iotaoff) && PATCH_FIB_ISVALID(iotasize)) {
        /* Strip off the "validity" bits */
        iotaoff = PATCH_FIB_VAL(iotaoff);
        iotasize = PATCH_FIB_VAL(iotasize);

        /* Are the offset and sizes both multiples of 8 and properly bounded? */
        if ((iotaoff % sizeof(nvm_flash_word_t)) == 0
            && (iotasize % sizeof(nvm_flash_word_t)) == 0
            && iotaoff < HWDEF_MEMMAP_FLASH_SIZE
            && (iotaoff + iotasize) <= HWDEF_MEMMAP_FLASH_SIZE) {

            extents.startpage = iotaoff / HWDEF_MEMMAP_FLASH_PAGE_SIZE;
            extents.npages = ROUNDUPDIV(iotasize, HWDEF_MEMMAP_FLASH_PAGE_SIZE);
        }
    }
#endif       /* defined(PATCHOPT_FLASH_SOFTPART) */

    /*
     * Now pass the values up to the caller.
     */
    return extents;
}

/*
 * Find the beginnings and ends of the mission firmware based
 *  on the partioning defined in the FIB.
 */
static patch_extents_t
patch_mission_extentsfind(void)
{
#if defined(PATCHOPT_FLASH_SOFTPART)
    uint32_t         missionsize;
#endif
    patch_extents_t  extents;

    extents.startpage = ((HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE
                          - HWDEF_MEMMAP_SROM_FLASH_BASE)
                         / HWDEF_MEMMAP_FLASH_PAGE_SIZE);
    extents.npages = ROUNDUPDIV(HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE,
                                HWDEF_MEMMAP_FLASH_PAGE_SIZE);

#if defined(PATCHOPT_FLASH_SOFTPART)
    /*
     * Read the FIB location containing the mission length.
     */

    missionsize = PATCH_FIB_READ32(HWDEF_FIB_MISSIONSIZE);
    if (PATCH_FIB_ISVALID(missionsize)){
        /* Strip off the "validity" bits */
        missionsize = PATCH_FIB_VAL(missionsize);

        /* Is it a reasonable size? */
        if ((missionsize % sizeof(nvm_flash_word_t)) == 0
            && missionsize <= HWDEF_MEMMAP_FLASH_SIZE) {

            extents.npages = ROUNDUPDIV(missionsize,
                                        HWDEF_MEMMAP_FLASH_PAGE_SIZE);
        }
    }
#endif       /* defined(PATCHOPT_FLASH_SOFTPART) */

    /*
     * Now pass the values up to the caller.
     */
    return extents;
}

/*
 * Erase the given extent.
 */
static int
patch_extents_erase(patch_extents_t extents)
{
    uint32_t         pagenum;

    for (pagenum = extents.startpage;
         pagenum < (extents.startpage + extents.npages); pagenum++) {
        if (!patch_pageisblank(pagenum)
            && nvm_page_erase(pagenum) != NVM_SUCCESS) {
            return -1;
        }
    }

    return 0;
}

/*
 * Erase the given page.  Period.
 */
static int
patch_page_erase(uint32_t pagenum)
{
    unsigned int    loopcount;
    uint32_t        regval, opstatus, flashstatus;

    regval = REG_READ32(NVM_FLASH_SEC_STATUS);

    if ((regval & NVM_FLASH_SEC_STATUS_PROGRAM_INHIBIT) != 0) {
        return NVM_OPERATION_IS_INHIBITED;
    }
    REG_WRITE32(NVM_FLASH_OP_ADDR, (pagenum
                                    * (HWDEF_MEMMAP_FLASH_PAGE_SIZE
                                       / sizeof(nvm_flash_word_t))));

    /*
     * Wait for the controller.
     */
    loopcount = 0;      /* allow the loop to run for a full 2^32 iterations */
    do {
        opstatus = REG_READ32(NVM_FLASH_OP_STATUS);
        flashstatus = REG_READ32(NVM_FLASH_STATUS);
        loopcount--;
    } while (((opstatus & (NVM_FLASH_OP_STATUS_FLASH_BUSY
                           | NVM_FLASH_OP_STATUS_SMART_BUSY)) != 0
              || (flashstatus & (NVM_FLASH_STATUS_CORE_BUSY
                                 | NVM_FLASH_STATUS_SMART_BUSY)) != 0)
             && loopcount != 0);

    if (loopcount == 0) {
        return NVM_CONTROLLER_TIMEOUT;
    }

    /* Actually start the erasure. */
    REG_WRITE32(NVM_FLASH_OP_REQUEST, NVM_FLASH_OP_REQUEST_ERASE_PAGE_START);
        
    /*
     * Wait for the controller.
     */
    loopcount = 0;      /* allow the loop to run for a full 2^32 iterations */
    do {
        opstatus = REG_READ32(NVM_FLASH_OP_STATUS);
        flashstatus = REG_READ32(NVM_FLASH_STATUS);
        loopcount--;
    } while (((opstatus & (NVM_FLASH_OP_STATUS_FLASH_BUSY
                           | NVM_FLASH_OP_STATUS_SMART_BUSY)) != 0
              || (flashstatus & (NVM_FLASH_STATUS_CORE_BUSY
                                 | NVM_FLASH_STATUS_SMART_BUSY)) != 0)
             && loopcount != 0);

    if (loopcount == 0) {
        return NVM_CONTROLLER_TIMEOUT;
    }

    /* Was there an error? */
    if ((opstatus & NVM_FLASH_OP_STATUS_SMART_LOOP_ERROR) != 0) {
        /* Clear the error flag first. */
        REG_WRITE32(NVM_FLASH_OP_REQUEST,
                    NVM_FLASH_OP_REQUEST_CLEAR_ERROR_FLAG);
        return NVM_ERROR;
    }

    return NVM_SUCCESS;
}

