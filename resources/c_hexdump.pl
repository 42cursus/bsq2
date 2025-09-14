#!/usr/bin/env perl
use strict;
use warnings;

# c_hexdump.pl [file]
# Pure Perl hexdump with C-style escapes on the right

my $in;
if (@ARGV) {
    open $in, "<", $ARGV[0] or die "c_hexdump.pl: open: $ARGV[0]: $!\n";
} else {
    $in = *STDIN;
}
binmode($in);
binmode(STDOUT);

my $off = 0;
my $buf;
while (1) {
    my $n = read($in, $buf, 16);
    defined $n or die "c_hexdump.pl: read error: $!\n";
    last if $n == 0;

    my @bytes = unpack("C*", $buf);

    # hex column
    my $hex = "";
    my $i = 0;
    while ($i < @bytes) {
        $hex .= sprintf "%02x", $bytes[$i];
        $hex .= " " if $i != $#bytes;
        $i += 1;
    }

    # ascii column with C-style escapes
    my $asc = "";
    $i = 0;
    while ($i < @bytes) {
        my $c = $bytes[$i];
        if    ($c == 9)  { $asc .= "\\t"; }   # tab
        elsif ($c == 10) { $asc .= "\\n"; }   # newline
        elsif ($c == 13) { $asc .= "\\r"; }   # carriage return
        elsif ($c == 11) { $asc .= "\\v"; }   # vertical tab
        elsif ($c == 12) { $asc .= "\\f"; }   # form feed
        elsif ($c == 7)  { $asc .= "\\a"; }   # bell
        elsif ($c == 8)  { $asc .= "\\b"; }   # backspace
        elsif ($c >= 32 && $c < 127) { $asc .= chr($c); }
        else { $asc .= sprintf "\\x%02x", $c; }
        $i += 1;
    }

    printf "%08x: %-47s %s\n", $off, $hex, $asc;
    $off += scalar @bytes;
}

