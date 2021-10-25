#!/bin/sh

redo-ifchange config.sh

mode=$2
. ./config.sh

cat << EOF > $3
$cc $cflags $cflags_extra "\$@"
EOF
chmod +x $3
