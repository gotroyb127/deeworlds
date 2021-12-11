#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "draw.h"
#include "vecmath.h"
#include "world.h"
#include "input.h"

#include "deeworldsConfig.h"

/* static variables */
static GLFWwindow *sWin;

/* static function declerations */
static void onErr(int err, const char *desc);
static void onFrmBufResz(GLFWwindow *win, FPARS(int, w, h));
static void onNeedRefresh(GLFWwindow *win);
static void onFocus(GLFWwindow *win, int focused);
static void onKey(GLFWwindow *win, FPARS(int, key, code, actn, mods));
static void onMsBtn(GLFWwindow *win, FPARS(int, btn, actn, mods));
static void onCursMv(GLFWwindow *win, FPARS(double, x, y));
static int initWin(void);
static void term(int status);
static void run(void);

/* static function implementations */
static void
onErr(int err, const char *desc)
{
	eprintf("received glfw error no. %d: %s.\n", err, desc);
}

static void
onFrmBufResz(GLFWwindow *win, FPARS(int, w, h))
{
	glViewport(0, 0, w, h);
	drawSetAspectRatio((float)w / h);
}

// but first make sure it is needed
static void
onNeedRefresh(GLFWwindow *win)
{
//	drawFrame();
//	glfwSwapBuffers(win);
}

static void
onFocus(GLFWwindow *win, int focused)
{
	if (focused)
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

static void
onKey(GLFWwindow *win, FPARS(int, key, code, actn, mods))
{
	static int toggl = 0;
	int focused;

	/* little hack to fake (un)focus events for releasing the cursor */
	if (key == GLFW_KEY_ESCAPE && mods == GLFW_MOD_ALT && actn == GLFW_PRESS) {
		focused = glfwGetWindowAttrib(win, GLFW_FOCUSED);
		onFocus(win, !focused || toggl);
		toggl = !toggl;
		return;
	}

	inputKey(key, mods, actn);
}

static void
onMsBtn(GLFWwindow *win, FPARS(int, btn, actn, mods))
{
	inputMsBtn(btn, mods, actn);
}

static void
onCursMv(GLFWwindow *win, FPARS(double, x, y))
{
	inputCursMv(x, y);
}

static int
initWin(void)
{
	glfwSetErrorCallback(&onErr);

	if (!glfwInit())
		return 0;

	if (!(sWin = glfwCreateWindow(640, 480, sWinTitle, NULL, NULL)))
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(sWin);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(sConfig.vsync);

	glfwSetFramebufferSizeCallback(sWin, &onFrmBufResz);
	glfwSetWindowRefreshCallback(sWin, &onNeedRefresh);
	glfwSetWindowFocusCallback(sWin, &onFocus);

	glfwSetKeyCallback(sWin, &onKey);
	glfwSetMouseButtonCallback(sWin, &onMsBtn);
	glfwSetCursorPosCallback(sWin, &onCursMv);

	glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPos(sWin, 0.0, 0.0);

	return 1;
}

static void
run(void)
{
	while (!glfwWindowShouldClose(sWin)) {
		if (wldUpdate(glfwGetTime())) {
			drawFrame();
			glfwSwapBuffers(sWin);
		}

//		glfwWaitEvents();
		glfwPollEvents();
	}
}

static void
term(int status)
{
	glfwDestroyWindow(sWin);
	glfwTerminate();

	exit(status);
}

int
main(int argc, char *argv[])
{
	if (!initWin() || !drawInit())
		term(1);

	wldInit();
	run();
	term(0);
}
