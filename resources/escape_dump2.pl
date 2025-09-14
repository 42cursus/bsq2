#!/usr/bin/env perl

use strict;
use warnings;

# Ensure binary mode for portable handling of binary data across different OSes.
binmode(STDOUT);

# The diamond operator (<>) handles both file arguments and STDIN.
# A filehandle is implicitly opened for each argument in @ARGV.
# If @ARGV is empty, <> reads from STDIN. same as
BEGIN { @ARGV = ('-') unless @ARGV }  # default to STDIN if no args
while (<>) {
  # Set the current filehandle `ARGV` to binary mode, required for portability.
  if (tell(ARGV) == 0) { binmode(ARGV, ":raw") }

  my $sbuf = "";
  my $len = length($_);
  my $i = 0;

  while ($i < $len) {
    my $c = vec($_, $i, 8);  # read one byte as integer 0..255

    if    ($c == 0x07) { $sbuf .= "\\a"; }
    elsif ($c == 0x08) { $sbuf .= "\\b"; }
    elsif ($c == 0x09) { $sbuf .= "\\t"; }
    elsif ($c == 0x0A) { $sbuf .= "\\n"; }
    elsif ($c == 0x0B) { $sbuf .= "\\v"; }
    elsif ($c == 0x0C) { $sbuf .= "\\f"; }
    elsif ($c == 0x0D) { $sbuf .= "\\r"; }
    elsif ($c >= 0x20 && $c <= 0x7E) { $sbuf .= chr($c); }
    else { $sbuf .= sprintf "\\x%02x", $c; }

    $i++;
  }

  printf "%s\n", $sbuf;
}
