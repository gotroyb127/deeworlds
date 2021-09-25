/* vecmath.h */

#define M_PI    3.14159265358979323846	/* pi */
#define M_PI_2  1.57079632679489661923	/* pi/2 */

typedef float vec3[3];
typedef float vec4[4];

typedef vec4 mat4[4];

void mat4_identity(mat4);
void mat4_cpy(mat4 dst, mat4 src);
void mat4_mul(mat4 res, mat4 a, mat4 b);

void mat4_translate(mat4 res, mat4 m, vec3 point);
void mat4_perspective(mat4 m, float yFov, float aspect, float zN, float zF);

void mat4_rotX(mat4 res, mat4 m, float angl);
void mat4_rotY(mat4 res, mat4 m, float angl);
void mat4_rotZ(mat4 res, mat4 m, float angl);
