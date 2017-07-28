/* -*- mode: c; tab-width: 4 -*- */

/*
 * Copyright (c) 2007-2015 Synaptics Incorporated.  All rights reserved.
 */

/*
 * Access functions for the Denali internal flash memory
 *
 * Jim Paul, July 2015.
 *
 */

#ifndef __NVM_H
#define __NVM_H

#define NVM_WORD_SIZE_BYTES               8
#define NVM_PAGE_SIZE_BYTES               2048
#define NVM_WORDS_PER_PAGE                (NVM_PAGE_SIZE_BYTES \
                                          / NVM_WORD_SIZE_BYTES )
#define NVM_MAIN_ALLOCATED_PAGES          96
#define NVM_INFO_ALLOCATED_PAGES          2
#define NVM_IOTA_ALLOCATED_PAGES          32

#define NVM_MISSION_FW_ALLOCATED_PAGES    64

#define NVM_TOTAL_PAGES (NVM_MAIN_ALLOCATED_PAGES+NVM_IOTA_ALLOCATED_PAGES+NVM_INFO_ALLOCATED_PAGES)

/* Some things to keep in mind, 
 * The Flash controller has 64 bit interface
 * The controller's addressing scheme is zero based.
 * For Denali the info block was part of the FIB
 * For Hayes the Info Block is separate.
 */

/* Dimensions of NVM main area in terms of pages and addresses */
#define NVM_MAIN_PAGE_OFFSET              0
#define NVM_MAIN_FIRST_PAGE  NVM_MAIN_PAGE_OFFSET
#define NVM_MAIN_LAST_PAGE  (NVM_MAIN_PAGE_OFFSET + NVM_MAIN_ALLOCATED_PAGES -1)
#define NVM_MAIN_FIRST_ADDRESS  0
#define NVM_MAIN_LAST_ADDRESS                                           \
    (((NVM_MAIN_FIRST_PAGE + NVM_MAIN_ALLOCATED_PAGES) *NVM_WORDS_PER_PAGE) - 1)
	
/* Dimensions of NVM Info area in terms of pages and addresses */
#define NVM_INFO_PAGE_OFFSET	128     
#define NVM_INFO_FIRST_PAGE  NVM_INFO_PAGE_OFFSET
#define NVM_INFO_LAST_PAGE  (NVM_INFO_PAGE_OFFSET + NVM_MAIN_ALLOCATED_PAGES -1)
#define NVM_INFO_LAST_ADDRESS                                           \
    (((NVM_INFO_FIRST_PAGE + NVM_INFO_ALLOCATED_PAGES) *NVM_WORDS_PER_PAGE) - 1)

/*Dimensions of NVM Info Block, in terms of addresses */
#define NVM_INFO_BLOCK_FIRST_ADDRESS     0x8000

/* Denali: 
 * The 64 bit interface requires us to multiply the memory address by 8 bytes.
 * 0x8000 * 8 = 0x40000 + 0x50800000 (Flash base address) = 0x50840000
 * This maps to HWDEF_MEMMAP_SROM_FLASH_INFO_BLOCK_BASE	0x50840000
 */
#define NVM_INFO_BLOCK_LAST_ADDRESS                                     \
(NVM_INFO_BLOCK_FIRST_ADDRESS+(NVM_INFO_ALLOCATED_PAGES *NVM_WORDS_PER_PAGE) -1)


#define NVM_SUCCESS                        0
#define NVM_INVALID_ADDRESS               -1
#define NVM_INVALID_PAGE_NUM              -2
#define NVM_OPERATION_IS_INHIBITED        -3
#define NVM_ERROR                         -4
#define NVM_CONTROLLER_TIMEOUT            -5


typedef struct {
    uint32_t   lsw;
    uint32_t   msw;
} nvm_flash_word_t;

/* Enable erase and program operations */
int nvm_unlock();

/* Inhibit erase and program operations */
int nvm_lock();

/* Erase one main area flash page */
int nvm_page_erase(uint32_t page_num);

int nvm_info_page_erase(uint32_t page_num);

/* Erase all flash pages */
int nvm_mass_erase(bool_t infotoo);

/* Read one flash word from main flash area */
int nvm_word_read(uint32_t addr, nvm_flash_word_t * data);

/* Write one flash word to main flash area */
int nvm_word_write(uint32_t addr, nvm_flash_word_t * data);

/* Read one flash word from info block area */
int nvm_info_word_read(uint32_t addr, nvm_flash_word_t * data);

/* Write one flash word to info block area */
int nvm_info_word_write(uint32_t addr, nvm_flash_word_t * data);

/* Read a chunk of consecutive flash words from main flash area */
int nvm_read(uint32_t addr, uint32_t size, nvm_flash_word_t * data);

/* Read a chunk of consecutive flash words from info flash area */
int nvm_info_read(uint32_t addr, uint32_t size, nvm_flash_word_t * data);

/* Write a chunk of consecutive flash words to main flash area */
int nvm_write(uint32_t addr, uint32_t size, nvm_flash_word_t * data);

/* Write a chunk of consecutive flash words to info flash area */
int nvm_info_write(uint32_t addr, uint32_t size, nvm_flash_word_t * data);


#endif /* __NVM_H */
