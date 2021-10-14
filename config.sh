#!/bin/sh

cc=cc
#cc=clang

incs='-Isrc/glad/include'
defs=
cppflags="$incs $defs"

cflags="-std=c99 -Wall -pedantic $cppflags"
ldflags='-lglfw -ldl -lm'

case $mode in
(debug)
	cflags_extra='-Werror -g -Og'
#	cflags_extra="$cflags_extra -Wextra"
	ldflags_extra=
;;
(optimized)
	cflags_extra='-O2'
	ldflags_extra=
;;
(*)
	echo 'unknown mode (or none given)' >&2
	exit 1
;;
esac
