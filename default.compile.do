#!/bin/sh

redo-ifchange config.sh

mode=$(cat make.mode)
. ./config.sh

cat << EOF > $3
$cc $cflags $cflags_extra "\$@"
EOF
chmod +x $3
