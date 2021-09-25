#!/bin/sh

redo-ifchange config.sh
. ./config.sh

cat << EOF > $3
$cc $cflags "\$@"
EOF
chmod +x $3
