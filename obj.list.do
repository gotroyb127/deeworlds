#!/bin/sh

redo-ifchange src.list

while read f deps
do
	printf '%s\n' "obj/${f%.c}.o"
done < src.list
