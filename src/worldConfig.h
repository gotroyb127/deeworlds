/* worldConfig.h */

static const struct {
	struct motionPars mtnGnd, mtnAir;
	struct hookPars   hookPars;
	struct hammerPars hammerPars;
	struct laserPars  laserPars;
} sWldPrsCfg = {
	.mtnGnd = {
		.accel    = 2.6f,
//		.maxSpeed = 1.6f,
		.friction = 0.01f,
//		.accel    = 26.6f,
		.maxSpeed = 106.6f,
//		.friction = 0.95f,
		.jumpVel  = 24.52f,
		.gravity  = 0.f,
	},
	.mtnAir = {
		.accel    = 2.f,
		.maxSpeed = 7.57f,
		.friction = 0.005f,
		.jumpVel  = 24.52f,
		.gravity  = 46.27f,
	},
	.hookPars = {
		.length    = 12.66f,
		.throwVel  = 2.66f,
		.dragAccel = 0.1f,
		.dragSpeed = 0.5f,
		.duration  = 1.25,
	},
	.hammerPars = {
		.strength     = 1.f,
		.fireDelayDef = 0.125,
		.fireDelayHit = 0.320,
	},
	.laserPars = {
		.reachDist   = 26.6f,
		.fireDelay   = 0.800,
		.bounceNum   = 1000,
		.bounceDelay = 0.125,
	},
};

static const struct {
	struct vec3 plrBox;
	double timeScal;
} sWldCfg = {
	.plrBox = {0.92f, 0.92f, 0.92f},
	.timeScal = 1.0,
};
