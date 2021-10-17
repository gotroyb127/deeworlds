/* worldParamsConfig.h */

typedef struct {
	MotionParams motionGround, motionAir;
	HookParams hookParams;
	HammerParams hammerParams;
	LaserParams laserParams;
} WorldConfig;

static WorldConfig const sWorldCfg = {
	.motionGround = {
		.accel    = 26.6f,
		.maxSpeed = 16.6f,
		.friction = 0.5f,
//		.maxSpeed = 106.6f,
//		.friction = 0.95f,
		.jumpVel  = 24.52f,
		.gravity  = 0.f,
	}, .motionAir = {
		.accel    = 0.05f,
		.maxSpeed = 7.57f,
		.friction = 0.95f,
		.jumpVel  = 0.4f,
		.gravity  = 46.27f,
	}, .hookParams = {
		.length    = 12.66f,
		.throwVel  = 2.66f,
		.dragAccel = 0.1f,
		.dragSpeed = 0.5f,
		.duration  = 1.25,
	}, .hammerParams = {
		.strength     = 1.f,
		.fireDelayDef = 0.125,
		.fireDelayHit = 0.320,
	}, .laserParams = {
		.reachDist   = 26.6f,
		.fireDelay   = 0.800,
		.bounceNum   = 1000,
		.bounceDelay = 0.125,
	},
};
