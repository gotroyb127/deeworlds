/* worldAction.h */

#ifndef _glfw3_h_
#error include GLFW/glfw3.h first
#endif

typedef union {
	vec3 v3;
} WorldActionArg;

typedef void (*WorldActionFunc)(WorldActionArg);

typedef struct {
	int key;
	int action;
	int mods;
	WorldActionFunc func;
	WorldActionArg arg;
} WorldActionKey;

void worldActionMove(WorldActionArg);
