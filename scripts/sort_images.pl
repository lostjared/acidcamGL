#!/usr/bin/perl

use File::stat;
use Time::localtime;

my $dir = shift @ARGV;
my $src = shift @ARGV;
opendir $d, $dir or die("Could not open directory");
my @f = readdir $d;
closedir $d;
my %list = ();
foreach $fi (@f) {
    my $pos = index $fi, $src;
    if($pos != -1) {
        my $pos2 = index $fi, "_", $pos;
        if($pos2 != -1) {
            my $pos3 = index $fi, "_", $pos2+1;
            if($pos3 != -1) {
                my $r = substr($fi, $pos, $pos2-$pos);
                if ($r =~ m/(\d+).(\d+).(\d+)/) {
                    $list{$2 . $3 . $fi} = $fi;
                    
                }
            }
        }
    }
}
foreach my $i (sort keys %list) {
    print $list{$i} . "\n";
}
