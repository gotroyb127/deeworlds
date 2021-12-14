#include <math.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "worldParams.h"
#include "player.h"
#include "playerAction.h"
#include "do.h"

#define TRUNC(X, A, B) ((X) < (A) ? (A) : (X) > (B) ? (B) : (X))

/* non-static function implementations */
void
plrActnAclSet(const union actionArg *arg)
{
	struct vec3 *selAcl = &wldCurrPlr()->selAcl;
	const struct vec3 *set = &arg->v3;

	#define m(I, C) \
		selAcl->C = TRUNC(set->C, -1.f, 1.f);
	do3(m);
	#undef m
}

void
plrActnAclAdd(const union actionArg *arg)
{
	struct vec3 *selAcl = &wldCurrPlr()->selAcl;
	const struct vec3 *add = &arg->v3;

	#define m(I, C) \
		selAcl->C = TRUNC(selAcl->C + add->C, -1.f, 1.f);
	do3(m);
	#undef m
}

void
plrActnAclMul(const union actionArg *arg)
{
	struct vec3 *selAcl = &wldCurrPlr()->selAcl;
	const struct vec3 *mul = &arg->v3;

	#define m(I, C) \
		selAcl->C = TRUNC(selAcl->C * mul->C, -1.f, 1.f);
	do3(m);
	#undef m
}

void
plrActnCamRot(const union actionArg *arg)
{
	struct vec3 *vDir = &wldCurrPlr()->viewDir;
	const struct vec3 *addDir = &arg->v3;

	vDir->x = TRUNC(vDir->x + addDir->x, -M_PI_2, M_PI_2);
	vDir->y = fmodf(vDir->y + addDir->y, 2 * M_PI);
	/* z component is not used */
}
