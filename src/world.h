/* world.h */

#include "vecmath.h"

typedef struct {
	int type;
	vec3 pos;
} WorldObject;

enum /*WorldObjectType*/ {
	WORLD_BLK_EMPTY,
	WORLD_BLK_HOOKABLE,
	WORLD_BLK_UNHOOKABLE,
	WORLD_BLK_LAST,
};

/* returns wheter the screen needs to be redrawn */
int worldUpdateState(void);

void worldNeedDraw(int);

void worldGetEyePos(vec3 eyePos, vec3 viewDir);

void worldObjIter(int (*visit)(const WorldObject*, void*), void *data);
