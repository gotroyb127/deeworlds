#include <stdio.h>
#include <math.h>
#include <glad/gl.h>

#include "util.h"
#include "world.h"

#define DEG_TO_RAD(d) (d / 180.f * M_PI)
#define print_mat4(m) \
	for (int i = 0; i < 4; i++) { \
		printf("%s: ", #m); \
		for (int j = 0; j < 4; j++) \
			printf("%.1f ", m[j][i]); \
		printf("\n"); \
	}


static const char vertShadSrc[] = {
	#include "shaders/vert.shaderStr"
};

static const char fragShadSrc[] = {
	#include "shaders/frag.shaderStr"
};

static mat4 modelToWorld, worldToView, worldToView;
static GLuint modelToWorldUnf, worldToViewUnf;
static GLuint prog, vao, vertBuf, idxBuf;

#define _r 1.f, 0.f, 0.f
#define _g 0.f, 1.f, 0.f
#define _b 0.f, 0.f, 1.f

const GLfloat vertData[] = {
	0.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 1.f, 0.f,

	_r, _g, _b,
//	0.f, 0.f, 0.f,
//	1.f, 0.f, 0.f,
//	1.f, 1.f, 0.f,
//	0.f, 1.f, 0.f,
//
//	0.f, 1.f, 1.f,
//	0.f, 0.f, 1.f,
//	1.f, 0.f, 1.f,
//	1.f, 1.f, 1.f,
//
//	_r, _g, _b, _b,
//	_r, _g, _b, _b,
};

const GLushort idxArr[] = {
	0, 1, 2,
//	0, 2, 1,
//	0, 3, 2,
//
//	0, 4, 3,
//	0, 5, 4,
//
//	4, 5, 6,
//	4, 6, 7,
//
//	2, 3, 4,
//	2, 4, 7,
//
//	1, 2, 7,
//	1, 7, 6,
//
//	0, 1, 5,
//	1, 6, 5,
};

static int
compileShad(GLuint *ret_shad, GLenum shadType, const char *src)
{
	GLint status;
	GLuint shad;

	shad = glCreateShader(shadType);
	glShaderSource(shad, 1, &src, NULL);
	glCompileShader(shad);

	glGetShaderiv(shad, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint logLen;
		const char *shTypeStr;

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
		}

		glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &logLen);

		GLchar infoLog[logLen + 1];
		glGetShaderInfoLog(shad, logLen, NULL, infoLog);

		eprintf("Compile failure in %s shader:\n%s",
			shTypeStr, infoLog);
		return 0;
	}

	*ret_shad = shad;
	return 1;
}

static int
linkProg(GLuint *prog, size_t n, GLuint shads[n])
{
	int i;
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
//	int i, err;
//	for (i = 0; (err = glGetError()) != GL_NO_ERROR; i = 1)
//		eprintf("GLerror no. %d\n", err);
//	if (i)
//		exit(1);
//}

static int
initProg(void)
{
	int i;
	GLuint shads[2];

	if (!compileShad(&shads[0], GL_VERTEX_SHADER, vertShadSrc)
	|| !compileShad(&shads[1], GL_FRAGMENT_SHADER, fragShadSrc)
	|| !linkProg(&prog, LENGTH(shads), shads))
		return 0;

	for (i = 0; i < LENGTH(shads); i++)
		glDeleteShader(shads[i]);

	worldToViewUnf = glGetUniformLocation(prog, "worldToView");
	modelToWorldUnf = glGetUniformLocation(prog, "modelToWorld");

//	const float frustrumScale = 0.3f;
//	const float zNear = 1.0f, zFar = 45.0f;

//	mat4_identity(camToClip);
//	mat4_perspective(camToClip, 1.3f, .3f, zNear, zFar);
//	camToClip[0][0] = frustrumScale;
//	camToClip[1][1] = frustrumScale;
//	camToClip[2][2] = (zFar + zNear) / (zNear - zFar);
//	camToClip[3][3] = -1.f;
//	camToClip[3][2] = (2 * zFar * zNear) / (zNear - zFar);

//	camToClip[2][2] = -1.f;

//	print_mat4(camToClip);

	mat4_identity(modelToWorld);

	glUseProgram(prog);
	glUniformMatrix4fv(worldToViewUnf, 1, GL_FALSE, worldToView[0]);
	glUniformMatrix4fv(modelToWorldUnf, 1, GL_FALSE, modelToWorld[0]);
	glUseProgram(0);

	return 1;
}

static void
initBufs(void)
{
	GLuint bufs[2]; /* vert, idx buffers */

	glGenBuffers(LENGTH(bufs), bufs);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	vertBuf = bufs[0];

	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertData,
		vertData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	idxBuf = bufs[1];

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idxArr,
		idxArr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0,
		(void*)(sizeof vertData / 2));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

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

	return 1;
}

static int
drawObject(const WorldObject *wob, void *data)
{
	if (wob->type == WORLD_BLK_EMPTY)
		return 1;

	eprintf("drawObj: %d, (%.0f, %.0f, %.0f)\n", wob->type,
		wob->pos[0], wob->pos[1], wob->pos[2]);

	modelToWorld[3][0] = wob->pos[0];
	modelToWorld[3][1] = wob->pos[1];
	modelToWorld[3][2] = wob->pos[2];

	glUniformMatrix4fv(worldToViewUnf, 1, GL_FALSE, worldToView[0]);
	glUniformMatrix4fv(modelToWorldUnf, 1, GL_FALSE, modelToWorld[0]);

//	print_mat4(modelToWorld);

	glDrawElements(GL_TRIANGLES, LENGTH(idxArr), GL_UNSIGNED_SHORT, 0);

//	return 0; // debug
	return 1;
}

void
drawFrame(void)
{
	static size_t n;
	eprintf("drawingFrame(): %zu\n", ++n);

//	glClearColor(0.0f, 0.15f, 0.3f, 0.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(prog);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuf);

	mat4 m;
	vec3 eyePos, viewDir;
	float yFov, aspectRatio, zNear, zFar;

	yFov = DEG_TO_RAD(110);
	aspectRatio = 1.f;
	zNear = 0.5f;
	zFar = 5.f;

//	yFov = M_PI_2;

	worldGetEyePos(eyePos, viewDir);

	mat4_perspective(m, yFov, aspectRatio, zNear, zFar);
	mat4_translate(m, m, eyePos);

//	mat4_identity(m);

//	mat4_rotX(m, m, viewDir[1]);
//	mat4_rotY(m, m, viewDir[2]);

	mat4_cpy(worldToView, m);

	print_mat4(worldToView);

	worldObjIter(drawObject, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
