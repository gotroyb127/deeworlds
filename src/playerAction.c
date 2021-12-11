#include <GLFW/glfw3.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "worldParams.h"
#include "player.h"
#include "playerAction.h"

/* non-static function implementations */
void
plrActnAclSet(const union actionArg *arg)
{
	plrCtlAclSet(wldCurrPlr(), &arg->v3);
}

void
plrActnAclAdd(const union actionArg *arg)
{
	plrCtlAclAdd(wldCurrPlr(), &arg->v3);
}

void
plrActnCamRot(const union actionArg *arg)
{
	plrCtlCamRot(wldCurrPlr(), &arg->v3);
}
