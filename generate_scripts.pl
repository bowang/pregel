#!/usr/bin/perl -w
use strict;

my $home = '/filer1/vol3/class/cs315a-spr11/junhao/final_proj';

my @tests = (
    'pagerank',
    'shortestpath',
    'bipartitematching'
);

my @cpus = ('1', '2', '4', '8', '16');
my @configs = ('s', 'e', 'a');

foreach my $test (@tests) {
    my $out_file = $home.'/run_'.$test;
    open(OUT, "> $out_file") || die "fail to open $out_file\n";

    print OUT "# declare a name for this job\n";
    print OUT "#PBS -N $test\n";
    print OUT "# request processors\n";
    print OUT "#PBS -l nodes=1:h23:ppn=8\n";
    print OUT "# estimate run time\n";
    print OUT "#PBS -l cput=2:00:00\n";
    print OUT "# combine stdout and stderr\n";
    print OUT "#PBS -j oe\n";
    print OUT "# commands to run\n";

    print OUT "cd ~/final_proj\n";
    print OUT "pwd\n";
    print OUT "rm -f tmp\n";
    my $dir = '~/final_proj/results/'.$test;
    print OUT "rm -rf $dir\n";
    print OUT "mkdir $dir\n";
    foreach my $config (@configs) {
        foreach my $cpu (@cpus) {
            print OUT "./$test $cpu $config $test.in > $dir/$config$cpu.out\n";
            print OUT "mv tmp $dir/$config$cpu.result\n";
        }
        print OUT "\n";
    }
    close OUT;
}
