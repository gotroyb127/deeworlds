#include <math.h>
#include <string.h>

#include "vecmath.h"

void
mat4_identity(mat4 m)
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m[i][j] = i == j ? 1.f : 0.f;
}

void
mat4_cpy(mat4 dst, mat4 src)
{
	memcpy(dst, src, sizeof(mat4));
}

void
mat4_mul(mat4 res, mat4 a, mat4 b)
{
	int c, r, k;
	mat4 tmp;
	for (c = 0; c < 4; c++) {
		for (r = 0; r < 4; r++) {
			tmp[c][r] = 0.f;
			for (k = 0; k < 4; k++)
				tmp[c][r] += a[k][r] * b[c][k];
		}
	}
	mat4_cpy(res, tmp);
}

void
mat4_perspective(mat4 m, float yFov, float aspect, float zN, float zF)
{
	float const a = 1.f / tanf(yFov / 2.f);

	m[0][0] = a / aspect;
	m[0][1] = 0.f;
	m[0][2] = 0.f;
	m[0][3] = 0.f;

	m[1][0] = 0.f;
	m[1][1] = a;
	m[1][2] = 0.f;
	m[1][3] = 0.f;

	m[2][0] = 0.f;
	m[2][1] = 0.f;
	m[2][2] = 1.f;
	m[2][3] = 0.f;

	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;

	m[2][2] = -(zN + zF) / (zN - zF);
	m[3][2] = (2.f * zN * zF) / (zN - zF);
	m[2][3] = -1.f;
	m[3][3] = 0.f;

//	float const f = zF, n = zN;
//	float const a = 1.f / tanf(yFov / 2.f);
//
//	m[0][0] = a / aspect;
//	m[0][1] = 0.f;
//	m[0][2] = 0.f;
//	m[0][3] = 0.f;
//
//	m[1][0] = 0.f;
//	m[1][1] = a;
//	m[1][2] = 0.f;
//	m[1][3] = 0.f;
//
//	m[2][0] = 0.f;
//	m[2][1] = 0.f;
//	m[2][2] = -((f + n) / (f - n));
//	m[2][3] = -1.f;
//
//	m[3][0] = 0.f;
//	m[3][1] = 0.f;
//	m[3][2] = -((2.f * f * n) / (f - n));
//	m[3][3] = 0.f;
}

void
mat4_translate(mat4 res, mat4 m, vec3 point)
{
	int i;

	mat4_cpy(res, m);
	for (i = 0; i < 3; i++)
		res[3][i] -= point[i];
}

void
mat4_rotX(mat4 res, mat4 m, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);
	mat4 rotm = {
		{ 1.f, 0.f, 0.f, 0.f },
		{ 0.f,   c,   s, 0.f },
		{ 0.f,  -s,   c, 0.f },
		{ 0.f, 0.f, 0.f, 1.f },
	};
//	mat4_mul(res, rotm, m);
	mat4_mul(res, m, rotm);
}

void
mat4_rotY(mat4 res, mat4 m, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);
	mat4 rotm = {
		{   c, 0.f,  -s, 0.f },
		{ 0.f, 1.f, 0.f, 0.f },
		{   s, 0.f,   c, 0.f },
		{ 0.f, 0.f, 0.f, 1.f },
	};
//	mat4_mul(res, rotm, m);
	mat4_mul(res, m, rotm);
}

void
mat4_rotZ(mat4 res, mat4 m, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);
	mat4 rotm = {
		{   c,   s, 0.f, 0.f },
		{  -s,   c, 0.f, 0.f },
		{ 0.f, 0.f, 1.f, 0.f },
		{ 0.f, 0.f, 0.f, 1.f },
	};
	mat4_mul(res, rotm, m);
}
