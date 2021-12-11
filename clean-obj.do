#!/bin/sh

while read mode
do
	while read exe
	do
		objs=$exe.$mode.objs
		[ ! -e $objs ] &&
			continue
		rm -f $(cat $objs) $objs
	done < exes
done < modes
