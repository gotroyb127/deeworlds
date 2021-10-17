#include <math.h>
#include <string.h>

#include "vecmath.h"
#include "world.h"
#include "worldProtected.h"
#include "worldParams.h"
#include "worldParamsConfig.h"
#include "player.h"

#define WORLD_MAX_X 60
#define WORLD_MAX_Y 60
#define WORLD_WIDTH 6

typedef struct {
	enum WorldObjectType objs[WORLD_MAX_X][WORLD_MAX_Y];
	ivec3 spawn;
} WorldMap;

/* static variables */
static double const sTimeScale = 1.0;

#define E WORLD_BLK_EMPTY,
#define A WORLD_BLK_AIR,
#define H WORLD_BLK_HOOKABLE,
#define U WORLD_BLK_UNHOOKABLE,
static WorldMap const sMap = {
	.objs = {
		{ A A A H A H H },
		{ H H H A U H A },
		{ A A A H A A H },
		{ E E E E E E E },
	},
	.spawn = { 3, 3, 3 },
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

			if (sMap.objs[y][x] == WORLD_BLK_EMPTY)
				break;

			for (z = 0; z < WORLD_WIDTH; z++) {
				ob.pos.z = z;

				ob.type = sMap.objs[y][x];
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
	playerSpawn(&(vec3){ sMap.spawn.x, sMap.spawn.y, sMap.spawn.z });
}

int
worldUpdate(double sysTime)
{
	static double prevTime = 0.0;
	double dt;

	if (!prevTime)
		prevTime = sysTime;
	dt = sTimeScale * (sysTime - prevTime);
	prevTime = sysTime;

	playerMove(&sWorldCfg.motionGround, dt);

	return 1;
}
