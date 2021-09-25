#include <GLFW/glfw3.h>

#include "world.h"
#include "worldPrivate.h"
#include "worldAction.h"

void
worldActionMove(WorldActionArg arg)
{
	gWorldEyePos[0] += arg.v3[0];
	gWorldEyePos[1] += arg.v3[1];
	gWorldEyePos[2] += arg.v3[2];

	worldNeedDraw(1);
}
