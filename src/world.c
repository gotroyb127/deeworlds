#include <math.h>
#include <string.h>

#include "vecmath.h"
#include "world.h"
#include "worldProtected.h"
#include "player.h"

#define WORLD_MAX_X 60
#define WORLD_MAX_Y 60
#define WORLD_WIDTH 6

/* static variables */
static double const sTimescale = 1.0;

#define E WORLD_BLK_EMPTY,
#define H WORLD_BLK_HOOKABLE,
#define U WORLD_BLK_UNHOOKABLE,
static int sWorldObjects[WORLD_MAX_X][WORLD_MAX_Y] = {
	{ E E E H E H H },
	{ H H H E U H E },
	{ E E E H E E H },
};
#undef E
#undef H
#undef U

/* non-static protected function implementations */
void
worldGetEyePos(vec3 *retEyePos, vec3 *retEyeDir)
{
	vec3 const eyeOffset = { 0.5f, 0.5f, 0.5f };

	playerGetPos(retEyePos, retEyeDir);
	vec3_add(retEyePos, retEyePos, &eyeOffset);
}

void
worldObjIter(int (*visit)(WorldObject const*, void*), void *data)
{
	int x, y, z;
	WorldObject ob;

	for (x = 0; x < WORLD_MAX_X; x++) {
		ob.pos.x = x;

		for (y = 0; y < WORLD_MAX_Y; y++) {
			ob.pos.y = y;

			for (z = 0; z < WORLD_WIDTH; z++) {
				ob.pos.z = z;

				ob.type = sWorldObjects[y][x];
				if (!visit(&ob, data))
					return;
			}
		}
	}
}

/* non-static function implementations */
void
worldInit(void)
{
	playerInit();
}

int
worldUpdateState(double sysTime)
{
	static double prevTime = 0.0;
	double tm, dt;

	if (!prevTime)
		prevTime = sysTime;
	dt = sTimescale * (sysTime - prevTime);
	tm = sTimescale * sysTime;

	playerGotoNextPos(tm, dt);

	return 1;
}
