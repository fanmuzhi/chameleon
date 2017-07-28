#!/usr/bin/perl
#
# Script to generate Nassau boot loader-compatible patches.
#
# Bjoren Davis, June 20, 2015.
#  Derived from the Eagle version
#   engineering/private/impl/micro/eagle/tools/genpatch.pl,v 1.8 2012/03/12 20:17:54 bdavis Exp $
#   Bjoren Davis, April 28, 2011.
#  Adapted for Savoy from engineering/private/impl/micro/windsor/patch/tools/genpatch.pl,v 1.24 2014/12/04 22:49:43 bdavis
#   by Bjoren Davis on February 22, 2015.
#
#  Version 3: Supports ELF file splitting to multiple patches, Firmware Flavor Selector.
#        3.1: detects signing security notes in ELF files
#        3.2: Adds iota payload support.
#        3.3: Writes to the correct directory, if a directory is specified.

use strict;
use warnings;
use Carp;
use Data::Dumper;
use List::Util qw(first);
use POSIX qw(strftime);
use File::Temp qw(tempfile tempdir);
use Sys::Hostname;
use File::Basename;

my $indquant = 4;
my $elf_file;
my $opt_payload_id = 0;
my $opt_id;
my $opt_payload_elf;
my $opt_bn;
my $opt_vmajor;
my $opt_vminor;
my $opt_custkey;
my $out_file;
my $opt_genc;
my $opt_genhdr;
my $opt_verbose;
my $opt_payload_ilist;
my $opt_iotaforce;
my $fwflav;
my $vpskfile;
my $sigfile;
my $keyfile;
my $nopad;
my %symtab;
my %symsbyname;
my %pl_symtab;
my %pl_symsbyname;
my $plainhdrtype = 0;
my $payloadmax;

#!!! is there a better way to automatically identify this thing:
my $scriptver = "genbootldrpatch.pl version 3.3";

#######################################
### CONSTANTS
#######################################

#
# The following values are from biometrics/b1210/common/include/b1210hwdef.h
# (Denali); they are no different for B1212 (Hayes):
#
my $HWDEF_MEMMAP_RAM_SIZE = 0x00010000;

# RAM mapped into read/execute space
my $HWDEF_MEMMAP_SROM_RAM_BASE = 0x507f0000;
my $HWDEF_MEMMAP_SROM_RAM_SIZE = $HWDEF_MEMMAP_RAM_SIZE;

# RAM mapped into read/write space
my $HWDEF_MEMMAP_SRAM_RAM_BASE = 0x60000000;
my $HWDEF_MEMMAP_SRAM_RAM_SIZE = $HWDEF_MEMMAP_RAM_SIZE;


# The flash memory
#  (192 KB = 0x3_0000 bytes)
my $HWDEF_MEMMAP_FLASH_SIZE = 0x00030000;

# Flash mapped into System ROM space
my $HWDEF_MEMMAP_SROM_FLASH_BASE = 0x50800000;
my $HWDEF_MEMMAP_SROM_FLASH_SIZE = $HWDEF_MEMMAP_FLASH_SIZE;

# The size of a flash page (2 KB = 0x800)
my $HWDEF_MEMMAP_FLASH_PAGE_SIZE = 0x00000800;

# RAM: from a SROM (read/executed) address to SRAM (read/write) address
#      E.g. 507f_xxxx -> 6000_xxxx for Denali
my $HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND = 0x0f810000;

# From bootldr.h:

my $BOOTLDR_AESCBC_BLOCK_SIZE = 16;  # in bytes
my $BOOTLDR_RSA_KEYLEN_2048 = 256;  # in bytes
my $BOOTLDR_AESCBC_KEYLEN_256 = 32;   # in bytes

my $IOTALIST_MAGIC = 0x12345678;   # Magic value for iota list identification
my $IOTALIST_MAGIC_SIZE = 4;   # number of bytes
my $VCSFW_IOTA_ITYPE_CONFIG_VERSION = 9; # iota list version identification
my $FLASHPROG_IOTA_VERSION_NOCHECK = 0xffff; # Do not check version/configid

#######################################
### PRIMITIVE TYPES
#######################################

my @uint8_primtype = (
    'vcsUint8_t', 1,
    sub { return unpack('C', $_[1]) },
    sub { return sprintf('0x%02x', $_[1]) },
    sub { return pack('C', $_[1]) }
);

my @sint8_primtype = (
    'vcsInt8_t', 1,
    sub { return unpack('c', $_[1]) },
    sub { return sprintf('%d', $_[1]) },
    sub { return pack('c', $_[1]) },
);

my @uint16_primtype = (
    'vcsUint16_t', 2,
    sub { return unpack('S', $_[1]) },
    sub { return sprintf('0x%04x', $_[1]) },
    sub { return pack('S', $_[1]) }
);

my @sint16_primtype = (
    'vcsInt16_t', 2,
    sub { return unpack('s', $_[1]) },
    sub { return sprintf('%d', $_[1]) },
    sub { return pack('s', $_[1]) }
);

my @uint32_primtype = (
    'vcsUint32_t', 4,
    sub { return unpack('L', $_[1]) },
    sub { return sprintf('0x%08x', $_[1]) },
    sub { return pack('L', $_[1]) }
);

my @sint32_primtype = (
    'vcsInt32_t', 4,
    sub { return unpack('l', $_[1]) },
    sub { return sprintf('%d', $_[1]) },
    sub { return pack('l', $_[1]) }
);

# A type is a list reference.
#  List element [0] is the type name (can be 'undef')
#  If list element [1] is a list reference, then the type is
#  a compound type -- elements [1] through [-1] comprise
#  the members of the compound type.
#  If it is a compound type each list reference in the list
#  looks like:
#    [0] member name
#    [1] reference to member type
#    [2] formatter.  Either a 'printf' format, or a reference to
#        hash (for an enumerated type), a function reference, or
#        'undef' (meaning to use the type's formatter).
#        If it's a function reference then the arguments to the
#        function are:
#        [0] reference to this member list
#        [1] results from getter.  Can be a single scalar,
#            a list reference (for an array), or a hash reference
#            (for a compound type)
#    [3] count.  For arrays.  If undef or not present, assume 1.
#  If list element [1] is not a list reference then it's a
#  primitive type, and the elements are as follows:
#  [1] type size (in bytes)
#  [2] getter
#      arguments:
#             [0] reference to this type
#             [1] binary data
#      returns:
#           a scalar, or a list reference (for an array), or
#           a hash reference (for a compound type)
#  [3] formatter
#      arguments:
#        [0] reference to this type
#        [1] results from getter.  Can be a single scalar,
#            a list reference (for an array), or a hash reference
#            (for a compound type)
#  [4] setter
#      arguments:
#        [0] reference to this type
#        [1] results from getter.  Can be a single scalar,
#            a list reference (for an array), or a hash reference
#            (for a compound type)
#      returns:
#           a binary string
#

sub bool_format {
    return $_[1] ? 'TRUE' : 'FALSE';
}

sub addr_format {
    my ($val) = ($_[1]);
    my $name = lookup($val, \%symtab);
    my $retstr = sprintf('0x%08x', $val);
    if (defined $name) {
        $retstr .= " ($name)";
    }
    return $retstr;
}

#
# Format up a UNIX-style date/time.
#

sub datetime_format {
    my @lt = localtime($_[1]);
    return sprintf('%s (0x%08x)',
                   strftime('%a, %d %b %Y  %H:%M:%S %z (%Z)', @lt), $_[1]);
}

#######################################
# PLAINTEXT HEADER
#######################################

#
# This hash describes the plaintext header
#  vcsfw_cmd_bootldr_patch_plainhdr_t:
#
my $VCSFW_PRODUCT_DENALI        = 53; # Denali aka b1210 (Nassau f/w)
my $VCSFW_PRODUCT_DENALIPBL     = 54; # Denali primary boot loader
my $VCSFW_PRODUCT_HAYES         = 56; # Hayes  aka b1212 (Nassau f/w)
my $VCSFW_PRODUCT_HAYESPBL      = 57; # Hayes  primary boot loader
my $VCSFW_PRODUCT_HAYESCP       = 58; # Hayes Crystal Peak mission

my %product_by_num = (
    $VCSFW_PRODUCT_DENALI => 'DENALI',
    $VCSFW_PRODUCT_DENALIPBL => 'DENALIPBL',
    $VCSFW_PRODUCT_HAYES => 'HAYES',
    $VCSFW_PRODUCT_HAYESPBL => 'HAYESPBL',
    $VCSFW_PRODUCT_HAYESCP => 'HAYESCP'
);


# values for vcsfw_reply_get_version_t::target
my $VCSFW_TARGET_ROM = 1;
my $VCSFW_TARGET_FPGA = 2;
my $VCSFW_TARGET_RTLSIM = 3;
# VCSFW_TARGET_ISS, below, is deprecated
my $VCSFW_TARGET_ISS = 4;
my $VCSFW_TARGET_FPGADBG = 5;

my %target_by_num = (
    $VCSFW_TARGET_ROM => 'ROM',
    $VCSFW_TARGET_FPGA => 'FPGA',
    $VCSFW_TARGET_RTLSIM => 'RTLSIM',
    $VCSFW_TARGET_ISS => 'ISS',
    $VCSFW_TARGET_FPGADBG => 'FPGADBG',
);

# vcsfw_cmd_bootldr_patch_plainhdr_t::flags:
# payload encrypted:
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_ENCRYPTED = 0x00000001;
# signature follows:
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_SIGNED = 0x00000002;
# patch is encrypted/signed using production keys:
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PROD = 0x00000004;
# patch contains an NVM (flash) programming payload (payload_* are valid )
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PAYLOAD = 0x00000008;
# patch is encrypted/signed using customer-injected keys:
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_CUSTKEY = 0x00000010;
# what type of plainhdr we have:
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PLAINHDRTYPE1 = 0x10000000;
my $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PLAINHDRTYPE2 = 0x20000000;

# The plainhdr size is defined in bootldr.h
my $BOOTLDR_PATCH_PLAINHDR_SIZE = 36;
my $BOOTLDR_PATCH_SHA256_SIZE = 32;

#
# These values are from fw/nassau/patch/bootldr/flashprog/patch_start.c
#
# flash data to be programmed at 'progaddr'
my $FLASHPROG_HDR_TYPE_FLASH = 0x00;
# mission hash.  prograddr ignored.  if correct will set MISSIONVALID.
my $FLASHPROG_HDR_TYPE_MISSIONHASH = 0x01;
# iota. progaddr ignored. added to end of iota chain
my $FLASHPROG_HDR_TYPE_IOTA = 0x02;
# iota list. progaddr ignored. added to end of iota chain
my $FLASHPROG_HDR_TYPE_IOTALIST = 0x03;

# the flashprog_hdr_t structure
my @flashprog_hdr_htype = (
    'flashprog_hdr_t',
    [ 'progaddr', \@uint32_primtype ],
    [ 'nwords', \@uint16_primtype ],
    [ 'type' , \@uint8_primtype,
      {
          $FLASHPROG_HDR_TYPE_FLASH => 'FLASH',
          $FLASHPROG_HDR_TYPE_MISSIONHASH => 'MISSIONHASH',
          $FLASHPROG_HDR_TYPE_IOTA => 'IOTA'
      } ],
    [ 'flags', \@uint8_primtype ]
    );

#
# The version readonly information.
# This structure is defined in fw/nassau/mission/version.h
#
my @version_roinfo_htype = (
    'version_roinfo_t',
    [ 'buildtime', \@uint32_primtype, \&datetime_format ],
    [ 'buildnum', \@uint32_primtype, '%04u' ],
    [ 'vmajor', \@uint8_primtype, '%u' ],
    [ 'vminor', \@uint8_primtype, '%u' ],
    [ 'target', \@uint8_primtype, \%target_by_num ],
    [ 'product', \@uint8_primtype, \%product_by_num ],
    [ 'siliconrev', \@uint8_primtype ],
    [ 'formalrel', \@uint8_primtype, \&bool_format ]
);

# [ pack/unpack letter, number of bytes, printf format ]
my @cmd_bootldr_patch_plainhdr_htype0 = (
    'vcsfw_cmd_bootldr_patch_plainhdr_t',
    [ 'flags', \@uint32_primtype ],
    [ 'id', \@uint32_primtype ],
    [ 'buildnum', \@uint32_primtype, '%u' ],
    [ 'payload_id', \@uint32_primtype ],
    [ 'payload_buildtime', \@uint32_primtype, \&datetime_format ],
    [ 'payload_buildnum', \@uint32_primtype ],
    [ 'payload_vmajor', \@uint8_primtype, '%u' ],
    [ 'payload_vminor', \@uint8_primtype, '%u' ],
    [ 'payload_target', \@uint8_primtype, \%target_by_num ],
    [ 'payload_product', \@uint8_primtype, \%product_by_num ],
    [ 'payload_siliconrev', \@uint8_primtype ],
    [ 'payload_formalrel', \@uint8_primtype, \&bool_format ],
    [ 'seq', \@uint8_primtype, '%u' ],
    [ 'vmajor', \@uint8_primtype, '%u' ],
    [ 'vminor', \@uint8_primtype, '%u' ],
    [ 'unused', \@uint8_primtype, undef, 3 ]
  );

# [ pack/unpack letter, number of bytes, printf format ]
my @cmd_bootldr_patch_plainhdr_htype1 = (
    'vcsfw_cmd_bootldr_patch_plainhdr_t',
    [ 'flags', \@uint32_primtype ],
    [ 'id', \@uint32_primtype ],
    [ 'buildnum', \@uint32_primtype, '%u' ],
    [ 'restrict_fwflavor', \@uint32_primtype ],
    [ 'payload_buildtime', \@uint32_primtype, \&datetime_format ],
    [ 'payload_buildnum', \@uint32_primtype ],
    [ 'payload_vmajor', \@uint8_primtype, '%u' ],
    [ 'payload_vminor', \@uint8_primtype, '%u' ],
    [ 'restrict_seclevel', \@uint8_primtype ],
    [ 'padding', \@uint8_primtype, undef, 3 ],
    [ 'seq', \@uint8_primtype, '%u' ],
    [ 'vmajor', \@uint8_primtype, '%u' ],
    [ 'vminor', \@uint8_primtype, '%u' ],
    [ 'unused', \@uint8_primtype, undef, 3 ]
  );

# [ pack/unpack letter, number of bytes, printf format ]
my @cmd_bootldr_patch_plainhdr_htype2 = (
    'vcsfw_cmd_bootldr_patch_plainhdr_t',
    [ 'flags', \@uint32_primtype ],
    [ 'id', \@uint32_primtype ],
    [ 'buildnum', \@uint32_primtype, '%u' ],
    [ 'restrict_configid1', \@uint32_primtype ],
    [ 'restrict_configid2', \@uint32_primtype ],
    [ 'restrict_configver', \@uint16_primtype ],
    [ 'padding', \@uint8_primtype, undef, 8 ],
    [ 'seq', \@uint8_primtype, '%u' ],
    [ 'vmajor', \@uint8_primtype, '%u' ],
    [ 'vminor', \@uint8_primtype, '%u' ],
    [ 'unused', \@uint8_primtype, undef, 3 ]
  );

my @cmd_bootldr_patch_enchdr_htype = (
    'vcsfw_cmd_bootldr_patch_enchdr_t',
    [ 'buildtime', \@uint32_primtype, \&datetime_format ],
    [ 'loadaddr', \@uint32_primtype, \&addr_format],
    [ 'startaddr', \@uint32_primtype, \&addr_format ],
    [ 'bssaddr', \@uint32_primtype, \&addr_format ],
    [ 'bsslen', \@uint32_primtype ]
);

my @flashprog_ilist_htype = (
    'flashprog_ilist_t',
    [ 'length', \@uint16_primtype ],
    [ 'itype', \@uint16_primtype ],
    [ 'version', \@uint16_primtype ],
    [ 'unused', \@uint8_primtype, undef, 2 ]
);

my @config_version_htype = (
    'vcsfw_config_version_t',
    [ 'config_id1', \@uint32_primtype ],
    [ 'config_id2', \@uint32_primtype ],
    [ 'version', \@uint16_primtype ],
    [ 'unused', \@uint16_primtype, undef, 3 ]
);

#
# This bit of cryptic magic adds the path of this script
#  to the INC list.
#
BEGIN {
    use File::Spec::Functions qw(rel2abs);
    use File::Basename qw(dirname);
    use lib dirname(rel2abs($0));
}

# $Data::Dumper::Terse=1;
$Data::Dumper::Useqq=1;

my $scriptdir = $0;
if ($scriptdir =~ s=/[^/]*$==) {
    push @INC, $scriptdir . '/../../../../../tools';
}

require 'elfio.pm';
#use 'elfio.pm';

#
# Return a numerically sorted list of keys for a hash

#
sub sortkeys {
    my ($hashref) = ($_[0]);

    return sort { $$hashref{$a} <=> $$hashref{$b} } keys %$hashref;
}


sub usage {
    my $cmdname = $0;
    $cmdname =~ s=.*/==;
    print STDERR << "EOF";
usage: $cmdname [-v] [-pi payload-id] [-id id] [-pf payload-elf]
                [-vv vmajor.vminor]
                [-bn buildnum]
                [-xpsk validity-private-key-pem-file]
                [-xsig signature-file]
                [-xkey aeskey-file]
                [-genc]
                [-genhdr]
                [-custkey]
                [-phtype plain-header-type]
                [-fwflav firmware-flavor]
                [-pil iota-list]
                [-iforce]
                elf-file -o output-file
  where:
         -v    be verbose with messages
         -pi   identify the payload (informational only -- used
               in the plaintext header).  Default is 0.
         -id   identify this patch (informational only -- used
               in the plaintext header).  Default is 0.
         -pf   extract the payload from the given ELF file.
         -vv   specify the major and minor version numbers of this
               patch.  Default is 0.0
         -bn   specify the build number of this patch.  Default is 0.
         -xpsk provides a path for a PEM file which contains the
               Validity Private Signing Key (VPSK).  This will cause
               the patch to be encrypted and signed.
         -xsig provides a path to a file containing a signature.  This
               option cannot be specified with -xpsk.
         -xkey provides a path for a binary file which contains the
               initial key and IV for the patch encryption.  If
               this argument does not appear, and -xpsk does, then
               the patch will be encrypted with the engineering
               key.
         -genc cause the script to output a C-includable data
            file containing the patch (as an array of 32-bit words).
         -genhdr cause the script to output a nice ASCII header
         -custkey
               Indicate that the keys are customer-injected keys.
         -phtype
               Indicate plain header type. Default is 0.
               Reference: vcsfw_cmd_bootldr_patch_plainhdr_type*_t
               defintions in vcsfw_v4.h.
               Use 1 for mission firmware payload.
               Use 2 for iota payload.
         -fwflav
               Indicate the payload's firmware flavor. Required for
               vcsfw_cmd_bootldr_patch_plainhdr_type1_t in vcsfw_v4.h.
         -pil
               Indicate the file containing the iota list payload. Required for
               vcsfw_cmd_bootldr_patch_plainhdr_type2_t in vcsfw_v4.h.
         -iforce
               Allows forcing of iota list writing, regardless of config id
               and version checks! Intended for desktop experimentation only,
               NOT in the field.

         output-file  For multiple output files, this is the template
                      for the file. A sequence number is inserted
                      before the file suffix.
                      For a single output file, this is the filename.
         elf-file     the ELF file containing the patch
         payload-elf  an ELF file containing payload firmware being
                      programmed by this patch.  This is used
                      to extract the version ID information.
         sequence-val small integer specifying where this patch
                      appears in a sequence of patches (useful for
                      creating a series of flash-programming patches)
         vmajor.vminor patch version major and minor numbers
         buildnum     patch build number
         plain-header-type integer version of plain header to choose
                      the format of the plain header defined in
                      vcsfw_v4.h.
         iota-list    File containing a list of iotas in the
                      format used for IotaTool and BioVision.
EOF
    croak;
}

#
# Given an ELF file, create a symbol table hash (addr => name)
#
sub gensymtab {
    my ($elfref) = ($_[0]);
    my %rethash;
    my %rethashbyname;

    foreach my $secref ( grep { ($$_{'sh_type'} == elfio::SHT_SYMTAB() ) }
                         @{$$elfref->{'secs'}} ) {

        foreach my $entryref (@{$secref->{'entries'}}) {
            if (exists $entryref->{'name'}) {
                # Priority of names: begins with '.', with '_', without either
                my $val = $entryref->{'st_value'};
                my $name = $entryref->{'name'};
                $rethashbyname{$name} = $val;
                if (exists $rethash{$val}) {
                    my $oldfirst = $rethash{$val};
                    my $newfirst = $name;

                    for my $ref (\$oldfirst, \$newfirst) {
                        if ($$ref =~ /^\..*/) {
                            $$ref = 0;
                        }
                        elsif ($$ref =~ /^_.*/) {
                            $$ref = 1;
                        }
                        else {
                            $$ref = 2;
                        }
                    }
                    if ($newfirst > $oldfirst) {
                        $rethash{$val} = $name;
                    }
                }
                else {
                    $rethash{$val} = $name;
                }
            }
        }
    }

    return (\%rethash, \%rethashbyname );
}

#
# Given an address and a symbol table reference,
#  find out the string that describes the address.
#

sub lookup {
    my ($addr, $symtabref) =($_[0], $_[1]);
    my $key;
    my $name;

#    print "lookup $addr\n";
    # Is this an exact match?
    if (defined ($name = $symtabref->{$addr})) {
        return $name;
    }

#    $key = first { $_ <= $addr } @sortedsymvals;
    # Do a binary search for the first address <= the desired address.
    my @sortedsymvals = sort { $b <=> $a } keys %{$symtabref};
    my $srchlo = 0;
    my $srchhi = scalar(@sortedsymvals)-1;
    my $newidx;

    if ($addr >= $sortedsymvals[$srchhi]) {
        while (($srchhi - $srchlo) > 1) {
            $newidx = int(($srchlo + $srchhi)/2);
            my $tstval = $sortedsymvals[$newidx];
            if ($tstval < $addr) {
                $srchhi = $newidx;
            }
            elsif ($tstval > $addr) {
                $srchlo = $newidx;
            }
            else {
                # Shouldn't happen
                die "Shouldn't happen.\n";
            }
        }
#       printf("done: addr = 0x%08x, srchlo = %u (0x%08x), srchhi = %u (0x%08x)\n",
#              $addr, $srchlo, $sortedsymvals[$srchlo], $srchhi, $sortedsymvals[$srchhi]);

        $key = $sortedsymvals[$srchhi];

        my $delta=$addr-$key;
        # assert $delta > 0
        $name="$symtabref->{$key}+0x" . (sprintf "%x", $delta);
    }

    return $name;
}

#
# Given a string of binary data, return
#  a decoded value and the length (in bytes) of
#  data that uses that type.

# htype format is:
#   [0] = type name
#   [1] = reference to array describing first member if compound type
#         -- or --
#         scalar integer indicating size (in bytes) if primitive type
#   [2] = reference to subsequent member if compound type
#         -- or --
#         reference to unpacking function (args: [0] = reference to type,
#                                                [1] = binary data (string)
#   [3] = reference to subsequent member if compound type
#         -- or --
#         reference to formatting function (args: [0] = reference to type,
#                                                 [1] = decoded datum)
#   [4...$] = references to subsequent members if compound type
#
# For compound types, each member looks like:
#   [0] = member name (string)
#   [1] = member type reference
#   [2] = printf format (string)
#         -- or --
#         reference to formatting function (args: [0] = member reference,
#                                                 [1] = decoded datum)
#         -- or --
#         reference to hash of datum->name values (enums)
#   [3] = array count
#
#
#
sub htype_unpack {
    my ($data, $htyperef) = ($_[0], $_[1]);
    my @retval;

    #
    # Is this a compound type?
    #
#    print "htype_unpack $htyperef->[0] [1] type is " . ref($htyperef->[1]) . "\n";
    if (ref($htyperef->[1])) {
        my $offset = 0;
        #
        # Yes.  Go through each member and decode them.
        #
        my %replyhash;
        foreach my $membidx (1 .. (@{$htyperef}-1)) {
            my $memberref = $htyperef->[$membidx];
#           print "memberref $memberref->[0]\n";
            #
            # Is this an array of elements, or is it just one?
            #
            if (defined $memberref->[3] && $memberref->[3] > 1) {
                my @arrayvals;

                foreach my $arrayidx (0 .. ($memberref->[3]-1)) {
                    if (length($data) <= $offset) {
                        $Carp::Verbose = 1;
                        croak "htyperef $htyperef->[0] memberreff $memberref->[0] arrayidx $arrayidx";
                    }
                    my ($membval, $membsize) =
                        htype_unpack(substr($data, $offset), $memberref->[1]);
                    $offset += $membsize;
                    push @arrayvals, $membval;
                }
                $replyhash{$memberref->[0]} = \@arrayvals;
            }
            else {
                my ($membval, $membsize) =
                    htype_unpack(substr($data, $offset), $memberref->[1]);
                $offset += $membsize;
                $replyhash{$memberref->[0]} = $membval;
            }
        }
        @retval = (\%replyhash, $offset);
    }
    else {
        # It's a primitive type.
        my $coderef = $htyperef->[2];
        my $datum = &$coderef($htyperef, $data);
        @retval = ($datum, $htyperef->[1]);
    }
    return @retval;
}

#
# Figure out the size of a hierarchical type
#
# arg1 = type reference
#

sub htype_size {
    my ($htyperef) = ($_[0]);
    my $offset;

    $offset = 0;

    #
    # Is this a compound type?
    #
    if (ref($htyperef->[1])) {
        #
        # Yes.  Go through each member and sum them up
        #
        foreach my $membidx (1 .. (@{$htyperef}-1)) {
            my $memberref = $htyperef->[$membidx];
            my $membsize = htype_size($memberref->[1]);

            #
            # Is this an array of elements, or is it just one?
            #
            if (defined $memberref->[3] && $memberref->[3] > 1) {
                $offset += ($memberref->[3] * $membsize);
            }
            else {
                $offset += $membsize;
            }
        }
    }
    else {
        # It's a primitive type.
        $offset += $htyperef->[1];
    }
    return $offset;
}

#
# Given a structure and a decoded instance of that structure,
#  format up the elements in a nice neat string.
#
sub htype_format {
    my ($datum, $htyperef) = ($_[0], $_[1]);
    my $retstr = '';

    #
    # Is this a compound type?
    #
#    print "formatting $htyperef->[0]\n";
    if (ref($htyperef->[1])) {
#       print "  it's a compound type.\n";
        #
        # Yes.  Go through each member and format them individually
        #

        # First, go through them and find the longest member name
        my $maxmembnamelen = 0;
        foreach my $membidx (1 .. (@{$htyperef}-1)) {
            my $memberref = $htyperef->[$membidx];
            if (length($memberref->[0]) > $maxmembnamelen) {
                $maxmembnamelen = length($memberref->[0]);
            }
        }

        $retstr .= "{\n";
        foreach my $membidx (1 .. (@{$htyperef}-1)) {
            my $memberref = $htyperef->[$membidx];
#           print "    member $memberref->[0]\n";
            $retstr .= ' ' x $indquant;
            $retstr .= sprintf('%*s ', -$maxmembnamelen,
                               $memberref->[0]);   # The name
            my $subdatum = $datum->{$memberref->[0]};

            #
            # Is this an array of elements, or is it just one?
            #
            if (defined $memberref->[3] && $memberref->[3] > 1) {
#               print "       is an array\n";
                $retstr .= sprintf('[%u] =', $memberref->[3]);
                # Do all the members have the same value?
                if (keys %{{ map {$_, 1} @{$subdatum} }} != 1) {
                    my $indstr = ' ' x $indquant;
                    my $guts = join(', ',
                                     map { htype_format($_, $memberref->[1]) }
                                     @{$subdatum});

                    $guts =~ s/^/$indstr/gm;
                    $retstr .= $guts;
                }
                else {
                    $retstr .= sprintf('{ %u x %s }', scalar @{$subdatum},
                       htype_format($subdatum->[0], $memberref->[1]));
                }
            }
            else {
#               print "       is a singleton\n";
                my $piecestr;
                #
                # Just a single element.
                #
                # Do we have a formatter?
                #
                if (defined $memberref->[2]) {
#                   print "         has a formatter\n";
                    if (! ref($memberref->[2])) {
#                       print "           which is a printf format\n";
                        # It's a scalar, not a reference.  Let's
                        #  assume it's a printf format.
                        $piecestr = sprintf($memberref->[2], $subdatum);
                    }
                    elsif (ref($memberref->[2]) eq 'CODE') {
#                       print "           which is code\n";
                        #
                        # It's a function.
                        #
                        my $coderef = $memberref->[2];
                        $piecestr = &$coderef($memberref, $subdatum);
                    }
                    elsif (ref($memberref->[2]) eq 'HASH') {
#                       print "           which is a hash\n";
                        my $hashref = $memberref->[2];
                        $piecestr = htype_format($subdatum, $memberref->[1]);

                        if (exists $hashref->{$subdatum}) {
                            $piecestr .= " ($hashref->{$subdatum})";
                        }
                        else {
                            $piecestr .= ' (?)';
                        }
                    }

                }
                else {
#                   print "         does not have a formatter\n";
                    $piecestr = htype_format($subdatum, $memberref->[1]);
                }
                if ($piecestr =~ /\n/) {
                    #
                    # If it's got a newline in it, then
                    #  shift it over by indquant.
                    #
                    my $indstr = ' ' x $indquant;
                    $piecestr =~ s/^/$indstr/gm;
                }
                $retstr .= $piecestr;
            }
            if (substr($retstr, -1) ne "\n") {
                $retstr .= "\n";
            }
        }
        $retstr .= "}\n";
    }
    else {
        #
        # It's a primitive type.
        #
        my $coderef = $htyperef->[3];
#       print "coderef = " . Dumper($coderef) . "\n";
        if (! ref($coderef) || ref($coderef) ne 'CODE') {
            confess "not code.";
        }
        $retstr = &$coderef($htyperef, $datum);
    }
    return $retstr;
}

#
# Given a structure and a decoded instance of that structure,
#  construct a packed binary stream corresponding to the structure.
#
sub htype_pack {
    my ($datum, $htyperef) = ($_[0], $_[1]);
    my $retstr = '';

    #
    # Is this a compound type?
    #
#    print "formatting $htyperef->[0]\n";
    if (ref($htyperef->[1])) {
#       print "  it's a compound type.\n";
        #
        # Yes.  Go through each member and format them individually
        #

        foreach my $membidx (1 .. (@{$htyperef}-1)) {
            my $memberref = $htyperef->[$membidx];
#           print "    member $memberref->[0]\n";
            my $subdatum = $datum->{$memberref->[0]};

            #
            # Is this an array of elements, or is it just one?
            #
            if (defined $memberref->[3] && $memberref->[3] > 1) {
#               print "       is an array\n";
                $retstr .= join('', map { htype_pack($_, $memberref->[1]) }
                                @{$subdatum});
            }
            else {
#               print "       is a singleton\n";
                #
                # Just a single element.
                #
                $retstr .= htype_pack($subdatum, $memberref->[1]);
            }
        }
    }
    else {
        #
        # It's a primitive type.
        #
        my $coderef = $htyperef->[4];
#       print "coderef = " . Dumper($coderef) . "\n";
        if (! ref($coderef) || ref($coderef) ne 'CODE') {
            confess "not code.";
        }
        $retstr = &$coderef($htyperef, $datum);
    }
    return $retstr;
}

#
# Perform an openssl command.  Die if failed to execute.
#
sub openssl {
    my @args = @_;
    my $retval;

#    print "running: " . join(' ', 'openssl', @args) . "\n";
    if (($retval = system('openssl', @args)) != 0) {
        if ($? == -1) {
            print STDERR << "EOF";

Unable to run an openssl command.

Please make sure that openssl is installed and in your path.

EOF
            croak "Failed to execute openssl command: $!";
        }
        elsif (($? & 0x7f) != 0) {
            my $signum = ($? & 0x7f);
            my $didcoredump = ($? & 0x80) != 0 ? 'with' : 'without';
            croak
        "openssl child process died with signal $signum, $didcoredump coredump";
        }
        else {
            return $? >> 8;
        }
    }
    else {
        return 0;
    }
}

#
# Given a patch stream of data, encrypt using the given key and iv.
#
sub aesenc {
    my ($patchdata, $key, $iv) = ($_[0], $_[1], $_[2]);
    my ($patchtmpfh, $patchtmp) = tempfile();

    # First, save off the patchdata to the temporary file
    binmode $patchtmpfh;
    if (! defined syswrite($patchtmpfh, $patchdata)) {
        my $errtxt = $!;
        close $patchtmpfh;
        unlink $patchtmp;
        croak "Can't write patch data to temporary file $patchtmp: $errtxt";
    }
    close $patchtmpfh;

    # Next, create hex versions of the key and initial vector.
    my $hexkey = sprintf '%02x' x $BOOTLDR_AESCBC_KEYLEN_256, unpack('C*', $key);
    my $hexiv = sprintf '%02x' x $BOOTLDR_AESCBC_BLOCK_SIZE, unpack('C*', $iv);

    # Now actually do the encryption.
    my ($enctmpfh, $enctmp) = tempfile();
    close $enctmpfh;

    if (openssl('enc', '-in', $patchtmp, '-out', $enctmp, '-e',
                '-aes-256-cbc', '-K', $hexkey, '-iv', $hexiv) != 0) {
        unlink($patchtmp);
        unlink($enctmp);
        croak "Error encrypting patch";
    }
    unlink($patchtmp);

    my $encdata = readinbin($enctmp, 'encrypted temporary');
    unlink($enctmp);

    # Yuck.  'enc' apparently concatenates the final 'IV' onto
    #  the end of the file.
    $encdata = substr($encdata, 0, length($encdata) - $BOOTLDR_AESCBC_BLOCK_SIZE);

    return $encdata;
}

#
# Calculate a SHA-256 over the given data.
#  Really ugly -- uses temporary files and command files.
#
sub sha256hash {
    my ($data) = ($_[0]);
    my ($datatmpfh, $datatmpfn) = tempfile();

    # First, save off the data to the temporary file
    binmode $datatmpfh;
    if (! defined syswrite($datatmpfh, $data)) {
        my $errtxt = $!;
        close $datatmpfh;
        unlink $datatmpfn;
        croak "Can't write patch data to temporary file $datatmpfn: $errtxt";
    }
    close $datatmpfh;

    # Next calculate the SHA256 hash
    my ($hashtmpfh, $hashtmpfn) = tempfile();
    close $hashtmpfh;

    if (openssl('dgst', '-sha256', '-out',
                $hashtmpfn, '-binary', $datatmpfn) != 0) {
        unlink $datatmpfn;
        unlink $hashtmpfn;
        croak "Error calculating hash";
    }

    unlink $datatmpfn;

    my $hashdata = readinbin($hashtmpfn, 'hash temporary');
    unlink($hashtmpfn);
    return $hashdata;
}


#
# Given a patch stream of data, get an RSA signature
#
sub rsasign {
    my ($patchdata, $pkfile) = ($_[0], $_[1]);
    my ($patchtmpfh, $patchtmp) = tempfile();
    my $retdata;

    # First, save off the patchdata to the temporary file
    binmode $patchtmpfh;
    if (! defined syswrite($patchtmpfh, $patchdata)) {
        my $errtxt = $!;
        close $patchtmpfh;
        unlink $patchtmp;
        croak "Can't write patch data to temporary file $patchtmp: $errtxt";
    }
    close $patchtmpfh;

    # Next calculate the SHA256 hash
    my ($hashtmpfh, $hashtmp) = tempfile();
    close $hashtmpfh;

    if (openssl('dgst', '-sha256', '-out',
                $hashtmp, '-binary', $patchtmp) != 0) {
        unlink $patchtmp;
        unlink $hashtmp;
        croak "Error calculating patch hash";
    }

    unlink $patchtmp;
    my $signintmp;

    my $hashdata = readinbin($hashtmp, 'hash temporary');
    unlink($hashtmp);

    #debug:
    # printf("SHA-256 hash over 0x%x = %u bytes of data:\n",
    #      length($patchdata), length($patchdata));
    # print join (' ', map { sprintf '%02x', $_ } unpack('C*', $hashdata)) . "\n";
    # print "Original data = " . join(' ', map { sprintf '%02x', $_ } unpack('C*', $patchdata)) . "\n";

    #
    # Prepend the ASN.1 designation for
    # EMSA-PKCS1-v1_5 (see RFC 3447 section 9.2).
    #
    my $padhashdata = pack('C*',
                   0x30, 0x31, # Constructed sequence 49 bytes long
                       0x30, 0x0d, # Constructed sequence 13 bytes long
                           0x06, 0x09,   # Primitive OID 9 bytes long
                              0x60,      # (joint-iso-itu-t(2) * 40)
                                         # + country(16)
                              0x86, 0x48, # us(840 = (0x6 << 7) | 0x48)
                              0x01,      # organization(1)
                              0x65,      # gov(101)
                              0x03,      # csor(3)
                              0x04,      # nistalgorithm(4)
                              0x02,      # hashalgs(2)
                              0x01,      # 1 = sha256
                           # So the OID is actually: 2.16.840.1.101.3.4.2.1
                           # ASN.1: "{joint-iso-itu-t(2) country(16) us(840)
                           #         organization(1) gov(101) csor(3)
                           #         nistAlgorithm(4) hashAlgs(2)
                           #         sha256(1)}"
                           0x05, 0x00,   # Primitive NULL 0 bytes long
                       0x04, 0x20  # Primitive octet string 32 bytes long
                       ) . $hashdata;

    my ($padhashtmpfh, $padhashtmp) = tempfile();
    binmode $padhashtmpfh;
    if (! defined syswrite($padhashtmpfh, $padhashdata)) {
        croak "Error writing padded hash data to $padhashtmp: $!";
    }
    close $padhashtmpfh;
    $signintmp = $padhashtmp;

    #
    # Now sign the hash using the given private key
    #
    my ($signtmpfh, $signtmp) = tempfile();
    close $signtmpfh;

    if (openssl('rsautl', '-sign', '-in', $signintmp, '-inkey', $pkfile,
                '-out', $signtmp) != 0) {
        unlink $signintmp;
        unlink $signtmp;
        croak "Error signing patch hash";
    }
    unlink $signintmp;

    $retdata = readinbin($signtmp, 'signature temporary');
    unlink $signtmp;

    if (length($retdata) != $BOOTLDR_RSA_KEYLEN_2048) {
        croak "Patch signature is wrong size (got " . length($retdata) . " expected $BOOTLDR_RSA_KEYLEN_2048)";
    }

    return $retdata;
}

#
# Given a string, pad it to a multiple of the given quantum.
#
sub padtoquant {
    my ($data, $quant, $fillbyte) = ($_[0], $_[1], $_[2]);

    return $data . pack('C', $fillbyte) x ((-length($data) % $quant));
}

#
# Given a string, pad it to the given length.
#
sub padto {
    my ($data, $length, $fillbyte) = ($_[0], $_[1], $_[2]);
    if (length($data) >= $length) {
        return $data;
    }
    else {
        return $data . pack('C', $fillbyte) x ($length - length($data));
    }
}


#
# Convert a string to an integer.
#  Base 10 (decimal) is the default.  If
#  the value begins with 0x, or if it has [a-f] in it,
#  it is considered hex.  Does not handle base 8 (octal)
#  or base 2.
#
# Returns undefined if there is any other problem.
#
# I can't believe Perl doesn't have this kind of thing builtin.
#
sub toint {
    my ($str) = ($_[0]);
    my $retval;

    if ($str =~ /^0x/i || $str =~ /[a-f]/i) {
        my $shortstr = $str;
        $shortstr =~ s/^0x//i;    # strip the leading 0x
        if (!($shortstr =~ /[^0-9a-f]/i)) {
            $retval = hex($str);
        }
    }
    elsif (!($str =~ /[^0-9]/)) {
        $retval = 0 + $str;
    }

    return $retval;
}

#
# See if the given integer will fit in the given sized unsigned value.
#
sub intfits {
    my ($val, $nbits) = ($_[0], $_[1]);

    while ($nbits > 0 && $val != 0) {
        $val >>= 1;
        $nbits--;
    }

    return $val == 0;
}


#
# Does [addr, addr+size) fit entirely within [base, base+range)?
#

sub bounded {
    my ($addr, $size, $base, $range) = ($_[0], $_[1], $_[2], $_[3]);

    return ($addr >= $base
            && ($addr + $size) <= ($base + $range));
}


#
# Dump out a binary bunch of data in hexadecimal.
#

sub hexdump {
    my ($startaddr, $data) = ($_[0], $_[1]);
    my $addr;
    my @bytes = unpack('C*', $data);

    $addr = $startaddr & ~0xf;
    printf("%08x  ", $addr);
    for ( ; $addr < $startaddr; $addr++) {
        print "   ";
    }
    while ($addr < ($startaddr + length($data))) {
        printf(" %02x", shift(@bytes));
        $addr++;
        if (($addr & 0xf) == 0) {
            printf("\n%08x  ", $addr);
        }
    }
    if (($addr & 0xf) != 0) {
        print "\n";
    }
}


#
# Read in the given binary file.
#
sub readinbin {
    my ($fname, $type) = ($_[0], $_[1]);
    my $data;
    my $datablock;
    my $nbytes;

    open(my $ifh, '<', $fname) or croak "Can't open $fname for input as $type: $!";
    binmode $ifh;

    while (($nbytes = sysread($ifh, $datablock, 512)) > 0) {
        $data .= $datablock;
    }
    close $ifh;
    return $data;
}

#
# Given a list, return everything but the last element.
#
sub butlast {
    my @listfull = @_;
    my $nelem = scalar @listfull;

    return @listfull [0..($nelem-2)];
}

#
# Rewrite the given DWORDs in a patch image.
#
sub rewrite32 {
    my $datref = shift;
    my $rambase = shift;
    my $addr = shift;
    my @vals = @_;
    # printf("rewrite32(addr = 0x%08x, rambase = 0x%08x, vals = @vals) offset = 0x%x = %u\n",
    #     $addr, $rambase, $addr - $rambase, $addr - $rambase);
    if (length($$datref) < ($addr - $rambase)) {
        # Pad the string if it's not long enough
        $$datref = padto($$datref, $addr - $rambase, 0xff);
    }
    substr($$datref, $addr - $rambase, 4 * @vals) = pack('L*', @vals);
}

#
# Read the given DWORD in a patch image.
#
sub read32 {
    my $datref = shift;
    my $rambase = shift;
    my $addr = shift;
#    printf("read32(addr = 0x%08x, rambase = 0x%08x), offset = 0x%x=%u, datalen=0x%x=%u\n",
#          $addr, $rambase, $addr - $rambase, $addr - $rambase,
#          length($$datref), length($$datref));
    if (length($$datref) < ($addr - $rambase + 4)) {
        return undef;
    }
    else {
        return unpack('L', substr($$datref, $addr - $rambase, 4));
    }
}


#
# Load the given payload ELF file into memory.
#
sub loadpayload {
    my ($elfref, $flashbase, $flashsize) = ($_[0], $_[1], $_[2]);
    my @ramimage;

    foreach my $secref (@{$$elfref->{'secs'}}) {
        if ($secref->{'sh_type'} == elfio::SHT_PROGBITS()
            && $secref->{'sh_size'} != 0
            && exists $secref->{'loadaddr'}) {

            my $baseaddr = $secref->{'loadaddr'};

            if (bounded($baseaddr, $secref->{'sh_size'},
                        $flashbase, $flashsize)) {
                my @databytes = unpack('C*', $secref->{'data'});
                # extended the array. So stupid.
                @ramimage[$baseaddr - $flashbase
                          + scalar @databytes - 1] = undef;
                splice(@ramimage, $baseaddr - $flashbase,
                       scalar @databytes, @databytes);
            }
        }
    }

    return pack('C*', map { ! defined $_ ? 0xff : $_ } @ramimage);
}

# Sign and encrypt a patch
sub signencrypt {
    my ($binpatchref, $plainpatchref, $binplainhdrref, $vpskfileref, $keyfileref, $signatureref)
          = ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5]);
    my $aeskey;
    my $aesiv;

    my $sigdata = '';

    #
    # If we're encrypting then the entire plaintext patch must
    #  be a multiple of BOOTLDR_AESCBC_BLOCK_SIZE (16).
    # Signing isn't so important because we use consistent
    #  padding on both sides.
    #
    my $totallen = length($$plainpatchref) + $BOOTLDR_RSA_KEYLEN_2048;
    $$plainpatchref = padtoquant($$plainpatchref, $BOOTLDR_AESCBC_BLOCK_SIZE, 0);

    if (defined $$vpskfileref) {
        # Sign the patch.
        #debug:
        # printf("Signing 0x%x = %u bytes of patch (plainhdr = 0x%x = %u) + (plainpatchref = 0x%x = %u)\n",
        #        length($$binplainhdrref . $$plainpatchref),
        #        length($$binplainhdrref . $$plainpatchref),
        #        length($$binplainhdrref),
        #        length($$binplainhdrref),
        #        length($$plainpatchref),
        #        length($$plainpatchref));

        $sigdata = rsasign($$binplainhdrref . $$plainpatchref, $$vpskfileref);
        # Make it little endian.
        $sigdata = pack 'C*', reverse unpack('C*', $sigdata);
        #debug:
        # printf("Signature is %u bytes long\n", length($sigdata));
        $$signatureref = "This file contains a " . length($sigdata) . " byte RSA signature";
    }
    else {
        # Signature is provided externally.

        $sigdata = readinbin($sigfile, 'external signature file');
        $$signatureref = "This file contains an externally-provided RSA signature of " . length($sigdata) . " bytes.";
        if (length($sigdata) != $BOOTLDR_RSA_KEYLEN_2048) {
            croak "Key file $sigdata is not $BOOTLDR_RSA_KEYLEN_2048 bytes long";
        }
    }

    #
    # Put the signature data immediately after the padding
    #  after the rest of the plaintext patch.
    #
    $$plainpatchref .= $sigdata;

    if (defined $$keyfileref) {
        #
        # We've been provided with a keyfile.  Load it in.
        #

        my $aeskeyiv = readinbin($$keyfileref, 'key and iv file');

        if (length($aeskeyiv)
            < ($BOOTLDR_AESCBC_KEYLEN_256 + $BOOTLDR_AESCBC_BLOCK_SIZE)) {
            croak "File $$keyfileref is too short to contain AES key and IV";
        }
        elsif (length($aeskeyiv) > ($BOOTLDR_AESCBC_KEYLEN_256
                                    + $BOOTLDR_AESCBC_BLOCK_SIZE)) {
            warn "warning: file $$keyfileref has extra data past AES key and IV.  Ignoring.\n";
        }
        $aeskey = substr($aeskeyiv, 0, $BOOTLDR_AESCBC_KEYLEN_256);
        $aesiv = substr($aeskeyiv, $BOOTLDR_AESCBC_KEYLEN_256, $BOOTLDR_AESCBC_BLOCK_SIZE);
    }
    else {
        #
        # We haven't been given a keyfile.  Use the default
        #  engineering encryption key.
        #
        $aeskey = pack('C', 0xaa) x $BOOTLDR_AESCBC_KEYLEN_256;
        $aesiv = pack('C', 0xbb) x $BOOTLDR_AESCBC_BLOCK_SIZE;
    }

    # Encrypt the patch.
    my $encpatch = aesenc($$plainpatchref, $aeskey, $aesiv);
    #debug:
    # print "plain patch is " . length($$plainpatchref) . " bytes long, encrypted patch is " . length($encpatch) . " bytes long\n";

    $$binpatchref = $$binplainhdrref . $encpatch;
}

# Write an ASCII file
sub writeascii {
    my ($fh, $plainhdrref, $hdrtype, $plainpatchref, $payloadref,
        $enchdrref, $signatureref, $cmdlineref, $plainstartref)
        = ($_[0], $_[1], $_[2], $_[3], $_[4], $_[5], $_[6], $_[7], $_[8]);
    my $plainhdrstr = "vcsfw_cmd_bootldr_patch_plainhdr_t:\n";

    if ($$hdrtype == 0) {
        $plainhdrstr .= htype_format($plainhdrref,
                                       \@cmd_bootldr_patch_plainhdr_htype0);
    }
    elsif ($$hdrtype == 1) {
        $plainhdrstr .= htype_format($plainhdrref,
                                       \@cmd_bootldr_patch_plainhdr_htype1);
    }
    elsif ($$hdrtype == 2) {
        $plainhdrstr .= htype_format($plainhdrref,
                                       \@cmd_bootldr_patch_plainhdr_htype2);
    }
    else {
      die "hdrtype '$$hdrtype' is not valid here.";
    }
    my $enchdrstr = "vcsfw_cmd_bootldr_patch_enchdr_t:\n";
    $enchdrstr .= htype_format($$enchdrref, \@cmd_bootldr_patch_enchdr_htype);
    my $username = getpwuid $<;
    my $host = hostname;
    my $uname = qx(uname -a);
    chomp($uname);
    my @lt = localtime;
    my $curtime=strftime("%a, %d %b %Y  %H:%M:%S %z (%Z)", @lt);

    my $blockcomment =
"  By:      $username
  On:      $host
  Uname:   $uname
  Running: $^O
  At:      $curtime
  Script:  $scriptver
  CWD:     $ENV{PWD}
  Command: $$cmdlineref
  $$signatureref\n";
    my $payloadnotes = '';
    if (defined $$payloadref) {
        $payloadnotes = "This patch contains a payload file.\n";
        if (exists $symsbyname{'_patch_payload_sizequant'}) {
            $payloadnotes .= sprintf(
                "    &_patch_payload_sizequant = 0x%x = %u\n",
                                     $symsbyname{'_patch_payload_sizequant'},
                                     $symsbyname{'_patch_payload_sizequant'});
        }
        $payloadnotes .= sprintf(
            "    &_patch_payload_sizemax = 0x%x = %u\n",
                                 $symsbyname{'_patch_payload_sizemax'},
                                 $symsbyname{'_patch_payload_sizemax'});
        for my $symname ('_patch_payload_size',
                         '_patch_payload_loc', '_patch_payload_missionfinal',
                         '_patch_payload_missionhash') {
            if (exists $symsbyname{$symname}) {
                $payloadnotes .= sprintf("    %s (%s0x%08x) = 0x%x = %u\n",
                                         $symname, '@',
                                         $symsbyname{$symname},
                                         read32($plainpatchref,
                                                $$plainstartref,
                                                $symsbyname{$symname}),
                                         read32($plainpatchref,
                                                $$plainstartref,
                                                $symsbyname{$symname}));
            }
        }
    }


    #
    # We have an ASCII header.
    #
    if (! defined $opt_genc) {
        syswrite $fh, "// Validity Sensors V4 Patch File\r\n";
        syswrite $fh, "//  This contains a Nassau boot loader patch.\r\n";
        syswrite $fh, "//\r\n";
        my $allstr = $plainhdrstr . "\n" . $enchdrstr . "\n" . $blockcomment
            . "\n" . $payloadnotes;
        $allstr =~ s=^=// =gm;
        $allstr =~ s/\n/\r\n/g;
        syswrite $fh, $allstr;
        syswrite $fh, "\x00\x1a";
    }
    else {
        print $fh "/* -*- mode: c; tab-width: 4 -*- */\n";
        print $fh '/* $' . 'Header' . "\$ */\n";
        print $fh "\
/*\
* This is an auto-generated source file.\
*  This contains a Nassau-firmware boot loader patch.\
* Do not edit this file.\
*/\
\n";

        for my $bc ($plainhdrstr, $enchdrstr, $blockcomment, $payloadnotes) {
            $bc =~ s/^/ * /gm;
            $bc = "/*\n" . $bc . " */\n\n";
            print $fh $bc;
        }
    }
}

# Generate a C file
sub writecfile {
    my ($fh, $bindataref) = ($_[0], $_[1]);
    my $bindata;
    my $offset;

    $offset = 0;

    $bindata = $$bindataref;
    printf $fh "/* Total number of bytes = %u = 0x%x */\n",
            length($bindata), length($bindata);

    #
    # Unpack the data as an array of 32-bit unsigned values.
    #
    my @textbytes = ();

    foreach my $val (unpack('C*', $bindata)) {
        push @textbytes, sprintf '0x%02x', $val;
    }
    my $textdata = join(', ', @textbytes);
    $textdata =~ s/((\S+\s*){1,8})/$1\n/g;
    foreach my $line (split("\n", $textdata)) {
        printf $fh "    $line    /* [%04x] */\n", $offset;
        $offset += 8;
    }
}

sub rounddown16 {
   my ($x) = $_[0];
   return $x - $x % 16;
}

#
# Given an ELF file, go through and find and print out
#  all warnings and errors.  If we're attempting to production-sign
#  a patch if there are lockouts return FALSE.
#  arguments:
#    [0] elfref.  Reference to the ELF object being looked at
#    [1] elffn.  The name of the file from which the ELF object was loaded.
#    [2] ispayload.  Boolean.  Indicates the given ELF file is a payload
#        file.  Otherwise it's considered to be a patch file.
#    [3] obeylockout.  Boolean.  If a SIGNSEC_LOCKOUT() is found in
#        the ELF file and this is true then we must fail in an error.
#    [4] verbose.  Boolean.  Spit out warnings.
#

sub signsec_check {
    my ($elfref, $elffn, $ispayload, $obeylockout, $verbose)
        = ($_[0], $_[1], $_[2], $_[3], $_[4]);

    my @warnings = ();
    my @errors = ();

    if ($ispayload) {
        $elffn = "payload ELF $elffn";
    }
    else {
        $elffn = "patch ELF $elffn";
    }

    foreach my $secref (@{$elfref->{'secs'}}) {
        if ($secref->{'sh_type'} == elfio::SHT_PROGBITS()
            && ($secref->{'name'} eq '.signsec_warnings'
                || $secref->{'name'} eq '.signsec_lockouts')) {
            # Scan through the warnings/lockouts
            my $secdata = $secref->{'data'};
            my $offset = 0;
            my $seclen = length($secdata);
            my $islockout = ($secref->{'name'} eq '.signsec_lockouts');

            while ($offset < $seclen) {
                # Format is:
                #  uint32_t linenum;
                #  string   filename;
                #  string   text;
                my ($linenum, $filename, $text)
                    = unpack('LZ*Z*', substr($secdata, $offset));
                my $overallsize =
                    (4 + length($filename) + 1 + length($text) + 1);
                # Pad up to a nice 4-byte aligned next value.
                $overallsize += (-$overallsize % 4);
                $offset += $overallsize;

                # Indent the text by 4 spaces.
                $text =~ s/^/    /gm;
                # All messages are warnings if we're not production signing
                my $msgtext;
                if ($islockout) {
                    if (! $obeylockout) {
                        $msgtext .= 'WARNING: SIGNSEC_LOCKOUT';
                    }
                    else {
                        $msgtext .= 'ERROR: SIGNSEC_LOCKOUT';
                    }
                }
                else {
                    $msgtext .= 'WARNING: SIGNSEC_WARNING';
                }
                $msgtext .= " in $elffn\n";
                $msgtext .= sprintf("  (source %s:%u)\n", $filename, $linenum);
                $msgtext .= $text;

                if (! $islockout || ! $obeylockout) {
                    push @warnings, $msgtext;
                }
                else {
                    push @errors, $msgtext;
                }
            }
        }
    }

    if ($verbose || scalar(@errors) != 0) {
        if (scalar(@warnings) != 0) {
            print join("\n", @warnings) . "\n";
        }

        if (scalar(@errors) != 0) {
            print join("\n", @errors) . "\n";
        }
    }

    return scalar(@errors) == 0;
}

# Maximum payload for each piece, taking overhead into account
# This includes aligning to a 16-byte boundary for the signature.
sub payloadmax_set {
    return rounddown16($symsbyname{'_patch_payload_sizemax'}
                 - htype_size(\@flashprog_hdr_htype)
                 - $BOOTLDR_PATCH_PLAINHDR_SIZE
                 - htype_size(\@cmd_bootldr_patch_enchdr_htype)
                 - $BOOTLDR_RSA_KEYLEN_2048);
}

sub iotalist_setup {
    my ($ilistbinref) = ($_[0]);

    my $magicsize = htype_size(\@uint32_primtype);
    my $lastversion;
    my $lastconfigid1;
    my $lastconfigid2;

    # Input file iota list format is:
    # [uint32_t magic_value]
    # [uint16_t itype][uint16_t length][iota data]
    # [uint16_t itype][uint16_t length][iota data]
    #  .
    #  .
    #  .
    # [uint16_t itype][uint16_t length][iota data]
    #
    # Transforming this uses the following, to
    # allow the flashprog patch to write flash while
    # traversing the list:
    # typedef struct flashprog_ilist_hdr_s {
    #     uint16_t length;  /* length in bytes of payload data */
    #     uint16_t itype;   /* iota type */
    #     uint16_t version; /* config version the iota corresponds to */
    #     uint8_t  unused[2];
    # } flashprog_ilist_hdr_t;

    # And for extracting the version, an iota of type
    # VCSFW_IOTA_ITYPE_CONFIG_VERSION is expected to appear
    # after version of iota data, in order.
    # [iota][iota][version 1]
    # [iota][iota][version 2]
    # .
    # .
    # [iota][iota][version N]

    # If the iota force option is in place, then the version in the payload list
    # is overridden to force the writing of the iotas without version or config id checking.

    if (length($$ilistbinref) < $magicsize) {
        croak "Iota list file is too short to contain even the magic value '$IOTALIST_MAGIC'.";
    }

    my ($magic) = htype_unpack($$ilistbinref, \@uint32_primtype);
    if ($magic != $IOTALIST_MAGIC) {
        printf "Iota list file does not start with the magic value 0x%08x\n", $IOTALIST_MAGIC;
        croak;
    }

    my $tmplist = substr($$ilistbinref, $IOTALIST_MAGIC_SIZE,
                  length($$ilistbinref) - $IOTALIST_MAGIC_SIZE);
    my @tmpiotas;
    my $iotalist = "";
    my $tmpnotversioned;
    my $tmpversioned;
    my $numiotas = 0;
    while (length($tmplist) >= 4) {  # itype and length
        my ($itype) = htype_unpack($tmplist, \@uint16_primtype);
        $tmplist = substr($tmplist, 2, length($tmplist) - 2);
        # print "Found iota itype $itype\n";

        my ($length) = htype_unpack($tmplist, \@uint16_primtype);
        $tmplist = substr($tmplist, 2, length($tmplist) - 2);
        # print "iota length is $length\n";

        my $data = substr($tmplist, 0, $length);
        $tmplist = substr($tmplist, $length, length($tmplist) - $length);
        # print "iota data length is ", length($data), "\n";

        push @tmpiotas, { itype => $itype, length => $length, data => $data };

        if ($itype == $VCSFW_IOTA_ITYPE_CONFIG_VERSION) {
           my ($iota) = htype_unpack($data, \@config_version_htype);
           # printf "iota configid {id2,id1} is 0x%08x,0x%08x\n", $iota->{'config_id2'} , $iota->{'config_id1'};
           # print "iota version is ", $iota->{'version'}, "\n";

           if (defined $lastversion && ($iota->{'version'} <= $lastversion)) {
             print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
             print "WARNING:Versions in iota file are not incrementing. \
                    Previous version was $lastversion, current version is $iota->{'version'}";
             print "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
           }
           $lastversion = $iota->{'version'};

           if (defined $lastconfigid1) {
               if ($iota->{'config_id2'} . $iota->{'config_id1'} != ($lastconfigid2 . $lastconfigid1)) {
                 croak "Mismatching config IDs in the iota list. Expected $lastconfigid2 . $lastconfigid1, \
                        found $iota->{'config_id2'} . $iota->{'config_id1'} . ";
               }
           }
           else {
               $lastconfigid1 = $iota->{'config_id1'};
               $lastconfigid2 = $iota->{'config_id2'};
           }

           $numiotas += scalar @tmpiotas;
           while (scalar @tmpiotas > 0) {
              $tmpnotversioned = shift(@tmpiotas);
              $tmpversioned->{'itype'} = $tmpnotversioned->{'itype'};
              $tmpversioned->{'length'} = $tmpnotversioned->{'length'};
              if (defined $opt_iotaforce) {
                  $tmpversioned->{'version'} = $FLASHPROG_IOTA_VERSION_NOCHECK;
              }
              else {
                  $tmpversioned->{'version'} = $iota->{'version'};
              }
              $tmpversioned->{'unused'} = [ 0, 0 ];
              $iotalist .= htype_pack($tmpversioned, \@flashprog_ilist_htype)
                . $tmpnotversioned->{'data'};
           }
        }
    }
    if (length($tmplist) > 0) {
        croak "Iota list file contains " . length($tmplist) . " extra bytes. Ensure version iotas appear after each set of iota data.";
    }
    # If we are using the force option, write out any iotas that appear
    # in the list without a version iota following them.  There is no
    # restriction on use of iota versions in this case.
    if (scalar @tmpiotas > 0 && defined $opt_iotaforce) {
       while (scalar @tmpiotas > 0) {
          $tmpnotversioned = shift(@tmpiotas);
          $tmpnotversioned->{'version'} = $FLASHPROG_IOTA_VERSION_NOCHECK;
          $tmpnotversioned->{'unused'} = [ 00, 00 ];
          $iotalist .= htype_pack($tmpnotversioned, \@flashprog_ilist_htype)
            . $tmpnotversioned->{'data'};
          $numiotas++;
       }
    }
    my $expectedlen = length($$ilistbinref)
         - $IOTALIST_MAGIC_SIZE + 4 * $numiotas;
    printf "Iota list lengths: input file %d, output file %d, expected: %d\n",
            length($$ilistbinref), length($iotalist), $expectedlen;
    if (length($iotalist) != $expectedlen) {
        croak "Check the iota list file format. Ensure version iotas appear after each set of iota data.";
    }
    print "Opening output file 'debug.bin'.\n";
    open(my $fh, '>', "debug.bin") or croak $!;
    binmode $fh;
    syswrite($fh, $iotalist);
    close ($fh);

    if (!defined $opt_iotaforce) {
       if (! defined $lastconfigid1 || !defined $lastconfigid2 || ! defined $lastversion) {
          croak "ERROR: No version iotas were found in the iota file.";
       }
    }
    else {
       print "WARNING: Iota version checking is DISABLED! OVERRIDDEN with the iforce option.\n";
       $lastconfigid1 = 0xffffffff;
       $lastconfigid2 = 0xffffffff;
       $lastversion = 0xffff;
    }
    return ($iotalist, $lastconfigid1, $lastconfigid2, $lastversion);
}
##
## MAIN
##

my $cmdline="$0 " . join(' ', @ARGV);

while ($#ARGV >= 0) {
    my $flag = shift;
    my $flagopt;
    if ($flag eq '-o') {
        if (! defined ($out_file = shift)) {
            carp "Missing output file";
            usage;
        }
    }
    elsif ($flag eq '-v') {
        $opt_verbose = 1;
    }
    elsif ($flag =~ /^-pi$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing payload id after $flag";
            usage;
        }
        if (! defined ($opt_payload_id = toint($thisarg))) {
            carp "Invalid payload id ($thisarg) after $flag";
            usage;
        }
        if (! intfits($opt_payload_id, 32)) {
            carp "Payload id $thisarg won't fit in a 32-bit integer after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-id$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing id after $flag";
            usage;
        }
        if (! defined ($opt_id = toint($thisarg))) {
            carp "Invalid id ($thisarg) after $flag";
            usage;
        }
        if (! intfits($opt_id, 32)) {
            carp "id $thisarg won't fit in a 32-bit integer after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-pf$/i) {
        if (! defined ($opt_payload_elf = shift)) {
            carp "Missing payload ELF file after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-bn$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing build number after $flag";
            usage;
        }
        if (! defined ($opt_bn = toint($thisarg))) {
            carp "Invalid build number ($thisarg) after $flag";
            usage;
        }
        if (! intfits($opt_bn, 32)) {
            carp "Build number $thisarg won't fit in a 32-bit integer after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-vv$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing major.minor version number after $flag";
            usage;
        }
        if ($thisarg =~ /^([0-9]+)\.([0-9]+)$/) {
            if (! defined ($opt_vmajor = toint($1))) {
                carp "Invalid major version number $1 in $thisarg after $flag";
                usage;
            }
            if (! defined ($opt_vminor = toint($2))) {
                carp "Invalid minor version number $2 in $thisarg after $flag";
                usage;
            }
            if (! intfits($opt_vmajor, 8)) {
                carp "Major version number $1 won't fit in an 8-bit integer after $flag";
                usage;
            }
            if (! intfits($opt_vminor, 8)) {
                carp "Minor version number $2 won't fit in an 8-bit integer after $flag";
                usage;
            }
        }
        else {
            carp "Invalid major.minor version number ($thisarg) after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-xpsk$/i) {
        if (defined $vpskfile) {
            carp "Duplicate instances of $flag";
            usage;
        }

        if (! defined ($vpskfile = shift)) {
            carp "Missing private key PEM file after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-xsig$/i) {
        if (defined $sigfile) {
            carp "Duplicate instances of $flag";
            usage;
        }

        if (! defined ($sigfile = shift)) {
            carp "Missing signature file after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-xkey$/i) {
        if (defined $keyfile) {
            carp "Duplicate instances of $flag";
            usage;
        }

        if (! defined ($keyfile = shift)) {
            carp "Missing key file after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-tn$/i) {
        carp "The -tn flag is deprecated.";
        shift;
    }
    elsif ($flag =~ /^-genc$/i) {
        $opt_genc = 1;
    }
    elsif ($flag =~ /^-genhdr$/i) {
        $opt_genhdr = 1;
    }
    elsif ($flag =~ /^-phtype$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing plain header type after $flag";
            usage;
        }
        if (! defined ($plainhdrtype = toint($thisarg))) {
            carp "Invalid plain header type ($thisarg) after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-custkey$/i) {
        $opt_custkey = 1;
    }
    elsif ($flag =~ /^-fwflav$/i) {
        my $thisarg;
        if (! defined ($thisarg = shift)) {
            carp "Missing firmware flavor after $flag";
            usage;
        }
        if (! defined ($fwflav = toint($thisarg))) {
            carp "Invalid firmware flavor ($thisarg) after $flag";
            usage;
        }
        if (! intfits($fwflav, 32)) {
            carp "Firmware flavor $thisarg won't fit in a 32-bit integer after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-pil$/i) {
        if (! defined ($opt_payload_ilist = shift)) {
            carp "Missing iota list file after $flag";
            usage;
        }
    }
    elsif ($flag =~ /^-iforce$/i) {
        $opt_iotaforce = 1;
    }
    elsif ($flag =~ "^-") {
        carp "Unknown flag: $flag";
        usage;
    }
    elsif (defined $elf_file) {
        carp "Only one ELF file allowed ($elf_file), trying to set $flag";
        usage;
    }
    else {
        $elf_file = $flag;
    }
}

if (defined $vpskfile && defined $sigfile) {
    carp "Can't specify both a VPS key file (-xpsk) and a signature file (-xsig)";
    usage;
}

if (defined $keyfile && (!defined $vpskfile && !defined $sigfile)) {
    carp "Missing -xpsk/-xsig argument.  Patch cannot be encrypted and not signed";
    usage;
}

if (defined $opt_custkey && ((!defined $vpskfile && !defined $sigfile)
                             || !defined $keyfile)) {
    carp "-custkey requires -xkey and -xpsk/-xsig";
    usage;
}


if (! defined $elf_file) {
    carp "Missing ELF file";
    usage;
}


######################################################################
# Load flashprog ELF
######################################################################
my $elfio = elfio->new();
my $pl_elfio = elfio->new();
#print "elfio:\n";
#print  Dumper($elfio);

my $elfdb=$elfio->elfin($elf_file);

if (! defined $out_file) {
    carp "No output file specified";
    usage;
}

# Scan the patch ELF file for signing security issues
if (! signsec_check($elfdb, $elf_file, 0, defined $keyfile, $opt_verbose)) {
    die "Signature security lockouts in patch prevent signing patch";
}

######################################################################
# Load payload ELF
######################################################################
my @symtmp = gensymtab(\$elfdb);
%symtab = %{$symtmp[0]};
%symsbyname = %{$symtmp[1]};
# print "symbol table: ", Dumper(%symtab), "\n";
my @sortedsymvals  = sort { $b <=> $a } keys %symtab;

#print "elf file: ", Dumper($elfdb), "\n";

my $pl_elfdb;
# Payload ELF file
if (defined $opt_payload_elf) {
    $pl_elfdb=$pl_elfio->elfin($opt_payload_elf);

    # Scan the payload ELF file for signing security issues
    if (! signsec_check($pl_elfdb, $opt_payload_elf, 1, defined $keyfile,
                        $opt_verbose)) {
        die "Signature security lockouts in payload prevent signing patch";
    }

    my @pl_symtmp = gensymtab(\$pl_elfdb);
    %pl_symtab = %{$pl_symtmp[0]};
    %pl_symsbyname = %{$pl_symtmp[1]};
    # print "Payload symbol table: ", Dumper(%pl_symtab), "\n";
    my @pl_sortedsymvals  = sort { $b <=> $a } keys %pl_symtab;
}

# print $fh "Payload elf file: ", Dumper($pl_elfdb), "\n";


######################################################################
# Go through and find all data in the ELF file that correspond
#  to the canonical mappings of the RAM in DRAM and IRAM space.
######################################################################
my %patchdata;
my $patchlowaddr;
my $patchhighaddr;

my $data_base;
my $data_size;
my $code_base;
my $code_size;
my $code_to_data_addend;

$data_base = $HWDEF_MEMMAP_SRAM_RAM_BASE;
$data_size = $HWDEF_MEMMAP_SRAM_RAM_SIZE;
$code_base = $HWDEF_MEMMAP_SROM_RAM_BASE;
$code_size = $HWDEF_MEMMAP_SROM_RAM_SIZE;
$code_to_data_addend = $HWDEF_MEMMAP_SROM_TO_SRAM_RAM_ADDEND;

######################################################################
# Go through the patch data and cook up bounds
#  for the thing(patchlowaddr, patchhighaddr).
#   We do all of our thinking in "data" space.
######################################################################
foreach my $secref (@{$elfdb->{'secs'}}) {
    if ($secref->{'sh_type'} == elfio::SHT_PROGBITS()
        && $secref->{'sh_size'} != 0
        && exists $secref->{'loadaddr'}) {

        my $baseaddr = $secref->{'loadaddr'};

        if (bounded($baseaddr, $secref->{'sh_size'}, $data_base, $data_size)) {
        }
        elsif (defined $code_base
               && bounded($baseaddr, $secref->{'sh_size'},
                          $code_base, $code_size)) {
            $baseaddr += $code_to_data_addend;
        }
        else {
            next;
        }

        if (! defined $patchlowaddr || $baseaddr < $patchlowaddr) {
            $patchlowaddr = $baseaddr;
        }

        if (! defined $patchhighaddr
            || ($baseaddr + $secref->{'sh_size'} - 1) > $patchhighaddr) {
            $patchhighaddr = $baseaddr + $secref->{'sh_size'} - 1;
        }

        #
        # Copy the patch data into place
        #
        $patchdata{$baseaddr} = $secref->{'data'};
    }
}


my @sortedaddrs = sort { $a <=> $b} keys %patchdata;

if (! defined($sortedaddrs[0])) {
    croak "Patch appears to be empty.";
}

my $plainpatch = '';
my $binpatch;
my $signature;

######################################################################
# Prepare the binary patch.  We prepare a string called
#  plainpatch which contains the patch.  We may choose
#  to encrypt this patch as 'binpatch'.
######################################################################

#
# Generate a padded version of the patch.
#

# Extract the binary patch data and make it nice and
#  contiguous.
my $lastend;
my $plainstart;
foreach my $addr (@sortedaddrs) {
    # printf "block at 0x%08x\n", $addr;
    if (! defined $plainstart || $addr < $plainstart) {
        $plainstart = $addr;
    }
    if (defined $lastend && $addr > $lastend) {
        $plainpatch .= "\0" x ($addr - $lastend);
    }
    $plainpatch .= $patchdata{$addr};
    $lastend = $addr + length($patchdata{$addr});
}

# Now pull out the soon-to-be-encrypted header from the existing
# patch.
if (length($plainpatch) < htype_size(\@cmd_bootldr_patch_enchdr_htype)) {
    croak "Patch is too short to contain its encrypted header.";
}

# printf "Length of plainpatch is %d\n", length($plainpatch);
my ($enchdr) = htype_unpack($plainpatch, \@cmd_bootldr_patch_enchdr_htype);
my $tmp .= htype_format($enchdr, \@cmd_bootldr_patch_enchdr_htype);
# print "header " . join(', ', map {sprintf("0x%x", $_)} unpack('C*', $tmp)) . "\n";

######################################################################
# Generate the plain header
######################################################################
my %plainhdr;
$plainhdr{'flags'} = 0;

my %attrsfromelf = (
#   NOTE: These fields are common to all plainhdr types!
    'buildnum' => [ \$opt_bn, '_patch_buildnum', 0 ],
    'id' => [ \$opt_id, '_patch_id', 0 ],
    'vmajor' => [ \$opt_vmajor, '_patch_vmajor', 0 ],
    'vminor' => [ \$opt_vminor, '_patch_vminor', 0 ]
    );

for my $attr (keys %attrsfromelf) {
    my $attrref = $attrsfromelf{$attr};
    my $optref = $attrref->[0];
    if (defined $$optref) {
        if (exists $symsbyname{$attrref->[1]}) {
            carp "Patch $attr set inside ELF file ($symsbyname{$attrref->[1]}) is overridden by command line ($$optref)";
        }
        $plainhdr{$attr} = $$optref;
    }
    elsif (exists $symsbyname{$attrref->[1]}) {
        $plainhdr{$attr} = $symsbyname{$attrref->[1]};
    }
    else {
        $plainhdr{$attr} = $attrref->[2];
    }
}

# Undefined for non-mission payloads
my $payload_mission;
# Undefined for non-iota payloads
my $payload_iota;
if ($plainhdrtype == 0) {
  # Plain header vcsfw_cmd_bootldr_patch_plainhdr_type0_t
  $plainhdr{'payload_id'} = $opt_payload_id;
  if (defined $opt_payload_elf) {
      # make sure we have all of the symbols we need
      for my $symname ('_patch_payload_sizemax', '_patch_payload_size',
          '_patch_payload_mission_base', '_patch_payload_mission_size') {
          if (! exists $symsbyname{$symname}) {
              croak "Patch does not define $symname";
          }
      }
      # fetch the payload data from the payload ELF file
      $payload_mission = loadpayload(\$pl_elfdb,
                                     $symsbyname{'_patch_payload_mission_base'},
                                     $symsbyname{'_patch_payload_mission_size'});
      $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PAYLOAD;
      # Extract the payload information from the readonly version information
      #  defined by the mission.x file.
      my ($verinfo) = htype_unpack(substr($payload_mission, 0x300, 0x10),
                                   \@version_roinfo_htype);
      $plainhdr{'payload_buildtime'} = $verinfo->{'buildtime'};
      $plainhdr{'payload_buildnum'} = $verinfo->{'buildnum'};
      $plainhdr{'payload_vmajor'} = $verinfo->{'vmajor'};
      $plainhdr{'payload_vminor'} = $verinfo->{'vminor'};
      $plainhdr{'payload_target'} = $verinfo->{'target'};
      $plainhdr{'payload_product'} = $verinfo->{'product'};
      $plainhdr{'payload_siliconrev'} = $verinfo->{'siliconrev'};
      $plainhdr{'payload_formalrel'} = $verinfo->{'formalrel'};
  }
  else {
      $plainhdr{'payload_buildtime'} = 0;
      $plainhdr{'payload_buildnum'} = 0;
      $plainhdr{'payload_vmajor'} = 0;
      $plainhdr{'payload_vminor'} = 0;
      $plainhdr{'payload_target'} = 0;
      $plainhdr{'payload_product'} = 0;
      $plainhdr{'payload_siliconrev'} = 0;
      $plainhdr{'payload_formalrel'} = 0;
  }
  $plainhdr{'seq'} = 0;
  $plainhdr{'unused'} = [ 0, 0, 0 ];
}
elsif ($plainhdrtype == 1) {
  # Plain header vcsfw_cmd_bootldr_patch_plainhdr_type1_t
  if (!defined $opt_payload_elf) {
    croak "A payload ELF file is not defined for plain header type 1";
  }
  # make sure we have all of the symbols we need from the patch ELF
  for my $symname ('_patch_payload_sizemax', '_patch_payload_size',
      '_patch_payload_mission_base', '_patch_payload_mission_size') {
      if (! exists $symsbyname{$symname}) {
          croak "Patch does not define $symname";
      }
  }
  # make sure we have all of the symbols we need from the payload ELF
  if (! exists $pl_symsbyname{'_mission_seclevel'}) {
      croak "Payload does not define '_mission_seclevel'";
  }
  # fetch the payload data from the payload ELF file
  $payload_mission = loadpayload(\$pl_elfdb,
                                 $symsbyname{'_patch_payload_mission_base'},
                                 $symsbyname{'_patch_payload_mission_size'});
  # Extract the payload information from the readonly version information
  #  defined by the mission.x file.
  my ($verinfo) = htype_unpack(substr($payload_mission, 0x300, 0x10),
                               \@version_roinfo_htype);
  $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PAYLOAD;
  if (! defined $fwflav) {
     croak "The firmware flavor is undefined for plain header type 1";
  }
  $plainhdr{'restrict_fwflavor'} = $fwflav;
  $plainhdr{'payload_buildtime'} = $verinfo->{'buildtime'};
  $plainhdr{'payload_buildnum'} = $verinfo->{'buildnum'};
  $plainhdr{'payload_vmajor'} = $verinfo->{'vmajor'};
  $plainhdr{'payload_vminor'} = $verinfo->{'vminor'};
  $plainhdr{'restrict_seclevel'} = $pl_symsbyname{'_mission_seclevel'};
  $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PLAINHDRTYPE1;
  $plainhdr{'padding'} = [ 0, 0, 0 ];
  $plainhdr{'seq'} = 0;
  $plainhdr{'unused'} = [ 0, 0, 0 ];
}
elsif ($plainhdrtype == 2) {
    if (!defined $opt_payload_ilist) {
        croak "The plainhdrtype requires a payload iota list.";
    }
    $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PLAINHDRTYPE2;
    # Plain header vcsfw_cmd_bootldr_patch_plainhdr_type2_t
    # fetch the payload data from the payload iota file
    $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PAYLOAD;
    $plainhdr{'padding'} = [ 0, 0, 0, 0, 0, 0, 0, 0 ];
    $plainhdr{'seq'} = 0;
    $plainhdr{'unused'} = [ 0, 0, 0 ];

    # Load the iota payload.
    my $ilistbin = readinbin($opt_payload_ilist, 'iota list file');
    if (!defined $ilistbin) {
      croak "The iota list file is empty.";
    }

    # Remove the magic value, add the version into the header, and pass the result to the patch.
    ($payload_iota, $plainhdr{'restrict_configid1'}, $plainhdr{'restrict_configid2'},
       $plainhdr{'restrict_configver'}) = iotalist_setup(\$ilistbin);
    # Pad this to the size of the nvm_flash_word_t, which is 8
    $payload_iota = padtoquant($payload_iota, 8, 0);
    $payloadmax = payloadmax_set();

}
else {
  croak "The plainhdrtype '$plainhdrtype' is invalid.";
}

if (defined $opt_custkey) {
    $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_CUSTKEY;
}
if (defined $vpskfile || defined $sigfile) {
    # We're encrypting and signing

    $plainhdr{'flags'} |= ($VCSFW_CMD_BOOTLDR_PATCH_FLAGS_ENCRYPTED
                           | $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_SIGNED);

    if (defined $keyfile) {
        # We're being given a production key file
        $plainhdr{'flags'} |= $VCSFW_CMD_BOOTLDR_PATCH_FLAGS_PROD;
    }
}

######################################################################
# If we have a payload, split it up and generate individual patches,
# appending the payload pieces to the end of each patch
# (subject to the length and padding requirements)
######################################################################
my $payload;
if (defined $payload_mission) {
    my $writehash;
    my $seqnum;
    my $payloadleft;
    my $progaddr;
    my $payloadstart;
    my $hashhdr;
    my $hashwritten;

    # printf("Mission payload starts at  0x%x=%u bytes\n",
    #        length($payload_mission), length($payload_mission));
    # Make sure the mission firmware is padded to the quantum
    if (exists $symsbyname{'_patch_payload_sizequant'}) {
        # Pad the payload to the size quantum
        $payload_mission = padtoquant($payload_mission,
                                       $symsbyname{'_patch_payload_sizequant'},
                                       0xff);
    }
    # At least, pad the payload up to the NVM flash quantum size
    # printf("Padded mission payload #1 is  0x%x=%u bytes\n",
    #        length($payload_mission), length($payload_mission));
    $payload_mission = padtoquant($payload_mission, 8, 0xff);

    # Calculate the hash on the full mission firmware image.
    my $mission_hash = sha256hash(
        padto($payload_mission, $symsbyname{'_patch_payload_mission_size'},
              0xff));
    # printf("Mission hash is  0x%x=%u bytes\n",
    #        length($mission_hash), length($mission_hash));
    $hashhdr = htype_pack({
        'progaddr' => 0,
        'nwords' => length($mission_hash) / 8,
        'type' => $FLASHPROG_HDR_TYPE_MISSIONHASH,
        'flags' => 0}, \@flashprog_hdr_htype);

######################################################################
# This loop may generate multiple patch files.
######################################################################
    # Maximum payload for each piece, taking overhead into account
    # This includes aligning to a 16-byte boundary for the signature.
    $payloadmax = payloadmax_set();
    # printf "payloadmax is %d\n", $payloadmax;
    $payloadleft = length($payload_mission);
    $seqnum = 0;
    $progaddr = $symsbyname{'_patch_payload_mission_base'};
    $payloadstart = 0;
    $hashwritten = 0;
    while ($payloadleft > 0 || ! $hashwritten) {
        my $payloadsize;
        my $hdr;
        my $multifile_name;
        my $plainpatchcopy;

        $payloadsize = $payloadleft - $payloadmax >= 0 ? $payloadmax : $payloadleft;
        #printf "Loop TOP payloadleft %d  progaddr %x payloadstart %d payloadsize %d\n",
        # $payloadleft, $progaddr, $payloadstart, $payloadsize;
        if ($payloadleft > 0) {
            # Construct the full programming payloads.
            $hdr = htype_pack({
                'progaddr' => $progaddr,
                'nwords' => $payloadsize / 8,
                'type' => $FLASHPROG_HDR_TYPE_FLASH,
                'flags' => 0}, \@flashprog_hdr_htype);
            # print "header $seqnum = " . join(', ', map {sprintf("0x%x", $_)} unpack('C*', $hdr)) . "\n";
            $payload = $hdr . substr($payload_mission, $payloadstart, $payloadsize);
            # printf "payload length after hdr + payload %d\n", length($payload);
        }

        # printf "payloadleft %d <= payloadmax %d - hashhdr %d - mission_hash %d\n",
        # $payloadleft, $payloadmax, htype_size(\@flashprog_hdr_htype), length($mission_hash);

        # If we have room for the mission firmware hash, put it in.
        if ($payloadleft <= $payloadmax - htype_size(\@flashprog_hdr_htype)
             - length($mission_hash)) {
            $payload .= $hashhdr . $mission_hash;
            $hashwritten = 1;
            # print "mission hash header = " . join(', ', map {sprintf("0x%x", $_)} unpack('C*', $hashhdr)) . "\n";
        }

        # printf("Full payload is 0x%x=%u bytes\n", length($payload), length($payload));
        if (length($payload) > $symsbyname{'_patch_payload_sizemax'}) {
            croak "Patch is bigger than the maximum payload size $symsbyname{'_patch_payload_sizemax'}";
        }

        # Update the sequence number in the header.
        $plainhdr{'seq'} = $seqnum;

        $plainpatchcopy = $plainpatch;

        #
        # Now set the variables.
        #
        rewrite32(\$plainpatchcopy, $plainstart, $symsbyname{'_patch_payload_size'},
                  length($payload));
        # printf "plainpatchcopy length before payload is %d\n", length($plainpatchcopy);
        $plainpatchcopy .= $payload;
        # printf "plainpatchcopy length with payload %d\n", length($plainpatchcopy);

        # Generate the binary plaintext header
        my $binplainhdr;
        if ($plainhdrtype != 1) {
           croak "With a mission firmware payload, the plainhdr type must be 1. It is currently $plainhdrtype.";
        }
        $binplainhdr = htype_pack(\%plainhdr, \@cmd_bootldr_patch_plainhdr_htype1);


        # Option: We're encrypting and signing the patch.
        if (defined $vpskfile || defined $sigfile) {
            signencrypt(\$binpatch, \$plainpatchcopy, \$binplainhdr,
                   \$vpskfile, \$keyfile, \$signature);
        }
        else {
            $binpatch = $binplainhdr . $plainpatchcopy;
            $signature = "This file does not contain a signature";
        }
        # printf "binpatch length %d, binplainhdr length %d, plainpatch length %d\n",
        #  length($binpatch), length($binplainhdr), length($plainpatchcopy);

        my ($basename,$dir,$suffix) = fileparse($out_file, qr/\.[^.]*/);
        my $multiout_file = $dir . $basename . $seqnum . $suffix;
        print "Opening output file '$multiout_file'.\n";
        open(my $fh, '>', $multiout_file) or croak $!;
        binmode $fh;

        # Option: Generating an ASCII header
        if (defined $opt_genhdr || defined $opt_genc) {
            writeascii($fh, \%plainhdr, \$plainhdrtype, \$plainpatchcopy, \$payload,
            \$enchdr, \$signature, \$cmdline, \$plainstart);
        }

        # Option: Generating a C file
        if (defined $opt_genc) {
            writecfile($fh, \$binpatch);
        }
        else {
            syswrite $fh, $binpatch;
        }

        close $fh;

        $seqnum++;
        $payloadstart += $payloadsize;
        $progaddr += $payloadsize;
        $payloadleft -= $payloadmax;
        undef $hdr;
        undef $payload;
    }
    printf "Load address = 0x%08x\n", $sortedaddrs[0];
    printf "Start address = 0x%08x\n", $elfdb->{'ehdr'}->{'e_entry'};
}
elsif (defined $payload_iota) {

   my $plainpatchcopy;
   my $payloadsize = 0;
   # Construct the payload header
   my $hdr = htype_pack({
       'progaddr' => 0,
       'nwords' => length($payload_iota) / 8,
       'type' => $FLASHPROG_HDR_TYPE_IOTALIST,
       'flags' => 0}, \@flashprog_hdr_htype);
   # print "header = " . join(', ', map {sprintf("0x%x", $_)} unpack('C*', $hdr)) . "\n";
   $payload = $hdr . $payload_iota;

   printf("Full payload is 0x%x=%u bytes\n", length($payload), length($payload));
   if (length($payload) > $symsbyname{'_patch_payload_sizemax'}) {
      croak "Patch is bigger than the maximum payload size $symsbyname{'_patch_payload_sizemax'}";
   }

   $plainpatchcopy = $plainpatch;

   #
   # Now set the variables.
   #
   rewrite32(\$plainpatchcopy, $plainstart, $symsbyname{'_patch_payload_size'},
             length($payload));
   # printf "plainpatchcopy length before payload is %d\n", length($plainpatchcopy);
   $plainpatchcopy .= $payload;
   # printf "plainpatchcopy length with payload %d\n", length($plainpatchcopy);

   # Generate the binary plaintext header
   my $binplainhdr;
   if ($plainhdrtype != 2) {
      croak "With an iota payload, the plainhdr type must be 2. It is currently $plainhdrtype.";
   }
   $binplainhdr = htype_pack(\%plainhdr, \@cmd_bootldr_patch_plainhdr_htype2);

   # Option: We're encrypting and signing the patch.
   if (defined $vpskfile || defined $sigfile) {
       signencrypt(\$binpatch, \$plainpatchcopy, \$binplainhdr,
              \$vpskfile, \$keyfile, \$signature);
   }
   else {
       $binpatch = $binplainhdr . $plainpatchcopy;
       $signature = "This file does not contain a signature";
   }
   # printf "binpatch length %d, binplainhdr length %d, plainpatch length %d\n",
   #  length($binpatch), length($binplainhdr), length($plainpatchcopy);

   print "Opening output file '$out_file'.\n";
   open(my $fh, '>', $out_file) or croak $!;
   binmode $fh;

   # Option: Generating an ASCII header
   if (defined $opt_genhdr || defined $opt_genc) {
       writeascii($fh, \%plainhdr, \$plainhdrtype, \$plainpatchcopy, \$payload,
       \$enchdr, \$signature, \$cmdline, \$plainstart);
   }

   # Option: Generating a C file
   if (defined $opt_genc) {
       writecfile($fh, \$binpatch);
   }
   else {
       syswrite $fh, $binpatch;
   }

   close $fh;
}
else {
######################################################################
# No mission firmware payload here
######################################################################
   # Generate the binary plaintext header
   my $binplainhdr;
   if ($plainhdrtype == 0) {
     $binplainhdr = htype_pack(\%plainhdr, \@cmd_bootldr_patch_plainhdr_htype0);
   }
   elsif ($plainhdrtype == 1) {
     $binplainhdr = htype_pack(\%plainhdr, \@cmd_bootldr_patch_plainhdr_htype1);
   }
   elsif ($plainhdrtype == 2) {
     $binplainhdr = htype_pack(\%plainhdr, \@cmd_bootldr_patch_plainhdr_htype2);
   }

   # Option: We're encrypting and signing the patch.
   if (defined $vpskfile || defined $sigfile) {
       signencrypt(\$binpatch, \$plainpatch, \$binplainhdr,
              \$vpskfile, \$keyfile, \$signature);
   }
   else {
       $binpatch = $binplainhdr . $plainpatch;
       $signature = "This file does not contain a signature";
   }

   open(my $fh, '>', $out_file) or croak $!;
   binmode $fh;

   # Option: Generating an ASCII header
   if (defined $opt_genhdr || defined $opt_genc) {
       writeascii($fh, \%plainhdr, \$plainhdrtype, \$plainpatch, \$payload,
       \$enchdr, \$signature, \$cmdline, \$plainstart);
   }

   # Option: Generating a C file
   if (defined $opt_genc) {
       writecfile($fh, \$binpatch);
   }
   else {
       syswrite $fh, $binpatch;
   }

   close $fh;

   printf "Load address = 0x%08x\n", $sortedaddrs[0];

   printf "Start address = 0x%08x\n", $elfdb->{'ehdr'}->{'e_entry'};
}
