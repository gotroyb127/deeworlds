/* drawLoad.h */

/* returned pointer needs to be free(3)'d (when not NULL), errmsg doesn't */
void *drawLoadImg(const char *fnm, FPARS(uint32_t, *width, *height),
	const char **errmsg);
