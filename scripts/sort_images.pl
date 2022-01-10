#!/usr/bin/perl

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
                my $r = substr($fi, $pos+1, $pos3-$pos-1);
                my $t = substr($r, $ti, length ($r));
                
                if ($r =~ m/(\d+).(\d+).(\d+)/) {
                    
                    $list{$r} = $fi;
                }
            }
        }
    }
}
foreach my $i (sort keys %list) {
    print $list{$i} . "\n";
    #print $i . "\n";
}
