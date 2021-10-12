/* worldProtected.h */

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

void worldGetEyePos(vec3 *retEyePos, vec3 *retEyeDir);
void worldObjIter(int (*visit)(WorldObject const*, void*), void *data);
