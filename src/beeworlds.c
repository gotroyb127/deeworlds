#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "draw.h"
#include "world.h"
#include "worldAction.h"

static GLFWwindow *sWin;

/* TODO: add declerations of static function */

#include "config.h"

void
handleErr(int err, const char *desc)
{
	eprintf("received glfw error no. %d: %s.\n", err, desc);
}

void
handleKey(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	int i;
	WorldActionKey const *wa;

	if (action != GLFW_PRESS) {
		worldNeedDraw(0);
		return;
	}

	for (i = 0; i < LENGTH(sWorldActionKeys); i++) {
		wa = &sWorldActionKeys[i];

		if (wa->key == key
		&& wa->action == action
		&& wa->mods == mods) {
			assert(wa->func != NULL);
			wa->func(wa->arg);
		}
	}

	switch (key) {
	case GLFW_KEY_R:
		worldNeedDraw(1);
		break;
	case GLFW_KEY_LEFT_BRACKET:
		if (mods != GLFW_MOD_CONTROL)
			break;
		/* FALLTHROUGH */
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		break;
	default:
		worldNeedDraw(0);
	}
}

void
handleFrambufferResize(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
}

// but first make sure if it is needed
void
handleNeedRefresh(GLFWwindow *win)
{
	drawFrame();
	glfwSwapBuffers(win);
}

int
initGLFW(void)
{
	glfwSetErrorCallback(handleErr);

	if (!glfwInit())
		return 0;

	if (!(sWin = glfwCreateWindow(640, 480, sWinTitle, NULL, NULL)))
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(sWin);
	gladLoadGL(glfwGetProcAddress);

	glfwSetKeyCallback(sWin, handleKey);
	glfwSetFramebufferSizeCallback(sWin, handleFrambufferResize);
	glfwSetWindowRefreshCallback(sWin, handleNeedRefresh);

	return 1;
}

void
run(void)
{
	while (!glfwWindowShouldClose(sWin)) {
		if (worldUpdateState()) {
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
	if (!initGLFW() || !drawInit())
		term(1);

	run();
	term(0);
}
