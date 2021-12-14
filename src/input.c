#include <math.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "playerAction.h"
#include "input.h"
#include "do.h"

#include "inputConfig.h"

void
inputKey(FPARS(int, key, mods, actn))
{
	const struct actionKey *ak;
	unsigned int i;

	for (i = 0; i < NELM(sActnKeys); i++) {
		ak = &sActnKeys[i];
		if (ak->key == key && ak->actn == actn &&
		(ak->mods == mods || ak->mods == MODANY))
			ak->func(&ak->arg);
	}
}

void
inputMsBtn(FPARS(int, btn, mods, actn))
{
	const struct actionMsBtn *ab;
	unsigned int i;

	for (i = 0; i < NELM(sActnMsBtns); i++) {
		ab = &sActnMsBtns[i];
		if (ab->btn == btn && ab->actn == actn &&
		(ab->mods == mods || ab->mods == MODANY))
			ab->func(&ab->arg);
	}
}

void
inputCursMv(FPARS(double, x, y))
{
	static double xp, yp;
	static int first = 1;
	const struct actionCurs *ac;
	union actionArg arg;
	struct vec2 dm;
	unsigned int i;

	if (first) {
		dm = vec2_zero;
		first = 0;
	} else
		vec2_set(&dm, x - xp, y - yp);
	xp = x;
	yp = y;

	for (i = 0; i < NELM(sActnCurs); i++) {
		ac = &sActnCurs[i];
		#define m(I, C) \
			arg.v3.C = vec2_dot(&ac->v2s[I], &dm);
		do3(m);
		#undef m
		ac->func(&arg);
	}
}
