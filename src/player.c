#include <math.h>

#include "vecmath.h"
#include "worldParams.h"
#include "player.h"

#define TRUNC(X, A, B) ((X) < (A) ? (A) : (X) > (B) ? (B) : (X))

typedef struct {
	vec3 pos, vel;
	vec3 reqMov; /* requested movement */
	vec3 viewDir;
} Player;

/* static variables */
static Player sPlayer;

/* non-static function implementations */
void
playerInit(void)
{
	sPlayer = (Player){0};
}

void
playerSpawn(vec3 const *pos)
{
	sPlayer.pos = *pos;
	sPlayer.vel = vec3_zero;
}

void
playerSetAccel(vec3 const *accp)
{
	sPlayer.reqMov = *accp;
}

void
playerAddAccel(vec3 const *accp)
{
	vec3 *plAccp = &sPlayer.reqMov;

	*plAccp = (vec3){
		TRUNC(plAccp->x + accp->x, -1.f, 1.f),
		TRUNC(plAccp->y + accp->y, -1.f, 1.f),
		TRUNC(plAccp->z + accp->z, -1.f, 1.f),
	};
}

void
playerSubAccel(vec3 const *accp)
{
	vec3 *plAccp = &sPlayer.reqMov;

	*plAccp = (vec3){
		TRUNC(plAccp->x - accp->x, -1.f, 1.f),
		TRUNC(plAccp->y - accp->y, -1.f, 1.f),
		TRUNC(plAccp->z - accp->z, -1.f, 1.f),
	};
}

void
playerRotCam(vec3 const *addDir)
{
	vec3 *vDir = &sPlayer.viewDir;

	vDir->x = TRUNC(vDir->x + addDir->x, -M_PI_2, M_PI_2);
	vDir->y = fmodf(vDir->y + addDir->y, 2 * M_PI);
	/* unused:
	vDir->z = fmodf(vDir->z + addDir->z, 2 * M_PI);
	*/
}

void
playerGetPos(vec3 *plPos, vec3 *plDir)
{
	*plPos = sPlayer.pos;
	*plDir = sPlayer.viewDir;
}

#define DEBUG 0
#if DEBUG
# include <stdio.h>
#endif
void
playerMove(MotionParams const *mpar, double dt)
{
	mat3 rotm[1];
	vec3 *pos, *vel, *reqMov;
	vec3 tmpv[1], movDir[1];
	vec3 hVel[1], unwantedVel[1], wantedVel[1];
	float hSpeed, tmpf;
#if DEBUG
	vec3 reqMovSave[1];
	*reqMovSave = sPlayer.reqMov;
#endif

	pos = &sPlayer.pos;
	vel = &sPlayer.vel;
	reqMov = &sPlayer.reqMov;

	/* pos += vel * dt */
	vec3_scale(tmpv, vel, dt);
	vec3_add(pos, pos, tmpv);

	/* jump */
	if (reqMov->y > 0)
		vel->y = reqMov->y * mpar->jumpVel;
	reqMov->y = 0;
	if (mpar->gravity)
		vel->y -= mpar->gravity * dt;

	vec3_normalize(movDir, &(vec3){ reqMov->x, 0.f, reqMov->z });
	mat3_rot_y(rotm, &mat3_identity, -sPlayer.viewDir.y);
	mat3_mul_vec3(movDir, rotm, movDir);

	hVel->x = vel->x;
	hVel->y = 0.f;
	hVel->z = vel->z;

	tmpf = vec3_dot(hVel, movDir);
	vec3_scale(wantedVel, movDir, tmpf > 0 ? tmpf : 0.f);
	vec3_sub(unwantedVel, hVel, wantedVel);

	tmpf = mpar->accel * dt;
	wantedVel->x += movDir->x * tmpf;
	wantedVel->z += movDir->z * tmpf;

	/* apply friction to unwated velocity */
	tmpf = powf(mpar->friction, 1 / dt);
	unwantedVel->x *= tmpf;
	unwantedVel->z *= tmpf;

	vec3_add(hVel, wantedVel, unwantedVel);

	if ((hSpeed = vec3_norm(hVel)) >= mpar->maxSpeed)
		vec3_scale(hVel, hVel, mpar->maxSpeed / hSpeed);
	vel->x = hVel->x;
	vel->z = hVel->z;
#if DEBUG
#define VEC3_PRINT_N(v) VEC3_PRINTF(v, " %-10f\n", vec3_norm(&(v)))
#define VEC3_PRINT(v) \
		printf("%-15s: (%-10f, %-10f, %-10f)\n", #v, (v).x, (v).y, (v).z)
#define VEC3_PRINTF(v, ...) \
	do { \
		printf("%-15s: (%-10f, %-10f, %-10f)", #v, (v).x, (v).y, (v).z); \
		printf(__VA_ARGS__); \
	} while (0)

	if (tmpf == 0.f)
		return;

	static unsigned int n = 0;
	if (n++)
		printf("\033[9A\033[K");
	printf("hSpeed: %-10f\n, friction: %-10f\n", vec3_norm(vel), tmpf);
	printf("dt: %-10g, fps: %-10g\n", dt, 1 / dt);

	VEC3_PRINT(*reqMovSave);
	VEC3_PRINT_N(*movDir);
	VEC3_PRINT_N(*wantedVel);
	VEC3_PRINT_N(*unwantedVel);
	VEC3_PRINT(*pos);
	VEC3_PRINT(*vel);
#endif /* DEBUG */
}
