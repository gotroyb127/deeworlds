#!/bin/sh

redo-ifchange config.sh

mode=$2
. ./config.sh

cat << EOF > $3
$cc "\$@" $ldflags $ldflags_extra
EOF
chmod +x $3
