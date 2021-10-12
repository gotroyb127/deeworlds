/* vecmath.h */

#define M_PI    3.14159265358979323846	/* pi */
#define M_PI_2  1.57079632679489661923	/* pi/2 */

#define DEG_TO_RAD(D) ((D)/ 180.f * M_PI)

#define VEC2_COMP_IDX(v, i) (*(float*[]){ &(v).x, &(v).y }[i])
#define VEC3_COMP_IDX(v, i) (*(float*[]){ &(v).x, &(v).y, &(v).z }[i])
#define MAT4_INIT( \
	a00, a01, a02, a03, \
	a10, a11, a12, a13, \
	a20, a21, a22, a23, \
	a30, a31, a32, a33, unused) \
	{{ \
		{ a00, a10, a20, a30 }, \
		{ a01, a11, a21, a31 }, \
		{ a02, a12, a22, a32 }, \
		{ a03, a13, a23, a33 }, \
	}}

#define VEC3_PRINT(v) \
	do { \
		printf("%s: (%-9.g, %-9.g, %-9.g)\n", #v, (v).x, (v).y, (v).z); \
	} while (0)
#define MAT4_PRINT(m) \
	do { \
		for (unsigned int r = 0; r < 4; r++) { \
			printf("%s: ", #m); \
			for (unsigned int c = 0; c < 4; c++) \
				printf("%-9.g", (m).raw[c][r]); \
			printf("\n"); \
		} \
	} while (0)

typedef struct {
	float x, y;
} vec2;

typedef struct {
	float x, y, z;
} vec3;

typedef struct {
	float raw[4][4];
} mat4;

void vec2_add(vec2 *ret, vec2 const *v1, vec2 const *v2);
void vec2_sub(vec2 *ret, vec2 const *v1, vec2 const *v2);
void vec2_negate(vec2 *ret, vec2 const *v);
void vec2_scale(vec2 *ret, vec2 const *vec, float f);
float vec2_cross(vec2 const *v1, vec2 const *v2);

void vec3_add(vec3 *ret, vec3 const *v1, vec3 const *v2);
void vec3_sub(vec3 *ret, vec3 const *v1, vec3 const *v2);
void vec3_negate(vec3 *ret, vec3 const *v);
void vec3_scale(vec3 *ret, vec3 const *vec, float f);
float vec3_cross(vec3 const *v1, vec3 const *v2);

void mat4_identity(mat4 *ret);
void mat4_mul(mat4 *ret, mat4 const *m1, mat4 const *m2);

void mat4_perspective(mat4 *ret, float yFov, float asp, float zN, float zF);
void mat4_translation_mat(mat4 *ret, vec3 const *point);
void mat4_translate(mat4 *ret, mat4 const *m, vec3 const *point);

void mat4_apply_cam(mat4 *ret, mat4 const *mat, vec3 const *eyePos, vec3 const *eyeDir);

void mat4_rot_x(mat4 *res, mat4 const *mat, float angl);
void mat4_rot_y(mat4 *res, mat4 const *mat, float angl);
void mat4_rot_z(mat4 *res, mat4 const *mat, float angl);
