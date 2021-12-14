/* player.h */

struct player {
	struct vec3 pos, vel;
	struct vec3 selAcl;
	struct vec3 viewDir;
	struct vec3 box;
	const struct motionPars *mtnPars;
};

void plrInit(struct player *pl, const struct vec3 *box);
void plrMove(struct player *pl, double dt);
