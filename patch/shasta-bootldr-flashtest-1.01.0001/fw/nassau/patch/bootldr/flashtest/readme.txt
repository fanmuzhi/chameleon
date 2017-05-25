
    Denali Bootloader Patch Description

Patch Name
----------
    flasherase

Patch Version
-------------
    2

Date Created
------------
    May 12, 2016

Created By
----------
   Matt Waldron

Target Firmware Version(s)
--------------------------


Target Builds
-------------
    FPGA, ROM

Purpose
-------
    Patches the FLASH_ERASE command into the Nassau bootloader.

Signature
---------


Release History
---------------
    + denali-bootldr-flasherase-1.00.0002 May 13, 2016
    + hayes-bootldr-flasherase-1.01.0004 August 10, 2016
    + shasta-bootldr-flasherase-1.01.0006 January 31, 2017


Patch Pieces
------------

    bugfixes
    --------

    features
    --------
    + FLASH_ERASE command

Description
-----------
The flasherase patch adds the FLASH_ERASE command to the bootloader command
handler.  This allows the user to erase the mission firmware, iota area, and
FIB, somewhat independently.  The section to erase is specified with in the
flags word following the command byte.  The valid options are:
     Flags  |  FIB  |  Config  |  Mission
       1    |       |          |     X
       2    |       |     X    |
       3    |       |     X    |     X
       7    |   X   |     X    |     X


Usage and Caveats
-----------------
This patch only supports the FLASH_ERASE command


Revision History
----------------

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
