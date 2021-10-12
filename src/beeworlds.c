#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "draw.h"
#include "world.h"
#include "input.h"

/* static variables */
static GLFWwindow *sWin;

#include "config.h"

/* static function declerations */
static void handleErr(int err, char const *desc);
static void handleFrambufferResize(GLFWwindow *win, int w, int h);
static void handleNeedRefresh(GLFWwindow *win);
static void handleFocus(GLFWwindow *win, int focused);
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
	drawFrame();
	glfwSwapBuffers(win);
}

void
handleFocus(GLFWwindow *win, int focused)
{
	if (focused)
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

	glfwSetFramebufferSizeCallback(sWin, &handleFrambufferResize);
	glfwSetWindowRefreshCallback(sWin, &handleNeedRefresh);
	glfwSetWindowFocusCallback(sWin, &handleFocus);

	glfwSetInputMode(sWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(sWin, 0.0, 0.0);

	glfwSetKeyCallback(sWin, &inputKey);
	glfwSetMouseButtonCallback(sWin, &inputMouseButton);
	glfwSetCursorPosCallback(sWin, &inputCursorPos);

	return 1;
}

void
run(void)
{
	while (!glfwWindowShouldClose(sWin)) {
		if (worldUpdateState(glfwGetTime())) {
			drawFrame();
			glfwSwapBuffers(sWin);
		}

		glfwWaitEvents();
//		glfwPollEvents();
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
