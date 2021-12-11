#include <math.h>
#include <float.h>

#include "util.h"
#include "vecmath.h"
#include "do.h"

/* compile-time checks that [i]vec[23]'s components are adjacent */
struct ivec2_chk { char ok[sizeof(struct ivec2) == 2 * sizeof(int) ? 1 : -1]; };
struct ivec3_chk { char ok[sizeof(struct ivec3) == 3 * sizeof(int) ? 1 : -1]; };
struct vec2_chk { char ok[sizeof(struct vec2) == 2 * sizeof(float) ? 1 : -1]; };
struct vec3_chk { char ok[sizeof(struct vec3) == 3 * sizeof(float) ? 1 : -1]; };

/* ivec3 */
struct vec3 *
ivec3_to_vec3(struct vec3 *d, const struct ivec3 *v)
{
	return vec3_set(d, v->x, v->y, v->z);
}

struct ivec3 *
ivec3_set(struct ivec3 *d, FPARS(int, x, y, z))
{
	*d = (struct ivec3){ x, y, z };
	return d;
}

struct ivec3 *
vec3_to_ivec3(struct ivec3 *d, const struct vec3 *v)
{
	#define m(I, C) \
		d->C = floorf(v->C);
	do3(m);
	#undef m
	return d;
}

/* vec2 */
const struct vec2 vec2_zero = { 0.f, 0.f };

struct vec2 *
vec2_set(struct vec2 *d, FPARS(float, x, y))
{
	*d = (struct vec2){ x, y };
	return d;
}

struct vec2 *
vec2_cpy(struct vec2 *d, const struct vec2 *vec)
{
	*d = *vec;
	return d;
}

struct vec2 *
vec2_add(struct vec2 *d, FPARS(const struct vec2, *a, *b))
{
	#define m(I, C) \
		d->C = a->C + b->C;
	do2(m);
	#undef m
	return d;
}

struct vec2 *
vec2_sub(struct vec2 *d, FPARS(const struct vec2, *a, *b))
{
	#define m(I, C) \
		d->C = a->C - b->C;
	do2(m);
	#undef m
	return d;
}

struct vec2 *
vec2_neg(struct vec2 *d, const struct vec2 *v)
{
	#define m(I, C) \
		d->C = -v->C;
	do2(m);
	#undef m
	return d;
}

struct vec2 *
vec2_scal(struct vec2 *d, const struct vec2 *v, float f)
{
	#define m(I, C) \
		d->C = v->C * f;
	do2(m);
	#undef m
	return d;
}

float
vec2_dot(FPARS(const struct vec2, *a, *b))
{
	#define e(I, C) \
		a->C * b->C
	return do2e(e, +);
	#undef e
}

float
vec2_norm2(const struct vec2 *v)
{
	return vec2_dot(v, v);
}

float
vec2_norm(const struct vec2 *v)
{
	return sqrt(vec2_norm2(v));
}

int
vec2_iseq(FPARS(const struct vec2, *a, *b))
{
	#define e(I, C) \
		fabsf(a->C - b->C) < FLT_EPSILON
	return do2e(e, &&);
	#undef e
}

/* vec3 */
const struct vec3 vec3_zero = { 0.f, 0.f, 0.f };

struct vec3 *
vec3_set(struct vec3 *d, FPARS(float, x, y, z))
{
	*d = (struct vec3){ x, y, z };
	return d;
}

struct vec3 *
vec3_cpy(struct vec3 *d, const struct vec3 *v)
{
	*d = *v;
	return d;
}

struct vec3 *
vec3_add(struct vec3 *d, FPARS(const struct vec3, *a, *b))
{
	#define m(I, C) \
		d->C = a->C + b->C;
	do3(m);
	#undef m
	return d;
}

struct vec3 *
vec3_addi(struct vec3 *a, const struct vec3 *b)
{
	return vec3_add(a, a, b);
}

struct vec3 *
vec3_sub(struct vec3 *d, FPARS(const struct vec3, *a, *b))
{
	#define m(I, C) \
		d->C = a->C - b->C;
	do3(m);
	#undef m
	return d;
}

struct vec3 *
vec3_subi(struct vec3 *a, const struct vec3 *b)
{
	return vec3_sub(a, a, b);
}

struct vec3 *
vec3_neg(struct vec3 *d, const struct vec3 *v)
{
	#define m(I, C) \
		d->C = -v->C;
	do3(m);
	#undef m
	return d;
}

struct vec3 *
vec3_negi(struct vec3 *v)
{
	return vec3_neg(v, v);
}

struct vec3 *
vec3_scal(struct vec3 *d, const struct vec3 *v, float f)
{
	#define m(I, C) \
		d->C = v->C * f;
	do3(m);
	#undef m
	return d;
}

struct vec3 *
vec3_scali(struct vec3 *v, float f)
{
	return vec3_scal(v, v, f);
}

struct vec3 *
vec3_scalv(struct vec3 *d, FPARS(const struct vec3, *a, *b))
{
	#define m(I, C) \
		d->C = a->C * b->C;
	do3(m);
	#undef m
	return d;
}

struct vec3 *
vec3_scalvi(struct vec3 *a, const struct vec3 *b)
{
	return vec3_scalv(a, a, b);
}

struct vec3 *
vec3_normlz(struct vec3 *d, const struct vec3 *v)
{
	float n;

	if ((n = vec3_norm(v)))
		return vec3_scal(d, v, 1 / n);
	return vec3_cpy(d, &vec3_zero);
}

float
vec3_dot(FPARS(const struct vec3, *a, *b))
{
	#define e(I, C) \
		a->C * b->C
	return do3e(e, +);
	#undef e
}

float
vec3_norm2(const struct vec3 *v)
{
	return vec3_dot(v, v);
}

float
vec3_norm(const struct vec3 *v)
{
	return sqrt(vec3_norm2(v));
}

int
vec3_iseq(FPARS(const struct vec3, *a, *b))
{
	#define e(I, C) \
		fabsf(a->C - b->C) < FLT_EPSILON
	return do3e(e, &&);
	#undef e
}

/* mat3 */
const struct mat3 mat3_identity = MAT3_INIT(
	1.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f,
);

struct mat4 *
mat3_to_mat4(struct mat4 *d, const struct mat3 *m)
{
	#define elm(C, _, R) \
		d->m[C][R] = m->m[C][R];
	#define row3(R, _) \
		do3v(elm, R);
	do3(row3);
	#undef elm
	#undef row3
	#define col4(R, _) \
		d->m[3][R]
	#define row4(C, _) \
		d->m[C][3]
	do3r(col4,,, =) = do3r(row4,,, =) = 0.f;
	#undef col4
	#undef row4
	d->m[3][3] = 1.f;
	return d;
}

struct mat3 *
mat3_mul(struct mat3 *d, FPARS(const struct mat3, *a, *b))
{
	struct mat3 r;

	#define elme(I, _, R, C) \
		a->m[I][R] * b->m[C][I]
	#define elm(C, _, R) \
		r.m[C][R] = do3ev(elme, +, R, C);
	#define row(R, _) \
		do3v(elm, R);
	do3(row);
	#undef elme
	#undef elm
	#undef row
	*d = r;
	return d;
}

struct mat3 *
mat3_rotx_mat(struct mat3 *d, float angl)
{
	float s = sinf(angl), c = cosf(angl);

	*d = (struct mat3)MAT3_INIT(
		1.f, 0.f, 0.f,
		0.f,   c,  -s,
		0.f,   s,   c,
	);
	return d;
}

struct mat3 *
mat3_roty_mat(struct mat3 *d, float angl)
{
	float s = sinf(angl), c = cosf(angl);

	*d = (struct mat3)MAT3_INIT(
		  c, 0.f,   s,
		0.f, 1.f, 0.f,
		 -s, 0.f,   c,
	);
	return d;
}

struct mat3 *
mat3_rotz_mat(struct mat3 *d, float angl)
{
	float s = sinf(angl), c = cosf(angl);

	*d = (struct mat3)MAT3_INIT(
		  c,   s, 0.f,
		 -s,   c, 0.f,
		0.f, 0.f, 1.f,
	);
	return d;
}

struct mat3 *
mat3_rotx(struct mat3 *d, const struct mat3 *m, float angl)
{
	struct mat3 rotm;

	return mat3_mul(d, m, mat3_rotx_mat(&rotm, angl));
}

struct mat3 *
mat3_roty(struct mat3 *d, const struct mat3 *m, float angl)
{
	struct mat3 rotm;

	return mat3_mul(d, m, mat3_roty_mat(&rotm, angl));
}

struct mat3 *
mat3_rotz(struct mat3 *d, const struct mat3 *m, float angl)
{
	struct mat3 rotm;

	return mat3_mul(d, m, mat3_rotz_mat(&rotm, angl));
}

struct vec3 *
mat3_mulv(struct vec3 *d, const struct mat3 *m, const struct vec3 *v)
{
	struct vec3 r;

	#define elme(I, C, R) \
		m->m[I][R] * v->C
	#define elm(R, C) \
		r.C = do3ev(elme, +, R);
	do3(elm);
	#undef elme
	#undef elm
	return vec3_cpy(d, &r);
}

/* mat4 */
const struct mat4 mat4_identity = MAT4_INIT(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f,
);

struct mat4 *
mat4_mul(struct mat4 *d, FPARS(const struct mat4, *m1, *m2))
{
	struct mat4 res;

	#define elme(I, _, R, C) \
		m1->m[I][R] * m2->m[C][I]
	#define elm(C, _, R) \
		res.m[C][R] = do4ev(elme, +, R, C);
	#define row(R, _) \
		do4v(elm, R);
	do4(row);
	#undef elme
	#undef elm
	#undef row
	*d = res;
	return d;
}

struct mat4 *
mat4_persp(struct mat4 *d, FPARS(float, yfov, asp, n, f))
{
	float a = 1.f / tanf(yfov / 2.f);

	*d = (struct mat4){{
		[0][0] = a / asp,
		[1][1] = a,
		[2][2] = -(n + f) / (n - f),
		[2][3] = 1.f,
		[3][2] = (2.f * n * f) / (n - f),
	}};
	return d;
}

struct mat4 *
mat4_trn_mat(struct mat4 *d, const struct vec3 *p)
{
	*d = (struct mat4)MAT4_INIT(
		1.f, 0.f, 0.f, -p->x,
		0.f, 1.f, 0.f, -p->y,
		0.f, 0.f, 1.f, -p->z,
		0.f, 0.f, 0.f, 1.f,
	);
	return d;
}

struct mat4 *
mat4_trlt(struct mat4 *d, const struct mat4 *m, const struct vec3 *p)
{
	struct mat4 trm;

	return mat4_mul(d, m, mat4_trn_mat(&trm, p));
}

struct mat4 *
mat4_trlti(struct mat4 *d, const struct vec3 *p)
{
	return mat4_trlt(d, d, p);
}

struct mat4 *
mat4_cam_mat(struct mat4 *d, FPARS(const struct vec3, *eyePos, *eyeDir))
{
	float sa = sinf(eyeDir->x), ca = cosf(eyeDir->x),
	      sb = sinf(eyeDir->y), cb = cosf(eyeDir->y);
	/* eyeDir->z is ignored */

	*d = (struct mat4)MAT4_INIT(
		cb,       0.f, sb,       0.f,
		sa * sb,  ca,  -sa * cb, 0.f,
		-ca * sb, sa,  ca * cb,  0.f,
		0.f,      0.f, 0.f,      1.f,
	);
	return mat4_trlti(d, eyePos);
}

struct mat4 *
mat4_cam_apply(struct mat4 *d, const struct mat4 *mat,
	FPARS(const struct vec3, *eyePos, *eyeDir))
{
	struct mat4 cmat;

	return mat4_mul(d, mat, mat4_cam_mat(&cmat, eyePos, eyeDir));
}

struct mat4 *
mat4_cam_applyi(struct mat4 *d, FPARS(const struct vec3, *eyePos, *eyeDir))
{
	return mat4_cam_apply(d, d, eyePos, eyeDir);
}

struct mat4 *
mat4_rotx_mat(struct mat4 *d, float angl)
{
	struct mat3 rotm;

	return mat3_to_mat4(d, mat3_rotx_mat(&rotm, angl));
}

struct mat4 *
mat4_roty_mat(struct mat4 *d, float angl)
{
	struct mat3 rotm;

	return mat3_to_mat4(d, mat3_roty_mat(&rotm, angl));
}

struct mat4 *
mat4_rotz_mat(struct mat4 *d, float angl)
{
	struct mat3 rotm;

	return mat3_to_mat4(d, mat3_rotz_mat(&rotm, angl));
}

struct mat4 *
mat4_rotx(struct mat4 *d, const struct mat4 *m, float angl)
{
	struct mat4 rotm;

	return mat4_mul(d, m, mat4_rotx_mat(&rotm, angl));
}

struct mat4 *
mat4_roty(struct mat4 *d, const struct mat4 *m, float angl)
{
	struct mat4 rotm;

	return mat4_mul(d, m, mat4_roty_mat(&rotm, angl));
}

struct mat4 *
mat4_rotz(struct mat4 *d, const struct mat4 *m, float angl)
{
	struct mat4 rotm;

	return mat4_mul(d, m, mat4_rotz_mat(&rotm, angl));
}

struct mat3 *
mat4_to_mat3(struct mat3 *d, const struct mat4 *m)
{
	#define elm(C, _, R) \
		d->m[C][R] = m->m[C][R];
	#define row(R, _) \
		do3v(elm, R);
	do3(row);
	#undef elm
	#undef row
	return d;
}
