#!/bin/sh

sed -E '
	s/"/\"/g
	s/\\/\\\\/g
	s/(.*)/"\1\\n"/
' $2.shaderSrc