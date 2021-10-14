#include <math.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "playerAction.h"
#include "input.h"

#include "inputConfig.h"

void
inputKey(int key, int mods, int action)
{
	ActionKey const *ak;
	unsigned int i;

	for (i = 0; i < NELEM(sActionsKey); i++) {
		ak = &sActionsKey[i];

		if (ak->key == key
		&& ak->mods == mods
		&& ak->action == action
		)
			ak->func(&ak->arg);
	}
}

void
inputMouseButton(int button, int mods, int action)
{
	ActionMouseButton const *amb;
	unsigned int i;

	for (i = 0; i < NELEM(sActionsMouseButton); i++) {
		amb = &sActionsMouseButton[i];

		if (amb->button == button
		&& amb->mods == mods
		&& amb->action == action
		)
			amb->func(&amb->arg);
	}
}

void
inputCursorPos(double xPos, double yPos)
{
	static double xPrev, yPrev;
	static int once = 0;

	ActionCursor const *ac;
	ActionArg arg;
	vec2 dm;
	unsigned int i, c;

	if (!once) {
		dm.x = 0.f;
		dm.y = 0.f;
		once = 1;
	} else {
		dm.x = xPos - xPrev;
		dm.y = yPos - yPrev;
	}
	xPrev = xPos;
	yPrev = yPos;

	for (i = 0; i < NELEM(sActionsCursor); i++) {
		ac = &sActionsCursor[i];
		for (c = 0; c < 3; c++)
			VEC3_COMP_IDX(arg.v3, c) = vec2_dot(&ac->v2s[c], &dm);
		ac->func(&arg);
	}
}
