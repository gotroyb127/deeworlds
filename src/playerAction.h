/* playerAction.h */

typedef union {
	vec3 v3;
	/* maybe more fields be added if needed */
} ActionArg;

typedef void (*ActionFunc)(ActionArg const*);

typedef struct {
	int key;
	int mods;
	int action;
	ActionFunc func;
	ActionArg arg;
} ActionKey;

typedef struct {
	int button;
	int mods;
	int action;
	ActionFunc func;
	ActionArg arg;
} ActionMouseButton;

typedef struct {
	ActionFunc func;
	vec2 v2s[3];
} ActionCursor;

/* sets accel on each axis relative to player's position
   the following functions expect v3 */
void playerActionSetAccel(ActionArg const *arg);
void playerActionAddAccel(ActionArg const *arg);
void playerActionSubAccel(ActionArg const *arg);

/* adjust the player camera, expects v3 */
void playerActionRotCam(ActionArg const *arg);
