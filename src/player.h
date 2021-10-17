/* player.h */

void playerInit(void);
void playerSpawn(vec3 const *pos);

void playerSetAccel(vec3 const *accp);
void playerAddAccel(vec3 const *accp);
void playerSubAccel(vec3 const *accp);
void playerRotCam(vec3 const *addDir);

void playerGetPos(vec3 *plPos, vec3 *plDir);
void playerMove(MotionParams const *mpar, double dt);
