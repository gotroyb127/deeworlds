#!/bin/sh

redo-ifchange src.list

mode=$(cat make.mode)

while read f deps
do
	printf '%s\n' "obj/$mode/${f%.c}.o"
done < src.list
