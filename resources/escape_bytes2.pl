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
    # Process each character individually.
    for (split //, $_) {
        if    ($_ eq "\a") { print "\\a" }
        elsif ($_ eq "\b") { print "\\b" }
        elsif ($_ eq "\t") { print "\\t" }
        elsif ($_ eq "\n") { print "\\n" }
        elsif (ord($_) == 0x0B) { print "\\v" }
        elsif ($_ eq "\f") { print "\\f" }
        elsif ($_ eq "\r") { print "\\r" }
        elsif (/[[:print:]]/) { print }
        # Fallback to hex escape for any other non-printable character.
        else { printf "\\x%02x", ord($_) }
    }
    printf "%s\n", $sbuf;
}


