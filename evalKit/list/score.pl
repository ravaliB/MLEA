#!/usr/bin/perl
use strict;

open(FIN,"test.lst");
open(FOUT,">score.txt");

while(<FIN>)
  {
    my $s = rand(10) - 5;
    my $t ="f";
    if ($s >= 0)
      {
	$t = "t";
      }
    chomp;
    print FOUT "$_ ${s} ${t}\n";
  }
close(FIN);
close(FOUT);
