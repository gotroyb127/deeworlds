/* player.h */

struct player {
	struct vec3 pos, vel;
	struct vec3 box;
	struct vec3 selAcl;
	struct vec3 viewDir;
	const struct motionPars *mtnPars;
};

void plrInit(struct player *pl, const struct vec3 *box);

void plrCtlAclSet(struct player *pl, const struct vec3 *set);
void plrCtlAclAdd(struct player *pl, const struct vec3 *add);
void plrCtlCamRot(struct player *pl, const struct vec3 *add);

void plrMove(struct player *pl, double dt);
