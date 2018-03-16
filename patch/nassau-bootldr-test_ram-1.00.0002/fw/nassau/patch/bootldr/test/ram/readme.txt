                        Nassau RAM Diagnostics
                              Release 1
                             Bjoren Davis
                         bdavis@synaptics.com
                            March 5, 2018

Introduction
------------

This release consists a suite of RAM patches designed to test the RAMs
in a Denali (b1210), Hayes (b1212), Shasta (b1216) or SC24 (b2224)
part.

Usage
-----

Each patch is standalone (to minimize the possibility that a RAM
problem would prevent the running of a patch), self running, and
self-unloading.

    RAM Testing Patches (test_ram_dcd, test_ram_dct, test_ram_dev,
                         test_ram_icd, test_ram_ict, test_ram_sys)
    --------------------------------------------------------

All RAM-testing patches return a 2 byte status of VCSFW_STATUS_OKAY,
followed by an optional 12 additional bytes (only if an error is found):
    failptr  - 4 bytes of pointer indicating the location of the
               failure.  If this value is 0 then the test completed
               without finding a failure.
    expected - 4 bytes indicating the value that was expected at the
               given address.
    actual   - 4 bytes indicating the actual value read from the given
               address.

Each test concludes after finding its first failure.

It is recommended to run the tests in the following order:
    1) test_ram_ict
    2) test_ram_icd
    3) test_ram_dct
    4) test_ram_dcd
    5) test_ram_sys
    6) test_ram_dev

The reason for this order is the it roughly follows dependencies.  No
patch can run without a functional instruction cache.  The data cache
is important to patch operation, but a little less so (it's only
needed for literals).  It's vital to have known-functional instruction
and data caches before testing the system RAM (because the system RAM
test runs from the instruction and data caches), and finally it's
important to know the caches and main RAMs are working before
attempting to test the device RAMs.

Here is an example of a fully successful run of all 5 tests:

    $ ~/denali/tools/dactyl/dactyl.exe  -o bigep0,ep1crc,vio=3300,vdd=3300 spi writeep3 nomission eng_jtag \; spi reset \; getver                                   spimpc04_devenum: failure putting together device list.
    No WinUSB devices found.
    FTDI device serial number is FTA4EZ65A (m5)
    spiftdi_devconf: vdd=3300, vio=3300, vled=0, vtx=3300, speed=6000 kHz, is M5
    writing value 010000_00010000 to DFT word
    Resetting device using 0xf0 endpoint select byte.
    Reset select byte successfully sent.

    VCSFW_CMD_GET_VERSION:
    buildtime  = 11-Apr-2017 08:36:16 PM ric
    buildnum   = 36
    vmajor     = 1
    vminor     = 5
    target     = 1 (ROM)
    product    = 64 (SC24PBL)
    siliconrev = 0
    formalrel  = 1
    platform   = 0 0x00
    patch      = 1
    serial_number = 0x16 b0 12 66 18 aa
    security[2]= 0x00 00
    patchsig   = 0x00000000
                 phase:
                 iface:
              security:
    iface      = 3 (SPI)
    otpsig[3]  = 0x00 00 00
    otpspare1  = 0x0000
    reserved   = 0x00
    device_type= 0x00
    $ ~/denali/tools/dactyl/dactyl.exe  -o bigep0,ep1crc,vio=3300,vdd=3300 send -b 125 ict/obj/sc24-rom/test_ram_ict.blpbin \; send -b 125 icd/obj/sc24-rom/test_ram_icd.blpbin \; send -b 125 dct/obj/sc24-rom/test_ram_dct.blpbin \; send -b 125 dcd/obj/sc24-rom/test_ram_dcd.blpbin \; send -b 125 sys/obj/sc24-rom/test_ram_sys.blpbin \; send -b 125 dev/obj/sc24-rom/test_ram_dev.blpbin
    spimpc04_devenum: failure putting together device list.
    No WinUSB devices found.
    FTDI device serial number is FTA4EZ65A (m5)
    spiftdi_devconf: vdd=3300, vio=3300, vled=0, vtx=3300, speed=6000 kHz, is M5
    send: sending command: cmd=125, length 325
    send: got 2 (0x2) bytes of reply data:
    0000   00 00
    send: sending command: cmd=125, length 320
    send: got 2 (0x2) bytes of reply data:
    0000   00 00
    send: sending command: cmd=125, length 437
    send: got 2 (0x2) bytes of reply data:
    0000   00 00
    send: sending command: cmd=125, length 461
    send: got 2 (0x2) bytes of reply data:
    0000   00 00
    send: sending command: cmd=125, length 641
    send: got 2 (0x2) bytes of reply data:
    0000   00 00
    send: sending command: cmd=125, length 365
    send: got 2 (0x2) bytes of reply data:
    0000   00 00

The tests include:

        test_ram_dcd
        ------------

Tests the data cache RAM (4096 bytes).  Works by deactivating the data
cache, reprogramming the cache in the [2000_0000, 3fff_ffff] region to
be in "isolate[d]" mode, and testing the RAM in the low 4096 bytes of the
mapped space.

When it's completed it restores the data cache to normal operation.

Does nothing to preserve the contents.

Failure addresses will be of the form 2000_0xxx.

        test_ram_dct
        ------------

Tests the data cache tag RAM (128 words of 25 bits each -- mask is
0xfffff80f).

Works by deactivating the data cache, and then using SDCT and LDCT
instructions to write and read (respectively) the data cache tag RAM.

When it's completed it restores the data cache to normal operation.

Does nothing to preserve the contents.

Failures addresses will be of the form d000_0xx0.

        test_ram_icd
        ------------

Tests the instruction cache data RAM (8192 bytes).

Works by deactivating the instruction cache, and then using the SICW
and the LICW instructions to write and read (respectively) the
instruction cache data RAM.

When it's completed it restores the instruction cache to normal
operation.

Does nothing to preserve the contents.

Failures addresses will be of the form e000_xxxx.


        test_ram_ict
        ------------

Tests the instruction cache tag RAM (256 words of 21 bits each -- mask
is 0xfffff007).

Works by deactivating the instruction cache, and then using the SICT
and the LICT instructions to write and read (respectively) the
instruction cache data RAM.

When it's completed it restores the instruction cache to normal
operation.

Does nothing to preserve the contents.

Failures addresses will be of the form f000_xxx0.

        test_ram_dev
        ------------

Tests the peripheral RAMs of the device.

These memories are (base pointer and length, in hexadecimal):

                Denali      Hayes       Shasta      SC24
                (b1210)     (b1212)     (b1216)     (b2224)
---------------+-----------+-----------+-----------+-----------+
FW_BL_RAM      | 8008_0000 | 8008_0000 | 8008_0000 | 8008_4000 |
               |      4000 |      4000 |      4000 |      4000 |
---------------+-----------+-----------+-----------+-----------+
IMGRAM         | 8008_4000 | 8008_4000 | 800a_0000 | 8008_4000 |
               |      4000 |      4000 |      4000 |      4000 |
---------------+-----------+-----------+-----------+-----------+
LNA_BL_RAM     | 8008_8000 | 8008_8000 | 8008_8000 | 8008_8000 |
               |      4000 |      4000 |      4000 |      4000 |
---------------+-----------+-----------+-----------+-----------+
DMRAM          | 8008_c000 | 8008_c000 | 8009_8000 | 8008_c000 |
               |      1800 |      1800 |      1000 |      1800 |
---------------+-----------+-----------+-----------+-----------+
CDMRAM         | 8008_e000 | 8008_e000 | 8009_a000 | 8008_e000 |
               |      1000 |      1000 |       800 |      1000 |
---------------+-----------+-----------+-----------+-----------+
ADCRAM0        | N/A       | N/A       | 8009_0000 | N/A       |
               |           |           |      1000 |           |
---------------+-----------+-----------+-----------+-----------+
ADCRAM1        | 8008_f000 | 8008_f000 | 8009_2000 | 8008_f000 |
               |      1000 |      1000 |      1000 |      1000 |
---------------+-----------+-----------+-----------+-----------+
ADCRAM2        | 8009_0000 | 8009_0000 | 8009_4000 | 8009_0000 |
               |      1000 |      1000 |      1000 |      1000 |
---------------+-----------+-----------+-----------+-----------+
ADCRAM3        | N/A       | N/A       | 8009_6000 | N/A       |
               |           |           |      1000 |           |
---------------+-----------+-----------+-----------+-----------+
PUF_RAM        | N/A       | 8009_1000 | N/A       | 8009_1000 |
               |           |        80 |           |        80 |
---------------+-----------+-----------+-----------+-----------+
SCRATCH_RAM    | 8009_4000 | N/A       | 8009_b000 | N/A       |
               |      1000 |           |      1000 |           |
---------------+-----------+-----------+-----------+-----------+

        test_ram_sys
        ------------

This tests the entire system RAM.

It works by setting up the instruction cache to hold the patch and its
literals and setting up the data cache in "isolate[d]" mode to hold
the parts stack.  It then does destructive testing on the entire
system RAM at once.

                Denali      Hayes       Shasta      SC24
                (b1210)     (b1212)     (b1216)     (b2224)
---------------+-----------+-----------+-----------+-----------+
SRAM_RAM       | 6000_0000 | 6000_0000 | 6000_0000 | 6000_0000 |
               |    1_0000 |    1_0000 |    1_0000 |    1_0000 |
---------------+-----------+-----------+-----------+-----------+

    The Basic Algorithm (ramtest_a1)
    --------------------------------

The algorithm used to test each RAM is derived from the BIST RAM test
used on Falcon/Raptor.

Here is the original algorithm (described in an email from Greg Dean
"RE: Calibration Failure and Rescreening", Wed 10/2/2013 4:12 PM EDT):

    I ran the Raptor Memory BIST simulation and confirmed that it
    follows the same basic algorithm as Falcon, which I sent earlier.

    Here's the actual algorithm for the main memory.  The algorithm
    for the USB and SCM memories is equivalent.

    for each word (starting at addr=0)
    write to all 0 (2 times)

    for each word (starting at addr=0)
    read all 0 (3 times)
    write to all 1 (2 times)
    read all 1 (3 times)

    for each word (starting at addr=0)
    read all 1 (3 times)
    write to all 0 (2 times)
    read all 0 (3 times)

    for each word (starting at addr=12k)
    read all 0 (3 times)
    write to all 1 (2 times)
    read all 1 (3 times)

    for each word (starting at addr=12k)
    read all 1 (3 times)
    write to all 0 (2 times)
    read all 0 (3 times)

    for each word (starting at addr=12k)
    read all 0 (3 times)

    The address sequences are slightly swizzled.  Instead
    0,1,2,3,etc., it uses 0,1,3,2,etc.


For our purposes, we don't bother with the "address swizzling"
described at the bottom, and the high address depends on the RAM being
tested.

Release History
---------------

    + nassau-bootldr-test_ram-1.00.0002.  March 5, 2018.

Release Notes
---------------

+ [ nassau-bootldr-test_ram-1.00.0002 ] initial release

This initial release provides only the ramtest_a1 algorithm and builds
for Denali (b1210), Hayes (b1212), Shasta (b1216), and the Steller
Controller SC24 (b2224).

