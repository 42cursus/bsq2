#!/usr/bin/env perl

use strict;
use warnings;

# Ensure binary mode for portable handling of binary data across different OSes.
binmode(STDIN);
binmode(STDOUT);

# The diamond operator (<>) handles both file arguments and STDIN.
# A filehandle is implicitly opened for each argument in @ARGV.
# If @ARGV is empty, <> reads from STDIN. same as
# BEGIN { @ARGV = ('-') unless @ARGV }  # default to STDIN if no args
while (<>) {
    # Set the current filehandle `ARGV` to binary mode, required for portability.
    if (tell(ARGV) == 0) { binmode(ARGV, ":raw") }

    my $sbuf = "";
    # Process each character individually and append it to the stringbuffer
    for (split //, $_) {
        if    ($_ eq "\a")  { $sbuf .= "\\a"; }   # bell
        elsif ($_ eq "\b")  { $sbuf .= "\\b"; }   # backspace
        elsif ($_ eq "\t")  { $sbuf .= "\\t"; }   # horiz tab
        elsif ($_ eq "\n") { $sbuf .= "\\n"; }   # newline
        elsif (ord($_) == 0x0B) { $sbuf .= "\\v"; }   # vertical tab
        elsif ($_ eq "\f") { $sbuf .= "\\f"; }   # form feed
        elsif ($_ eq "\r") { $sbuf .= "\\r"; }   # carriage return
        elsif (/[[:print:]]/) { $sbuf .= $_; }
        else { $sbuf .= sprintf "\\x%02x", ord($_); }
    }
    printf "%s\n", $sbuf;
}


