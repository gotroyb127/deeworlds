/* worldProtected.h */

enum WorldObjectType {
	WORLD_BLK_EMPTY = -1,
	WORLD_BLK_AIR = 0,
	WORLD_BLK_HKABLE,
	WORLD_BLK_UHABLE,
	WORLD_BLK_LAST,
};

typedef struct {
	enum WorldObjectType type;
	vec3 pos;
} WorldObject;

void worldGetEyePos(vec3 *retEyePos, vec3 *retEyeDir);
void worldObjIter(int (*visit)(WorldObject const*, void*), void *data);
