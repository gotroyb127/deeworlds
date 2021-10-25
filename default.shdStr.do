#!/bin/sh

redo-ifchange $2.shdSrc

sed -E '
	s/"/\"/g
	s/\\/\\\\/g
	s/(.*)/"\1\\n"/
' $2.shdSrc