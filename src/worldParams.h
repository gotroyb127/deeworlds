/* worldParams.h */

struct motionPars {
	float accel;
	float maxRblSpd; /* max reachable speed through accel (horizontal) */
	float maxSpeed;  /* max speed: never speed higher than this */
	float friction;
	float jumpVel;
	float gravity;
};

struct hookPars {
	double duration;
	float dragAccel;
	float dragSpeed;
	float throwVel;
	float length;
};

struct hammerPars {
	double fireDelayDef;
	double fireDelayHit;
	float strength;
};

struct laserPars {
	double fireDelay;
	double bounceDelay;
	float reachDist;
	float bounceCost;
	int bounceNum;
};
