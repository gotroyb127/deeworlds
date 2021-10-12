#!/bin/sh

redo clean-obj

rm -f deeworlds
while read mode
do
	rm -f $mode.obj.list $mode.compile $mode.link
done < modes.list
