#!/bin/sh

allsrcsf=all.srcs

allsrcs() {
	while read l
	do
		printf '%s\n' "$l"
	done <<- EOF
	deeworlds.c    util.h draw.h world.h input.h deeworldsConfig.h \
	               glad/include/glad/gl.h
	draw.c         util.h draw.h drawUtil.h drawLoad.h vecmath.h world.h \
	               worldProtected.h shaders/vert.shdStr shaders/frag.shdStr \
	               glad/include/glad/gl.h
	drawLoad.c     drawLoad.h
	drawUtil.c     util.h drawUtil.h glad/include/glad/gl.h
	input.c        util.h vecmath.h world.h playerAction.h input.h inputConfig.h
	player.c       vecmath.h player.h worldParams.h
	playerAction.c vecmath.h world.h player.h
	vecmath.c      vecmath.h
	world.c        vecmath.h world.h worldProtected.h worldParams.h \
	               worldParamsConfig.h player.h
	glad/src/gl.c  glad/include/glad/gl.h
	EOF
}

deeworldsSrcs() {
	for src in deeworlds.c draw.c drawLoad.c drawUtil.c input.c \
		player.c playerAction.c vecmath.c world.c glad/src/gl.c
	do
		grep "^$src\>" $1
	done
}

case $2 in
(all)
	allsrcs
;;
(deeworlds)
	redo-ifchange $allsrcsf
	deeworldsSrcs $allsrcsf
;;
(*)
	printf '%s\n' "unknown target: $ (or none given)" >&2
	exit 1
;;
esac
