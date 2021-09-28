#!/bin/sh

redo clean-obj

rm -f beeworlds
while read mode
do
	rm -f $mode.obj.list $mode.compile $mode.link
done < modes.list
