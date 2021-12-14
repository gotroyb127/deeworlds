/* inputConfig.h */

#define ACTN(A) plrActn##A
#define EVT(T)  GLFW_##T
#define KEY(K)  GLFW_KEY_##K
#define MOD(M)  GLFW_MOD_##M
#define MBT(B)  GLFW_MOUSE_BUTTON_##B

#define MODANY  (-1)

#define MVKEYS(M, F, R, B, L, V) \
	{ KEY(F), M, EVT(PRESS), ACTN(AclAdd), {.v3 = {.z = +V}} }, \
	{ KEY(R), M, EVT(PRESS), ACTN(AclAdd), {.v3 = {.x = +V}} }, \
	{ KEY(B), M, EVT(PRESS), ACTN(AclAdd), {.v3 = {.z = -V}} }, \
	{ KEY(L), M, EVT(PRESS), ACTN(AclAdd), {.v3 = {.x = -V}} }
#define MVKEYSRLS(M, F, R, B, L) \
	{ KEY(F), M, EVT(RELEASE), ACTN(AclMul), {.v3 = {1.f, 1.f, 0.f}} }, \
	{ KEY(R), M, EVT(RELEASE), ACTN(AclMul), {.v3 = {0.f, 1.f, 1.f}} }, \
	{ KEY(B), M, EVT(RELEASE), ACTN(AclMul), {.v3 = {1.f, 1.f, 0.f}} }, \
	{ KEY(L), M, EVT(RELEASE), ACTN(AclMul), {.v3 = {0.f, 1.f, 1.f}} }

static const struct actionKey sActnKeys[] = {
	/* key               mods     action         function      argument */
	MVKEYS(0,            E, F, D, S, 1.f),
	MVKEYS(MOD(SHIFT),   E, F, D, S, 0.5f),
	MVKEYS(MOD(CONTROL), E, F, D, S, 0.25f),
	MVKEYSRLS(MODANY,    E, F, D, S),

	{ KEY(LEFT_SHIFT),   MODANY, EVT(PRESS),   ACTN(AclMul), {.v3 = {0.5f, 0.5f, 0.5f}} },
	{ KEY(LEFT_SHIFT),   MODANY, EVT(RELEASE), ACTN(AclMul), {.v3 = {2.f, 2.f, 2.f}} },
	{ KEY(LEFT_CONTROL), MODANY, EVT(PRESS),   ACTN(AclMul), {.v3 = {0.25f, 0.25f, 0.25f}} },
	{ KEY(LEFT_CONTROL), MODANY, EVT(RELEASE), ACTN(AclMul), {.v3 = {4.f, 4.f, 4.f}} },

	/* jumping */
	{ KEY(SPACE), 0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 1.f}} },
	{ KEY(SPACE), MOD(SHIFT),   EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 0.5f}} },
	{ KEY(SPACE), MOD(CONTROL), EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 0.25f}} },
};

static const struct actionMsBtn sActnMsBtns[] = {
	/* button   mods action   function     argument */
	{ MBT(LEFT),  0, EVT(PRESS),   ACTN(CamRot), {.v3 = {0.f,  M_PI_2, 0.f}} },
	{ MBT(LEFT),  0, EVT(RELEASE), ACTN(CamRot), {.v3 = {0.f, -M_PI_2, 0.f}} },
	{ MBT(RIGHT), 0, EVT(PRESS),   ACTN(CamRot), {.v3 = {0.f, -M_PI_2, 0.f}} },
	{ MBT(RIGHT), 0, EVT(RELEASE), ACTN(CamRot), {.v3 = {0.f,  M_PI_2, 0.f}} },
};

const float radsppxl = 1 / 450.f; /* radians per pixel */

static const struct actionCurs sActnCurs[] = {
	/* function       argument */
	{ ACTN(CamRot), {{0.f, -radsppxl}, {-radsppxl, 0.f}, {0}} },
};
