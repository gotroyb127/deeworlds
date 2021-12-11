#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "drawLoad.h"
#include "do.h"

#define IMG_WD_MAX 4096
#define IMG_HT_MAX IMG_WD_MAX

void *
drawLoadImg(const char *fnm, FPARS(uint32_t, *width, *height),
	const char **errmsg)
{
	FILE *fp;
	uint32_t hdr[4], w, h;
	uint16_t rgba[4], *rgbap;
	unsigned int closf, x, y;
	char const *ff = "farbfeld";

#define loadErr(errstr) \
	do { \
		*errmsg = errstr; \
		if (closf) \
			fclose(fp); \
		return NULL; \
	} while (0)

	*errmsg = NULL;
	closf = 0;
	if (!(fp = fopen(fnm, "r")))
		loadErr(strerror(errno));
	closf = 1;

	if (fread(hdr, sizeof hdr, 1, fp) != 1)
		loadErr(strerror(errno));
	if (memcmp(hdr, ff, sizeof ff - 1))
		loadErr("invalid magic word, expected a valid farbfeld image file");

	w = ntohl(hdr[2]);
	h = ntohl(hdr[3]);

	if (w > IMG_WD_MAX || h > IMG_HT_MAX)
		loadErr("image that big is not supported");

	uint64_t (*pxls)[h][w];
	if (!(pxls = malloc(sizeof *pxls)))
		loadErr(strerror(errno));

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			if (fread(rgba, sizeof rgba, 1, fp) != 1)
				loadErr(strerror(errno));
			rgbap = (void *)&(*pxls)[h-1 - y][x];
			#define m(I, C) \
				rgbap[I] = ntohs(rgba[I]);
			do4(m);
			#undef m
		}
	}

	if (fclose(fp))
		*errmsg = strerror(errno);

	*width = w;
	*height = h;
	return pxls;
#undef loadErr
}
