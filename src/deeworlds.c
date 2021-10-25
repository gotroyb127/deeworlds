#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "draw.h"
#include "world.h"
#include "input.h"

#include "deeworldsConfig.h"

/* static variables */
static GLFWwindow *sWin;

/* static function declerations */
static void handleErr(int err, char const *desc);
static void handleFrambufferResize(GLFWwindow *win, int w, int h);
static void handleNeedRefresh(GLFWwindow *win);
static void handleFocus(GLFWwindow *win, int focused);
static void handleKey(GLFWwindow *win, int key, int scancode, int action, int mods);
static void handleMouseButton(GLFWwindow *win, int button, int action, int mods);
static void handleCursorPos(GLFWwindow *win, double xPos, double yPos);
static int initWin(void);
static void term(int status);
static void run(void);

/* function implementations */
void
handleErr(int err, char const *desc)
{
	eprintf("received glfw error no. %d: %s.\n", err, desc);
}

void
handleFrambufferResize(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
	drawSetAspectRatio((float)w / h);
}

// but first make sure if it is needed
void
handleNeedRefresh(GLFWwindow *win)
{
//	drawFrame();
//	glfwSwapBuffers(win);
}

void
handleFocus(GLFWwindow *win, int focused)
{
	if (focused)
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void
handleKey(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	static int toggle = 0;
	int focused;

	/*
	 * little hack to fake (un)focus events to prevent
	 * the cursor from getting grabbed
	 */
	if (key == GLFW_KEY_ESCAPE && mods == GLFW_MOD_ALT && action == GLFW_PRESS) {
		focused = glfwGetWindowAttrib(win, GLFW_FOCUSED);
		handleFocus(win, !focused || toggle);
		toggle = !toggle;
		return;
	}

	inputKey(key, mods, action);
}

void
handleMouseButton(GLFWwindow *win, int button, int action, int mods)
{
	inputMouseButton(button, mods, action);
}

void
handleCursorPos(GLFWwindow *win, double xPos, double yPos)
{
	inputCursorPos(xPos, yPos);
}

int
initWin(void)
{
	glfwSetErrorCallback(&handleErr);

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

	glfwSetFramebufferSizeCallback(sWin, &handleFrambufferResize);
	glfwSetWindowRefreshCallback(sWin, &handleNeedRefresh);
	glfwSetWindowFocusCallback(sWin, &handleFocus);

	glfwSetKeyCallback(sWin, &handleKey);
	glfwSetMouseButtonCallback(sWin, &handleMouseButton);
	glfwSetCursorPosCallback(sWin, &handleCursorPos);

	glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPos(sWin, 0.0, 0.0);

	return 1;
}

void
run(void)
{
	while (!glfwWindowShouldClose(sWin)) {
		if (worldUpdate(glfwGetTime())) {
			drawFrame();
			glfwSwapBuffers(sWin);
		}

//		glfwWaitEvents();
		glfwPollEvents();
	}
}

void
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

	worldInit();
	run();
	term(0);
}
