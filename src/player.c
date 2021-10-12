#include <math.h>

#include "vecmath.h"
#include "player.h"

#define TRUNC(X, A, B) ((X) < (A) ? (A) : (X) > (B) ? (B) : (X))

typedef struct {
	vec3 pos, vel, accel;
	vec3 reqAccel; /* requested quotient of max acceleration */
	vec3 viewDir;
} Player;

/* static variables */
static Player sPlayer;

/* non-static functions */
void
playerInit(void)
{
	sPlayer = (Player){0};
}

void
playerSetAccel(vec3 const *accp)
{
	sPlayer.reqAccel = *accp;
}

void
playerAddAccel(vec3 const *accp)
{
	vec3 *plAccp = &sPlayer.reqAccel;

	*plAccp = (vec3){
		TRUNC(plAccp->x + accp->x, -1.f, 1.f),
		TRUNC(plAccp->y + accp->y, -1.f, 1.f),
		TRUNC(plAccp->z + accp->z, -1.f, 1.f),
	};
}

void
playerSubAccel(vec3 const *accp)
{
	vec3 *plAccp = &sPlayer.reqAccel;

	*plAccp = (vec3){
		TRUNC(plAccp->x - accp->x, -1.f, 1.f),
		TRUNC(plAccp->y - accp->y, -1.f, 1.f),
		TRUNC(plAccp->z - accp->z, -1.f, 1.f),
	};
}

//#include <stdio.h>
//#include "util.h"
//#define vec3_print(v) eprintf("%s: (%g, %g, %g)\n", #v, (v).x, (v).y, (v).z);
void
playerRotCam(vec3 const *addDir)
{
	vec3 *vDir = &sPlayer.viewDir;

	vDir->x = fmodf(vDir->x + addDir->x, 2 * M_PI);
	vDir->y = fmodf(vDir->y + addDir->y, 2 * M_PI);
	/* unused:
	vDir->z = fmodf(vDir->z + addDir->z, 2 * M_PI);
	*/

//	static unsigned int n = 0;
//	if (n++ > 0)
//		eprintf("\033[1A\033[K");
//	vec3_print(*vDir);

}

void
playerGetPos(vec3 *plPos, vec3 *plDir)
{
	*plPos = sPlayer.pos;
	*plDir = sPlayer.viewDir;
}

void
playerGotoNextPos(double totalTime, double dt)
{
	Player *pp = &sPlayer;
	vec3 tmp;

	vec3_scale(&tmp, &pp->vel, dt);
	vec3_add(&pp->pos, &pp->pos, &tmp);

	vec3_scale(&tmp, &pp->accel, dt);
	vec3_add(&pp->vel, &pp->vel, &tmp);

	pp->accel = (vec3){
		pp->accel.x + dt * pp->reqAccel.x / 10000.f,
		pp->accel.y + dt * pp->reqAccel.y / 10000.f,
		pp->accel.z + dt * pp->reqAccel.z / 10000.f,
	};
}
