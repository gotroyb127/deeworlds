#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glad/gl.h>

#include "util.h"
#include "draw.h"
#include "drawUtil.h"
#include "drawLoad.h"
#include "vecmath.h"
#include "world.h"
#include "worldProtected.h"

typedef struct {
	vec3 pos;
	vec2 texPos;
} VertexData;

typedef struct {
	const char *texFnm;
	GLuint tex;
} BlockTexture;

typedef struct {
	mat4 mat;
	GLuint unf;
} MatrixUniform;

/* static funcitions declerations */
static int initProg(void);
static int initBufs(void);
static int initTexs(void);
static int drawObject(WorldObject const *wob, void *data);

/* static variables */
static char const *sVertShadSrc = {
	#include "shaders/vert.shdStr"
};

static char const *sFragShadSrc = {
	#include "shaders/frag.shdStr"
};

static VertexData const sVertData[] = {
	/*  pos                texPos  */
	{ { 0.f, 0.f, 0.f }, { 0.f, 0.f } },
	{ { 0.f, 1.f, 0.f }, { 0.f, 1.f } },
	{ { 1.f, 1.f, 0.f }, { 1.f, 1.f } },
	{ { 1.f, 0.f, 0.f }, { 1.f, 0.f } },
	{ { 1.f, 0.f, 1.f }, { -2.f, 0.f } },
	{ { 0.f, 0.f, 1.f }, { -1.f, 0.f } },
	{ { 0.f, 1.f, 1.f }, { -1.f, 1.f } },
	{ { 1.f, 1.f, 1.f }, { -2.f, 1.f } },

	{ { 1.f, 1.f, 0.f }, { -3.f, 1.f } },
	{ { 1.f, 0.f, 0.f }, { -3.f, 0.f } },

	{ { 0.f, 1.f, 1.f }, { 0.f, 2.f } },
	{ { 1.f, 1.f, 1.f }, { 1.f, 1.f } },

	{ { 1.f, 0.f, 1.f }, { 1.f, 0.f } },
	{ { 0.f, 0.f, 1.f }, { 0.f, 0.f } },
	{ { 0.f, 0.f, 0.f }, { 0.f, 1.f } },
	{ { 1.f, 0.f, 0.f }, { 1.f, 1.f } },
};

static GLushort const sIdxArr[] = {
	 0,  1,  3,   3,  1, 2,
	 0,  5,  6,   0,  6, 1,
	 5,  4,  7,   5,  7, 6,
	 4,  9,  8,   4,  8, 7,
	 1, 10,  2,   2, 10, 11,
	13, 14, 12,  12, 14, 15,
};

static MatrixUniform sModelToWorld, sWorldToView;
static GLuint sProg, sVao, sVertBuf, sIdxBuf;
static GLuint sCurrTexUnf;
static float sAspectRatio = 1.f;
static BlockTexture sBlkTexs[WORLD_BLK_LAST] = {
	[WORLD_BLK_AIR]    = {.texFnm = NULL},
//	[WORLD_BLK_HKABLE] = {.texFnm = "data/texs/clown.ff"},
//	[WORLD_BLK_UHABLE] = {.texFnm = "data/texs/clown.ff"},
	[WORLD_BLK_HKABLE] = {.texFnm = "data/texs/blk_hkable.ff"},
	[WORLD_BLK_UHABLE] = {.texFnm = "data/texs/blk_uhable.ff"},
};

/* static function implementations */
int
initProg(void)
{
	unsigned int i;
	GLuint shds[2];

	if (!compileShd(&shds[0], GL_VERTEX_SHADER, sVertShadSrc)
	|| !compileShd(&shds[1], GL_FRAGMENT_SHADER, sFragShadSrc)
	|| !linkProg(&sProg, NELEM(shds), shds))
		return 0;

	for (i = 0; i < NELEM(shds); i++)
		glDeleteShader(shds[i]);

	sWorldToView.unf = glGetUniformLocation(sProg, "worldToView");
	sModelToWorld.unf = glGetUniformLocation(sProg, "modelToWorld");

	sCurrTexUnf = glGetUniformLocation(sProg, "currTex");

	sModelToWorld.mat = mat4_identity;

	glUseProgram(sProg);
	glUniformMatrix4fv(sWorldToView.unf, 1, GL_FALSE, &sWorldToView.mat.raw[0][0]);
	glUniformMatrix4fv(sModelToWorld.unf, 1, GL_FALSE, &sModelToWorld.mat.raw[0][0]);
	glUniform1i(sCurrTexUnf, 0);
	glUseProgram(0);

	return 1;
}

int
initBufs(void)
{
	GLuint bufs[2]; /* vert, idx buffers */

	glGenBuffers(NELEM(bufs), bufs);
	glGenVertexArrays(1, &sVao);

	glBindVertexArray(sVao);

	sVertBuf = bufs[0];

	glBindBuffer(GL_ARRAY_BUFFER, sVertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof sVertData,
		sVertData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	sIdxBuf = bufs[1];

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIdxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof sIdxArr,
		sIdxArr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, sVertBuf);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof sVertData[0], (void *)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof sVertData[0], (void *)offsetof(VertexData, texPos));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 1;
}

int
initTexs(void)
{
	char const *errmsg;
	char *imgData;
	uint32_t width, height;
	unsigned int i;
	BlockTexture *bt;

	for (i = 0; i < NELEM(sBlkTexs); i++) {
		bt = &sBlkTexs[i];

		if (!bt->texFnm)
			continue;

		imgData = drawLoadImg(bt->texFnm, &width, &height, &errmsg);
		if (!imgData) {
			eprintf("fatal: drawLoadImg(%s): %s\n", bt->texFnm, errmsg);
			return 0;
		} else if (errmsg) {
			eprintf("drawLoadImg(%s): %s\n", bt->texFnm, errmsg);
		}

		glGenTextures(1, &bt->tex);
		glBindTexture(GL_TEXTURE_2D, bt->tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_SHORT, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		free(imgData);
	}

	glActiveTexture(GL_TEXTURE0);

	return 1;
}

int
drawObject(WorldObject const *wob, void *data)
{
	BlockTexture const *bt;
	vec3 pos;

	bt = &sBlkTexs[wob->type];
	if (!bt->texFnm)
		return 1;

	glBindTexture(GL_TEXTURE_2D, bt->tex);

	vec3_negate(&pos, &wob->pos);
	mat4_translation_mat(&sModelToWorld.mat, &pos);

	glUniformMatrix4fv(sModelToWorld.unf, 1, GL_FALSE,
		&sModelToWorld.mat.raw[0][0]);

	glDrawElements(GL_TRIANGLES, NELEM(sIdxArr), GL_UNSIGNED_SHORT, 0);

	return 1;
}

/* non-static function implementations */
int
drawInit(void)
{
	if (!initProg()
	|| !initBufs()
	|| !initTexs()
	)
		return 0;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glClearColor(0.0f, 0.15f, 0.3f, 0.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	return 1;
}

void
drawSetAspectRatio(float asp)
{
	sAspectRatio = asp;
}

void
drawFrame(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(sProg);
	glBindVertexArray(sVao);
	glBindBuffer(GL_ARRAY_BUFFER, sVertBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIdxBuf);

	mat4 *mat;
	vec3 eyePos, eyeDir;
	float yFov, aspectRatio, zNear, zFar;

	yFov = M_DEG_TO_RAD(110.f);
	aspectRatio = sAspectRatio;
	zNear = 0.25f;
	zFar = 100.f;

	worldGetEyePos(&eyePos, &eyeDir);

	mat = &sWorldToView.mat;
	mat4_perspective(mat, yFov, aspectRatio, zNear, zFar);
#if 1
	mat4_apply_cam(mat, mat, &eyePos, &eyeDir);
#else
	mat4_rot_z(mat, mat, eyeDir.z);
	mat4_rot_x(mat, mat, eyeDir.x);
	mat4_rot_y(mat, mat, eyeDir.y);
	mat4_translate(mat, mat, &eyePos);
#endif
	glUniformMatrix4fv(sWorldToView.unf, 1, GL_FALSE, &sWorldToView.mat.raw[0][0]);
#if 0
	static unsigned int n;
	if (n++)
		printf("\033[4A\033[K");
	MAT4_PRINT(sWorldToView.mat);
#elif 0
	static unsigned int n;
	if (n++)
		printf("\033[2A\033[K");
	VEC3_PRINT(eyePos);
	VEC3_PRINT(eyeDir);
#endif
	worldObjIter(&drawObject, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
