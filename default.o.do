#!/bin/sh

set -- "$1" "src/${2#obj/*/}" "$3"

getDeps() {
	awk "/^${1##*/}/"' {
		for (i = 1; i <= NF; i++)
			print "src/" $i
		exit
	}' src.list
}

mode=$(cat make.mode)
redo-ifchange $mode.compile $(getDeps $2.c)

./$mode.compile -c $2.c -o $3
