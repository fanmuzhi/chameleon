/*
 * This unpublished code is Copyright (c) 2006-2015, Synaptics Inc.
 * All rights reserved.
 */


#include "fpscan_parse.h"

#ifndef BUILD_FOR_HAYES
#  define BUILD_FOR_HAYES  0
#define BUILD_FOR_SHASTA
#endif /* BUILD_FOR_HAYES */


/* Most of this code is copied from Denali FW and should be cleaned-up */

#ifdef BUILD_FOR_SHASTA
#  include "b1216reg.h"
#  include "b1216hwdef.h"
#else
#if BUILD_FOR_HAYES
#  include "b1212reg.h"
#  include "b1212hwdef.h"
#else /* BUILD_FOR_HAYES */
#  include "b1210reg.h"
#  include "b1210hwdef.h" 
#endif
#endif


#define FPSCAN_BYTES_PER_PIXEL          (2)
#define FPSCAN_FW_BL_NUM_BYTES          (HWDEF_AFE_TOTAL_PIXELS * FPSCAN_BYTES_PER_PIXEL)
#define FPSCAN_LNA_BL_NUM_BYTES         (HWDEF_AFE_TOTAL_PIXELS * FPSCAN_BYTES_PER_PIXEL)

#define REG_READ32(ptr)                                                     \
    (*((volatile uint32_t *) (ptr))) // Doesn't need checking

#define REG_VARWRITE32(ptr, val)                                            \
    ((*((volatile uint32_t *) (ptr))) = (val))


typedef uint8_t vcsUint8_t;
/* We purposefully rewrite these types to non-scalar types to avoid
 *  accidental in-code use of the VCS_PACKED structures.
 */
typedef uint8_t vcsUint16_t[2];
typedef uint8_t vcsUint32_t[4];

typedef int8_t vcsInt8_t;
typedef uint8_t vcsInt16_t[2];
typedef uint8_t vcsInt32_t[4];

typedef uint8_t vcsBool_t[4];

//#ifndef VCS_PACKED
#   define VCS_PACKED ATTR_PACKED_STRUCT
//#endif   /* VCS_PACKED */
//#include "vcsfw_v4.h"

/* Error codes */
#define VCSFW_STATUS_OKAY                       0
#define VCSFW_STATUS_ERR_FLAG                   0x0400
#define VCSFW_STATUS_ERR_FRAME_TAG_TOOSHORT                                 \
                                                (VCSFW_STATUS_ERR_FLAG | 441)
#define VCSFW_STATUS_ERR_FRAME_TAG_TOOLONG                                  \
                                                (VCSFW_STATUS_ERR_FLAG | 442)
#define VCSFW_STATUS_ERR_FRAME_TAG_REG32BLK_TOOSHORT                        \
                                                (VCSFW_STATUS_ERR_FLAG | 443)
#define VCSFW_STATUS_ERR_FRAME_TAG_REG32BLK_TOOLONG                         \
                                                (VCSFW_STATUS_ERR_FLAG | 444)
#define VCSFW_STATUS_ERR_FRAME_TAG_DIMS_TOOSHORT                            \
                                                (VCSFW_STATUS_ERR_FLAG | 445)
#define VCSFW_STATUS_ERR_FRAME_TAG_DIMS_TOOLONG                             \
                                                (VCSFW_STATUS_ERR_FLAG | 446)
#define VCSFW_STATUS_ERR_FRAME_TAG_REG16BLK_TOOSHORT                        \
                                                (VCSFW_STATUS_ERR_FLAG | 447)
#define VCSFW_STATUS_ERR_FRAME_TAG_REG16BLK_TOOLONG                         \
                                                (VCSFW_STATUS_ERR_FLAG | 448)
#define VCSFW_STATUS_ERR_FRAME_READ_NO_MEMORY                               \
                                                (VCSFW_STATUS_ERR_FLAG | 449)
#define VCSFW_STATUS_ERR_DRIVEMATRIXTOOSHORT                                \
                                                (VCSFW_STATUS_ERR_FLAG | 450)
#define VCSFW_STATUS_ERR_DRIVEMATIXTOOLONG                                  \
                                                (VCSFW_STATUS_ERR_FLAG | 451)
#define VCSFW_STATUS_ERR_DECODEMATRIXTOOSHORT                               \
                                                (VCSFW_STATUS_ERR_FLAG | 452)
#define VCSFW_STATUS_ERR_DECODEMATRIXTOOLONG                                \
                                                (VCSFW_STATUS_ERR_FLAG | 453)
#define VCSFW_STATUS_ERR_LNABASELINETOOSHORT                                \
                                                (VCSFW_STATUS_ERR_FLAG | 454)
#define VCSFW_STATUS_ERR_LNABASELINETOOLONG                                 \
                                                (VCSFW_STATUS_ERR_FLAG | 455)
#define VCSFW_STATUS_ERR_ADCBASELINETOOSHORT                                \
                                                (VCSFW_STATUS_ERR_FLAG | 456)
#define VCSFW_STATUS_ERR_ADCBASELINETOOLONG                                 \
                                                (VCSFW_STATUS_ERR_FLAG | 457)
#define VCSFW_STATUS_ERR_INVALID_FRAME_TAG                                  \
                                                (VCSFW_STATUS_ERR_FLAG | 458)

/* frame tags  */
#define VCSFW_FRAME_TAG_FW_BL    1 /* "Firmware" baseline RAM
                                     (HWDEF_MEMMAP_FW_BL_RAM) */
#define VCSFW_FRAME_TAG_LNA_BL   2/*LNA baseline RAM (HWDEF_MEMMAP_LNA_BL_RAM)*/
#define VCSFW_FRAME_TAG_DM       3 /* Drive matrix (HWDEF_MEMMAP_DMRAM) */
#define VCSFW_FRAME_TAG_CDM      4 /* Code-division multiplex decode
                                    matrix (HWDEF_MEMMAP_CDMRAM) */
#define VCSFW_FRAME_TAG_REG32BLK 5 /* A block of 32-bit registers */
#define VCSFW_FRAME_TAG_REG16BLK 6 /* A block of 16-bit registers */
#define VCSFW_FRAME_TAG_DIMS     7 /* Frame tag units */
#define VCSFW_FRAME_TAG_ACQOPT   8 /* Acqusition options (HOST ONLY) */



/* VCSFW_FRAME_TAG */
//typedef struct VCS_PACKED vcsfw_frame_tag_s {
typedef struct vcsfw_frame_tag_s {
    vcsUint16_t   nwords;    /* number of 32-bit to follow */
    vcsUint8_t    flags;
    vcsUint8_t    tagid;
} vcsfw_frame_tag_t;

/* VCSFW_FRAME_TAG_REG32BLK */
//typedef struct VCS_PACKED vcsfw_frame_tag_reg32blk_s {
typedef struct vcsfw_frame_tag_reg32blk_s {
    vcsUint16_t  regbase;
    vcsUint16_t  nregs;
} vcsfw_frame_tag_reg32blk_t;


/* VCSFW_FRAME_TAG_REG16BLK */
//typedef struct VCS_PACKED vcsfw_frame_tag_reg16blk_s {
typedef struct vcsfw_frame_tag_reg16blk_s {
    vcsUint16_t  regbase;
    vcsUint16_t  nregs;
} vcsfw_frame_tag_reg16blk_t;

typedef struct fwcmd_frame_tag_s {
    uint16_t   nwords;    /* number of 32-bit to follow */
    uint8_t    flags;
    uint8_t    tagid;
} fwcmd_frame_tag_t;

typedef struct _fpscan_frame_tag_entry_t {
    uint16_t          tagid;
    uint16_t          length;
    uint16_t          status_tooshort;  /* status if too short */
    uint16_t          status_toolong;   /* status if too long */
    unsigned int    (*handlerp)(const uint32_t *bufp, unsigned int nwords,
                                unsigned int flags);
} fpscan_frame_tag_entry_t;

//#define FPSCAN_BYTES_PER_PIXEL          (2)
//#define FPSCAN_FW_BL_NUM_BYTES          (8064 * FPSCAN_BYTES_PER_PIXEL)
//            /* (144*56) */
//#define FPSCAN_LNA_BL_NUM_BYTES         (8064 * FPSCAN_BYTES_PER_PIXEL)
           /* (144*56) */
#define FPSCAN_CDMRAM_CDM144_MAX_BYTES  (2592)    /* up to 648 32-bit words */
#define FPSCAN_CDMRAM_CDM72_MAX_BYTES   (128)     /* (32*4) */

#ifdef BUILD_FOR_SHASTA
#define FPSCAN_CDMRAM_SYM_NUM_BYTES     (1568)    /* 
                                                   * For symmetric CDM matrix.
                                                   * (112 rows ^2)/32 bits/word 
                                                   * = up to 392 32-bit words
                                                   * * 4 bytes/word = 1568 bytes
                                                   */
#define FPSCAN_DMRAM_NUM_BYTES          (3136)    /*
                                                   * Max: 7 words/row * 112 rows
                                                   * = up to 784 32-bit words
                                                   * * 4 bytes/word = 3136 bytes
                                                   */
#else
#if BUILD_FOR_HAYES
#  define FPSCAN_DMRAM_NUM_BYTES          (2016)   /*
                                                   * 84/16 == 5.25, rounded up
                                                   * to 6. 6*84*4 = 504 * 4 == 2016
                                                   */
#else /* BUILD_FOR_HAYES */
#  define FPSCAN_DMRAM_NUM_BYTES          (5184)    /*((9*32*144)/8) */
#endif /* BUILD_FOR_HAYES */
#endif
static unsigned int
fpscan_frame_tag_reg32blk_handler(const uint32_t *bufp, unsigned int nwords,
                                  unsigned int flags);
static unsigned int
fpscan_frame_tag_reg16blk_handler(const uint32_t *bufp, unsigned int nwords,
                                  unsigned int flags);
static unsigned int
fpscan_frame_tag_cdm_handler(const uint32_t *bufp, unsigned int nwords,
                             unsigned int flags);
static unsigned int
fpscan_frame_tag_dm_handler(const uint32_t *bufp,
                            unsigned int nwords, unsigned int flags);

static unsigned int
fpscan_frame_tag_fw_bl_handler(const uint32_t *bufp, unsigned int nwords,
                                       unsigned int flags);

static unsigned int
fpscan_frame_tag_lna_bl_handler(const uint32_t *bufp, unsigned int nwords,
                                       unsigned int flags);

static const fpscan_frame_tag_entry_t fpscan_builtin_frame_tag_handlers[] = {
    {
        VCSFW_FRAME_TAG_FW_BL,                        /* tagid */
        FPSCAN_FW_BL_NUM_BYTES,                       /* length */
        VCSFW_STATUS_OKAY,                            /* status_tooshort */
        VCSFW_STATUS_ERR_ADCBASELINETOOLONG,          /* status_toolong */
        &fpscan_frame_tag_fw_bl_handler               /* handlerp */
    },
    {
        VCSFW_FRAME_TAG_LNA_BL,                       /* tagid */
        FPSCAN_LNA_BL_NUM_BYTES,                      /* length */
        VCSFW_STATUS_OKAY,                            /* status_tooshort */
        VCSFW_STATUS_ERR_LNABASELINETOOLONG,          /* status_toolong */
        &fpscan_frame_tag_lna_bl_handler              /* handlerp */
    },
    {
        VCSFW_FRAME_TAG_DM,                           /* tagid */
        FPSCAN_DMRAM_NUM_BYTES,                       /* length */
        VCSFW_STATUS_OKAY,                            /* status_tooshort */
        VCSFW_STATUS_ERR_DRIVEMATIXTOOLONG,           /* status_toolong */
        &fpscan_frame_tag_dm_handler                  /* handlerp */
    },
    {
        VCSFW_FRAME_TAG_CDM,                          /* tagid */
        0,                                            /* length */
        VCSFW_STATUS_OKAY,                            /* status_tooshort */
        VCSFW_STATUS_OKAY,                            /* status_toolong */
        &fpscan_frame_tag_cdm_handler                 /* handlerp */
    },
    {
        VCSFW_FRAME_TAG_REG32BLK,                     /* tagid */
        sizeof(vcsfw_frame_tag_reg32blk_t),           /* length */
        VCSFW_STATUS_ERR_FRAME_TAG_REG32BLK_TOOSHORT, /* status_tooshort */
        VCSFW_STATUS_OKAY,                            /* status_toolong */
        &fpscan_frame_tag_reg32blk_handler            /* handlerp */
    },
    {
        VCSFW_FRAME_TAG_REG16BLK,                     /* tagid */
        sizeof(vcsfw_frame_tag_reg16blk_t),           /* length */
        VCSFW_STATUS_ERR_FRAME_TAG_REG16BLK_TOOSHORT, /* status_tooshort */
        VCSFW_STATUS_OKAY,                            /* status_toolong */
        &fpscan_frame_tag_reg16blk_handler            /* handlerp */
    },
};


static uint32_t  registers_written; 
uint32_t         rd_reg_val;
static uint32_t  reg_diff_after_write; 



#define MAX_REG_CHECK_FAIL_LIST_SZ    30
static uint32_t          n_reg_fail_list;
static reg_check_fail_t  reg_fail_list[MAX_REG_CHECK_FAIL_LIST_SZ];

void add_to_reg_check_fail_list(uint32_t address, uint32_t expected_val, uint32_t read_val)
{
    if (MAX_REG_CHECK_FAIL_LIST_SZ > n_reg_fail_list)
    {
        reg_fail_list[n_reg_fail_list].address = address;
        reg_fail_list[n_reg_fail_list].expected_val = expected_val;
        reg_fail_list[n_reg_fail_list].read_val = read_val;
        n_reg_fail_list++;
    }
}

void get_reg_check_fail_list(reg_check_fail_t **pp_list, uint32_t *p_n_list)
{
    *pp_list = &reg_fail_list[0];
    *p_n_list = n_reg_fail_list;
}


void copy_reg_check_vail_list(uint32_t *p_list)
{
    uint32_t  i;
    uint32_t  *p_copy_from;

    p_list[0] = n_reg_fail_list;

    p_copy_from = (uint32_t *)&reg_fail_list[0];

    for(i=0; i<n_reg_fail_list*3; i++)
    {
        p_list[i+1] = p_copy_from[i];
    }
}


typedef struct fwcmd_frame_tag_reg32blk_s {
    uint16_t  regbase;
    uint16_t  nregs;
} fwcmd_frame_tag_reg32blk_t;

static unsigned int
fpscan_frame_tag_reg32blk_handler(const uint32_t *bufp, unsigned int nwords,
                                  unsigned int flags)
{
    unsigned int                         i;
    //unsigned int                         j;
    const uint32_t                      *val32;
    uint32_t                             address;
    const fwcmd_frame_tag_reg32blk_t    *reg32blk_p;

    i     = 0;
    val32 = NULL;
    reg32blk_p = (const fwcmd_frame_tag_reg32blk_t *) bufp;

    if ((nwords * sizeof(uint32_t)) >
        (sizeof(fwcmd_frame_tag_reg32blk_t)
         + (reg32blk_p->nregs * sizeof(uint32_t)))) {
        return VCSFW_STATUS_ERR_FRAME_TAG_REG32BLK_TOOLONG;
    }
    else if ((nwords * sizeof(uint32_t)) <
             (sizeof(fwcmd_frame_tag_reg32blk_t)
              + (reg32blk_p->nregs * sizeof(uint32_t)))) {
        return VCSFW_STATUS_ERR_FRAME_TAG_REG32BLK_TOOSHORT;
    }
    else {
        val32 = (const uint32_t *) &(reg32blk_p[1]);
        address = reg32blk_p->regbase + HWDEF_MEMMAP_REGISTER_BASE;

        i = reg32blk_p->nregs;
        while (i != 0) {
             
            rd_reg_val = REG_READ32(address);
            if (rd_reg_val != *val32)
            {
                add_to_reg_check_fail_list(address, *val32, rd_reg_val);
                REG_VARWRITE32(address, *val32);
                registers_written++;

                rd_reg_val = REG_READ32(address);
                if (rd_reg_val != *val32)
                {
                    reg_diff_after_write++;
                }
            }
            address += 4;
            val32++;
            i--;
        }
    }
    return VCSFW_STATUS_OKAY;
}

typedef struct fwcmd_frame_tag_reg16blk_s {
    uint16_t  regbase;
    uint16_t  nregs;
} fwcmd_frame_tag_reg16blk_t;


void register_bulk_write(uint32_t start_address, const uint32_t *data, uint32_t nwords)
{
    uint32_t i;
    uint32_t address;

    address = start_address; 
    for(i=0; i<nwords; i++, address +=4)
    {
        rd_reg_val = REG_READ32(address);
        if (rd_reg_val != data[i])
        {
            add_to_reg_check_fail_list(address, data[i], rd_reg_val);
            REG_VARWRITE32(address, data[i]);
            registers_written++;
            rd_reg_val = REG_READ32(address);
            if (rd_reg_val != data[i])
            {
                reg_diff_after_write++;
            }
        }
        //REG_VARWRITE32(address, data[i]);        
    }
}

void register_bulk_set(uint32_t start_address, uint32_t data, uint32_t nwords)
{
    uint32_t i;
    uint32_t address;

    address = start_address; 
    for(i=0; i<nwords; i++, address +=4)
    {
        REG_VARWRITE32(address, data);        
    }
}




static unsigned int
fpscan_frame_tag_reg16blk_handler(const uint32_t *bufp, unsigned int nwords,
                                  unsigned int flags)
{
    uint32_t address;
    const uint16_t *val16;
    unsigned int expectedsize;
    unsigned int i;
    const fwcmd_frame_tag_reg16blk_t *reg16blk_p;

    i     = 0;
    val16 = NULL;
    reg16blk_p = (const fwcmd_frame_tag_reg16blk_t *) bufp;

    /* compute bytes expected, accounting for 32-bit alignment */
    if (((reg16blk_p->nregs) & 1) == 1) {
        expectedsize = (reg16blk_p->nregs * sizeof(uint16_t)) + 2;
    }
    else {
        expectedsize = (reg16blk_p->nregs * sizeof(uint16_t));
    }

    if (((nwords * sizeof(uint32_t)) - sizeof(fwcmd_frame_tag_reg16blk_t)) <
        expectedsize) {
        return VCSFW_STATUS_ERR_FRAME_TAG_REG16BLK_TOOLONG;
    }
    if (((nwords * sizeof(uint32_t)) - sizeof(fwcmd_frame_tag_reg16blk_t)) >
         expectedsize) {
        return VCSFW_STATUS_ERR_FRAME_TAG_REG16BLK_TOOSHORT;
    }
    else {
        i = 0;
        val16 = (const uint16_t *) &(reg16blk_p[1]);
        address = reg16blk_p->regbase + HWDEF_MEMMAP_REGISTER_BASE;

        i = (unsigned int) reg16blk_p->nregs;
        while (i != 0) {
            rd_reg_val = REG_READ32(address);
            if ((rd_reg_val & 0xffff) != *val16)
            {
                add_to_reg_check_fail_list(address, *val16, (rd_reg_val&0xffff));
                REG_VARWRITE32(address, *val16);
                registers_written++;

                rd_reg_val = REG_READ32(address);
                if ((rd_reg_val&0xffff) != *val16)
                {
                    reg_diff_after_write++;
                }
            }
            address += sizeof(uint32_t);
            val16++;
            i--;
        }
    }
    return VCSFW_STATUS_OKAY;
}

#if BUILD_FOR_HAYES
static unsigned int
fpscan_frame_tag_cdm_handler(const uint32_t *bufp, unsigned int nwords,
                             unsigned int flags)
{
    //REG_BLKWRITE32(HWDEF_MEMMAP_CDMRAM_BASE, bufp, nwords);
    register_bulk_write(HWDEF_MEMMAP_CDMRAM_BASE, bufp, nwords);
    return VCSFW_STATUS_OKAY;
}

#else /* BUILD_FOR_HAYES */
static unsigned int
fpscan_frame_tag_cdm_handler(const uint32_t *bufp, unsigned int nwords,
                             unsigned int flags)
{
    uint32_t         cdm_mode;
    unsigned int     expected_length;

    /* The max size can vary by whether CDM144 or CDM72 is
     * currently configured. The matrix size is allowed to be
     * smaller than the max.
     */

    /*TODO - the code below handles the case where the matrix is
    * the maximum size per mode, but the b1210 spec seems to indicate
    * the matrix can be small than the max. If so, this code doesn't
    * handle that case. If the matrix is smaller than the max, where
    * do we store the actual size and how does the CDM know how big it
    * is?
    *
    * Maybe we just get/set the entire CDMRAM size always?
    * */
#ifdef BUILD_FOR_SHASTA
		expected_length = FPSCAN_CDMRAM_SYM_NUM_BYTES;
#else
    expected_length = FPSCAN_CDMRAM_CDM144_MAX_BYTES;

    cdm_mode = (REG_READ32(CDM_CDMCFG) & CDM_CDMCFG_CDM_MODE)
        >> CDM_CDMCFG_CDM_MODE_B;
    if ( cdm_mode == CDM_CDMCFG_CDM_MODE_CDM72 ) {
        expected_length = FPSCAN_CDMRAM_CDM72_MAX_BYTES;
    }
#endif
    if ((nwords * sizeof(uint32_t)) < expected_length) {
        return VCSFW_STATUS_ERR_DECODEMATRIXTOOSHORT;
    }
    else if ((nwords * sizeof(uint32_t)) > expected_length) {
        return VCSFW_STATUS_ERR_DECODEMATRIXTOOLONG;
    }
    else 
    {
        //REG_BLKWRITE32(HWDEF_MEMMAP_CDMRAM_BASE, bufp, nwords);
        register_bulk_write(HWDEF_MEMMAP_CDMRAM_BASE, bufp, nwords);
    }
    return VCSFW_STATUS_OKAY;
}
#endif /* BUILD_FOR_HAYES */


static unsigned int
fpscan_frame_tag_dm_handler(const uint32_t *bufp,
                            unsigned int nwords, unsigned int flags)
{
    //REG_BLKWRITE32(HWDEF_MEMMAP_DMRAM_BASE, bufp, nwords);
    register_bulk_write(HWDEF_MEMMAP_DMRAM_BASE, bufp, nwords);

    return VCSFW_STATUS_OKAY;
}

static unsigned int
fpscan_frame_tag_fw_bl_handler(const uint32_t *bufp, unsigned int nwords,
                                       unsigned int flags)
{
    //REG_BLKWRITE32(HWDEF_MEMMAP_FW_BL_RAM_BASE, bufp, nwords);
    register_bulk_write(HWDEF_MEMMAP_FW_BL_RAM_BASE, bufp, nwords);

    return VCSFW_STATUS_OKAY;
}

static unsigned int
fpscan_frame_tag_lna_bl_handler(const uint32_t *bufp, unsigned int nwords,
                                unsigned int flags)
{
    //REG_BLKWRITE32(HWDEF_MEMMAP_LNA_BL_RAM_BASE, bufp, nwords);
    register_bulk_write(HWDEF_MEMMAP_LNA_BL_RAM_BASE, bufp, nwords);

    return VCSFW_STATUS_OKAY;
}


uint32_t fpscan_tagsparse(const uint32_t *tagp, 
                                uint32_t nbytes, 
                                  bool_t dm_only, 
                                uint32_t *p_n_tags_used, 
                                uint32_t *p_n_tags_skip)
{
    int                              status;
    const uint32_t                  *parsep;
    const fwcmd_frame_tag_t         *frame_tagp;
    const fpscan_frame_tag_entry_t  *tvep;
    unsigned int                     tagid, nwords, flags;
    unsigned int                    n_tags_skip;
    unsigned int                    n_tags_used;


    n_tags_skip = 0;
    n_tags_used = 0;
    parsep = tagp;

    registers_written = 0;
    reg_diff_after_write = 0;
    n_reg_fail_list = 0;

    while (nbytes >= sizeof(fwcmd_frame_tag_t)) {
        /* Assert that this thing is properly aligned */
        //VFK_ASSERT((((uint32_t) parsep) & 0x3) == 0);

        frame_tagp = (const fwcmd_frame_tag_t *) parsep;
        parsep = (const uint32_t *) &(parsep[1]);
        nbytes -= sizeof(vcsfw_frame_tag_t);

        tagid  = (unsigned int) frame_tagp->tagid;
        nwords = (unsigned int) frame_tagp->nwords;
        flags  = (unsigned int) frame_tagp->flags;


        /* Is there enough data left to actually make up this tag? */
        if ((nwords * sizeof(uint32_t)) > nbytes) {
            return VCSFW_STATUS_ERR_FRAME_TAG_TOOSHORT;
        }

        /* Look for the tag handler */
        tvep = NULL;
        if ((FALSE == dm_only) || ((TRUE == dm_only) && (VCSFW_FRAME_TAG_DM == tagid)))
        {
            unsigned int i;
            
            for(i=0; i<NELEM(fpscan_builtin_frame_tag_handlers); i++)
            {
                if (tagid == fpscan_builtin_frame_tag_handlers[i].tagid)
                {
                    tvep = &fpscan_builtin_frame_tag_handlers[i];
                    break;
                }
            }
        }

        /* Simply ignore those tags we can't recognize */
        if (tvep != NULL) {
            if (tvep->status_tooshort != VCSFW_STATUS_OKAY
                && (nwords * sizeof(uint32_t)) < tvep->length) {
                return tvep->status_tooshort;
            }
            if (tvep->status_toolong != VCSFW_STATUS_OKAY
                && (nwords * sizeof(uint32_t)) > tvep->length) {
                return tvep->status_toolong;
            }

            status = (*tvep->handlerp)(parsep, nwords, flags);
            if (status != VCSFW_STATUS_OKAY) {
                return status;
            }

            /* Remember that we saw that tag */
            //fpscan_state.tagsseen |= (1 << tagid);
            n_tags_used++;
        }
        else
        {
            n_tags_skip++;
        }
        
        parsep += nwords;
        nbytes -= (nwords * sizeof(uint32_t));
    }

    if (NULL != p_n_tags_used)
    {
        *p_n_tags_used = n_tags_used;
        //*p_n_tags_used = registers_written;
    }
    if (NULL != p_n_tags_skip)
    {
        *p_n_tags_skip = n_tags_skip;
        //*p_n_tags_skip = reg_diff_after_write;
    }

    return VCSFW_STATUS_OKAY;
}
 

typedef struct nvmiota_header_s 
{
    uint16_t        nbytes;         /* number of bytes of iota payload */
    uint16_t        itype;          /* type of iota */
    uint8_t         reserved[4];    /* reserved for future use */
} nvmiota_header_t;

uint32_t parse_iotas(uint32_t iota_addr, 
                     uint16_t iota_type,
                     uint32_t *p_n_tags_used, 
                     uint32_t *p_n_tags_skip)
{
    const uint32_t     *iptr;
    //uint32_t           j;
    uint32_t           size;
    uint32_t           k;
    uint32_t           tot_tags_used;
    uint32_t           tot_tags_skip;
    nvmiota_header_t   *p_iota_hdr;

    tot_tags_used = 0;
    tot_tags_skip = 0;
    //iptr = (const uint32_t *)(HWDEF_MEMMAP_SROM_FLASH_IOTA_BASE);
    iptr = (const uint32_t *)(iota_addr);
    //j = 0; 
    while (*iptr != 0xffffffff)
    {
        p_iota_hdr = (nvmiota_header_t *) iptr;

        if (iota_type == p_iota_hdr->itype)
        {
            uint32_t tags_used;
            uint32_t tags_skip;

            tags_used = 0;
            tags_skip = 0;

            //tags_to_parse = ;
            fpscan_tagsparse((const uint32_t *)(iptr + (sizeof(nvmiota_header_t)/sizeof(uint32_t))), 
                p_iota_hdr->nbytes, 0, &tags_used, &tags_skip);

            //tags_used = 1;

            tot_tags_used += tags_used;
            tot_tags_skip += tags_skip;
        }


        size = (p_iota_hdr->nbytes) + sizeof(nvmiota_header_t);
        k = size%8;
        if (0 != k) size += (8-k); 
        iptr += (size/sizeof(uint32_t));

    }

    if (NULL != p_n_tags_used)
    {
        *p_n_tags_used = tot_tags_used;
    }
    if (NULL != p_n_tags_skip)
    {
        *p_n_tags_skip = tot_tags_skip;
    }

    return VCSFW_STATUS_OKAY;

}


uint32_t find_iota_addr(uint32_t iota_start, uint32_t *p_match_data, uint32_t match_len, bool_t b_skip_hdr_cmp)
{
    const uint32_t        *iptr;
    uint32_t              j;
    uint32_t              size;
    uint32_t              k;
    nvmiota_header_t      *p_iota_hdr;
    uint32_t              ret_val;
    uint32_t              offset;


    ret_val = FIND_IOTA_FAIL_ADDRESS;
    iptr = (const uint32_t *)(iota_start);
    j = 0; 

    offset = 0;
    if (b_skip_hdr_cmp) offset = 2;

    while (*iptr != 0xffffffff)
    {
        p_iota_hdr = (nvmiota_header_t *) iptr;

        for(j=0; j<match_len; j++)
        {
            if (iptr[j+offset] != p_match_data[j])
            {
                break;
            }
        }
        if (j == match_len)
        {
            ret_val = (uint32_t) iptr;
        }

        size = (p_iota_hdr->nbytes) + sizeof(nvmiota_header_t);
        k = size%8;
        if (0 != k) size += (8-k); 
        iptr += (size/sizeof(uint32_t));

    }

    return ret_val;

}





void get_iota_tag_list(uint32_t iota_start, uint32_t *p_data, uint32_t *p_n_data)
{
    const uint32_t           *iptr;
    uint32_t                 size;
    uint32_t                 k;
    const nvmiota_header_t   *p_iota_hdr;
    uint32_t                 pos;
    const uint32_t           *p_tag_start;
    const fwcmd_frame_tag_t  *frame_tagp;
    uint32_t                 tag_pos;
    

    iptr = (const uint32_t *)(iota_start);
    pos = 0;
    while (*iptr != 0xffffffff)
    {
        p_iota_hdr = (nvmiota_header_t *) iptr;

        if (pos < *p_n_data)
        {
            p_data[pos] = iptr[0];
            pos++;
        }

        /* Now parse the frame tags */
        tag_pos = 0;
        p_tag_start = (const uint32_t *)&iptr[2];
        while (tag_pos < (p_iota_hdr->nbytes/sizeof(uint32_t)))
        {
            if (pos < *p_n_data)
            {
                p_data[pos] = p_tag_start[tag_pos];
                pos++;
            }
            frame_tagp = (const fwcmd_frame_tag_t *)&p_tag_start[tag_pos];
            tag_pos++; /*Increment for tag header */
            tag_pos += frame_tagp->nwords;  /* Increment for tag payload */
        }

        /* Increment to next iota */
        size = (p_iota_hdr->nbytes) + sizeof(nvmiota_header_t);
        k = size%8;
        if (0 != k) size += (8-k); 
        iptr += (size/sizeof(uint32_t));
    }

    *p_n_data = pos;
}



