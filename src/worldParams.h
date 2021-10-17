/* worldParams.h */

typedef struct {
	float accel;
	float maxSpeed;
	float friction;
	float jumpVel;
	float gravity;
} MotionParams;

typedef struct {
	float length;
	float throwVel;
	float dragAccel;
	float dragSpeed;
	double duration;
} HookParams;

typedef struct {
	float strength;
	double fireDelayDef;
	double fireDelayHit;
} HammerParams;

typedef struct {
	float reachDist;
	double fireDelay;
	int bounceNum;
	double bounceDelay;
	float bounceCost;
} LaserParams;
