#include <math.h>
#include <string.h>

#include "world.h"

//#define WORLD_MAX_X 60
//#define WORLD_MAX_Y 60
//#define WORLD_WIDTH 6
#define WORLD_MAX_X 3
#define WORLD_MAX_Y 3
#define WORLD_WIDTH 2

#define E WORLD_BLK_EMPTY,
#define H WORLD_BLK_HOOKABLE,
#define U WORLD_BLK_UNHOOKABLE,

//static int worldObjects[WORLD_MAX_X][WORLD_MAX_Y] = {
//	{E E E H H H H},
//	{H H H E U H E},
//	{E E E H E E H},
//};
static int worldObjects[WORLD_MAX_X][WORLD_MAX_Y] = {
	{E},
	{H},
	{H},
//	{E E E},
//	{U H H},
//	{E H U},
};
#undef E
#undef H
#undef U

//vec3 gWorldEyePos = { 0.f, 1.f, 0.f };
vec3 gWorldEyePos = { -1.f, 2.f, -1.f };
vec3 gWorldEyeDir = { 1.f, 0.f, M_PI_2 };

static int sNeedDraw;

void
worldNeedDraw(int b)
{
	sNeedDraw = b;
}

int
worldUpdateState(void)
{
	return sNeedDraw;
}

void
worldGetEyePos(vec3 eyePos, vec3 viewDir)
{
	memcpy(eyePos, gWorldEyePos, sizeof(vec3));
	memcpy(viewDir, gWorldEyeDir, sizeof(vec3));

//	eyePos[0] = 0.f;
//	eyePos[1] = 1.f;
//	eyePos[2] = 0.f;
//
//	viewDir[0] = 1.f;
//	viewDir[1] = 0.f;
//	viewDir[2] = 0;
//
//	eyePos[0] = -1.f;
////	eyePos[1] = 2.f;
//	eyePos[2] = -1.f;
////	viewDir[2] = M_PI_2;
}

void
worldObjIter(int (*visit)(const WorldObject*, void*), void *data)
{
	int x, y, z;
	WorldObject ob;

	for (x = 0; x < WORLD_MAX_X; x++) {
		ob.pos[0] = x;

		for (y = 0; y < WORLD_MAX_Y; y++) {
			ob.pos[1] = WORLD_MAX_Y-1 - y; /* flip y */

			for (z = 0; z < WORLD_WIDTH; z++) {
				ob.pos[2] = z;
				ob.type = worldObjects[y][x];
				if (!visit(&ob, data))
					return;
			}
		}
	}
}
