/* playerAction.h */

union actionArg {
	struct vec3 v3;
	/* maybe more fields be added if needed */
};

typedef void (*ActionFunc)(const union actionArg *);

struct actionKey {
	int key, mods, actn;
	ActionFunc func;
	union actionArg arg;
};

struct actionMsBtn {
	int btn, mods, actn;
	ActionFunc func;
	union actionArg arg;
};

struct actionCurs {
	ActionFunc func;
	struct vec2 v2s[3];
};

void plrActnAclSet(const union actionArg *arg);
void plrActnAclAdd(const union actionArg *arg);
void plrActnCamRot(const union actionArg *arg);
