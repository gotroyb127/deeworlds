#!/bin/sh

set -- $1 src/${2#obj/*/} $3

srcsf=all.srcs
mode=${1#obj/}
mode=${mode%%/*}
compile=$mode.compile

getDeps() {
	awk "/^${1##*/}/"' {
		for (i = 1; i <= NF; i++)
			print "src/" $i
		exit
	}' $2
}

redo-ifchange $compile $(getDeps $2.c $srcsf)

./$compile -c $2.c -o $3
