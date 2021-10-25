#!/bin/sh

mode=${2##*.}
srcsf=${2%.*}.srcs

redo-ifchange $srcsf

while read f deps
do
	printf '%s\n' "obj/$mode/${f%.c}.o"
done < $srcsf
