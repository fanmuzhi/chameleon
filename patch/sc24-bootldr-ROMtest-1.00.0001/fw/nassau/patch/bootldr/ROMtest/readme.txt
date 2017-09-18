
    Denali Bootloader Patch Description

Patch Name
----------
    SRAMtest

Patch Version
-------------
    1

Date Created
------------
    Jul 17, 2017

Created By
----------
   Junjie Yin

Target Firmware Version(s)
--------------------------


Target Builds
-------------
    FPGA, ROM

Purpose
-------
    Patches the SRAM_TEST command into the Nassau bootloader.

Signature
---------


Release History
---------------
    + sc24-bootldr-sramtest-1.00.0001 Jul 17, 2017

Patch Pieces
------------

    bugfixes
    --------

    features
    --------


Description
-----------
The sramtest patch adds the SRAM_TEST command to the bootloader command
handler.    

Usage and Caveats
-----------------
This patch only supports the SRAM_TEST command


Revision History
----------------

+ [sc24-bootldr-flasherase-1.02.0012] initial SC24 support

+ [shasta-bootldr-flasherase-1.02.0010] Shasta production-signing support

Per BPM-696/BPM-711, the patch is released with the purpose of allowing
production signing of this patch. As a temporary solution, a check for
the security level (FIB location HWDEF_FIB_SECLEVEL) is made for
production-signed devices only, allowing the patch to run for
unprogrammed, SECLEVEL 0, and SECLEVEL 1 sensors.

+ [shasta-bootldr-flasherase-1.01.0008] initial Shasta support

Bjoren updated nvm_prog code, so I am releasing this since that
code is used here.

+ [shasta-bootldr-flasherase-1.01.0006] initial Shasta support

This is the first formal release for Shasta.

+ [hayes-bootldr-flasherase-1.01.0004] initial Hayes support

This is the first release officially for Hayes (though the
denali-bootldr-flasherase-1.00.0002 release would work alright on
Hayes).

+ [hayes-bootldr-flasherase-1.01.0004] switch to cmdmgr

Previously this patch carried with it all of the SPI code needed to
receive commands and send replies.  Unfortunately, this tied the patch
very tightly to Denali and Hayes and excluded USB use.

This release switches over to the cmdmgr, which will allow USB
operation as well.

+ [hayes-bootldr-flasherase-1.01.0004] support for SDB erasure

This release includes support for the new VCSFW_CMD_FLASH_ERASE_SDB
option (though it's not implemented on Hayes).

+ [hayes-bootldr-flasherase-1.01.0004] support erasure of individual DWORDs

This release allows the host to list a bunch of flash DWORD addresses
after the vcsfw_cmd_flash_erase_t header.  These addresses may be in
the main flash or in the FIB.

This patch will erase the sector in which the address is located, and
then rewrite the sector with all data except for the given DWORD.

+ [hayes-bootldr-flasherase-1.01.0004] support for flash partitioning

This release includes support for the new HWDEF_FIB_IOTAOFF,
HWDEF_FIB_IOTASIZE, and HWDEF_FIB_MISSIONSIZE FIB values.  These
values, when programmed, allow soft-partitioning of the flash area.

Previously this patch only supported fixed flash partitioning.
