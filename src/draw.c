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

struct vertexData {
	struct vec3 pos;
	struct vec2 texPos;
};

struct blkTex {
	const char *texFnm;
	GLuint tex;
};

struct matrixUnf {
	struct mat4 m;
	GLuint unf;
};

/* static funcitions declerations */
static int initProg(void);
static int initBufs(void);
static int initTexs(void);
static int drawObj(const struct worldObj *wob, void *arg);

/* static variables */
static const char *sVertShadSrc = {
	#include "shaders/vert.shdStr"
};

static const char *sFragShadSrc = {
	#include "shaders/frag.shdStr"
};

static const struct vertexData sVertData[] = {
	/*  pos                texPos  */
	{{0.f, 0.f, 0.f}, {0.f, 0.f}},
	{{0.f, 1.f, 0.f}, {0.f, 1.f}},
	{{1.f, 1.f, 0.f}, {1.f, 1.f}},
	{{1.f, 0.f, 0.f}, {1.f, 0.f}},
	{{1.f, 0.f, 1.f}, {-2.f, 0.f}},
	{{0.f, 0.f, 1.f}, {-1.f, 0.f}},
	{{0.f, 1.f, 1.f}, {-1.f, 1.f}},
	{{1.f, 1.f, 1.f}, {-2.f, 1.f}},

	{{1.f, 1.f, 0.f}, { -3.f, 1.f}},
	{{1.f, 0.f, 0.f}, { -3.f, 0.f}},

	{{0.f, 1.f, 1.f}, {0.f, 2.f}},
	{{1.f, 1.f, 1.f}, {1.f, 1.f}},

	{{1.f, 0.f, 1.f}, {1.f, 0.f}},
	{{0.f, 0.f, 1.f}, {0.f, 0.f}},
	{{0.f, 0.f, 0.f}, {0.f, 1.f}},
	{{1.f, 0.f, 0.f}, {1.f, 1.f}},
};

static const GLushort sIdxArr[] = {
	 0,  1,  3,   3,  1, 2,
	 0,  5,  6,   0,  6, 1,
	 5,  4,  7,   5,  7, 6,
	 4,  9,  8,   4,  8, 7,
	 1, 10,  2,   2, 10, 11,
	13, 14, 12,  12, 14, 15,
};

static struct matrixUnf sMdlToWld, sWldToView;
static GLuint sProg, sVao, sVertBuf, sIdxBuf;
static GLuint sCurrTexUnf;
static float sAspRatio = 1.f;
static struct blkTex sBlkTexs[WLD_BLK_LAST] = {
	[WLD_BLK_AIR]    = {.texFnm = NULL},
	[WLD_BLK_HKBL] = {.texFnm = "data/texs/blk_hkbl.ff"},
	[WLD_BLK_UHBL] = {.texFnm = "data/texs/blk_uhbl.ff"},
};

/* static function implementations */
int
initProg(void)
{
	unsigned int i;
	GLuint shds[2];

	if (!compileShd(&shds[0], GL_VERTEX_SHADER, sVertShadSrc) ||
	!compileShd(&shds[1], GL_FRAGMENT_SHADER, sFragShadSrc) ||
	!linkProg(&sProg, NELM(shds), shds))
		return 0;

	for (i = 0; i < NELM(shds); i++)
		glDeleteShader(shds[i]);

	sWldToView.unf = glGetUniformLocation(sProg, "worldToView");
	sMdlToWld.unf = glGetUniformLocation(sProg, "modelToWorld");
	sCurrTexUnf = glGetUniformLocation(sProg, "currTex");

	return 1;
}

int
initBufs(void)
{
	GLuint bufs[2]; /* vert, idx buffers */

	glGenBuffers(NELM(bufs), bufs);
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
		sizeof sVertData[0], (void *)offsetof(struct vertexData, texPos));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 1;
}

int
initTexs(void)
{
	struct blkTex *bt;
	void *imgData;
	uint32_t width, height;
	unsigned int i;
	const char *errmsg;

	for (i = 0; i < NELM(sBlkTexs); i++) {
		if (!(bt = &sBlkTexs[i])->texFnm)
			continue;
		if (!(imgData = drawLoadImg(bt->texFnm, &width,
			&height, &errmsg))
		) {
			eprintf("fatal: drawLoadImg(%s): %s\n",
				bt->texFnm, errmsg);
			return 0;
		} else if (errmsg)
			eprintf("drawLoadImg(%s): %s\n", bt->texFnm, errmsg);

		glGenTextures(1, &bt->tex);
		glBindTexture(GL_TEXTURE_2D, bt->tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_SHORT, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);

		free(imgData);
	}

	glActiveTexture(GL_TEXTURE0);

	return 1;
}

int
drawObj(const struct worldObj *wob, void *arg)
{
	const struct blkTex *bt;
	struct vec3 p;

	if (!(bt = &sBlkTexs[wob->type])->texFnm)
		return 1;

	glBindTexture(GL_TEXTURE_2D, bt->tex);

	mat4_trn_mat(&sMdlToWld.m, vec3_neg(&p, &wob->pos));

	glUniformMatrix4fv(sMdlToWld.unf, 1, GL_FALSE,
		&sMdlToWld.m.m[0][0]);

	glDrawElements(GL_TRIANGLES, NELM(sIdxArr), GL_UNSIGNED_SHORT, 0);

	return 1;
}

/* non-static function implementations */
int
drawInit(void)
{
	if (!initProg() || !initBufs() || !initTexs())
		return 0;

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glClearColor(0.0f, 0.15f, 0.3f, 0.0f);

	return 1;
}

void
drawSetAspectRatio(float asp)
{
	sAspRatio = asp;
}

void
drawFrame(void)
{
	struct mat4 *m;
	struct vec3 eyePos, eyeDir;
	float yFov, zNear, zFar;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(sProg);
	glBindVertexArray(sVao);
	glBindBuffer(GL_ARRAY_BUFFER, sVertBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIdxBuf);

	yFov = M_DEG_TO_RAD(110.f);
	zNear = 0.025f;
	zFar = 100.f;

	wldGetEye(&eyePos, &eyeDir);

	m = &sWldToView.m;
	mat4_persp(m, yFov, sAspRatio, zNear, zFar);
#if 1
	mat4_cam_apply(m, m, &eyePos, &eyeDir);
#else
	mat4_rotz(m, m, eyeDir.z);
	mat4_rotx(m, m, eyeDir.x);
	mat4_roty(m, m, eyeDir.y);
	mat4_trlt(m, m, &eyePos);
#endif
	glUniformMatrix4fv(sWldToView.unf, 1, GL_FALSE,
		&sWldToView.m.m[0][0]);
	wldObjIter(&drawObj, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
