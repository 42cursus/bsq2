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

    my $sbuf = $_;
    # escape the known controls
    $sbuf =~ s/\x07/\\a/g;
    $sbuf =~ s/\x08/\\b/g;
    $sbuf =~ s/\x09/\\t/g;
    $sbuf =~ s/\x0a/\\n/g;
    $sbuf =~ s/\x0b/\\v/g;
    $sbuf =~ s/\x0c/\\f/g;
    $sbuf =~ s/\x0d/\\r/g;

    # hex-escape remaining non-ASCII-printables
    $sbuf =~ s/([^\x20-\x7E])/sprintf "\\x%02x", ord($1)/eg;
    printf "%s\n", $sbuf;
}
