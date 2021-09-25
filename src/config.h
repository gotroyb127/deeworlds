/* config.h */

static const char *sWinTitle = "hehe";

static WorldActionKey const sWorldActionKeys[] = {
	{ GLFW_KEY_E, GLFW_PRESS, 0, &worldActionMove, {.v3 = { 0.f, 0.f, 1.f }}, },
	{ GLFW_KEY_D, GLFW_PRESS, 0, &worldActionMove, {.v3 = { 0.f, 0.f, -1.f }}, },
	{ GLFW_KEY_S, GLFW_PRESS, 0, &worldActionMove, {.v3 = { -1.f, 0.f, 0.f }}, },
	{ GLFW_KEY_F, GLFW_PRESS, 0, &worldActionMove, {.v3 = { 1.f, 0.f, 0.f }}, },
};
