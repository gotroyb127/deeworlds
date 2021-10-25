/* drawLoad.h */

/*
 * returned pointer needs to be free(3)'d (when not NULL), errmsg doesn't
 */
char *drawLoadImg(const char *fnm, uint32_t *width, uint32_t *height, const char **errmsg);
