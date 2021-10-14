/* vecmath.h */

#define M_PI    3.14159265358979323846	/* pi */
#define M_PI_2  1.57079632679489661923	/* pi/2 */

#define M_DEG_TO_RAD(D) ((D)/ 180.f * M_PI)
#define M_SQR(X)        ((X) * (X))

#define VEC2_COMP_IDX(v, i) (*(float*[]){ &(v).x, &(v).y }[i])
#define VEC3_COMP_IDX(v, i) (*(float*[]){ &(v).x, &(v).y, &(v).z }[i])
#define MAT3_INIT( \
	a00, a01, a02, \
	a10, a11, a12, \
	a20, a21, a22, unused) \
	{{ \
		{ a00, a10, a20, }, \
		{ a01, a11, a21, }, \
		{ a02, a12, a22, }, \
	}}
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

#define VEC2_PRINT(v) printf("%s: (%-10f, %-10f)\n", #v, (v).x, (v).y).z)
#define VEC3_PRINT(v) printf("%s: (%-10f, %-10f, %-10f)\n", #v, (v).x, (v).y, (v).z)
#define MAT3_PRINT(m) \
	do { \
		for (unsigned int r = 0; r < 3; r++) { \
			printf("%s: ", #m); \
			for (unsigned int c = 0; c < 3; c++) \
				printf("%-10f", (m).raw[c][r]); \
			printf("\n"); \
		} \
	} while (0)
#define MAT4_PRINT(m) \
	do { \
		for (unsigned int r = 0; r < 4; r++) { \
			printf("%s: ", #m); \
			for (unsigned int c = 0; c < 4; c++) \
				printf("%-10f", (m).raw[c][r]); \
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
	float raw[3][3];
} mat3;

typedef struct {
	float raw[4][4];
} mat4;

void vec2_add(vec2 *ret, vec2 const *v1, vec2 const *v2);
void vec2_sub(vec2 *ret, vec2 const *v1, vec2 const *v2);
void vec2_negate(vec2 *ret, vec2 const *vec);
void vec2_scale(vec2 *ret, vec2 const *vec, float f);
float vec2_dot(vec2 const *v1, vec2 const *v2);
float vec2_norm(vec2 const *vec);

void vec3_add(vec3 *ret, vec3 const *v1, vec3 const *v2);
void vec3_sub(vec3 *ret, vec3 const *v1, vec3 const *v2);
void vec3_negate(vec3 *ret, vec3 const *vec);
void vec3_scale(vec3 *ret, vec3 const *vec, float f);
float vec3_dot(vec3 const *v1, vec3 const *v2);
float vec3_norm(vec3 const *vec);

void mat3_identity(mat3 *ret);
void mat3_mul(mat3 *ret, mat3 const *m1, mat3 const *m2);
void mat3_mul_vec3(vec3 *ret, mat3 const *mat, vec3 const *vec);
void mat3_rot_x_mat(mat3 *ret, float angl);
void mat3_rot_y_mat(mat3 *ret, float angl);
void mat3_rot_z_mat(mat3 *ret, float angl);
void mat3_rot_x(mat3 *ret, mat3 const *mat, float angl);
void mat3_rot_y(mat3 *ret, mat3 const *mat, float angl);
void mat3_rot_z(mat3 *ret, mat3 const *mat, float angl);
void mat3_to_mat4(mat4 *ret, mat3 const *mat);

void mat4_identity(mat4 *ret);
void mat4_mul(mat4 *ret, mat4 const *m1, mat4 const *m2);
void mat4_perspective(mat4 *ret, float yFov, float asp, float n, float f);
void mat4_translation_mat(mat4 *ret, vec3 const *point);
void mat4_translate(mat4 *ret, mat4 const *mat, vec3 const *point);
void mat4_camera_mat(mat4 *ret, vec3 const *eyePos, vec3 const *eyeDir);
void mat4_apply_cam(mat4 *ret, mat4 const *mat, vec3 const *eyePos, vec3 const *eyeDir);
void mat4_rot_x_mat(mat4 *ret, float angl);
void mat4_rot_y_mat(mat4 *ret, float angl);
void mat4_rot_z_mat(mat4 *ret, float angl);
void mat4_rot_x(mat4 *ret, mat4 const *mat, float angl);
void mat4_rot_y(mat4 *ret, mat4 const *mat, float angl);
void mat4_rot_z(mat4 *ret, mat4 const *mat, float angl);
void mat4_to_mat3(mat3 *ret, mat4 const *mat);
