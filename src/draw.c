#include <stdio.h>
#include <math.h>
#include <glad/gl.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "worldProtected.h"

typedef struct {
	mat4 mat;
	GLuint unf;
} MatrixUniform;

/* static funcitions declerations */
static int compileShad(GLuint *retShad, GLenum shadType, char const *src);
static int linkProg(GLuint *prog, size_t n, GLuint shads[n]);
static int initProg(void);
static void initBufs(void);
static int drawObject(WorldObject const *wob, void *data);

/* static variables */
static float sAspectRatio = 1.f;

static char const *sVertShadSrc = {
	#include "shaders/vert.shaderStr"
};

static char const *sFragShadSrc = {
	#include "shaders/frag.shaderStr"
};

static MatrixUniform sModelToWorld, sWorldToView;
static GLuint sProg, sVao, sVertBuf, sIdxBuf;

#define r_ 1.f, 0.f, 0.f
#define g_ 0.f, 1.f, 0.f
#define b_ 0.f, 0.f, 1.f

static GLfloat const sVertData[] = {
	0.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 1.f, 0.f,
	0.f, 1.f, 0.f,

	0.f, 1.f, 1.f,
	0.f, 0.f, 1.f,
	1.f, 0.f, 1.f,
	1.f, 1.f, 1.f,

	r_, g_, b_, b_,
	r_, g_, b_, b_,
};

static GLushort const sIdxArr[] = {
	0, 2, 1,
	0, 3, 2,

	0, 4, 3,
	0, 5, 4,

	4, 5, 6,
	4, 6, 7,

	2, 3, 4,
	2, 4, 7,

	1, 2, 7,
	1, 7, 6,

	0, 1, 5,
	1, 6, 5,
};

/* static function implementations */
int
compileShad(GLuint *retShad, GLenum shadType, char const *src)
{
	GLint status;
	GLuint shad;

	shad = glCreateShader(shadType);
	glShaderSource(shad, 1, &src, NULL);
	glCompileShader(shad);

	glGetShaderiv(shad, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint logLen;
		char const *shTypeStr;

		switch (shadType) {
		case GL_VERTEX_SHADER:
			shTypeStr = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			shTypeStr = "geometry";
			break;
		case GL_FRAGMENT_SHADER:
			shTypeStr = "fragment";
			break;
		default:
			shTypeStr = "(unknown)";
		}

		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &logLen);

		GLchar infoLog[logLen + 1];
		glGetShaderInfoLog(shad, logLen, NULL, infoLog);

		eprintf("Compile failure in %s shader:\n%s",
			shTypeStr, infoLog);
		return 0;
	}

	*retShad = shad;
	return 1;
}

int
linkProg(GLuint *prog, size_t n, GLuint shads[n])
{
	unsigned int i;
	GLint status;
	GLuint pr;

	pr = glCreateProgram();
	for (i = 0; i < n; i++)
		glAttachShader(pr, shads[i]);
	glLinkProgram(pr);

	glGetProgramiv(pr, GL_LINK_STATUS, &status);
	if (!status) {
		GLint logLen;
		glGetProgramiv(pr, GL_INFO_LOG_LENGTH, &logLen);

		GLchar infoLog[logLen + 1];
		glGetProgramInfoLog(pr, logLen, NULL, infoLog);
		eprintf("Linker failure: %s", infoLog);

		return 0;
	}

	*prog = pr;
	return 1;
}

//static void
//checkErr(void)
//{
//	unsigned int i;
//	int err;
//	for (i = 0; (err = glGetError()) != GL_NO_ERROR; i = 1)
//		eprintf("GLerror no. %d\n", err);
//	if (i)
//		exit(1);
//}

int
initProg(void)
{
	unsigned int i;
	GLuint shads[2];

	if (!compileShad(&shads[0], GL_VERTEX_SHADER, sVertShadSrc)
	|| !compileShad(&shads[1], GL_FRAGMENT_SHADER, sFragShadSrc)
	|| !linkProg(&sProg, NELEM(shads), shads))
		return 0;

	for (i = 0; i < NELEM(shads); i++)
		glDeleteShader(shads[i]);

	sWorldToView.unf = glGetUniformLocation(sProg, "worldToView");
	sModelToWorld.unf = glGetUniformLocation(sProg, "modelToWorld");

	sModelToWorld.mat = mat4_identity;

	glUseProgram(sProg);
	glUniformMatrix4fv(sWorldToView.unf, 1, GL_FALSE, &sWorldToView.mat.raw[0][0]);
	glUniformMatrix4fv(sModelToWorld.unf, 1, GL_FALSE, &sModelToWorld.mat.raw[0][0]);
	glUseProgram(0);

	return 1;
}

void
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
		(void*)(sizeof sVertData / 2));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

int
drawObject(WorldObject const *wob, void *data)
{
	vec3 pos;

	if (wob->type == WORLD_BLK_AIR)
		return 1;

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
	if (!initProg())
		return 0;
	initBufs();

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
