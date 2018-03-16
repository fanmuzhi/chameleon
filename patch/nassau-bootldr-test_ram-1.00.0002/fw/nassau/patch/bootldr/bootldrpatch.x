/*
 * INTERNAL USE ONLY
 *
 * Copyright (c) 2010-2016 Synaptics Incorporated.  All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner unless
 * Synaptics has otherwise provided express, written permission.
 */

/*
 * The linker script for the Nassau boot loader patches
 *  (code that is designed to run from RAM).
 *
 * Bjoren Davis, June 19, 2015.
 */

#include "bootldr.h"
#include "hwdef.h"


/* For some reason, this version of the GNU linker has trouble with
 *  the ALIGN() operator.  This macro will round the value x up
 *  to make sure the low n bits are zeroes.
 *   ALIGN(x, 0) = LOW0S(x, 0)
 *   ALIGN(x, 2) = LOW0S(x, 1)
 *   ALIGN(x, 4) = LOW0S(x, 2)
 *   ALIGN(x, 8) = LOW0S(x, 3)
 *   ALIGN(x, 16) = LOW0S(x, 4)
 */
#define LOW0S(x, n)         (((x) + ((1 << (n)) - 1)) & ~((1 << (n)) - 1))

#define STACK_SIZE	(90 * 16)


MEMORY
{

    /*
     * R O M
     */


    /* The next segment is for the function pointer table. */
    srom_funcps_seg : org = BOOTLDR_MEMMAP_ROM_FUNCPS_BASE,
                          len = BOOTLDR_MEMMAP_ROM_FUNCPS_SIZE

    /*
     * RAM
     */

    /* We receive the command into the 0x6000003.  After the
     *  command byte we start with the header. */

    sram_ram_seg : org = HWDEF_MEMMAP_SRAM_RAM_BASE,
		   len = HWDEF_MEMMAP_SRAM_RAM_SIZE

    srom_ram_seg : org = HWDEF_MEMMAP_SROM_RAM_BASE,
		   len = HWDEF_MEMMAP_SROM_RAM_SIZE

}


/*  Default entry point:  */
ENTRY(patch_start);

SECTIONS
{
  /*
   * The exported functions table.
   */
  .bootldr.funcps :
  {
      bootldr_funcps = .;
  } >srom_funcps_seg

  /*
   * The to-be-encrypted header.
   */
  .bootldrpatch.enchdr (HWDEF_MEMMAP_SRAM_RAM_BASE + 4 + BOOTLDR_PATCH_PLAINHDR_SIZE) : ALIGN(4)
  {
      _bootldrpatch_enchdr_start = ABSOLUTE(.);
	/* what follows is vcsfw_cmd_bootldr_patch_enchdr_t: */
      LONG(_bootldrpatch_buildtime);/* ::buildtime */
      LONG(_bootldrpatch_loadaddr); /* ::loadaddr */
      LONG(patch_start);	    /* ::startaddr */
      LONG(_bss_start);		    /* ::bssaddr */
      LONG(_bss_end - _bss_start);  /* ::bsslen */

      _bootldrpatch_enchdr_end = ABSOLUTE(.);
      _bootldrpatch_loadaddr = ABSOLUTE(.);
  } >sram_ram_seg

  /*
   * The literals
   */
  .literal LOW0S(_bootldrpatch_enchdr_end - HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND, 2) : ALIGN(4)
  {
      _literal_start = ABSOLUTE(.);
      *(.literal .literal.*)
      _literal_end = ABSOLUTE(.);
  } >srom_ram_seg

  /*
   * Yuck.  If we don't have any literals then the "dot" doesn't move
   *  to the location past the bootldrpatch_enchdr_end.
   *  So we have to add a reference to _literal_end to 
   *  situation .text.
   */

  /*
   * The code.
   */
  .text LOW0S(_literal_end, 2) : ALIGN(4)
  {
      _text_start = ABSOLUTE(.);
      *(.text)
      _text_end = ABSOLUTE(.);
  } >srom_ram_seg

  /*
   * The readonly data.
   */
  .rodata LOW0S(_text_end + HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND, 2) : ALIGN(4)
  {
      _rodata_start = ABSOLUTE(.);
      *(.rodata)
      _rodata_end = ABSOLUTE(.);
  } >sram_ram_seg

  /*
   * The initialized data.
   */
  .data LOW0S(_rodata_end, 2) : ALIGN(4)
  {
      _data_start = ABSOLUTE(.);
      *(.data)
      _data_end = ABSOLUTE(.);
  } >sram_ram_seg

  /*
   * Payload parameters section.
   */
  .patch_payload_params _data_end : ALIGN(4)
  {
      _patch_payload_params_start = ABSOLUTE(.);
      *(.patch_payload_params)
      _patch_payload_params_end = ABSOLUTE(.);
  } >sram_ram_seg


  /*
   * The bss data.  This is last because it should be immediately
   *  above the patch.  In fact, it will overlap the signature when
   *  that's applied by the generator script.
   */
  .bss _patch_payload_params_end : ALIGN(4)
  {
      _bss_start = ABSOLUTE(.);
      *(.bss)
      _bss_end = ABSOLUTE(.);
  } >sram_ram_seg

  /*
   * The payload area.  This is a variable length space
   *  that will be filled up by flash programming payload data.
   */
  .patch_payload _bss_end : ALIGN(4)
  {
      _patch_payload_start = ABSOLUTE(.);
      /*
       * Note that we don't define _patch_payload_end.  That's
       *  because there is a variable with the size of the payload
       *  stored in it.  That variable is _patch_payload_size, and it
       *  is written to by genbootldrpatch.pl
       */
  } >sram_ram_seg

  .debug  0 :  { *(.debug) }
  .line  0 :  { *(.line) }
  .debug_srcinfo  0 :  { *(.debug_srcinfo) }
  .debug_sfnames  0 :  { *(.debug_sfnames) }
  .debug_aranges  0 :  { *(.debug_aranges) }
  .debug_pubnames  0 :  { *(.debug_pubnames) }
  .debug_info  0 :  { *(.debug_info) }
  .debug_abbrev  0 :  { *(.debug_abbrev) }
  .debug_line  0 :  { *(.debug_line) }
  .debug_frame  0 :  { *(.debug_frame) }
  .debug_str  0 :  { *(.debug_str) }
  .debug_loc  0 :  { *(.debug_loc) }
  .debug_macinfo  0 :  { *(.debug_macinfo) }
  .debug_weaknames  0 :  { *(.debug_weaknames) }
  .debug_funcnames  0 :  { *(.debug_funcnames) }
  .debug_typenames  0 :  { *(.debug_typenames) }
  .debug_varnames  0 :  { *(.debug_varnames) }
  .xt.insn 0 :
  {
    KEEP (*(.xt.insn))
    KEEP (*(.gnu.linkonce.x.*))
  }
  .xt.prop 0 :
  {
    KEEP (*(.xt.prop))
    KEEP (*(.xt.prop.*))
    KEEP (*(.gnu.linkonce.prop.*))
  }
  .xt.lit 0 :
  {
    KEEP (*(.xt.lit))
    KEEP (*(.xt.lit.*))
    KEEP (*(.gnu.linkonce.p.*))
  }
  .xt.profile_range 0 :
  {
    KEEP (*(.xt.profile_range))
    KEEP (*(.gnu.linkonce.profile_range.*))
  }
  .xt.profile_ranges 0 :
  {
    KEEP (*(.xt.profile_ranges))
    KEEP (*(.gnu.linkonce.xt.profile_ranges.*))
  }
  .xt.profile_files 0 :
  {
    KEEP (*(.xt.profile_files))
    KEEP (*(.gnu.linkonce.xt.profile_files.*))
  }
}

/*
 * If this patch is a programming patch then it
 *  cannot also have .bss data (because they'd occupy the
 *  same bit of RAM).
 * This assertion enforces that in a crude way.  If the
 *  patch put any data into .patch_payload_params (e.g., the
 *  _patch_payload_size variable) then we assume the
 *  patch is a programming patch and therefore should
 *  not have any data in .bss.
 */
ASSERT((SIZEOF(.patch_payload_params) == 0 || SIZEOF(.bss) == 0), "Programming patches cannot have .bss data.")
/*
 * Hack.  To support patches that program payloads we add
 *  the following readonly symbols.
 */
#if defined(PATCH_PAYLOAD_SIZEQUANT) 
_patch_payload_sizequant = PATCH_PAYLOAD_SIZEQUANT;
#endif
_patch_payload_sizemax = ((HWDEF_MEMMAP_SRAM_RAM_BASE + HWDEF_MEMMAP_SRAM_RAM_SIZE) - ADDR(.patch_payload) - BOOTLDR_MEMMAP_DATA_SIZE - BOOTLDR_MEMMAP_STACK_SIZE);
_patch_payload_mission_base = HWDEF_MEMMAP_SROM_FLASH_MISSION_BASE;
_patch_payload_mission_size = HWDEF_MEMMAP_SROM_FLASH_MISSION_SIZE;

/*
 * Here are some generic symbols that genbootldrpatch.pl can
 *  pick up on.
 */
#if defined(PATCH_ID)
_patch_id = PATCH_ID;
#endif
#if defined(PATCH_BUILDNUM)
_patch_buildnum = PATCH_BUILDNUM;
#endif
#if defined(PATCH_VMAJOR)
_patch_vmajor = PATCH_VMAJOR;
#endif
#if defined(PATCH_VMINOR)
_patch_vminor = PATCH_VMINOR;
#endif
