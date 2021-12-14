/* world.h */

enum {
	WLD_BLK_NONE = -1,
	WLD_BLK_AIR,
	WLD_BLK_HKBL, /* hookable */
	WLD_BLK_UHBL, /* unhookable */
	WLD_BLK_LAST,
};

struct worldObj {
	int type;
	struct vec3 pos;
};

struct player *wldCurrPlr(void);
void wldGetEye(FPARS(struct vec3, *eyePos, *eyeDir));
void wldObjIter(int (*visit)(const struct worldObj *, void *), void *arg);

void wldInit(void);
/* returns whether the screen needs to be redrawn */
int wldUpdate(double sysTime);
