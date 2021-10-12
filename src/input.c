#include <math.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "playerAction.h"
#include "input.h"

#include "inputConfig.h"

void
inputKey(GLFWwindow *win, int key, int scancode, int action, int mods)
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
inputMouseButton(GLFWwindow *win, int button, int action, int mods)
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
inputCursorPos(GLFWwindow *win, double xPos, double yPos)
{
	static double xPrev, yPrev;
	static int once;

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
			VEC3_COMP_IDX(arg.v3, c) = vec2_cross(&ac->v2s[c], &dm);
		ac->func(&arg);
	}
}
