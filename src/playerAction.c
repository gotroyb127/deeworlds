#include <GLFW/glfw3.h>

#include "vecmath.h"
#include "world.h"
#include "player.h"
#include "playerAction.h"

/* non-static functions */
void
playerActionSetAccel(ActionArg const *arg)
{
	playerSetAccel(&arg->v3);
}

void
playerActionAddAccel(ActionArg const *arg)
{
	playerAddAccel(&arg->v3);
}

void
playerActionSubAccel(ActionArg const *arg)
{
	playerSubAccel(&arg->v3);
}

void
playerActionRotCam(ActionArg const *arg)
{
	playerRotCam(&arg->v3);
}
