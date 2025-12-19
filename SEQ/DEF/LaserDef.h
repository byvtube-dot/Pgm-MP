#pragma once
enum LaserList
{
	LASER					= 0,
	laserMAX,
};

enum seqInput
{
	iLaserAuto				=0,
	iLaserReset				=1,
	iLaser0002				=2,
	iLaser0003				=3,

	iLaserReady				=100,
	iLaserBusy				=101,
	iLaserError				=102,
	iLaserRecvData			=103,
	iLaser0104				=104,
	iLaserDevice			=105,

	iLaserMAX,
};

enum seqOutput
{
	oLaserEmo				=0,
	oLaserAuto				=1,
	oLaserReset				=2,
	oLaser003				=3,

	oLaserTrig				=100,	// 각 레이어별 트리거..
	oLaserSendData			=101,
	oLaserSkip				=102,
	oLaser0103				=103,
	oLaserDevice			=104,

	oLaserMAX,
};

enum enLaserMode
{
	MODE_CROSSHAIR			= 0,
	MODE_1POINT_DONE		= 1,	// MODE_SCANCAL			= 1,
	MODE_PWR_TABLE			= 2,
	MODE_PWR_CHECK			= 3,
	MODE_NUM_MARKING		= 4,
	MODE_LOT_MARKING		= 5,
	MODE_ERASER_MARKING		= 6,
	MODE_FIELD_CORRECTION	= 7,
	MODE_MARKING_START		= 8,
	MODE_MARKING_END		= 9,

	MODE_TRIG_ALL			= 10,
	MODE_TRIG1				= 11,
	MODE_TRIG2				= 12,
	MODE_TRIG3				= 13,
	MODE_TRIG4				= 14,
	MODE_TRIG5				= 15,
	MODE_TRIG6				= 16,
	MODE_TRIG7				= 17,
	MODE_TRIG8				= 18,
	MODE_TRIG9				= 19,

	MODE_BCD_ALL			= 20,	
	MODE_BCD1				= 21,
	MODE_BCD2				= 22,
	MODE_BCD3				= 23,
	MODE_BCD4				= 24,
	MODE_BCD5				= 25,
	MODE_BCD6				= 26,
	MODE_BCD7				= 27,
	MODE_BCD8				= 28,
	MODE_BCD9				= 29,

	MODE_TEXT_ALL			= 30,	
	MODE_TEXT1				= 31,
	MODE_TEXT2				= 32,
	MODE_TEXT3				= 33,
	MODE_TEXT4				= 34,
	MODE_TEXT5				= 35,
	MODE_TEXT6				= 36,
	MODE_TEXT7				= 37,
	MODE_TEXT8				= 38,
	MODE_TEXT9				= 39,

	MODE_DEVICE				= 99,
};