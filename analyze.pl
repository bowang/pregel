#!/usr/bin/perl -w
use strict;

my $home = '/filer1/vol3/class/cs315a-spr11/junhao/final_proj';

my @tests = (
    'pagerank',
    'shortestpath',
#    'bipartitematching'
);

my @cpus = ('1', '2', '4', '8', '16');
my @configs = ('s', 'e', 'a');

foreach my $test (@tests) {
    my $out_file = $home.'/'.$test.'.xls';
    open(OUT, "> $out_file") || die "fail to open $out_file\n";

    print OUT "execution time,1,2,4,8,16,\n";
    foreach my $config (@configs) {
        if ($config eq 's') {
            print OUT "simple partition,";
        }
        elsif ($config eq 'e') {
            print OUT "even partition,";
        }
        elsif ($config eq 'a') {
            print OUT "adaptive partition,";
        }

        foreach my $cpu (@cpus) {
            my $in_file = $home.'/results/'.$test.'/'.$config.$cpu.'.out';
            open(IN, "$in_file") || die "fail to open $in_file\n";
            foreach my $line (<IN>) {
                if ($line =~ /execution time/) {
                    my $time = get_data($line, '\[Main\] task done! execution time = ');
                    print OUT "$time,";
                }
            }
            close IN;
        }
        print OUT "\n";
    }
    print OUT "\n";
    print OUT "speedup,1,2,4,8,16,\n";
    print OUT "simple partition,=B2/B2,=B2/C2,=B2/D2,=B2/E2,=B2/F2\n";
    print OUT "even partition,=B3/B3,=B3/C3,=B3/D3,=B3/E3,=B3/F3\n";
    print OUT "adaptive partition,=B4/B4,=B4/C4,=B4/D4,=B4/E4,=B4/F4\n";
    close OUT;
}

sub get_data {
    my $line = $_[0];
    my $reg_expr = $_[1];

    chop($line);                    #remove tailing newline
    $line =~ s/$reg_expr/ /;        #replace the regular expression with white space
    $line =~ tr/ //d;
    return $line;
}

