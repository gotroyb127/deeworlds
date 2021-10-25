#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "drawLoad.h"

#define IMG_WD_MAX 4096
#define IMG_HT_MAX IMG_WD_MAX

char *
drawLoadImg(const char *fnm, uint32_t *width, uint32_t *height, const char **errmsg)
{
	const char *ff = "farbfeld";
	FILE *fp;
	uint32_t hdr[4], w, h;
	uint16_t rgba[4];
	char *data;

	*errmsg = NULL;
	if (!(fp = fopen(fnm, "r"))) {
		*errmsg = strerror(errno);
		return NULL;
	}

	if (fread(hdr, 1, sizeof hdr, fp) != sizeof hdr) {
		*errmsg = "couldn't read enough data from file";
		fclose(fp);
		return NULL;
	}
	if (memcmp(hdr, ff, sizeof ff - 1)) {
		*errmsg = "invalid magic world, expected a valid farbfeld image file";
		fclose(fp);
		return NULL;
	}

	w = ntohl(hdr[2]);
	h = ntohl(hdr[3]);

	if (w > IMG_WD_MAX || h > IMG_HT_MAX)
		*errmsg = "image that big is not supported";

	if (!(data = malloc(w * h * sizeof rgba))) {
		*errmsg = strerror(errno);
		fclose(fp);
		return NULL;
	}

	if (fread(data, sizeof rgba, w * h, fp) != w * h) {
		*errmsg = "couldn't read enough data from file";
		fclose(fp);
		return NULL;
	}

	if (fclose(fp))
		*errmsg = strerror(errno);

	*width = w;
	*height = h;
	return data;
}
