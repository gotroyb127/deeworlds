/* vecmath.h */

#define M_PI    3.14159265358979323846	/* pi */
#define M_PI_2  1.57079632679489661923	/* pi/2 */

#define M_DEG_TO_RAD(D) ((D)/ 180.f * M_PI)
#define M_SQR(X)        ((X) * (X))

#define VEC_RAW(V)    (&(V)->x)

#define MAT3_INIT( \
	a00, a01, a02, \
	a10, a11, a12, \
	a20, a21, a22, unused) \
	{{ \
		{ a00, a10, a20 }, \
		{ a01, a11, a21 }, \
		{ a02, a12, a22 }, \
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

struct ivec2 {
	int x, y;
};

struct ivec3 {
	int x, y, z;
};

struct vec2 {
	float x, y;
};

struct vec3 {
	float x, y, z;
};

struct mat3 {
	float m[3][3];
};

struct mat4 {
	float m[4][4];
};

/* ivec3 */
struct vec3 *ivec3_to_vec3(struct vec3 *d, const struct ivec3 *v);
struct ivec3 *ivec3_set(struct ivec3 *d, FPARS(int, x, y, z));
struct ivec3 *vec3_to_ivec3(struct ivec3 *d, const struct vec3 *v);

/* vec2 */
extern const struct vec2 vec2_zero;

struct vec2 *vec2_set(struct vec2 *d, FPARS(float, x, y));
struct vec2 *vec2_cpy(struct vec2 *d, const struct vec2 *vec);
struct vec2 *vec2_add(struct vec2 *d, FPARS(const struct vec2, *a, *b));
struct vec2 *vec2_sub(struct vec2 *d, FPARS(const struct vec2, *a, *b));
struct vec2 *vec2_neg(struct vec2 *d, const struct vec2 *v);
struct vec2 *vec2_scal(struct vec2 *d, const struct vec2 *v, float f);
float vec2_dot(FPARS(const struct vec2, *a, *b));
float vec2_norm2(const struct vec2 *v);
float vec2_norm(const struct vec2 *v);
int vec2_iseq(FPARS(const struct vec2, *a, *b));

/* vec3 */
extern const struct vec3 vec3_zero;

struct vec3 *vec3_set(struct vec3 *d, FPARS(float, x, y, z));
struct vec3 *vec3_cpy(struct vec3 *d, const struct vec3 *v);
struct vec3 *vec3_add(struct vec3 *d, FPARS(const struct vec3, *a, *b));
struct vec3 *vec3_addi(struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3_sub(struct vec3 *d, FPARS(const struct vec3, *a, *b));
struct vec3 *vec3_subi(struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3_neg(struct vec3 *d, const struct vec3 *v);
struct vec3 *vec3_negi(struct vec3 *v);
struct vec3 *vec3_scal(struct vec3 *d, const struct vec3 *v, float f);
struct vec3 *vec3_scali(struct vec3 *v, float f);
struct vec3 *vec3_scalv(struct vec3 *d, FPARS(const struct vec3, *a, *b));
struct vec3 *vec3_scalvi(struct vec3 *a, const struct vec3 *b);
struct vec3 *vec3_normlz(struct vec3 *d, const struct vec3 *v, float *n);
float vec3_dot(FPARS(const struct vec3, *a, *b));
float vec3_norm2(const struct vec3 *v);
float vec3_norm(const struct vec3 *v);
int vec3_iseq(FPARS(const struct vec3, *a, *b));

/* mat3 */
extern const struct mat3 mat3_identity;

struct mat4 *mat3_to_mat4(struct mat4 *d, const struct mat3 *m);
struct mat3 *mat3_mul(struct mat3 *d, FPARS(const struct mat3, *a, *b));
struct mat3 *mat3_rotx_mat(struct mat3 *d, float angl);
struct mat3 *mat3_roty_mat(struct mat3 *d, float angl);
struct mat3 *mat3_rotz_mat(struct mat3 *d, float angl);
struct mat3 *mat3_rotx(struct mat3 *d, const struct mat3 *m, float angl);
struct mat3 *mat3_roty(struct mat3 *d, const struct mat3 *m, float angl);
struct mat3 *mat3_rotz(struct mat3 *d, const struct mat3 *m, float angl);
struct vec3 *mat3_mulv(struct vec3 *d, const struct mat3 *m, const struct vec3 *v);

/* mat4 */
extern const struct mat4 mat4_identity;

struct mat4 *mat4_mul(struct mat4 *d, FPARS(const struct mat4, *m1, *m2));
struct mat4 *mat4_persp(struct mat4 *d, FPARS(float, yfov, asp, n, f));
struct mat4 *mat4_trn_mat(struct mat4 *d, const struct vec3 *p);
struct mat4 *mat4_trlt(struct mat4 *d, const struct mat4 *m, const struct vec3 *p);
struct mat4 *mat4_trlti(struct mat4 *d, const struct vec3 *p);
struct mat4 *mat4_cam_mat(struct mat4 *d, FPARS(const struct vec3, *eyePos, *eyeDir));
struct mat4 *mat4_cam_apply(struct mat4 *d, const struct mat4 *mat,
	FPARS(const struct vec3, *eyePos, *eyeDir));
struct mat4 *mat4_cam_applyi(struct mat4 *d, FPARS(const struct vec3, *eyePos, *eyeDir));
struct mat4 *mat4_rotx_mat(struct mat4 *d, float angl);
struct mat4 *mat4_roty_mat(struct mat4 *d, float angl);
struct mat4 *mat4_rotz_mat(struct mat4 *d, float angl);
struct mat4 *mat4_rotx(struct mat4 *d, const struct mat4 *m, float angl);
struct mat4 *mat4_roty(struct mat4 *d, const struct mat4 *m, float angl);
struct mat4 *mat4_rotz(struct mat4 *d, const struct mat4 *m, float angl);
struct mat3 *mat4_to_mat3(struct mat3 *d, const struct mat4 *m);
