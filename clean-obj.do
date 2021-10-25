#!/bin/sh

while read mode
do
	while read exe
	do
		objs=$exe.$mode.objs
		printf 'objs: %s\n' "$objs" >&2
		[ ! -e $objs ] &&
			continue
		rm -f $(cat $objs)
	done < exes
done < modes
