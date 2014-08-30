#!/usr/bin/perl

use Getopt::Long;
use Pod::Usage;
use strict;

my $PATH= "/home/ubuntu/MLEA-PROJECT/evalKit";


my $help = 0;
my $man = 0;
my ($Pfa, $Cfa, $Cmiss);

my $result = GetOptions('Pfa:f' => \$Pfa,
			'Cfa:f' => \$Cfa,
			'Cmiss:f' => \$Cmiss,
		        'h|?' => \$help,
		        'man' => \$man) or pod2usage(2);

pod2usage(1) if $help;
pod2usage(-verbose => 2) if $man;
pod2usage("$0: No files given.")  if (@ARGV == 0);


((defined($Pfa) && defined($Cfa) && defined($Cmiss)) || !(defined($Pfa) || defined($Cfa) || defined($Cmiss))) || pod2usage("You must precise the three option -Pfa -Cmiss -Cfa or none");

my $indexT = "$PATH/Keys.targ";
my $indexN = "$PATH/Keys.non";

die("Can not found $indexT file\n") if !( -e $indexT);
die("Can not found $indexN file\n") if !( -e $indexN);

my %tar = loadKeys($indexT);
my %non = loadKeys($indexN);

mkdir "./plot";
mkdir "./plot/curves";
mkdir "./plot/det";

open (my $OutFile, ">./plot/figure.gnuplot");

my $firstarg = 1;
my $color = 1;
my $lineformat = "lines ${color} -1";

printGNUPLOTHeader($OutFile);

foreach my $filescore (@ARGV)
  {
    die ("$filescore Not found\n") if !( -e $filescore);
    my ($path, $base, $root, $ext) = parse_filename($filescore);

    split_det(\%tar, \%non, $filescore, "./plot/det/${base}.targ", "./plot/det/${base}.non");

    print "-------------------------------------------------------------------------------------------------\n";
    print "                               ${base} : \n";

    if (defined($Pfa))
      {
	print ("$PATH/plotDET --Pfa $Pfa --Cmiss $Cmiss --Cfa $Cfa ./plot/det/$base.targ ./plot/det/$base.non ./plot/curves/$base.plot\n");
	system("$PATH/plotDET --Pfa $Pfa --Cmiss $Cmiss --Cfa $Cfa ./plot/det/$base.targ ./plot/det/$base.non ./plot/curves/$base.plot") && die("Can't compute DET Curve");
      }
    else
      {
	system("$PATH/plotDET ./plot/det/$base.targ ./plot/det/$base.non ./plot/curves/$base.plot") && die("Can't compute DET Curve");
      }

    print "\n------------------------------------------------------------------------------------------------\n";
    if ($firstarg == 1)
      {
	$firstarg = 0;
      }
    else
      {
	print $OutFile ", \\\n";
      }
#    print $OutFile "\"${base}.plot.DCF\" using 3:2:(0.02) notitle with circles lc rgb \"black\" fill solid 1 border $color, \\\n";
    my $dcf = `cat ./plot/curves/${base}.plot.DCF|cut -d' ' -f1`;
    chomp $dcf;
    $dcf = sprintf("%.3f", $dcf);
    my $bestdcf = `cat ./plot/curves/${base}.plot.BestDCF|cut -d' ' -f1`;
    chomp $bestdcf;
    $bestdcf = sprintf("%.3f", $bestdcf);
    print $OutFile "\"./plot/curves/${base}.plot.DCF\" using 3:2 notitle with points pt 7 ps 1 lc rgb \"black\" lw 2, \\\n";
    print $OutFile "\"./plot/curves/${base}.plot\" using 2:1 title \"$root $dcf $bestdcf\" with lines lc $color lw 2 lt 1, \\\n";
    print $OutFile "\"./plot/curves/${base}.plot.BestDCF\" using 3:2 notitle with points pt 1 ps 1 lc \"black\" lw 2";
    $color = $color + 1;
  }
close $OutFile;
system("gnuplot ./plot/figure.gnuplot >figure.eps");
print "\n\n Send any comments or Bugs to <reda.dehak\@gmail.com>\n\n";


sub printGNUPLOTHeader()
  {
    my ($out) = @_;
    my $color = 1;
    my $TITLE = "";
    my $type = 1;
    print $out "## GNUPLOT command file\n".
               #"set terminal png\n".
               "set terminal postscript color\n".
	       "set style data lines\n".
	       #"set key .5,0\n".
	       "set key left bottom\n".
	       #"set key off\n".
	       "set size 0.78, 1.0\n".
	       "set noxtics\n".
	       "set noytics\n".
	       "set title '${TITLE}'\n".
	       "set ylabel \"Miss probability (in %)\"\n".
	       "set xlabel \"False Alarms probability (in %)\"\n".
	       "set grid\n".
	       "set ytics (\"0.001\" -4.26, \"0.01\" -3.72, \"0.1\" -3.08, \"0.5\" -2.57, \"1\" -2.32635, \"2\" -2.05, \"5\" -1.64,".
	       " \"10\" -1.28, \"20\" -0.84, \"30\" -0.52, \"40\" -0.25, \"50\" 0.0, \"80\" 0.84)\n".
	       #
	       "set xtics (\"0.001\" -4.26, \"0.01\" -3.72, \"0.1\" -3.08, \"0.5\" -2.57, \"1\" -2.32635, \"2\" -2.05, \"5\" -1.64,".
	       " \"10\" -1.28, \"20\" -0.84, \"30\" -0.52, \"40\" -0.25, \"50\" 0.0, \"80\" 0.84)\n".
	       #
	       #"plot [-3.290527:0.0] [-3.290527:0.0] ";
		"plot [-4.40:0.] [-3.290527:1.0] "; 
  }


sub parse_filename {
  my($FILENAME) = @_;
  my(@xx, $path, $base, $root, $ext);
  @xx = split("/",$FILENAME);
  $base = pop(@xx);
  $path = join("/",@xx);
  @xx = split(/\./,$base);
  if (@xx == 1)
    {
      $ext ="";
    }
  else
    {
      $ext = pop(@xx);
    }
  $root = join(".",@xx);

  return($path, $base, $root, $ext);
}

sub loadKeys()
  {
    my ($indexFile) = @_;

    my %key=();
    open(KEY,$indexFile) or die("no Key Index $!");
    while(<KEY>)
      {
        chomp();
        $_ = lc ($_);
	my @tabligne = split(/ /, $_);
	$key{"$tabligne[0] $tabligne[1]"}++;
      }
    close KEY;

    return (%key);
  }


sub split_det()
  {
   my ($tar, $non, $scoreFilename, $outT, $outN) = @_;

    open(RES,$scoreFilename) or die("no input $!");
    open(REST,">$outT") or die("$outT : $!");
    open(RESN,">$outN") or die("$outN : $!");

    while(<RES>)
      {
	$_ = lc($_);
	my $pattern = extractTrials($_);
        print REST $_ if (exists($$tar{"$pattern"})); #print when pattern is met
        print RESN $_ if (exists($$non{"$pattern"}));
      }
    close RES;
    close REST;
    close RESN;
  }

sub extractTrials()
  {
    my ($ligne) = @_;
    chomp $ligne;
    my @t = split(/ /, $ligne);
    my $p = join(" ", $t[0], $t[1]);

    return ($p);
  }

__END__

=head1 NAME

eval.pl - Plot Det curves and compute Optimal Thresholds and DCF values

Authors : Reda DEHAK <reda@dehak.org>

=head1 SYNOPSIS

eval.pl [options] [scorefiles ...]

Authors : Reda DEHAK <reda@dehak.org>

Options:
-Pfa <value> [default: 0.8]
-Cfa <value> [default: 1]
-Cmiss <value>[ default: 10]
-help
-man

=head1 OPTIONS

=over 8

=item B<-Pfa>

Probability of impostor acces [default: 0.8]

=item B<-Cfa>

Cost of false acceptance [default: 1]

=item B<-Cmiss>

Cost of Miss [default: 10]

=item B<-help>

Print a brief help message and exits.

=item B<-man>

Prints the manual page and exits.

=back

=head1 DESCRIPTION

B<This program> evaluate the score files given as arguments and plot de DET
curves into local directory.

=cut
