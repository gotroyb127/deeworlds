/* inputConfig.h */

#define ACTN(A) plrActn##A
#define EVT(T)  GLFW_##T
#define KEY(K)  GLFW_KEY_##K
#define MOD(M)  GLFW_MOD_##M
#define MBT(B)  GLFW_MOUSE_BUTTON_##B

static const struct actionKey sActnKeys[] = {
	/* key        mods          event type    function      argument */
	{ KEY(Q),     0,            EVT(PRESS),   ACTN(AclSet), {.v3 = {0}} },

	{ KEY(E),     0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.z =  1.f}} },
	{ KEY(E),     0,            EVT(RELEASE), ACTN(AclAdd), {.v3 = {.z = -1.f}} },

	{ KEY(D),     0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.z = -1.f}} },
	{ KEY(D),     0,            EVT(RELEASE), ACTN(AclAdd), {.v3 = {.z =  1.f}} },

	{ KEY(S),     0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.x = -1.f}} },
	{ KEY(S),     0,            EVT(RELEASE), ACTN(AclAdd), {.v3 = {.x = +1.f}} },

	{ KEY(F),     0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.x =  1.f}} },
	{ KEY(F),     0,            EVT(RELEASE), ACTN(AclAdd), {.v3 = {.x = -1.f}} },

	{ KEY(SPACE), 0,            EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 1.f}} },

	{ KEY(SPACE), MOD(SHIFT),   EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 0.7f}} },
	{ KEY(SPACE), MOD(CONTROL), EVT(PRESS),   ACTN(AclAdd), {.v3 = {.y = 0.3f}} },
};

static const struct actionMsBtn sActnMsBtns[] = {
	/* button   mods event type   function     argument */
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
