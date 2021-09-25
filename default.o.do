#!/bin/sh

set -- "$1" "src/${2#obj/}" "$3"

getDeps() {
	awk "/^${1##*/}/"' {
		for (i = 2; i <= NF; i++)
			print "src/" $i
		exit
	}' src.list
}

redo-ifchange compile $(getDeps $2.c)

./compile -c $2.c -o $3
