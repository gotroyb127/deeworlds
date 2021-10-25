#!/bin/sh

modef=make.mode
[ ! -e $modef ] &&
	echo debug > $modef
mode=$(cat $modef)
link=$mode.link
objsf=$2.$mode.objs

echo "mode: $mode" >&2

redo-ifchange $modef $link $objsf
redo-ifchange $(cat $objsf)

./$link -o $3 $(cat $objsf)
