#!/usr/bin/perl
#
# Generate hexadecimal listing file just as 'hexlist.sh' did
#  before, but do it faster and without external tools.
#
# Bjoren Davis, June 4, 2013.
#  Adapted for Savoy by Bjoren Davis on February 2, 2015.
#  Adapted for Nassau/Denali from git/biometrics/fw/savoy/tools.git:
#    hexlist.pl commit 4121d6710ad56011c856b56aadb2bb978f0080a7
#   by Bjoren Davis, June 6, 2015.
#

use strict;
use warnings;
use Carp;


#
# This bit of cryptic magic adds the path of this script
#  to the INC list.
#
BEGIN {
    use File::Spec::Functions qw(rel2abs);
    use File::Basename qw(dirname);
    use lib dirname(rel2abs($0));
}

my $scriptdir = $0;
if ($scriptdir =~ s=/[^/]*$==) {
    push @INC, $scriptdir . '/../../tools';
}

require 'elfio.pm';
#use 'elfio.pm';


sub usage {
    my $cmdname = $0;
    $cmdname =~ s=.*/==;
    print STDERR << "EOF";
usage: $cmdname elf-file
EOF
    croak;
}
#
# Given an ELF file and an address, unpack a byte
#  at a given address.
#
sub readbyte {
    my ($elfref, $addr, $alias) = ($_[0], $_[1], $_[2]);
    foreach my $secref (@{$$elfref->{'secs'}}) {
        if ($secref->{'sh_type'} == elfio::SHT_PROGBITS()
            && $secref->{'sh_size'} != 0
            && exists $secref->{'loadaddr'}) {

            my $baseaddr = $secref->{'loadaddr'};
            my $endaddr = $baseaddr + $secref->{'sh_size'};

            if ($addr >= $baseaddr && ($addr+1) <= $endaddr) {
                return unpack 'C', substr($secref->{'data'},
                                          $addr - $baseaddr);
            }
            elsif (defined $alias
                   && ($addr + $alias) >= $baseaddr
                   && ($addr + $alias + 1) <= $endaddr) {
                return unpack 'C', substr($secref->{'data'},
                                          $addr + $alias - $baseaddr);
            }
        }
    }
    return undef;
}

sub min {
    my ($ret) = sort { $a <=> $b} grep { defined $_} @_;
    return $ret;
}

sub max {
    my ($ret) = sort { $b <=> $a} grep { defined $_} @_;
    return $ret;
}

#
# Given a range and a loaded ELF file, figure out what the intersection
#  of the range and the data in the ELF file is, keeping into
#  account the possible alias.
# Returns a two-tuple: base, length, or undef if there is no intersection.
#
sub intersectrange {
    my ($elfref, $beg, $size, $alias) = ($_[0], $_[1], $_[2], $_[3]);
    my $end = $beg + $size;
    my $total_base;
    my $total_end;

    # printf "Testing addr %x size %x [%x, %x], alias is ",
    # $beg, $size, $beg, $beg+$size-1;
    # if (defined $alias) {
    #   printf "%s%x\n", $alias < 0 ? '-' : '+', $alias < 0 ? -$alias : $alias;
    # }
    # else {
    #   print "undefined\n";
    # }

    foreach my $secref (@{$$elfref->{'secs'}}) {
        if ($secref->{'sh_type'} == elfio::SHT_PROGBITS()
            && $secref->{'sh_size'} != 0
            && exists $secref->{'loadaddr'}) {

            my $section_base;
            my $section_end;
            my $baseaddr = $secref->{'loadaddr'};
            my $endaddr = $baseaddr + $secref->{'sh_size'};

            # printf("Evaluating section $secref->{'name'} [%08x, %08x]\n",
            #      $baseaddr, $endaddr-1);

            #
            # Is there an intersection?
            #
            $section_base = max($baseaddr, $beg);
            $section_end = min($endaddr, $end);

            if ($section_base < $section_end) {
                $total_base = min($total_base, $section_base);
                $total_end = max($total_end, $section_end);
            }
            if (defined $alias) {
                #
                # How about in the aliased space?
                #
                $section_base = max($baseaddr, ($beg + $alias));
                $section_end = min($endaddr, ($end + $alias));
                if ($section_base < $section_end) {
                    $total_base = min($total_base, ($section_base - $alias));
                    $total_end = max($total_end, ($section_end - $alias));
                }
            }
        }
    }

    if (defined $total_base) {
        # printf(" intersectrange result: [0x%x, 0x%x] len = 0x%x\n",
        #        $total_base, $total_end -1, $total_end - $total_base);
        return [$total_base, ($total_end - $total_base)];
    }
    else {
        # printf(" intersectrange result: undef\n");
        return undef;
    }
}


#
# Given a range of addresses and a possible alias, dump out
#  the hexadecimal listing of the range on the standard output.
#
sub dumprange {
    my ($elfref, $name, $beg, $size, $alias) =
        ($_[0], $_[1], $_[2], $_[3], $_[4]);
    my $rangeref;

    #
    # First, form the intersection of the ELF file and the particular
    #  range, taking into account the alias.  Note that the intersection
    #  range will be in the "canonical" (non-aliased) space.
    #
    $rangeref = intersectrange($elfref, $beg, $size, $alias);
    if (defined $rangeref) {
        print "\n---------------------------------------------------------\n";
        print "$name:\n";

        my $addr = $rangeref->[0] & ~0xf;  # keep things on paragraphs
        my $in_gap;

        while ($addr < ($rangeref->[0]+$rangeref->[1])) {
            if (defined intersectrange($elfref, $addr, 16, $alias)) {
                printf("%08x  ", $addr);

                foreach my $idx (0..3) {
                    print ' ';
                    foreach my $bytenum (0..3) {
                        my $thisbyte;
                        $thisbyte = readbyte($elfref, $addr+$idx*4+(3-$bytenum),
                                             $alias);
                        if (defined $thisbyte) {
                            printf '%02x', $thisbyte;
                        }
                        else {
                            print '  ';
                        }
                    }
                }
                print "\n";
                $in_gap = undef;
            }
            elsif (! defined $in_gap) {
                print "        .\n        .\n        .\n";
                $in_gap = 1;
            }
            $addr += 16;
        }
    }
}    

if ($#ARGV < 0) {
    usage;
}
my $elfio = elfio->new();
my $elffile = $elfio->elfin(shift);

dumprange(\$elffile, 'ROM', 0x50000000, 0x4000);
dumprange(\$elffile, 'RAM in SRAM space', 0x60000000, 0x10000);
dumprange(\$elffile, 'RAM in SROM space', 0x507f0000, 0x10000);
dumprange(\$elffile, 'Complete RAM', 0x60000000, 0x10000, -0x0f810000);
dumprange(\$elffile, 'Flash (main)', 0x50800000, 0x30000);
dumprange(\$elffile, 'Flash (info block)', 0x50840000, 0x30000);
