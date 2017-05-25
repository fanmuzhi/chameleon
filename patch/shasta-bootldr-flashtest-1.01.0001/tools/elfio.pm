# $Header: /test/engineering/private/impl/micro/eagle/tools/elfio.pm,v 1.6 2011/12/07 17:08:36 bdavis Exp $
#
# This unpublished code is Copyright (c) 2011-2012, Validity Sensors, Inc.
# All rights reserved.
#
# A generic ELF I/O library.
#
# Bjoren Davis, March 3, 2011.
#

#
# Note that the authoritative copy of this file lives in the
#  Validity CVS in engineering/private/impl/micro/eagle/tools/elfio.pm,v.
#  Other copies include:
#   + engineering/private/impl/micro/windsor/fwcmn/tools/elfio.pm
#
# Please make all edits to the authoritative copy in
#  engineering/private/impl/micro/eagle/tools/elfio.pm,v and then
#  copy the edits to the other copies.
#  

# References:
#   Tool Interface Standard (TIS) Executable and Linking Format
#         (ELF) Specification, Version 1.2, TIS Committee, May 1995,
#    available from: http://refspecs.freestandards.org/elf/elf.pdf
#
#  DWARF Debugging Information Format, Version 3, DWARF Debugging
#    Information Format Workgroup, December 20, 2005.
#         available from:  http://dwarfstd.org/doc/Dwarf3.pdf
#
#  GNU binutils 2.16.1 source code.
#    Available from http://ftp.gnu.org/gnu/binutils/binutils-2.16.1.tar.bz2
#    Specific files to look at are:
#       binutils-2.16.1/include/elf/common.h
#       binutils-2.16.1/include/elf/external.h
#

package elfio;

use strict;
use Carp;
use warnings;
use Data::Dumper;

=head1 NAME

elfio - Perl module to allow 

=head1 SYNOPSIS

    use elfio;

=head1 DESCRIPTION

The C<elfio> package allows you to read and write ELF files.

=cut

use vars qw($VERSION @ISA @EXPORT @EXPORT_OK);

$VERSION = '1.0';

@ISA               = qw(Exporter);
@EXPORT            = qw(elfin elfout);
@EXPORT_OK         = qw(elfin elfout);

my @known_opts_in     = qw();

my @known_opts_out    = qw();
my $strict_mode     = 0;
my $elf64_mode      = 0;
my $elf_bigendian   = 0;

use constant {
    ##
    # Elf32_Ehdr:
    ##
    # typedef struct {
    #     unsigned char e_ident[EI_NIDENT];
    #     Elf32_Half    e_type;
    #     Elf32_Half    e_machine;
    #     Elf32_Word    e_version;
    #     Elf32_Addr    e_entry;
    #     Elf32_Off     e_phoff;
    #     Elf32_Off     e_shoff;
    #     Elf32_Word    e_flags;
    #     Elf32_Half    e_ehsize;
    #     Elf32_Half    e_phentsize;
    #     Elf32_Half    e_phnum;
    #     Elf32_Half    e_shentsize;
    #     Elf32_Half    e_shnum;
    #     Elf32_Half    e_shstrndx;
    # } Elf32_Ehdr;

    # Values for Elf32_Ehdr::e_ident (16 unsigned char array)
    EI_MAG0 => 0,                       # File identification
    EI_MAG1 => 1,                       # File identification
    EI_MAG2 => 2,                       # File identification
    EI_MAG3 => 3,                       # File identification
    EI_CLASS => 4,                      # File class
    EI_DATA => 5,                       # Data encoding
    EI_VERSION => 6,                    # File version
    EI_PAD => 7,                        # Start of padding bytes
    EI_NIDENT => 16,                    # Size of e_ident[]

    # Values for Elf32_Ehdr::e_ident[EI_CLASS=4] (unsigned 8 bit integer):
    ELFCLASSNONE => 0,                  # Invalid class
    ELFCLASS32 => 1,                    # 32-bit objects
    ELFCLASS64 => 2,                    # 64-bit objects

    # Values for Elf32_Ehdr::e_ident[EI_DATA=5] (unsigned 8 bit integer):
    ELFDATANONE => 0,                   # Invalid data encoding
    ELFDATA2LSB => 1,                   # Little endian
    ELFDATA2MSB => 2,                   # Big endian

    # Values for Elf32_Ehdr::e_ident[EI_VERSION=6] (unsigned 8 bit integer):
    #  and Elf32_Ehdr::e_version (32-bit unsigned word):
    EV_NONE => 0,                       # Invalid version
    EV_CURRENT => 1,                    # Current version
    
    # Values for Elf32_Ehdr::e_type (16 bit unsigned integer)
    ET_NONE => 0,                       # No file type
    ET_REL => 1,                        # Relocatable file
    ET_EXEC => 2,                       # Executable file
    ET_DYN => 3,                        # Shared object file
    ET_CORE => 4,                       # Core file
    ET_LOPROC => 0xff00,                # Processor-specific
    ET_HIPROC => 0xffff,                # Processor-specific

    # Values for Elf32_Ehdr::e_machine (16 bit unsigned integer)

    # These values are from http://www.sco.com/developers/gabi/latest/ch4.eheader.html#machine,
    #  downloaded September 28, 2010.
    EM_NONE => 0,               # No machine
    EM_M32 => 1,                # AT&T WE 32100
    EM_SPARC => 2,              # SPARC
    EM_386 => 3,                # Intel 80386
    EM_68K => 4,                # Motorola 68000
    EM_88K => 5,                # Motorola 88000
                                # 6 is Reserved for future use (was EM_486)
    EM_860 => 7,                # Intel 80860
    EM_MIPS => 8,               # MIPS I Architecture
    EM_S370 => 9,               # IBM System/370 Processor
    EM_MIPS_RS3_LE => 10,       # MIPS RS3000 Little-endian
                                # 11-14 are Reserved for future use
    EM_PARISC => 15,            # Hewlett-Packard PA-RISC
                                # 16 Reserved for future use
    EM_VPP500 => 17,            # Fujitsu VPP500
    EM_SPARC32PLUS => 18,       # Enhanced instruction set SPARC
    EM_960 => 19,               # Intel 80960
    EM_PPC => 20,               # PowerPC
    EM_PPC64 => 21,             # 64-bit PowerPC
    EM_S390 => 22,              # IBM System/390 Processor
    EM_SPU => 23,               # IBM SPU/SPC
                                # 24-35 are Reserved for future use
    EM_V800 => 36,              # NEC V800
    EM_FR20 => 37,              # Fujitsu FR20
    EM_RH32 => 38,              # TRW RH-32
    EM_RCE => 39,               # Motorola RCE
    EM_ARM => 40,               # Advanced RISC Machines ARM
    EM_ALPHA => 41,             # Digital Alpha
    EM_SH => 42,                # Hitachi SH
    EM_SPARCV9 => 43,           # SPARC Version 9
    EM_TRICORE => 44,           # Siemens TriCore embedded processor
    EM_ARC => 45,               # Argonaut RISC Core, Argonaut Technologies Inc.
    EM_H8_300 => 46,            # Hitachi H8/300
    EM_H8_300H => 47,           # Hitachi H8/300H
    EM_H8S => 48,               # Hitachi H8S
    EM_H8_500 => 49,            # Hitachi H8/500
    EM_IA_64 => 50,             # Intel IA-64 processor architecture
    EM_MIPS_X => 51,            # Stanford MIPS-X
    EM_COLDFIRE => 52,          # Motorola ColdFire
    EM_68HC12 => 53,            # Motorola M68HC12
    EM_MMA => 54,               # Fujitsu MMA Multimedia Accelerator
    EM_PCP => 55,               # Siemens PCP
    EM_NCPU => 56,              # Sony nCPU embedded RISC processor
    EM_NDR1 => 57,              # Denso NDR1 microprocessor
    EM_STARCORE => 58,          # Motorola Star*Core processor
    EM_ME16 => 59,              # Toyota ME16 processor
    EM_ST100 => 60,             # STMicroelectronics ST100 processor
    EM_TINYJ => 61,             # Advanced Logic Corp. TinyJ embedded processor family
    EM_X86_64 => 62,            # AMD x86-64 architecture
    EM_PDSP => 63,              # Sony DSP Processor
    EM_PDP10 => 64,             # Digital Equipment Corp. PDP-10
    EM_PDP11 => 65,             # Digital Equipment Corp. PDP-11
    EM_FX66 => 66,              # Siemens FX66 microcontroller
    EM_ST9PLUS => 67,           # STMicroelectronics ST9+ 8/16 bit microcontroller
    EM_ST7 => 68,               # STMicroelectronics ST7 8-bit microcontroller
    EM_68HC16 => 69,            # Motorola MC68HC16 Microcontroller
    EM_68HC11 => 70,            # Motorola MC68HC11 Microcontroller
    EM_68HC08 => 71,            # Motorola MC68HC08 Microcontroller
    EM_68HC05 => 72,            # Motorola MC68HC05 Microcontroller
    EM_SVX => 73,               # Silicon Graphics SVx
    EM_ST19 => 74,              # STMicroelectronics ST19 8-bit microcontroller
    EM_VAX => 75,               # Digital VAX
    EM_CRIS => 76,              # Axis Communications 32-bit embedded processor
    EM_JAVELIN => 77,           # Infineon Technologies 32-bit embedded processor
    EM_FIREPATH => 78,          # Element 14 64-bit DSP Processor
    EM_ZSP => 79,               # LSI Logic 16-bit DSP Processor
    EM_MMIX => 80,              # Donald Knuth's educational 64-bit processor
    EM_HUANY => 81,             # Harvard University machine-independent object files
    EM_PRISM => 82,             # SiTera Prism
    EM_AVR => 83,               # Atmel AVR 8-bit microcontroller
    EM_FR30 => 84,              # Fujitsu FR30
    EM_D10V => 85,              # Mitsubishi D10V
    EM_D30V => 86,              # Mitsubishi D30V
    EM_V850 => 87,              # NEC v850
    EM_M32R => 88,              # Mitsubishi M32R
    EM_MN10300 => 89,           # Matsushita MN10300
    EM_MN10200 => 90,           # Matsushita MN10200
    EM_PJ => 91,                # picoJava
    EM_OPENRISC => 92,          # OpenRISC 32-bit embedded processor
    EM_ARC_COMPACT => 93,       # ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)
    EM_XTENSA => 94,            # Tensilica Xtensa Architecture
    EM_VIDEOCORE => 95,         # Alphamosaic VideoCore processor
    EM_TMM_GPP => 96,           # Thompson Multimedia General Purpose Processor
    EM_NS32K => 97,             # National Semiconductor 32000 series
    EM_TPC => 98,               # Tenor Network TPC processor
    EM_SNP1K => 99,             # Trebia SNP 1000 processor
    EM_ST200 => 100,            # STMicroelectronics (www.st.com) ST200 microcontroller
    EM_IP2K => 101,             # Ubicom IP2xxx microcontroller family
    EM_MAX => 102,              # MAX Processor
    EM_CR => 103,               # National Semiconductor CompactRISC microprocessor
    EM_F2MC16 => 104,           # Fujitsu F2MC16
    EM_MSP430 => 105,           # Texas Instruments embedded microcontroller msp430
    EM_BLACKFIN => 106,         # Analog Devices Blackfin (DSP) processor
    EM_SE_C33 => 107,           # S1C33 Family of Seiko Epson processors
    EM_SEP => 108,              # Sharp embedded microprocessor
    EM_ARCA => 109,             # Arca RISC Microprocessor
    EM_UNICORE => 110,          # Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University
    EM_EXCESS => 111,           # eXcess: 16/32/64-bit configurable embedded CPU
    EM_DXP => 112,              # Icera Semiconductor Inc. Deep Execution Processor
    EM_ALTERA_NIOS2 => 113,     # Altera Nios II soft-core processor
    EM_CRX => 114,              # National Semiconductor CompactRISC CRX microprocessor
    EM_XGATE => 115,            # Motorola XGATE embedded processor
    EM_C166 => 116,             # Infineon C16x/XC16x processor
    EM_M16C => 117,             # Renesas M16C series microprocessors
    EM_DSPIC30F => 118,         # Microchip Technology dsPIC30F Digital Signal Controller
    EM_CE => 119,               # Freescale Communication Engine RISC core
    EM_M32C => 120,             # Renesas M32C series microprocessors
                                # 121-130 are Reserved for future use
    EM_TSK3000 => 131,          # Altium TSK3000 core
    EM_RS08 => 132,             # Freescale RS08 embedded processor
                                # 133 is Reserved for future Analog Devices DSP microprocessor
    EM_ECOG2 => 134,            # Cyan Technology eCOG2 microprocessor
    EM_SCORE7 => 135,           # Sunplus S+core7 RISC processor
    EM_DSP24 => 136,            # New Japan Radio (NJR) 24-bit DSP Processor
    EM_VIDEOCORE3 => 137,       # Broadcom VideoCore III processor
    EM_LATTICEMICO32 => 138,    # RISC processor for Lattice FPGA architecture
    EM_SE_C17 => 139,           # Seiko Epson C17 family
    EM_TI_C6000 => 140,         # The Texas Instruments TMS320C6000 DSP family
    EM_TI_C2000 => 141,         # The Texas Instruments TMS320C2000 DSP family
    EM_TI_C5500 => 142,         # The Texas Instruments TMS320C55x DSP family
                                # 143-159 are Reserved for future use
    EM_MMDSP_PLUS => 160,       # STMicroelectronics 64bit VLIW Data Signal Processor
    EM_CYPRESS_M8C => 161,      # Cypress M8C microprocessor
    EM_R32C => 162,             # Renesas R32C series microprocessors
    EM_TRIMEDIA => 163,         # NXP Semiconductors TriMedia architecture family
    EM_QDSP6 => 164,            # QUALCOMM DSP6 Processor
    EM_8051 => 165,             # Intel 8051 and variants
    EM_STXP7X => 166,           # STMicroelectronics STxP7x family of configurable and extensible RISC processors
    EM_NDS32 => 167,            # Andes Technology compact code size embedded RISC processor family
    EM_ECOG1 => 168,            # Cyan Technology eCOG1X family
    EM_ECOG1X => 168,           # Cyan Technology eCOG1X family
    EM_MAXQ30 => 169,           # Dallas Semiconductor MAXQ30 Core Micro-controllers
    EM_XIMO16 => 170,           # New Japan Radio (NJR) 16-bit DSP Processor
    EM_MANIK => 171,            # M2000 Reconfigurable RISC Microprocessor
    EM_CRAYNV2 => 172,          # Cray Inc. NV2 vector architecture
    EM_RX => 173,               # Renesas RX family
    EM_METAG => 174,            # Imagination Technologies META processor architecture
    EM_MCST_ELBRUS => 175,      # MCST Elbrus general purpose hardware architecture
    EM_ECOG16 => 176,           # Cyan Technology eCOG16 family
    EM_CR16 => 177,             # National Semiconductor CompactRISC CR16 16-bit microprocessor
    EM_ETPU => 178,             # Freescale Extended Time Processing Unit
    EM_SLE9X => 179,            # Infineon Technologies SLE9X core
                                # 180-182 are Reserved for future Intel use
                                # 183-184 are Reserved for future ARM use
    EM_AVR32 => 185,            # Atmel Corporation 32-bit microprocessor family
    EM_STM8 => 186,             # STMicroeletronics STM8 8-bit microcontroller
    EM_TILE64 => 187,           # Tilera TILE64 multicore architecture family
    EM_TILEPRO => 188,          # Tilera TILEPro multicore architecture family
    EM_MICROBLAZE => 189,       # Xilinx MicroBlaze 32-bit RISC soft processor core
    EM_CUDA => 190,             # NVIDIA CUDA architecture
    EM_TILEGX => 191,           # Tilera TILE-Gx multicore architecture family
    EM_CLOUDSHIELD => 192,      # CloudShield architecture family

    # Values for Elf32_Ehdr::e_version, see above

    ##
    # Elf32_Shdr:
    ##
    # typedef struct {
    #     Elf32_Word sh_name;
    #     Elf32_Word sh_type;
    #     Elf32_Word sh_flags;
    #     Elf32_Addr sh_addr;
    #     Elf32_Off  sh_offset;
    #     Elf32_Word sh_size;
    #     Elf32_Word sh_link;
    #     Elf32_Word sh_info;
    #     Elf32_Word sh_addralign;
    #     Elf32_Word sh_entsize;
    # } Elf32_Shdr;

    # Values for Elf32_Shdr::sh_type (32 bit unsigned integer)
    SHT_NULL => 0,                      # Inactive, no associated section
    SHT_PROGBITS => 1,                  # Contains info. defined by program
    SHT_SYMTAB => 2,                    # Symbol table
    SHT_STRTAB => 3,                    # String table
    SHT_RELA => 4,                      # Relocation entries
    SHT_HASH => 5,                      # Symbol hash table
    SHT_DYNAMIC => 6,                   # Information for dynamic linking
    SHT_NOTE => 7,                      # "Information that marks the file"
    SHT_NOBITS => 8,                    # No space filling, like PROGBITS
    SHT_REL => 9,                       # Relocation entries w/o explicit
                                        #  addends
    SHT_SHLIB => 10,                    # Reserved
    SHT_DYNSYM => 11,                   # Symbol table
    SHT_LOPROC => 0x70000000,           # Processor-specific range, low bound
    SHT_HIPROC => 0x7fffffff,           # Processor-specific range, high bound
    SHT_LOUSER => 0x80000000,           # User-specific range, low bound
    SHT_HIUSER => 0xffffffff,           # User-specific range, high bound

    #
    # Section number indices:
    #
    SHN_UNDEF => 0,                     # Undefined, missing, irrelevant
    SHN_LORESERVE => 0xff00,            # Lower bound of range of reserved idxs
    SHN_LOPROC => 0xff00,               # Processor-specific range, low
    SHN_HIPROC => 0xff1f,               # Processor-specific range, high
    SHN_ABS => 0xfff1,                  # Absolute values
    SHN_COMMON => 0xfff2,               # FORTRAN COMMON block
    SHN_HIRESERVE => 0xffff,            # High bound of range of reserved idxs

    #
    # Section attribute flags (sh_flags):
    #
    SHF_WRITE => 0x1,                   # Writable during execution
    SHF_ALLOC => 0x2,                   # Occupies memory during execution
    SHF_EXECINSTR => 0x4,               # Contains executable instructions
    SHF_MASKPROC => 0xf0000000,         # Reserved mask for processor-specifics

    #
    # Symbol binding (encoded in st_info):
    #
    STB_LOCAL => 0,                     # Local symbol 
    STB_GLOBAL => 1,                    # Global symbol
    STB_WEAK => 2,                      # Weak global symbol
    STB_LOPROC => 13,                   # Beginning of processor specifics
    STB_HIPROC => 15,                   # End of processor specifics

    #
    # Symbol type (encoded in st_info):
    #
    STT_NOTYPE => 0,                    # Not specified
    STT_OBJECT => 1,                    # Data object (variable, array, etc.)
    STT_FUNC => 2,                      # Function or executable code
    STT_SECTION => 3,                   # Associated with a section
    STT_FILE => 4,                      # Associated with a file
    STT_LOPROC => 13,                   # Beginning of processor specifics
    STT_HIPROC => 15,                   # End of processor specifics

    #
    # Segment types (p_type)
    #
    PT_NULL => 0,                       # Unused.  Ignore this phdr
    PT_LOAD => 1,                       # Loadable segment
    PT_DYNAMIC => 2,                    # Contains dynamic linking information
    PT_INTERP => 3,                     # NULL-terminated interpreter name
    PT_NOTE => 4,                       # Contains auxiliary information
    PT_SHLIB => 5,                      # Reserved
    PT_PHDR => 6,                       # Program header
    PT_LOPROC => 0x70000000,            # Beginning of processor specifics
    PT_HIPROC => 0x7fffffff,            # End of processor specifics

    #
    # Symbol table entries
    #
    STN_UNDEF => 0,                     # Undefined.  Use 0 for relocation.

};

#
# Primitive types.
#
# signed, elf32 size, elf64 size, elf32 name, elf64 name
my @primtype_addr =   ( 0,     4,  8,  'Elf32_Addr', 'Elf64_Addr' );
my @primtype_half =   ( 0,     2,  2,  'Elf32_Half', 'Elf64_Half');
my @primtype_off =    ( 0,     4,  8,   'Elf32_Off', 'Elf64_Off' );
my @primtype_sword =  ( 1,     4,  4, 'Elf32_Sword', 'Elf64_Sword' );
my @primtype_word =   ( 0,     4,  4,  'Elf32_Word', 'Elf64_Word' );
my @primtype_xword =  ( 0, undef,  8,         undef, 'Elf64_Xword' );
my @primtype_sxword = ( 1, undef,  8,         undef, 'Elf64_Sxword' );
my @primtype_char =   ( 0,     1,  1,        'char', 'char' );
my @primtype_ident =  ( 0,    16, 16,    'char[16]', 'char[16]' );


#
# Some structures.
#
# Structures are arrays of list references.

# Elf32_Ehdr/Elf64_Ehdr:
my @struct_ehdr = (
# type, name, enumref
    [ \@primtype_ident, 'e_ident', undef ],
    [ \@primtype_half, 'e_type', undef ],
    [ \@primtype_half, 'e_machine', undef ],
    [ \@primtype_word, 'e_version', undef ],
    [ \@primtype_addr, 'e_entry', undef ],
    [ \@primtype_off, 'e_phoff', undef ],
    [ \@primtype_off, 'e_shoff', undef ],
    [ \@primtype_word, 'e_flags', undef ],
    [ \@primtype_half, 'e_ehsize', undef ],
    [ \@primtype_half, 'e_phentsize', undef ],
    [ \@primtype_half, 'e_phnum', undef ],
    [ \@primtype_half, 'e_shentsize', undef ],
    [ \@primtype_half, 'e_shnum', undef ],
    [ \@primtype_half, 'e_shstrndx', undef ]
);


# Elf32_Shdr/Elf64_Shdr:
my @struct_shdr = (
# type, name, enumref
    [ \@primtype_word, 'sh_name', undef ],
    [ \@primtype_word, 'sh_type', undef ],
    [ \@primtype_word, 'sh_flags', undef ],
    [ \@primtype_addr, 'sh_addr', undef ],
    [ \@primtype_off, 'sh_offset', undef ],
    [ \@primtype_word, 'sh_size', undef ],
    [ \@primtype_word, 'sh_link', undef ],
    [ \@primtype_word, 'sh_info', undef ],
    [ \@primtype_word, 'sh_addralign', undef ],
    [ \@primtype_word, 'sh_entsize', undef ]
);

# Elf32_Sym:
my @struct_sym32 = (
# type, name, enumref
    [ \@primtype_word, 'st_name', undef ],
    [ \@primtype_addr, 'st_value', undef ],
    [ \@primtype_word, 'st_size', undef ],
    [ \@primtype_char, 'st_info', undef ],
    [ \@primtype_char, 'st_other', undef ],
    [ \@primtype_half, 'st_shndx', undef ]
);

# Elf64_Sym:
my @struct_sym64 = (
# type, name, enumref
    [ \@primtype_word, 'st_name', undef ],
    [ \@primtype_char, 'st_info', undef ],
    [ \@primtype_char, 'st_other', undef ],
    [ \@primtype_half, 'st_shndx', undef ],
    [ \@primtype_addr, 'st_value', undef ],
    [ \@primtype_xword, 'st_size', undef ]
);

# Elf32_Rel:
my @struct_rel32 = (
# type, name, enumref
    [ \@primtype_addr, 'r_offset', undef ],
    [ \@primtype_word, 'r_info', undef ]
);

# Elf64_Rel:
my @struct_rel64 = (
# type, name, enumref
    [ \@primtype_addr, 'r_offset', undef ],
    [ \@primtype_xword, 'r_info', undef ]
);

# Elf32_Rela
my @struct_rela32 = (
# type, name, enumref
    [ \@primtype_addr, 'r_offset', undef ],
    [ \@primtype_word, 'r_info', undef ],
    [ \@primtype_sword, 'r_addend', undef ]
);

# Elf64_Rela:
my @struct_rela64 = (
# type, name, enumref
    [ \@primtype_addr, 'r_offset', undef ],
    [ \@primtype_xword, 'r_info', undef ],
    [ \@primtype_sxword, 'r_addend', undef ]
);

# Elf32_Phdr:
my @struct_phdr32 = (
# type, name, enumref
    [ \@primtype_word, 'p_type', undef ],
    [ \@primtype_off, 'p_offset', undef ],
    [ \@primtype_addr, 'p_vaddr', undef ],
    [ \@primtype_addr, 'p_paddr', undef ],
    [ \@primtype_word, 'p_filesz', undef ],
    [ \@primtype_word, 'p_memsz', undef ],
    [ \@primtype_word, 'p_flags', undef ],
    [ \@primtype_word, 'p_align', undef ]
);

# Elf64_Phdr:
my @struct_phdr64 = (
# type, name, enumref
    [ \@primtype_word, 'p_type', undef ],
    [ \@primtype_word, 'p_flags', undef ],
    [ \@primtype_off, 'p_offset', undef ],
    [ \@primtype_addr, 'p_vaddr', undef ],
    [ \@primtype_addr, 'p_paddr', undef ],
    [ \@primtype_xword, 'p_filesz', undef ],
    [ \@primtype_xword, 'p_memsz', undef ],
    [ \@primtype_xword, 'p_align', undef ]
);


#
# Wrapper for Exporter - handles ':strict'
#

sub import {
  # Handle the :strict tag
  
  $strict_mode = 1 if grep { /^:strict$/ } @_;

  # Pass everything else to Exporter.pm

  @_ = grep { !/^:strict$/ } @_;
  goto &Exporter::import;
}


#
# Constructor for object interface.
#

sub new {
  my $class = shift;

  if (@_ % 2) {
    croak 'Default options must be name=>value pairs (odd number supplied)';
  }

  my %known_opt;
  @known_opt{@known_opts_in, @known_opts_out} = (undef) x 100;

  my %raw_opt = @_;
  my %def_opt;
  while(my($key, $val) = each %raw_opt) {
    my $lkey = lc($key);
    $lkey =~ s/_//g;
    croak "Unrecognized option: $key" unless(exists($known_opt{$lkey}));
    $def_opt{$lkey} = $val;
  }
  my $self = { def_opt => \%def_opt };

  return(bless($self, $class));
}

#
# _get_object()
#
# Helper routine called from elfin() and elfout() to create an object if none
# was provided.  Note, this routine does mess with the caller's @_ array.
#

sub _get_object {
  my $self;
  if ($_[0]  and  UNIVERSAL::isa($_[0], 'elfio')) {
    $self = shift;
  }
  else {
    $self = elfio->new();
  }
  
  return $self;
}


#
# Read in the given ELF file.
#
sub elfin {
    my $self = &_get_object;      # note, @_ is passed implicitly
    my $fn = shift;
    my $headerdata;
    my $identdata;
    my $ehdrref;
    my $stabref;
    my %ret;
    my @ident;

    open(my $elffh, '<', $fn) || croak "elfin can't open $fn for input: $!";
    binmode($elffh) || croak "elfin can't switch $fn to bindmode: $!";

    # Read in the first 16 bytes in order to figure out the
    #  endianness and the size of what's to follow
    read $elffh, $identdata, EI_NIDENT 
        || croak "Can't read ident data from $fn: $!";

    @ident = unpack(sprintf('C%u', EI_NIDENT), $identdata);
    if ($ident[EI_MAG0] != 0x7f
        || $ident[EI_MAG1] != ord('E')
        || $ident[EI_MAG2] != ord('L')
        || $ident[EI_MAG3] != ord('F')) {
        croak "Invalid ELF header in $fn: incorrect magic number";
    }
    
    if ($ident[EI_CLASS] == ELFCLASS64) {
        $elf64_mode = 1;
    }
    elsif ($ident[EI_CLASS] != ELFCLASS32) {
        croak "Invalid ELF header in $fn: class is neither 32 nor 64 bit";
    }

    if ($ident[EI_DATA] == ELFDATA2MSB) {
        $elf_bigendian = 1;
    }
    elsif ($ident[EI_DATA] != ELFDATA2LSB) {
        croak "Invalid ELF header in $fn: class is neither big nor little endian";
    }

    if ($ident[EI_VERSION] != EV_CURRENT) {
        croak "Invalid ELF header in $fn: version is not current";
    }

    # Now we know the size of the rest of the header
    read $elffh, $headerdata,
       ($self->_sizestruct(\@struct_ehdr) - EI_NIDENT) 
           || croak "Can't read header from $fn: $!";

    $headerdata = $identdata . $headerdata;

    # Now we can decode the header
    $ehdrref = $self->_decodestruct($headerdata, \@struct_ehdr);

    $ret{'ehdr'} = $ehdrref;

    # Now read the program headers, if any exist.
    if ($$ehdrref{'e_phoff'} != 0) {
        my $phdrdata;
        my @phdrarray;
        my $hdridx;

        for ($hdridx = 0; $hdridx < $$ehdrref{'e_phnum'}; $hdridx++) {
            my $thisphdrref;
            seek($elffh, ($$ehdrref{'e_phoff'}
                          + ($hdridx * $$ehdrref{'e_phentsize'})), 0)
                || croak "Can't seek to read program header index $hdridx: $!";

            read($elffh, $phdrdata, $$ehdrref{'e_phentsize'})
                || croak "Can't read program header index $hdridx from $fn: $!";

            if ($elf64_mode) {
                $thisphdrref = $self->_decodestruct($phdrdata, \@struct_phdr64);
            }
            else {
                $thisphdrref = $self->_decodestruct($phdrdata, \@struct_phdr32);
            }
            $$thisphdrref{'original_index'} = $hdridx;
            push @phdrarray, $thisphdrref;
        }
        $ret{'phdrs'} = \@phdrarray;
    }
            
    # And finally the section headers and section data, if any
    if ($$ehdrref{'e_shoff'} != 0) {
        my $shdrdata;
        my @shdrarray;
        my $hdridx;

        for ($hdridx = 0; $hdridx < $$ehdrref{'e_shnum'}; $hdridx++) {
            my $shdrref;
            my $phdrref;

            seek($elffh, ($$ehdrref{'e_shoff'}
                          + ($hdridx * $$ehdrref{'e_shentsize'})), 0)
                || croak "Can't seek to read section header index $hdridx: $!";

            read($elffh, $shdrdata, $$ehdrref{'e_shentsize'})
                || croak "Can't read section header index $hdridx from $fn: $!";

            $shdrref = $self->_decodestruct($shdrdata, \@struct_shdr);

#           # Now actually read the data, if any

            if ($$shdrref{'sh_type'} != SHT_NOBITS
                && $$shdrref{'sh_type'} != SHT_NULL
                && $$shdrref{'sh_size'} > 0) {
                my $secdata;
                seek($elffh, $$shdrref{'sh_offset'}, 0)
                    || croak "Can't seek to read section data index $hdridx: $!";
                read($elffh, $secdata, $$shdrref{'sh_size'})
                    || croak "Can't read section data index $hdridx: $!";
                $$shdrref{'data'} = $secdata;
            }
            $$shdrref{'original_index'} = $hdridx;
            $phdrref = $self->_phdrlookup(\%ret, $$shdrref{'sh_offset'});
            if (defined $phdrref) {
                $$shdrref{'phdr_ref'} = $phdrref;
                #
                # Figure out what the base physical address
                #  this section corresponds to.
                #
                $$shdrref{'loadaddr'} = $phdrref->{'p_paddr'} +
                    ($$shdrref{'sh_offset'} - $phdrref->{'p_offset'});
            }
            push @shdrarray, $shdrref;
        }
        $ret{'secs'} = \@shdrarray;
    }

    #
    # We're done with the file.  We can close it out.
    #

    close $elffh or carp "Error closing $fn: $!";

    # Now, find the section header string table, if any, and translate.
    if ($$ehdrref{'e_shstrndx'} != SHN_UNDEF) {
        if (exists $ret{'secs'}[$$ehdrref{'e_shstrndx'}]) {
            my $shstrndx = $ret{'secs'}[$$ehdrref{'e_shstrndx'}];
            #
            # Grind through the section header names.
            #

            foreach my $secref (@{$ret{'secs'}}) {
                if ($$secref{'sh_name'} != 0) {
                    $$secref{'name'} = $self->_pullstr(\$$shstrndx{'data'},
                                                       $$secref{'sh_name'});
                }
            }
        }
    }

    my ($strtabref) = grep { defined $$_{'name'} && $$_{'name'} eq '.strtab' } @{$ret{'secs'}};

    #
    # Now decode the symbol table sections
    #

    foreach my $secref (grep { $$_{'sh_type'} == SHT_SYMTAB } 
                        (@{$ret{'secs'}})) {
        my @symentries = ();
        my $entsize = $$secref{'sh_entsize'};
        my $nentries = length($$secref{'data'}) / $entsize;
#           print "symbol table found, entsize=$entsize, nentries=$nentries.\n";

        for (my $idx = 0; $idx < $nentries; $idx++) {
            my $datum = substr($$secref{'data'}, $idx * $entsize, $entsize);
#               print "symbol entry $idx: ", Dumper($datum), "\n";
            my $seref;
            if ($elf64_mode) {
                $seref = $self->_decodestruct($datum, \@struct_sym64);
            }
            else {
                $seref = $self->_decodestruct($datum, \@struct_sym32);
            }

            if (defined($$seref{'st_name'})
                && $$seref{'st_name'} != 0) {
                $$seref{'name'} = $self->_pullstr(\$$strtabref{'data'},
                                                  $$seref{'st_name'});
            }

            $$seref{'type'} = $$seref{'st_info'} & 0x0f;
            $$seref{'bind'} = $$seref{'st_info'} >> 4;

            if (exists $ret{'secs'}[$$seref{'st_shndx'}]) {
                $$seref{'secref'} =
                    $ret{'secs'}[$$seref{'st_shndx'}];
            }

#               print "decodes to: ", Dumper(\%symentry), "\n";

            push @symentries, $seref;
        }

        if (defined $stabref) {
            carp "Multiple symbol table sections found in $fn";
        }
        $stabref = \@symentries;
        $$secref{'entries'} = \@symentries;
    }

    #
    # Now, decode all relocation tables.
    #

    foreach my $secref (
        grep { $$_{'sh_type'} == SHT_REL || $$_{'sh_type'} == SHT_RELA }
        (@{$ret{'secs'}})) {

        my @relentries = ();
        my $entsize = $$secref{'sh_entsize'};
        my $nentries = length($$secref{'data'}) / $entsize;

        #
        # First, find the referenced section.
        #
        $$secref{'relsec'} = $ret{'secs'}[$$secref{'sh_info'}];

        #
        # Now process each of the entries.
        #

        for (my $idx = 0; $idx < $nentries; $idx++) {
            my $datum = substr($$secref{'data'}, $idx * $entsize, $entsize);
            my $relref;
            if ($$secref{'sh_type'} == SHT_REL) {
                if ($elf64_mode) {
                    $relref = $self->_decodestruct($datum, \@struct_rel64);
                }
                else {
                    $relref = $self->_decodestruct($datum, \@struct_rel32);
                }
            }
            else {
                if ($elf64_mode) {
                    $relref = $self->_decodestruct($datum, \@struct_rela64);
                }
                else {
                    $relref = $self->_decodestruct($datum, \@struct_rela32);
                }
            }
            #
            # Now decode the 'info' field
            #
            if ($elf64_mode) {
                $$relref{'sym'} = $$relref{'r_info'} >> 32;
                $$relref{'type'} = $$relref{'r_info'} & 0xffffffff;
            }
            else {
                $$relref{'sym'} = $$relref{'r_info'} >> 8;
                $$relref{'type'} = $$relref{'r_info'} & 0xff;
            }

            #
            # And find the appropriate symbol entry, if any
            #
            if ($$relref{'sym'} != STN_UNDEF
                && exists $$stabref[$$relref{'sym'}]) {
                $$relref{'symref'} = $$stabref[$$relref{'sym'}];
            }

            push @relentries, $relref;
        }
        $$secref{'entries'} = \@relentries;
    }

    # And publish the reference to the symbol table.
    $ret{'symtabref'} = $stabref;

    return \%ret;
}

#
# Write the given ELF file back out to a disk file.
#
sub elfout {
    my $self = &_get_object;      # note, @_ is passed implicitly
    my $fn = shift;
    my $elfref = shift;
    my $blob;

    open(my $elffh, '>', $fn) || croak "elfout can't open $fn for output: $!";
    binmode($elffh) || croak "elfout can't switch $fn to bindmode: $!";

    $blob = $self->_encodestruct($$elfref{'ehdr'}, \@struct_ehdr);
    syswrite $elffh, $blob
        || croak "elfout can't write ELF file header to $fn: $!";

    #
    # Now, calculate the 
    close $elffh;

    # Silence perlcritic:
    return;
}



#
# Given the data for a string table, extract the numbered
#  string.
#
sub _pullstr {
    my $self = &_get_object;
    my $strdataref = shift;
    my $stridx = shift;

    return unpack(sprintf('@%u Z*', $stridx), $$strdataref);
}

#
# Given a structure reference, figure out how many bytes it corresponds to.
#
sub _sizestruct {
    my $self = &_get_object;
    my $structref = shift;
    my $totalsize = 0;

    foreach my $membref (@{$structref}) {
        my ($typeref, $name) = @{$membref};
        if ($elf64_mode) {
            $totalsize += $$typeref[2];
        }
        else {
            $totalsize += $$typeref[1];
        }
    }

    return $totalsize;
}


    
#
# Given a block of data and a structure reference, decode the
#  data to a new hash.
#
sub _decodestruct {
    my $self = &_get_object;
    my $indata = shift;
    my $structref = shift;
    my $offset;
    my %replyhash;

    $offset = 0;
    foreach my $membref (@{$structref}) {
        my $size;
        my $signed;
        my $scalarval;
        my @arrayval;
        my $packstr = sprintf('@%u ', $offset);
        my ($typeref, $name) = @{$membref};

        $signed = $$typeref[0];

        if ($elf64_mode) {
            $size = $$typeref[2];
        }
        else {
            $size = $$typeref[1];
        }

        # Special handling for ident decodes
        if ($typeref == \@primtype_ident) {
            @arrayval = unpack($packstr . "C[$size]", $indata);
            $replyhash{$name} = \@arrayval;
        }
        else {
            # Hack.  Newer versions of perl pack/unpack support
            #  the endianness designators ('<' and '>').  Unfortunately,
            #  older versions of perl do not.  So I'm using the
            #  old pack/unpack types and making up the rest by hand.
            if ($size == 8) {
                #
                # The only 8-byte types we have are 'q' and 'Q', which
                #  are not so good.
                #
                if ($elf_bigendian) {
                    @arrayval = reverse unpack($packstr . 'N[2]', $indata);
                }
                else {
                    @arrayval = unpack($packstr . 'V[2]', $indata);
                }

                if ($signed && $arrayval[1] >= 0x80000000) {
                    $arrayval[0] ^= 0xffffffff;
                    $arrayval[1] ^= 0xffffffff;
                    $scalarval = ($arrayval[1] << 32) | $arrayval[0];
                    $scalarval += 1;
                    $scalarval *= -1;
                }
                else {
                    $scalarval = ($arrayval[1] << 32) | $arrayval[0];
                }
            }
            elsif ($size == 4) {
                if ($elf_bigendian) {
                    $scalarval = unpack($packstr . 'N', $indata);
                }
                else {
                    $scalarval = unpack($packstr . 'V', $indata);
                }

                if ($signed && $scalarval >= 0x80000000) {
                    $scalarval ^= 0xffffffff;
                    $scalarval += 1;
                    $scalarval *= -1;
                }
            }
            elsif ($size == 2) {
                if ($elf_bigendian) {
                    $scalarval = unpack($packstr . 'n', $indata);
                }
                else {
                    $scalarval = unpack($packstr . 'v', $indata);
                }

                if ($signed && $scalarval >= 0x8000) {
                    $scalarval ^= 0xffff;
                    $scalarval += 1;
                    $scalarval *= -1;
                }
            }
            else {
                if ($signed) {
                    $scalarval = unpack($packstr . 'c', $indata);
                }
                else {
                    $scalarval = unpack($packstr . 'C', $indata);
                }
            }

            $replyhash{$name} = $scalarval;
        }

        $offset += $size;
    }

    return \%replyhash;
}

#
# Given an object and a type, cook up a binary blob with
#  the encoded data inside of it.
#  This is the opposite of _decodestruct
#
sub _encodestruct {
    my $self = &_get_object;
    my $dataref = shift;
    my $structref = shift;
    my $blob;
    my $datum;

    $blob = '';
    foreach my $membref (@{$structref}) {
        my $size;
        my $signed;
        my $scalarval;
        my $thisblob;
        my @arrayval;
        my ($typeref, $name) = @{$membref};

        $signed = $$typeref[0];

        if ($elf64_mode) {
            $size = $$typeref[2];
        }
        else {
            $size = $$typeref[1];
        }

        # Special handling for ident decodes
        if ($typeref == \@primtype_ident) {
            my $arrayref;

            if (exists $dataref->{$name}) {
                $arrayref = $dataref->{$name};
            }
            else {
                $arrayref = [ 0 x $size ];
            }
            $thisblob = pack("C[$size]", @{$arrayref});
        }
        else {
            if (exists $dataref->{$name}) {
                $scalarval = $dataref->{$name};
            }
            else {
                $scalarval = 0;
            }
            # Hack.  Newer versions of perl pack/unpack support
            #  the endianness designators ('<' and '>').  Unfortunately,
            #  older versions of perl do not.  So I'm using the
            #  old pack/unpack types and making up the rest by hand.
            if ($size == 8) {
                if ($elf_bigendian) {
                    $thisblob = pack('N[2]', ($scalarval >> 32),
                                     ($scalarval & 0xffffffff));
                }
                else {
                    $thisblob = pack('V[2]', ($scalarval & 0xffffffff),
                                     ($scalarval >> 32));
                }
            }
            elsif ($size == 4) {
                if ($elf_bigendian) {
                    $thisblob = pack('N', $scalarval);
                }
                else {
                    $thisblob = pack('V', $scalarval);
                }
            }
            elsif ($size == 2) {
                if ($elf_bigendian) {
                    $thisblob = pack('n', $scalarval);
                }
                else {
                    $thisblob = pack('v', $scalarval);
                }
            }
            else {
                $thisblob = pack('C', $scalarval);
            }

        }
        $blob .= $thisblob;
    }

    return $blob;
}
#
# Given an offset in a file, find the corresponding program header,
#  if any, and return a reference thereto.
#

sub _phdrlookup
{
    my $self = &_get_object;
    my $elfref = shift;
    my $offset = shift;

    foreach my $phdrref (@{$elfref->{'phdrs'}}) {
        if ($offset >= $phdrref->{'p_offset'}
            && $offset < ($phdrref->{'p_offset'} + $phdrref->{'p_filesz'})) {
            #
            # We found a match.
            #
            return $phdrref;
        }
    }
    return undef;
}

1;

__END__

