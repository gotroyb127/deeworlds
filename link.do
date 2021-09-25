#!/bin/sh

redo-ifchange config.sh
. ./config.sh

cat << EOF > $3
$cc "\$@" $ldflags
EOF
chmod +x $3
