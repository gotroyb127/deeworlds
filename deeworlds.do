#!/bin/sh

[ ! -e make.mode ] &&
	echo debug > make.mode
mode=$(cat make.mode)

echo "mode: $mode" >&2

redo-ifchange make.mode $mode.link $mode.obj.list
redo-ifchange $(cat $mode.obj.list)

./$mode.link -o $3 $(cat $mode.obj.list)
