#!/bin/sh

while read mode
do
	[ ! -e $mode.obj.list ] &&
		continue
	rm -f $(cat $mode.obj.list) >&2
done < modes.list
