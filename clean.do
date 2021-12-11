#!/bin/sh

redo clean-obj

rm -f all.srcs
while read mode
do
	rm -f $mode.compile $mode.link
	while read exe
	do
		rm -f $exe $exe.$mode.$objs $exe.srcs
	done < exes
done < modes
