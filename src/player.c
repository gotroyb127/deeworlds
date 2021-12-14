#include <math.h>

#include "util.h"
#include "vecmath.h"
#include "worldParams.h"
#include "player.h"
#include "do.h"

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

/* non-static function implementations */
void
plrInit(struct player *pl, const struct vec3 *box)
{
	*pl = (struct player){
		.box = *box,
	};
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
	struct vec3 selDir, hVel, unwVel, wanVel, v;
	struct vec3 *pos = &pl->pos, *vel = &pl->vel, *selAcl = &pl->selAcl,
		*viewDir = &pl->viewDir;
	float f, n, acl;

	vec3_addi(pos, vec3_scal(&v, vel, dt));
	/* jump */
	if (selAcl->y > 0.f)
		vel->y = selAcl->y * mpar->jumpVel;
	else
		vel->y -= mpar->gravity * dt;
	selAcl->y = 0.f;

	mat3_roty(&rotm, &mat3_identity, -viewDir->y);
	mat3_mulv(&selDir, &rotm, vec3_normlz(&v, selAcl, &n));

	f = vec3_dot(vec3_set(&hVel, vel->x, 0.f, vel->z), &selDir);
	vec3_scal(&wanVel, &selDir, f > 0.f ? f : 0.f);
	vec3_sub(&unwVel, &hVel, &wanVel);

	acl = mpar->accel * MIN(n, 1.f);
	if (vec3_norm2(&wanVel) < M_SQR(mpar->maxRblSpd)) {
		/* accelerate wanted velocity */
		#define m(I, C) \
			wanVel.C += selDir.C * acl * dt;
		do2xz(m);
		#undef m
		if ((f = vec3_norm2(&wanVel)) > M_SQR(mpar->maxRblSpd))
			vec3_scali(&wanVel, mpar->maxRblSpd / sqrt(f));
	}
	/* apply friction to unwated velocity */
	vec3_scali(&unwVel, powf(mpar->friction, dt));

	#define m(I, C) \
		vel->C = wanVel.C + unwVel.C;
	do2xz(m);
	#undef m

	/* limit velocity to mpar->maxSpeed */
	if ((f = vec3_norm2(vel)) > M_SQR(mpar->maxSpeed))
		vec3_scali(vel, mpar->maxSpeed / sqrt(f));
#if DEBUG
#define VEC3_PRINT_N(v) VEC3_PRINTF(v, " %-10f\n", vec3_norm(v))
#define VEC3_PRINT(v) \
		printf("%-15s: (%-10f, %-10f, %-10f)\n", #v, (v)->x, (v)->y, (v)->z)
#define VEC3_PRINTF(v, ...) \
	do { \
		printf("%-15s: (%-10f, %-10f, %-10f)", #v, (v)->x, (v)->y, (v)->z); \
		printf(__VA_ARGS__); \
	} while (0)

	static unsigned int i = 0;
	if (i++)
		printf("\033[8A\033[K");
	printf("hSpeed: %-10f, acl: %-10f, n: %-10f\n",
		vec3_norm(vec3_set(&hVel, vel->x, 0.f, vel->z)), acl, n);
	printf("dt: %-10g, fps: %-10g\n", dt, 1 / dt);

	VEC3_PRINT_N(selAcl);
	VEC3_PRINT_N(&selDir);
	VEC3_PRINT_N(&wanVel);
	VEC3_PRINT_N(&unwVel);
	VEC3_PRINT(pos);
	VEC3_PRINT_N(vel);
#endif /* DEBUG */
}
