#include <stdio.h>
#include <glad/gl.h>

#include "util.h"

int
compileShd(GLuint *retShd, GLenum shdType, char const *src)
{
	GLint status;
	GLuint shd;

	shd = glCreateShader(shdType);
	glShaderSource(shd, 1, &src, NULL);
	glCompileShader(shd);

	glGetShaderiv(shd, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint logLen;
		char const *shTypeStr;

		switch (shdType) {
		case GL_VERTEX_SHADER:
			shTypeStr = "vertex";
			break;
		case GL_GEOMETRY_SHADER:
			shTypeStr = "geometry";
			break;
		case GL_FRAGMENT_SHADER:
			shTypeStr = "fragment";
			break;
		default:
			shTypeStr = "(unknown)";
		}

		glGetShaderiv(shd, GL_INFO_LOG_LENGTH, &logLen);

		GLchar infoLog[logLen + 1];
		glGetShaderInfoLog(shd, logLen, NULL, infoLog);

		eprintf("Compile failure in %s shader:\n%s",
			shTypeStr, infoLog);
		return 0;
	}

	*retShd = shd;
	return 1;
}

int
linkProg(GLuint *prog, size_t n, GLuint shds[n])
{
	unsigned int i;
	GLint status;
	GLuint pr;

	pr = glCreateProgram();
	for (i = 0; i < n; i++)
		glAttachShader(pr, shds[i]);
	glLinkProgram(pr);

	glGetProgramiv(pr, GL_LINK_STATUS, &status);
	if (!status) {
		GLint logLen;
		glGetProgramiv(pr, GL_INFO_LOG_LENGTH, &logLen);

		GLchar infoLog[logLen + 1];
		glGetProgramInfoLog(pr, logLen, NULL, infoLog);
		eprintf("Linker failure: %s", infoLog);

		return 0;
	}

	*prog = pr;
	return 1;
}
