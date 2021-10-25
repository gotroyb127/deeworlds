/* drawUtil.h */

#define checkErr() \
	do { \
		unsigned int i; \
		int err; \
		for (i = 0; (err = glGetError()) != GL_NO_ERROR; i = 1) \
			eprintf("%s: %s[%d]: GLerror no. %d\n", __FILE__, __func__, __LINE__, err); \
		if (i) \
			exit(1); \
	} while (0)

int compileShd(GLuint *retShd, GLenum shdType, char const *src);
int linkProg(GLuint *prog, size_t n, GLuint shds[n]);
