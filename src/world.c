#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "vecmath.h"
#include "world.h"
#include "worldParams.h"
#include "player.h"
#include "do.h"

#include "worldConfig.h"

#define vec3_print(v) \
	printf(#v ": (%.6g, %.6g, %.6g)\n", (v)->x, (v)->y, (v)->z);
#define vec3_printf(f, v) \
	printf(f #v ": (%.6g, %.6g, %.6g)\n", (v)->x, (v)->y, (v)->z);
#define ivec3_print(v) \
	printf(#v ": (%d, %d, %d)\n", (v)->x, (v)->y, (v)->z);
#define ivec3_printf(f, v) \
	printf(f #v "%s: (%d, %d, %d)\n", (v)->x, (v)->y, (v)->z);

enum {
	MAP_MAX_X = 64,
	MAP_MAX_Y = 64,
	MAP_WIDTH = 6,
};

struct worldMap {
	int blks[MAP_MAX_Y][MAP_MAX_X];
	struct ivec3 spawn;
	struct ivec3 maxBlk;
	struct vec3 minPos, maxPos;
};

/* static variables */
static struct player sPlr;

#define A WLD_BLK_AIR,
#define H WLD_BLK_HKABLE,
#define U WLD_BLK_UHABLE,
static const struct worldMap sMap = {
	.blks = {
		{A A A H A H H},
		{H H H A U H A},
		{A A A H A A H},
	},
	.spawn = {3, 1, 3},
	.maxBlk = {6, 2, 6},
	.minPos = {-100.f, -100.f, -100.f},
	.maxPos = { 100.f,  100.f,  100.f},
};
#undef A
#undef H
#undef U

/* static function definitions */
static void plrSpawn(struct player *pl);
static int plrIsOnGnd(const struct player *pl);
static int plrPosOk(const struct player *pl);
static void plrOnColn(struct player *pl, struct vec3 *dp,
	const struct ivec3 *cl, int ci);
static int getMapBlk(const struct ivec3 *p);
static int getPntColn(struct ivec3 *cl, float *sc, const int in[3],
	FPARS(const struct vec3, *cp, *dp));
static int getBoxColn(FPARS(struct ivec3, *clf, *cll), float *sc,
	const int skp[3], FPARS(const struct vec3, *cp, *dp, *box));
static void applyColns(struct player *pl, FPARS(const struct vec3, *cp, *np));

/* static function implementations */
static void
plrSpawn(struct player *pl)
{
	ivec3_to_vec3(&pl->pos, &sMap.spawn);
	pl->vel = pl->viewDir = pl->selAcl = vec3_zero;
	pl->mtnPars = plrIsOnGnd(pl) ? &sWldPrsCfg.mtnGnd : &sWldPrsCfg.mtnAir;
}

static int
plrIsOnGnd(const struct player *pl)
{
	const struct vec3 *p = &pl->pos;
	struct vec3 dp = {.y = -0.25f};
	struct ivec3 clf, cll, cl;
	float sc;
	int skp[3] = {0}, t;

	if (p->y != floorf(p->y))
		return 0;
	if (getBoxColn(&clf, &cll, &sc, skp, p, &dp, &pl->box) != 1)
		return 0;

	#define r(I, C) \
		for (cl.C = clf.C; cl.C <= cll.C; cl.C++)
	do3r(r,,,) {
	#undef r
		if ((t = getMapBlk(&cl)) < 0)
			continue;
		if (t == WLD_BLK_HKABLE || t == WLD_BLK_UHABLE)
			return 1;
	}

	return 0;
}

static int
plrPosOk(const struct player *pl)
{
	#define e(I, C) \
		pl->pos.C >= sMap.minPos.C && \
		pl->pos.C <= sMap.maxPos.C
	return do3e(e, &&);
	#undef e
}

static void
plrOnColn(struct player *pl, struct vec3 *dp, const struct ivec3 *cl, int ci)
{
	int t;

	if ((t = getMapBlk(cl)) < 0)
		return;
	if (t == WLD_BLK_HKABLE || t == WLD_BLK_UHABLE) {
		if (ci == 1 && dp->y < 0.f)
			pl->mtnPars = &sWldPrsCfg.mtnGnd;
		(&dp->x)[ci] = 0.f;
		(&pl->vel.x)[ci] = 0.f;
	}
}

static int
getMapBlk(const struct ivec3 *p)
{
	#define e(I, C) \
		p->C < 0 || p->C > sMap.maxBlk.C
	if (do3e(e, ||))
	#undef e
		return -1;

	return sMap.blks[p->y][p->x];
}

/* A 0-dimensional object positioned at `cp`, tries to move to
   `cp + dp`. Detect the first block with which it would collide. If
   it wouldn't, return -1. Otherwise, set the block's position in `cl`,
   return an index representing the axis along whom it collides (that
   is 0 for x, 1 for y, 2 for z) and set `sc` to indicate the portion of
   `dp` (from 0.f to 1.f) which leads to the collision. About finding in
   which block the object is in (and hence with which it cannot collide):
   For (C, I) in ((x, 0), (y, 1), (z, 2)): if cp->C is not equal to an integer,
   the block's C component is floorf(cp->C). Otherwise, when cp->C is equal to
   an integer (in this case the object is at the border of two blocks), the
   `in` array indicates whether the block's component is cp->C (when in[I] is
   1) or `cp->C - 1`. For example with `cp` being (5.3, 4.0, 2.1) the object
   is inside (5, 4, 2) when in[1] == 1 and (5, 3, 2) otherwise. */
static int
getPntColn(struct ivec3 *cl, float *sc, const int in[3],
	FPARS(const struct vec3, *cp, *dp))
{
	const float *c = &cp->x, *d = &dp->x;
	float (*head[2])(float) = {floorf, ceilf};
	float s, ms = 2.f;
	const int next[2] = {-1, +1};
	int i, ci = -1;

	for (i = 0; i < 3; i++) {
		if (d[i] == 0.f)
			continue;
		if ((s = (head[d[i] > 0.f](c[i]) - c[i]) / d[i]) == 0.f)
			s = fabsf(1.f / d[i]) * ((d[i] > 0.f) == in[i]);
		if (ms > s) {
			ms = s;
			ci = i;
		}
	}
	if (ms > 1.f)
		return -1;
	vec3_to_ivec3(cl, cp);
	(&cl->x)[ci] += next[d[ci] > 0.f] - (ms == 0.f && !in[ci]);
	*sc = ms;
	return ci;
}

/* A 3D box with size `box` positioned at `cp` tries to move to
   `cp + dp`. find the blocks that the box first(ly?) collides. The `clf`
   and `cll` are set to indicate that the box collides with every block in
   the range (clf.x ~ cll.x, clf.y ~ cll.y, clf.z ~ cll.z). `sc` works as in
   getPntColn if `spk[I] == 1` then the first collision on the I axis is
   skipped. This is for when the collision is already applied and the box is
   position at the border for the next one */
static int
getBoxColn(FPARS(struct ivec3, *clf, *cll), float *sc, const int skp[3],
	FPARS(const struct vec3, *cp, *dp, *box))
{
	struct vec3 ep;
	struct ivec3 nedg, cl, oedg;
	float f, fl;
	int in[3], ci;

	#define m(I, C) \
		nedg.C = dp->C > 0.f; \
		ep.C = cp->C + nedg.C * box->C; \
		in[I] = nedg.C == skp[I];
	do3(m);
	#undef m
	if ((ci = getPntColn(&cl, sc, in, &ep, dp)) < 0)
		return ci;

	#define m(I, C) \
		f = (cp->C + dp->C * *sc) + !nedg.C * box->C; \
		fl = floorf(f); \
		oedg.C = fl - (fl == f && !nedg.C);
	do3(m);
	#undef m
	(&oedg.x)[ci] = (&cl.x)[ci];

	#define m(I, C) \
		clf->C =  nedg.C ? oedg.C : cl.C; \
		cll->C = !nedg.C ? oedg.C : cl.C;
	do3(m);
	#undef m

	return ci;
}

static void
applyColns(struct player *pl, FPARS(const struct vec3, *cp, *np))
{
	struct vec3 dp, p, dpsave, tmpv;
	struct ivec3 clf, cll, cl;
	float sc;
	int ci, skp[3] = {0};

	if (vec3_iseq(vec3_sub(&dp, np, cp), &vec3_zero))
		return;
	p = *cp;
	while ((ci = getBoxColn(&clf, &cll, &sc, skp, &p, &dp, &pl->box)) >= 0) {
		vec3_addi(&p, vec3_scal(&tmpv, &dp, sc));
		vec3_scali(&dp, 1.f - sc);
		dpsave = dp;

		#define r(I, C) \
			for (cl.C = clf.C; cl.C <= cll.C; cl.C++)
		do3r(r,,,)
		#undef r
			plrOnColn(pl, &dp, &cl, ci);

		if (vec3_iseq(&dp, &vec3_zero))
			goto end;
		skp[ci] = signbit((&dpsave.x)[ci]) == signbit((&dp.x)[ci]);
	}
	vec3_addi(&p, &dp);
end:
	pl->pos = p;
	if (!plrPosOk(pl))
		plrSpawn(pl);
	else if (!plrIsOnGnd(pl))
		pl->mtnPars = &sWldPrsCfg.mtnAir;
}

/* non-static function implementations */
struct player *
wldCurrPlr(void)
{
	return &sPlr;
}

void
wldGetEye(FPARS(struct vec3, *eyePos, *eyeDir))
{
	struct player *pl = wldCurrPlr();
	struct vec3 tmpv;

	*eyeDir = pl->viewDir;
	vec3_add(eyePos, &pl->pos, vec3_scal(&tmpv, &pl->box, 0.5f));
}

void
wldObjIter(int (*visit)(const struct worldObj *, void *), void *arg)
{
	int x, y, z;
	struct worldObj ob;

	for (x = 0; x <= sMap.maxBlk.x; x++) {
		ob.pos.x = x;
		for (y = 0; y <= sMap.maxBlk.y; y++) {
			ob.pos.y = y;
			for (z = 0; z <= sMap.maxBlk.z; z++) {
				ob.pos.z = z;
				ob.type = sMap.blks[y][x];
				if (!visit(&ob, arg))
					return;
			}
		}
	}
}

void
wldInit(void)
{
	plrInit(&sPlr, &sWldCfg.plrBox);
	plrSpawn(&sPlr);
}

int
wldUpdate(double sysTime)
{
	static double prevTime = 0.0;
	struct vec3 cp, np;
	double dt;

	if (!prevTime)
		prevTime = sysTime;
	dt = sWldCfg.timeScal * (sysTime - prevTime);
	prevTime = sysTime;

	cp = sPlr.pos;
	plrMove(&sPlr, dt);
	np = sPlr.pos;

	applyColns(&sPlr, &cp, &np);

	return 1;
}
