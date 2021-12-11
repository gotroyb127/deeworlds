#include <math.h>

#include "util.h"
#include "vecmath.h"
#include "worldParams.h"
#include "player.h"
#include "do.h"

#define TRUNC(X, A, B) ((X) < (A) ? (A) : (X) > (B) ? (B) : (X))

/* non-static function implementations */
void
plrInit(struct player *pl, const struct vec3 *box)
{
	*pl = (struct player){
		.box = *box,
	};
}

void
plrCtlAclSet(struct player *pl, const struct vec3 *set)
{
	#define m(I, C) \
		pl->selAcl.C = TRUNC(set->C, -1.f, 1.f);
	do3(m);
	#undef m
}

void
plrCtlAclAdd(struct player *pl, const struct vec3 *add)
{
	#define m(I, C) \
		pl->selAcl.C = TRUNC(pl->selAcl.C + add->C, -1.f, 1.f);
	do3(m);
	#undef m
}

void
plrCtlCamRot(struct player *pl, const struct vec3 *addDir)
{
	struct vec3 *vDir = &pl->viewDir;

	vDir->x = TRUNC(vDir->x + addDir->x, -M_PI_2, M_PI_2);
	vDir->y = fmodf(vDir->y + addDir->y, 2 * M_PI);
	/* unused:
	vDir->z = fmodf(vDir->z + addDir->z, 2 * M_PI);
	*/
}

#define DEBUG 1
#if DEBUG
# include <stdio.h>
#endif
void
plrMove(struct player *pl, double dt)
{
	const struct motionPars *mpar = pl->mtnPars;
	struct mat3 rotm;
	struct vec3 facDir, hVel, unwVel, wanVel, tmpv;
	struct vec3 *pos = &pl->pos, *vel = &pl->vel, *selAcl = &pl->selAcl,
		*viewDir = &pl->viewDir;
	float hSpeed, tmpf;

	vec3_addi(pos, vec3_scal(&tmpv, vel, dt));

	/* jump */
	if (selAcl->y > 0.f)
		vel->y = selAcl->y * mpar->jumpVel;
	else
		vel->y -= mpar->gravity * dt;
	selAcl->y = 0.f;

	mat3_roty(&rotm, &mat3_identity, -viewDir->y);
	mat3_mulv(&facDir, &rotm, vec3_normlz(&tmpv, selAcl));

	tmpf = vec3_dot(vec3_set(&hVel, vel->x, 0.f, vel->z), &facDir);
	vec3_scal(&wanVel, &facDir, tmpf > 0.f ? tmpf : 0.f);
	vec3_sub(&unwVel, &hVel, &wanVel);

	/* accelerate wanted velocity */
	tmpf = mpar->accel * dt;
	wanVel.x += facDir.x * tmpf;
	wanVel.z += facDir.z * tmpf;

	/* apply friction to unwated velocity */
	tmpf = powf(mpar->friction, dt);
	unwVel.x *= tmpf;
	unwVel.z *= tmpf;

	vec3_add(&hVel, &wanVel, &unwVel);

	if ((hSpeed = vec3_norm(&hVel)) > mpar->maxSpeed)
		vec3_scali(&hVel, mpar->maxSpeed / hSpeed);
	vel->x = hVel.x;
	vel->z = hVel.z;
#if DEBUG
#define VEC3_PRINT_N(v) VEC3_PRINTF(v, " %-10f\n", vec3_norm(v))
#define VEC3_PRINT(v) \
		printf("%-15s: (%-10f, %-10f, %-10f)\n", #v, (v)->x, (v)->y, (v)->z)
#define VEC3_PRINTF(v, ...) \
	do { \
		printf("%-15s: (%-10f, %-10f, %-10f)", #v, (v)->x, (v)->y, (v)->z); \
		printf(__VA_ARGS__); \
	} while (0)

	static unsigned int n = 0;
	if (n++)
		printf("\033[7A\033[K");
	printf("hSpeed: %-10f, friction: %-10f\n", vec3_norm(&hVel), tmpf);
	printf("dt: %-10g, fps: %-10g\n", dt, 1 / dt);

	VEC3_PRINT_N(&facDir);
	VEC3_PRINT_N(&wanVel);
	VEC3_PRINT_N(&unwVel);
	VEC3_PRINT(pos);
	VEC3_PRINT(vel);
#endif /* DEBUG */
}
