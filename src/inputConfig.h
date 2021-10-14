/* inputConfig.h */

#define ACTION(ACT) playerAction##ACT

static ActionKey const sActionsKey[] = {
	/* key            mods              action        function                argument */
	{ GLFW_KEY_E,     0,                GLFW_PRESS,   ACTION(AddAccel), {.v3 = { 0.f, 0.f, 1.f }} },
	{ GLFW_KEY_E,     0,                GLFW_RELEASE, ACTION(SubAccel), {.v3 = { 0.f, 0.f, 1.f }} },

	{ GLFW_KEY_D,     0,                GLFW_PRESS,   ACTION(AddAccel), {.v3 = { 0.f, 0.f, -1.f }} },
	{ GLFW_KEY_D,     0,                GLFW_RELEASE, ACTION(SubAccel), {.v3 = { 0.f, 0.f, -1.f }} },

	{ GLFW_KEY_S,     0,                GLFW_PRESS,   ACTION(AddAccel), {.v3 = { -1.f, 0.f, 0.f }} },
	{ GLFW_KEY_S,     0,                GLFW_RELEASE, ACTION(SubAccel), {.v3 = { -1.f, 0.f, 0.f }} },

	{ GLFW_KEY_F,     0,                GLFW_PRESS,   ACTION(AddAccel), {.v3 = { 1.f, 0.f, 0.f }} },
	{ GLFW_KEY_F,     0,                GLFW_RELEASE, ACTION(SubAccel), {.v3 = { 1.f, 0.f, 0.f }} },

	{ GLFW_KEY_SPACE, 0,                GLFW_PRESS,   ACTION(AddAccel), {.v3 = { 0.f, 1.f, 0.f }} },
	{ GLFW_KEY_SPACE, 0,                GLFW_RELEASE, ACTION(SubAccel), {.v3 = { 0.f, 1.f, 0.f }} },

	{ GLFW_KEY_SPACE, GLFW_MOD_CONTROL, GLFW_PRESS,   ACTION(AddAccel), {.v3 = { 0.f, -1.f, 0.f }} },
	{ GLFW_KEY_SPACE, GLFW_MOD_CONTROL, GLFW_RELEASE, ACTION(SubAccel), {.v3 = { 0.f, -1.f, 0.f }} },
};

float const radsPerPixel = 1 / 450.f;

static ActionMouseButton const sActionsMouseButton[] = {
	/* button                  mods  action        function  argument */
	{ GLFW_MOUSE_BUTTON_LEFT,  0,    GLFW_PRESS,   ACTION(RotCam), {.v3 = { 0.f,  M_PI_2, 0.f }} },
	{ GLFW_MOUSE_BUTTON_LEFT,  0,    GLFW_RELEASE, ACTION(RotCam), {.v3 = { 0.f, -M_PI_2, 0.f }} },
	{ GLFW_MOUSE_BUTTON_RIGHT, 0,    GLFW_PRESS,   ACTION(RotCam), {.v3 = { 0.f, -M_PI_2, 0.f }} },
	{ GLFW_MOUSE_BUTTON_RIGHT, 0,    GLFW_RELEASE, ACTION(RotCam), {.v3 = { 0.f,  M_PI_2, 0.f }} },
};

static ActionCursor const sActionsCursor[] = {
	/* function       argument */
	{ ACTION(RotCam), {{ 0.f, -radsPerPixel }, { -radsPerPixel, 0.f }, {0}} },
};
