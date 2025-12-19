#pragma once
enum AxisList
{										// [HOME DIRECTION]                 [ENCODER DIRECTION]
	// LEFT PART
	mtInPkX					= 0,		// RIGHT HOME						RIGHT(-) <-> LEFT(+)
	mtInPkZ					= 1,		// UP HOME							UP(-)	<-> DOWN(+)
	mtTrayShuttleY			= 2,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtTrayPusherX			= 3,		// LEFT HOME						UP(-)	<-> DOWN(+)
	mtMbbOpenZ				= 4,		// UP HOME							UP(-)	<-> DOWN(+)
	mtFoamSendElevZ			= 5,		// DOWN HOME						DOWN(-) <-> UP(+)
	mtFoamFlipT				= 6,		// COUNTER CLOCK WISE HOME(ORG)		CCW(-)	<-> CW(+)
	mtFoamRecvElevZ			= 7,		// DOWN HOME						DOWN(-) <-> UP(+)
	mtFoamPartialElevZ		= 8,		// DOWN HOME						DOWN(-) <-> UP(+)
	mtFoamPkX				= 9,		// LEFT HOME						LEFT(-) <-> RIGHT(+)
	mtFoamPkY				= 10,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtFoamPkY2				= 11,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtFoamPkZ				= 12,		// UP HOME							UP(-)	<-> DOWN(+)

	// RIGHT PART
	mtMbbShuttleY			= 13,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtMbbShuttleConv		= 14,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtMbbQcViY				= 15,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtMbbFoldPkX			= 16,		// LEFT HOME						LEFT(-) <-> RIGHT(+)
	mtMbbFoldPkZ			= 17,		// UP HOME							UP(-)	<-> DOWN(+)
	mtLabelLtShuttleY		= 18,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtLabelRtShuttleY		= 19,		// REAR HOME						REAR(-) <-> FRONT(+)
	mtLabelLaserX			= 20,		// LEFT HOME						LEFT(-) <-> RIGHT(+)
	mtLabelLtFeed			= 21,		// - HOME							CCW(-)	<-> CW(+)
	mtLabelRtFeed			= 22,		// - HOME							CCW(-)	<-> CW(+)
	mtMbbLtElevZ			= 23,		// DOWN HOME						DOWN(-) <-> UP(+)
	mtMbbRtElevZ			= 24,		// DOWN HOME						DOWN(-) <-> UP(+)
	mtMbbQcX				= 25,		// RIGHT HOME						RIGHT(-) <-> LEFT(+)

	motorMAX,
};

enum InterpolationMotorList
{
	//lnINDEX_1_HEAD			= 0,
	//lnINDEX_2_HEAD			= 1,
	//lnSORT_PK_XYT			= 2,
	interpolationMAX,
};

enum GantryList
{
	gtFoamPkY = 0,
	gtMAX,
};