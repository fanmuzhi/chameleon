/* -*- mode: c; tab-width: 4 -*- */

/*
 * Copyright (c) 2007-2015 Synaptics Incorporated.  All rights reserved.
 */

/*
 * Implementation of access functions for the Denali internal flash memory
 *
 * Jim Paul, July 2015.
 *
 */

#include "basetypes.h"
#include "hwreg.h"
#include "nvm_prog.h"
#include "reg.h"

/*
 * Set to 1 to do additional read-back verify of write/erase operations.
 * This should not be necessary though as controller should be verifying
 * itself...
 */
#define NVM_DO_VERIFY 0

#define NVM_FLASH_PATCH_LO_MAGIC_NUM  0xb9a6
#define NVM_FLASH_PATCH_HI_MAGIC_NUM  0xb434

typedef struct {
    uint16_t   short0;
    uint16_t   short1;
    uint16_t   short2;
    uint16_t   short3;
} nvm_flash_word_shorts_t;


/******************************************************************************/
/*
 *  Internal helper functions to hide some of the flash controller details and
 *  make the code more readable...
 */
/******************************************************************************/

/* nvm_addr_in_info_range returns TRUE  if the address in the info block,
 * FALSE if not...
 */
static int
nvm_addr_in_info_range(uint32_t addr) {
    if (
#if  NVM_INFO_BLOCK_FIRST_ADDRESS > 0
    /* This is to get rid of a warning about checking if an unsigned is
     * less than zero that occurs if NVM_MISSION_FW_LAST_ADDRESS is zero...
     */
      addr < NVM_INFO_BLOCK_FIRST_ADDRESS ||
#endif
      addr > NVM_INFO_BLOCK_LAST_ADDRESS ) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}


/* nvm_addr_in_main_range returns TRUE if the address is a valid nvm main
 * address, FALSE if not...
 */
static int
nvm_addr_in_main_range(uint32_t addr) {
    if (
#if  NVM_MAIN_FIRST_ADDRESS > 0
    /* This is to get rid of a warning about checking if an unsigned is
     * less than zero that occurs if NVM_FIRST_ADDRESS is zero...
     */
      addr < NVM_MAIN_FIRST_ADDRESS ||
#endif
      addr > NVM_MAIN_LAST_ADDRESS ) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}


/* page_in_nvm_range returns 1 if the page number is a valid nvm main page,
 * 0 if not...
 */
static int
nvm_page_in_main_range(uint32_t page_num) {
    if (
#if NVM_MAIN_FIRST_PAGE > 0
    /* This is to get rid of a warning about checking if an unsigned is
     * less than zero that occurs if NVM_FIRST_PAGE is zero...
     */
        page_num < NVM_MAIN_FIRST_PAGE ||
#endif
        page_num > NVM_MAIN_LAST_PAGE) {
            return FALSE;
        }
    else {
        return TRUE;
    }
}

/* page_in_nvm_range returns 1 if the page number is a valid nvm info page,
 * 0 if not...
 */
static int
nvm_page_in_info_range(uint32_t page_num) 
{
	if((page_num < NVM_TOTAL_PAGES) || (page_num >= (NVM_TOTAL_PAGES-NVM_INFO_ALLOCATED_PAGES)))
        return TRUE;
    else
        return FALSE;
}


/* Checks the nvm controller status and does not return until the
 * controller indicates it is in a non-busy state
 */
static int
nvm_wait_for_controller()
{
    uint32_t op_status;
    uint32_t flash_status;

    uint32_t op_busy_flags;
    uint32_t flash_busy_flags;

    op_busy_flags= NVM_FLASH_OP_STATUS_FLASH_BUSY |
                   NVM_FLASH_OP_STATUS_SMART_BUSY;

    flash_busy_flags = NVM_FLASH_STATUS_CORE_BUSY |
                       NVM_FLASH_STATUS_SMART_BUSY;

    uint32_t loopcount;

    /* wait for nvm controller to not be busy */
    loopcount = (int)-1;
    do {
        op_status = REG_READ32(NVM_FLASH_OP_STATUS);
        flash_status =  REG_READ32(NVM_FLASH_STATUS);
    } while ((op_status & op_busy_flags) != 0 &&
             (flash_status & flash_busy_flags) != 0 &&
              --loopcount != 0);


    if (loopcount == 0) {
      return NVM_CONTROLLER_TIMEOUT;
    }


    return NVM_SUCCESS;
}


/* If an error occurs, the error flag must be manually cleared... */
static void
nvm_clear_error_flag()
{
    REG_WRITE32( NVM_FLASH_OP_REQUEST, NVM_FLASH_OP_REQUEST_CLEAR_ERROR_FLAG);
}


/* returns whether or not the flash controller is in an error state */
uint32_t
error_flag_is_set()
{
    REG_WRITE32(0x60000000, REG_READ32(NVM_FLASH_OP_STATUS) );

    return(
       REG_READ32(NVM_FLASH_OP_STATUS) == NVM_FLASH_OP_STATUS_SMART_LOOP_ERROR ?
               TRUE:FALSE
    );
}


/* set the address that a read/write/erase operation is to act upon */
static void
nvm_set_active_nvm_address(uint32_t address){
    REG_WRITE32(NVM_FLASH_OP_ADDR, address);
}


/* Tell the flash controller to do something */
static void
start_nvm_operation(uint32_t operation){
    REG_WRITE32(NVM_FLASH_OP_REQUEST, operation);
}


/* Load the data to be written into the flash controller's data-in register */
static void
nvm_data_in_load(nvm_flash_word_t * data){
    nvm_flash_word_shorts_t* data_in;

    data_in = (nvm_flash_word_shorts_t*)(data);

    /* set the four 16-bit components of the flash word being written */
    REG_WRITE32(NVM_FLASH_OP_DIN_0, data_in->short0);
    REG_WRITE32(NVM_FLASH_OP_DIN_1, data_in->short1);
    REG_WRITE32(NVM_FLASH_OP_DIN_2, data_in->short2);
    REG_WRITE32(NVM_FLASH_OP_DIN_3, data_in->short3);
}


/* Retrieve the data the flash controller's data-out register */
static void
nvm_data_out_get(nvm_flash_word_t * data) {
    nvm_flash_word_shorts_t data_out;

    /* set the four 16-bit components of the flash word  */
    data_out.short0 = REG_READ32(NVM_FLASH_OP_DOUT_0);
    data_out.short1 = REG_READ32(NVM_FLASH_OP_DOUT_1);
    data_out.short2 = REG_READ32(NVM_FLASH_OP_DOUT_2);
    data_out.short3 = REG_READ32(NVM_FLASH_OP_DOUT_3);
    (*(nvm_flash_word_shorts_t*)data) = data_out;
}


/* Returns whether or not erase/program operations are enabled */
static uint32_t
nvm_flash_write_is_enabled() {

    return (((REG_READ32(NVM_FLASH_SEC_STATUS) &
            NVM_FLASH_SEC_STATUS_PROGRAM_INHIBIT) == 0 )? TRUE:FALSE);
}


/* Returns whether or not read operations are enabled */
static uint32_t
nvm_flash_read_is_enabled() {
    return ( (( REG_READ32(NVM_FLASH_SEC_STATUS) &
               NVM_FLASH_SEC_STATUS_CMD_READ_INHIBIT
             ) == 0) ? TRUE:FALSE);

}


/* enables flash programming operations */
static void
nvm_unlock_flash() {
    REG_WRITE32(NVM_FLASH_PATCH_LO, NVM_FLASH_PATCH_LO_MAGIC_NUM);
    REG_WRITE32(NVM_FLASH_PATCH_HI, NVM_FLASH_PATCH_HI_MAGIC_NUM);
}


/* Disables flash programming operations */
static void
nvm_lock_flash() {
    REG_WRITE32(NVM_FLASH_PATCH_LO, 0);
    REG_WRITE32(NVM_FLASH_PATCH_HI, 0);
}


/* Read one flash word */
static int
word_read(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    /*if there is a previous error, clear it */
    if ( error_flag_is_set() ) {
        nvm_clear_error_flag();
    }

    /* set the flash address to the requested flash word address */
    nvm_set_active_nvm_address(addr);

    if ( !nvm_flash_read_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* Tell the controller to read */
        start_nvm_operation(NVM_FLASH_OP_REQUEST_READ_START);

        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* check for an error */
        if ( error_flag_is_set() ) {
            nvm_clear_error_flag();
            rc = NVM_ERROR;
        }
        else {
            nvm_data_out_get(data);
        }
    }
    return rc;
}

/* Write one flash word */
static int
word_write(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    /*if there is a previous error, clear it */
    if ( error_flag_is_set() ) {
        nvm_clear_error_flag();
    }

    /* set the flash address to the address of the flash word to write */
    nvm_set_active_nvm_address(addr);

    if ( !nvm_flash_write_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else
    {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* provide the data to be written to the flash controller */
        nvm_data_in_load(data);

        /* Tell the controller to write */
        start_nvm_operation(NVM_FLASH_OP_REQUEST_PROG_START);

        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        };

        /* check for an error */
        if ( error_flag_is_set() ) {
            nvm_clear_error_flag();
            rc = NVM_ERROR;
        }
#if defined NVM_DO_VERIFY && NVM_DO_VERIFY == 1
        else {
            /* Verify the write */;
            start_nvm_operation(NVM_FLASH_OP_REQUEST_VERIFY_PROG_START);

            /* check for an error */
            if ( error_flag_is_set() ) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
            }
        }
#endif
    }
    return rc;
}



/* Write a chunk of consecutive flash words */
static int
bulk_write(uint32_t addr, uint32_t size, nvm_flash_word_t * data) {
    int rc;
    uint32_t count;

    rc = NVM_SUCCESS;
    count = 0;

    nvm_flash_word_shorts_t* data_in;
    data_in = (nvm_flash_word_shorts_t*)(data);

    /*if there is a previous error, clear it */
    if ( error_flag_is_set() ) {
        nvm_clear_error_flag();
    }

    /* set address register to address of first flash word to program */
    nvm_set_active_nvm_address(addr);

    if ( !nvm_flash_write_is_enabled()  ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        while (count < size )
        {
            /* provide the data to be written to the flash controller */
            nvm_data_in_load(data);

            /* Tell the controller to write */
            start_nvm_operation(NVM_FLASH_OP_REQUEST_PROG_START);

            /* wait for flash controller to not be busy */
            if (nvm_wait_for_controller() != NVM_SUCCESS) {
                return NVM_CONTROLLER_TIMEOUT;
            }

            /* check for an error */
            if (error_flag_is_set()) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
                break;
            }
    #if defined NVM_DO_VERIFY && NVM_DO_VERIFY == 1
            else {
                /* Verify the write */;
                start_nvm_operation(NVM_FLASH_OP_REQUEST_VERIFY_PROG_START);

                /* check for an error */
                if ( error_flag_is_set()) {
                    nvm_clear_error_flag();
                    rc = NVM_ERROR;
                }
            }
    #endif
            data++;
            count++;

            /* set address register to address of next flash word to program */
            nvm_set_active_nvm_address(addr+count);
        }
    }
    return rc;
}



/******************************************************************************/
/*
 *  Common nvm functions exposed through the nvm_prog header file
 */
/******************************************************************************/

/* Enable erase and program operations */
int
nvm_unlock(){
    nvm_unlock_flash();

    /* wait for flash controller to not be busy */
    if (nvm_wait_for_controller() != NVM_SUCCESS) {
        return NVM_CONTROLLER_TIMEOUT;
    }

    if (nvm_flash_write_is_enabled()) {
        return NVM_SUCCESS;
    }
    else
    {
        return NVM_ERROR;
    }
}


/* Inhibit erase and program operations */
int
nvm_lock() {
    nvm_lock_flash();

    /* wait for flash controller to not be busy */
    if (nvm_wait_for_controller() != NVM_SUCCESS) {
        return NVM_CONTROLLER_TIMEOUT;
    }

    if (!nvm_flash_write_is_enabled()) {
        return NVM_SUCCESS;
    }
    else {
        return NVM_ERROR;
    }
}


/* Erase one flash page in main flash area */
int
nvm_page_erase(uint32_t page_num) {
    int rc;

    rc = NVM_SUCCESS;

    /*if there is a previous error, clear it */
    if (error_flag_is_set()) {
        nvm_clear_error_flag();
    }

    /* set the flash address to operate on to the address of the first
     * flash word in the page being erased
     */
    nvm_set_active_nvm_address((NVM_MAIN_FIRST_PAGE+page_num)*
                                NVM_WORDS_PER_PAGE);

    if ( !nvm_flash_write_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else if (nvm_page_in_main_range(page_num) == 0) {
        rc = NVM_INVALID_PAGE_NUM;
    }
    else
    {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* Tell the controller to erase the page */
        start_nvm_operation(NVM_FLASH_OP_REQUEST_ERASE_PAGE_START);

        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* check for an error */
        if ( error_flag_is_set() ) {
            nvm_clear_error_flag();
            rc = NVM_ERROR;
        }
#if defined NVM_DO_VERIFY && NVM_DO_VERIFY == 1
        else {
            /* Verify the erase */
            nvm_set_active_nvm_address((NVM_MAIN_FIRST_PAGE+page_num)
                                        *NVM_WORDS_PER_PAGE);
            start_nvm_operation( NVM_FLASH_OP_REQUEST_VERIFY_ERASE_START) ;

            if ( error_flag_is_set() ) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
            }
        }
#endif
    }
    return rc;
}



/* Erase one flash page in info flash area */
int
nvm_info_page_erase(uint32_t page_num) {
    int rc;

    rc = NVM_SUCCESS;

    /*if there is a previous error, clear it */
    if (error_flag_is_set()) {
        nvm_clear_error_flag();
    }

    /* set the flash address to operate on to the address of the first
     * flash word in the page being erased
     */
    nvm_set_active_nvm_address(page_num * NVM_WORDS_PER_PAGE);

    if ( !nvm_flash_write_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else if (nvm_page_in_info_range(page_num) == 0) {
        rc = NVM_INVALID_PAGE_NUM;
    }
    else
    {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* Tell the controller to erase the page */
        start_nvm_operation(NVM_FLASH_OP_REQUEST_ERASE_PAGE_START);

        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* check for an error */
        if ( error_flag_is_set() ) {
            nvm_clear_error_flag();
            rc = NVM_ERROR;
        }
#if defined NVM_DO_VERIFY && NVM_DO_VERIFY == 1
        else {
            /* Verify the erase */
            nvm_set_active_nvm_address(page_num * NVM_WORDS_PER_PAGE);
            start_nvm_operation( NVM_FLASH_OP_REQUEST_VERIFY_ERASE_START) ;

            if ( error_flag_is_set() ) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
            }
        }
#endif
    }
    return rc;
}




/* Erase the entire flash */
int
nvm_mass_erase(bool_t infotoo) {
    int rc;

    rc = NVM_SUCCESS;

    /*if there is a previous error, clear it */
    if (error_flag_is_set()) {
        nvm_clear_error_flag();
    }

    /* set the flash address to operate on to the address of the first
     * flash word in the page being erased
     */
    nvm_set_active_nvm_address(infotoo ? NVM_INFO_BLOCK_FIRST_ADDRESS : 0);
        

    if ( !nvm_flash_write_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* Tell the controller to erase the page */
        start_nvm_operation(NVM_FLASH_OP_REQUEST_ERASE_MASS_START);

        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        /* check for an error */
        if ( error_flag_is_set() ) {
            nvm_clear_error_flag();
            rc = NVM_ERROR;
        }
    }
    return rc;
}


/* Read one flash word */
int
nvm_word_read(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    if ( !nvm_addr_in_main_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else {
       rc = word_read(addr, data);
    }
    return rc;
}


/* Read one info block flash word */
int
nvm_info_word_read(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    if ( !nvm_addr_in_info_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else {
       rc = word_read(addr, data);
    }
    return rc;
}



/* Write one info block flash word */
int
nvm_info_word_write(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    if ( !nvm_addr_in_info_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else {
        rc = word_write(addr, data);
    }
    return rc;
}


/* Write one main block flash word */
int
nvm_word_write(uint32_t addr, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

    if ( !nvm_addr_in_main_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else {
        rc = word_write(addr, data);
    }
    return rc;
}


/* Read a chunk of consecutive flash words from main block */
int
nvm_read(uint32_t addr, uint32_t size, nvm_flash_word_t * data) {
    int rc;
    uint32_t count;

    rc = NVM_SUCCESS;
    count = 0;

    /*if there is a previous error, clear it */
    if (error_flag_is_set()) {
        nvm_clear_error_flag();
    }

    /* set the flash address to the first requested flash word address */
    nvm_set_active_nvm_address(addr);

    if ( !nvm_flash_read_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else if ( !nvm_addr_in_main_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else
    {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        while (count < size)
        {
            /* Tell the controller to read */
            start_nvm_operation(NVM_FLASH_OP_REQUEST_READ_START);

            /* wait for flash controller to not be busy */
            if (nvm_wait_for_controller() != NVM_SUCCESS) {
                return NVM_CONTROLLER_TIMEOUT;
            }

            if (error_flag_is_set()) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
                break;
            }
            else {
                nvm_data_out_get(data);
                data ++;
                count++;
            }

          /* set the flash address to the next requested flash word address */
          nvm_set_active_nvm_address(addr+count);
        }
    }
    return rc;
}

/* Read a chunk of consecutive flash words from info block */
int
nvm_info_read(uint32_t addr, uint32_t size, nvm_flash_word_t * data) {
    int rc;
    uint32_t count;

    rc = NVM_SUCCESS;
    count = 0;

    /*if there is a previous error, clear it */
    if (error_flag_is_set()) {
        nvm_clear_error_flag();
    }

    /* set the flash address to the first requested flash word address */
    nvm_set_active_nvm_address(addr);

    if ( !nvm_flash_read_is_enabled() ) {
        rc = NVM_OPERATION_IS_INHIBITED;
    }
    else if ( !nvm_addr_in_info_range(addr) ) {
        rc = NVM_INVALID_ADDRESS;
    }
    else
    {
        /* wait for flash controller to not be busy */
        if (nvm_wait_for_controller() != NVM_SUCCESS) {
            return NVM_CONTROLLER_TIMEOUT;
        }

        while (count < size)
        {
            /* Tell the controller to read */
            start_nvm_operation(NVM_FLASH_OP_REQUEST_READ_START);

            /* wait for flash controller to not be busy */
            if (nvm_wait_for_controller() != NVM_SUCCESS) {
                return NVM_CONTROLLER_TIMEOUT;
            }

            if (error_flag_is_set()) {
                nvm_clear_error_flag();
                rc = NVM_ERROR;
                break;
            }
            else {
                nvm_data_out_get(data);
                data ++;
                count++;
            }

          /* set the flash address to the next requested flash word address */
          nvm_set_active_nvm_address(addr+count);
        }
    }
    return rc;
}


/* Write a chunk of main block consecutive flash words */
int
nvm_write(uint32_t addr, uint32_t size, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

   if ( !nvm_addr_in_main_range(addr) ) {
        rc =  NVM_INVALID_ADDRESS;
    }
    else {
        rc = bulk_write(addr, size, data);
    }

    return rc;
}



/* Write a chunk of info block consecutive flash words */
int
nvm_info_write(uint32_t addr, uint32_t size, nvm_flash_word_t * data) {
    int rc;

    rc = NVM_SUCCESS;

   if ( !nvm_addr_in_info_range(addr) ) {
        rc =  NVM_INVALID_ADDRESS;
    }
    else {
        rc = bulk_write(addr, size, data);
    }

    return rc;
}



