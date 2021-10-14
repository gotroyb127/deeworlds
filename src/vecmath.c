#include <math.h>

#include "vecmath.h"

void
vec2_add(vec2 *ret, vec2 const *v1, vec2 const *v2)
{
	*ret = (vec2){
		v1->x + v2->x,
		v1->y + v2->y,
	};
}

void
vec2_sub(vec2 *ret, vec2 const *v1, vec2 const *v2)
{
	vec2_negate(ret, v2);
	vec2_add(ret, v1, ret);
}

void
vec2_negate(vec2 *ret, vec2 const *vec)
{
	*ret = (vec2){
		-vec->x,
		-vec->y,
	};
}

void
vec2_scale(vec2 *ret, vec2 const *vec, float f)
{
	*ret = (vec2){
		vec->x * f,
		vec->y * f,
	};
}

float
vec2_dot(vec2 const *v1, vec2 const *v2)
{
	return v1->x * v2->x + v1->y * v2->y;
}

float
vec2_norm(vec2 const *vec)
{
	return sqrt(vec2_dot(vec, vec));
}

void
vec3_add(vec3 *ret, vec3 const *v1, vec3 const *v2)
{
	*ret = (vec3){
		v1->x + v2->x,
		v1->y + v2->y,
		v1->z + v2->z,
	};
}

void
vec3_sub(vec3 *ret, vec3 const *v1, vec3 const *v2)
{
	vec3_negate(ret, v2);
	vec3_add(ret, v1, ret);
}

void
vec3_negate(vec3 *ret, vec3 const *vec)
{
	*ret = (vec3){
		-vec->x,
		-vec->y,
		-vec->z,
	};
}

void
vec3_scale(vec3 *ret, vec3 const *vec, float f)
{
	*ret = (vec3){
		vec->x * f,
		vec->y * f,
		vec->z * f,
	};
}

float
vec3_dot(vec3 const *v1, vec3 const *v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

float
vec3_norm(vec3 const *vec)
{
	return sqrt(vec3_dot(vec, vec));
}

void
mat3_identity(mat3 *ret)
{
	*ret = (mat3)MAT3_INIT(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
	);
}

void
mat3_mul(mat3 *ret, mat3 const *m1, mat3 const *m2)
{
	int c, r, k;
	mat3 res;

	for (c = 0; c < 3; c++) {
		for (r = 0; r < 3; r++) {
			res.raw[c][r] = 0.f;
			for (k = 0; k < 3; k++)
				res.raw[c][r] += m1->raw[k][r] * m2->raw[c][k];
		}
	}
	*ret = res;
}

void
mat3_mul_vec3(vec3 *ret, mat3 const *mat, vec3 const *vec)
{
	*ret = (vec3){
		mat->raw[0][0] * vec->x + mat->raw[1][0] * vec->y + mat->raw[2][0] * vec->z,
		mat->raw[0][1] * vec->x + mat->raw[1][1] * vec->y + mat->raw[2][1] * vec->z,
		mat->raw[0][2] * vec->x + mat->raw[1][2] * vec->y + mat->raw[2][2] * vec->z,
	};
}

void
mat3_rot_x_mat(mat3 *ret, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);

	*ret = (mat3)MAT3_INIT(
		1.f, 0.f, 0.f,
		0.f,   c,  -s,
		0.f,   s,   c,
	);
}

void
mat3_rot_y_mat(mat3 *ret, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);

	*ret = (mat3)MAT3_INIT(
		  c, 0.f,   s,
		0.f, 1.f, 0.f,
		 -s, 0.f,   c,
	);
}

void
mat3_rot_z_mat(mat3 *ret, float angl)
{
	float s = sinf(angl);
	float c = cosf(angl);

	*ret = (mat3)MAT3_INIT(
		  c,   s, 0.f,
		 -s,   c, 0.f,
		0.f, 0.f, 1.f,
	);
}

void
mat3_rot_x(mat3 *ret, mat3 const *mat, float angl)
{
	mat3 rotm;
	mat3_rot_x_mat(&rotm, angl);
	mat3_mul(ret, mat, &rotm);
}

void
mat3_rot_y(mat3 *ret, mat3 const *mat, float angl)
{
	mat3 rotm;
	mat3_rot_y_mat(&rotm, angl);
	mat3_mul(ret, mat, &rotm);
}

void
mat3_rot_z(mat3 *ret, mat3 const *mat, float angl)
{
	mat3 rotm;
	mat3_rot_z_mat(&rotm, angl);
	mat3_mul(ret, mat, &rotm);
}

void
mat3_to_mat4(mat4 *ret, mat3 const *mat)
{
	*ret = (mat4){{
		{ mat->raw[0][0], mat->raw[0][1], mat->raw[0][2], 0.f },
		{ mat->raw[1][0], mat->raw[1][1], mat->raw[1][2], 0.f },
		{ mat->raw[2][0], mat->raw[2][1], mat->raw[2][2], 0.f },
		{ 0.f,            0.f,            0.f,            1.f },
	}};
}

void
mat4_identity(mat4 *ret)
{
	*ret = (mat4)MAT4_INIT(
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	);
}

void
mat4_mul(mat4 *ret, mat4 const *m1, mat4 const *m2)
{
	int c, r, k;
	mat4 res;

	for (c = 0; c < 4; c++) {
		for (r = 0; r < 4; r++) {
			res.raw[c][r] = 0.f;
			for (k = 0; k < 4; k++)
				res.raw[c][r] += m1->raw[k][r] * m2->raw[c][k];
		}
	}
	*ret = res;
}

void
mat4_perspective(mat4 *ret, float yFov, float asp, float n, float f)
{
	float a = 1.f / tanf(yFov / 2.f);

	*ret = (mat4){{
		[0][0] = a / asp,
		[1][1] = a,
		[2][2] = -(n + f) / (n - f),
		[2][3] = 1.f,
		[3][2] = (2.f * n * f) / (n - f),
	}};
}

void
mat4_translation_mat(mat4 *ret, vec3 const *point)
{
	*ret = (mat4)MAT4_INIT(
		1.f, 0.f, 0.f, -point->x,
		0.f, 1.f, 0.f, -point->y,
		0.f, 0.f, 1.f, -point->z,
		0.f, 0.f, 0.f, 1.f,
	);
}

void
mat4_translate(mat4 *ret, mat4 const *mat, vec3 const *point)
{
	mat4 tmat;

	mat4_translation_mat(&tmat, point);
	mat4_mul(ret, mat, &tmat);
}

void
mat4_camera_mat(mat4 *ret, vec3 const *eyePos, vec3 const *eyeDir)
{
	float sina = sinf(eyeDir->x);
	float cosa = cosf(eyeDir->x);
	float sinb = sinf(eyeDir->y);
	float cosb = cosf(eyeDir->y);
	/* eyeDir->z is ignored as unuseful */

	*ret = (mat4)MAT4_INIT(
		cosb,         0.f,  sinb,         0.f,
		sina * sinb,  cosa, -sina * cosb, 0.f,
		-cosa * sinb, sina, cosa * cosb,  0.f,
		0.f,          0.f,  0.f,          1.f,
	);
	mat4_translate(ret, ret, eyePos);
}

void
mat4_apply_cam(mat4 *ret, mat4 const *mat, vec3 const *eyePos, vec3 const *eyeDir)
{
	mat4 cmat;

	mat4_camera_mat(&cmat, eyePos, eyeDir);
	mat4_mul(ret, mat, &cmat);
}

void
mat4_rot_x_mat(mat4 *ret, float angl)
{
	mat3 tmp;
	mat3_rot_x_mat(&tmp, angl);
	mat3_to_mat4(ret, &tmp);
}

void
mat4_rot_y_mat(mat4 *ret, float angl)
{
	mat3 tmp;
	mat3_rot_y_mat(&tmp, angl);
	mat3_to_mat4(ret, &tmp);
}

void
mat4_rot_z_mat(mat4 *ret, float angl)
{
	mat3 tmp;
	mat3_rot_z_mat(&tmp, angl);
	mat3_to_mat4(ret, &tmp);
}

void
mat4_rot_x(mat4 *ret, mat4 const *mat, float angl)
{
	mat4 rotm;
	mat4_rot_x_mat(&rotm, angl);
	mat4_mul(ret, mat, &rotm);
}

void
mat4_rot_y(mat4 *ret, mat4 const *mat, float angl)
{
	mat4 rotm;
	mat4_rot_y_mat(&rotm, angl);
	mat4_mul(ret, mat, &rotm);
}

void
mat4_rot_z(mat4 *ret, mat4 const *mat, float angl)
{
	mat4 rotm;
	mat4_rot_z_mat(&rotm, angl);
	mat4_mul(ret, mat, &rotm);
}

void
mat4_to_mat3(mat3 *ret, mat4 const *mat)
{
	*ret = (mat3){{
		{ mat->raw[0][0], mat->raw[0][1], mat->raw[0][2] },
		{ mat->raw[1][0], mat->raw[1][1], mat->raw[1][2] },
		{ mat->raw[2][0], mat->raw[2][1], mat->raw[2][2] },
	}};
}
