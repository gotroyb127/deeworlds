#!/bin/sh

redo clean-obj

rm -f all.srcs
while read mode
do
	rm -f $mode.compile $mode.link
	while read exe
	do
		printf 'objs: %s\n' "$exe.$mode" >&2
		rm -f $exe $exe.$mode.$objs $exe.srcs
	done < exes
done < modes
