#pragma once
enum CylinderList
{
	cylInPkOc							= 0,		//	1	= Manual No.	
	cylBtmFoamTableUd					= 1,		//	2	= Manual No.
	cylBtmFoamStopUd					= 2,		//	3	= Manual No.
	cylBtmFoamFtClampOc					= 3,		//	4	= Manual No.
	cylBtmFoamRrClampOc					= 4,		//	4	= Manual No.
	cylTopFoamStopUd					= 5,		//	5	= Manual No.

	cylTrayShuttleNozzleUd				= 6,		//	6	= Manual No.
	cylTrayShuttleBridgeUd				= 7,		//	7	= Manual No.
	cylTrayShuttleBridgeFb				= 8,		//	8	= Manual No.
	cylTrayShuttleFtStopFb				= 9,		//	9	= Manual No.
	cylTrayShuttleRrStopFb				= 10,		//	9	= Manual No.
	cylTrayShuttleFtMaskUd				= 11,		//	10	= Manual No.
	cylTrayShuttleRrMaskUd				= 12,		//	10	= Manual No.
	cylMbbOpenBtmStopUd					= 13,		//	11	= Manual No. // Delete 25.07.22
	cylMbbOpenTopGuideFb				= 14,		//	12	= Manual No.
	cylMbbOpenTopVacUd					= 15,		//	13	= Manual No.
	cylMbbOpenTopClampUd				= 16,		//	14	= Manual No.

	cylSealNozzleFb						= 17,		//	15	= Manual No.
	cylSealMbbOpenGuideFtOc				= 18,		//	16	= Manual No.
	cylSealMbbOpenGuideRrOc				= 19,		//	16	= Manual No.
	cylSealNozzleMfcN2Oc				= 20,		//	17	= Manual No.
	cylSealBtmGuideUd					= 21,		//	18	= Manual No.
	cylSealerBtmUd						= 22,		//	19	= Manual No.
	cylSealerTopFtUd					= 23,		//	20	= Manual No.
	cylSealerTopRrUd					= 24,		//	20	= Manual No.
	cylSealTopGuideUd					= 25,		//	21	= Manual No.
	
	cylLtCobotLabelPkUd					= 26,		//	22	= Manual No.
	cylLtCobotLabelPkTurnFb				= 27,		//	23	= Manual No.

	cylFoamRecvElevLtAlignFb			= 28,		//	24	= Manual No.
	cylFoamRecvElevRrAlignFb			= 29,		//	25	= Manual No.
	cylFoamRecvElevLtStopFb				= 30,		//	26	= Manual No.
	cylFoamRecvElevRtStopFb				= 31,		//	26	= Manual No.
	cylFoamRrStackLiftUd				= 32,		//	27	= Manual No.
	cylFoamRrStackLtStopFb				= 33,		//	28	= Manual No.
	cylFoamRrStackRtStopFb				= 34,		//	28	= Manual No.
	cylFoamFtStackLiftUd				= 35,		//	29	= Manual No.
	cylFoamFtStackLtStopFb				= 36,		//	30	= Manual No.
	cylFoamFtStackRtStopFb				= 37,		//	30	= Manual No.
	cylFoamSendElevLtAlignFb			= 38,		//	31	= Manual No.
	cylFoamSendElevRtAlignFb			= 39,		//	32	= Manual No.
	cylFoamPartialElevLtAlignFb			= 40,		//	33	= Manual No.
	cylFoamPartialElevRrAlignFb			= 41,		//	34	= Manual No.

	cylMbbShuttleAlignFtFb				= 42,		//	35	= Manual No.
	cylMbbShuttleAlignRrFb				= 43,		//	35	= Manual No.
	cylMbbShuttleClampFtFb				= 44,		//	36	= Manual No.
	cylMbbShuttleClampRrFb				= 45,		//	36	= Manual No.

	cylQcSealedHeadBtmFtVacUd			= 46,		//	37	= Manual No.
	cylQcSealedHeadBtmRrVacUd			= 47,		//	37	= Manual No.
	cylQcSealedHeadTopFtClampUd			= 48,		//	38	= Manual No.
	cylQcSealedHeadTopRrClampUd			= 49,		//	38	= Manual No.
	cylQcSealedTrayStopUd				= 50,		//	39	= Manual No.
	cylQcSealedTrayAlignFtFb			= 51,		//	40	= Manual No.
	cylQcSealedTrayAlignRrFb			= 52,		//	40	= Manual No.

	cylMbbFoldAssistUd					= 53,		//	41	= Manual No.
	cylMbbFoldAlignFtFb					= 54,		//	42	= Manual No.
	cylMbbFoldAlignRrFb					= 55,		//	42	= Manual No.
	cylMbbFoldRtFb						= 56,		//	43	= Manual No.
	cylMbbFoldRtFtOc					= 57,		//	44	= Manual No.
	cylMbbFoldRtRrOc					= 58,		//	44	= Manual No.
	cylMbbFoldPkLtBlockTurnFb			= 59,		//	45	= Manual No.
	cylMbbFoldPkLtBlockFtOc				= 60,		//	46	= Manual No.
	cylMbbFoldPkLtBlockRrOc				= 61,		//	46	= Manual No.
	cylMbbFoldPkClampOc					= 62,		//	47	= Manual No.

	cylLtLabelFeederFb					= 63,		//	48	= Manual No.
	cylLtLabelFeederMountFb				= 64,		//	49	= Manual No.
	cylLtLabelFeederLockFtFb			= 65,		//	50	= Manual No.
	cylLtLabelFeederLockRrFb			= 66,		//	50	= Manual No.
	cylLtLabelRecvUd					= 67,		//	51	= Manual No.
	cylLtLabelRecvFb					= 68,		//	52	= Manual No.
	cylLtLabelRecvEjectPinLtUd			= 69,		//	53	= Manual No.
	cylLtLabelRecvEjectPinRtUd			= 70,		//	53	= Manual No.

	cylRtLabelFeederFb					= 71,		//	54	= Manual No.
	cylRtLabelFeederMountFb				= 72,		//	55	= Manual No.
	cylRtLabelFeederLockFtFb			= 73,		//	56	= Manual No.
	cylRtLabelFeederLockRrFb			= 74,		//	56	= Manual No.
	cylRtLabelRecvUd					= 75,		//	57	= Manual No.
	cylRtLabelRecvFb					= 76,		//	58	= Manual No.
	cylRtLabelRecvEjectPinLtUd			= 77,		//	59	= Manual No.
	cylRtLabelRecvEjectPinRtUd			= 78,		//	59	= Manual No.

	cylPowerMeterOc						= 79,		//	60	= Manual No.
	cylLabelQcViFb						= 80,		//	61	= Manual No.

	cylRtCobotLabelPkUd					= 81,		//	62	= Manual No.
	cylRtCobotLabelPkTurnFb				= 82,		//	63	= Manual No.

	cylLtMbbCstFb						= 83,		//	64	= Manual No.
	cylRtMbbCstFb						= 84,		//	65	= Manual No.

	cylMbbFoldPkLtFtAssist				= 85,		//	
	cylMbbFoldPkLtRrAssist				= 86,		//	

	cylSealMbbClampUd					= 87,		//	
	cylSealMbbClampFtFb					= 88,		//	
	cylSealMbbClampRrFb					= 89,		//	
	cylSealMbbClampFtOc					= 90,		//	
	cylSealMbbClampRrOc					= 91,		//	
	cylMbbShuttleClampUd			    = 92,		//

	cylinderMAX,
};

enum VacuumList
{
	vcMainAir1							= 0,
	vcMainAir2							= 1,
	vcMainAir3							= 2,
	vcMainAir4							= 3,
	vcMbbOpenBtm						= 4,		//	70	= Manual No.
	vcMbbOpenTop						= 5,		//	71	= Manual No.
	vcMbbOpenTop2						= 6,		//	71	= Manual No.
	vcSealNozzle						= 7,		//	72	= Manual No.
	vcSealNozzleN2						= 8,		//	73	= Manual No.
	vcLoadCellFt						= 9,
	vcLoadCellRr						= 10,
	vcLtCobotLabel						= 11,		//	74	= Manual No.
	vcLtCobotFoam						= 12,		//	75	= Manual No.
	vcLtCobotFoam2						= 13,		//	75	= Manual No.
	vcFoamPkLaser						= 14,
	vcFoamFlip							= 15,		//	76	= Manual No.
	vcFoamFlip2							= 16,		//	76	= Manual No.
	vcFoamPk							= 17,		//	77	= Manual No.
	vcFoamPk2							= 18,		//	77	= Manual No.
	vcFlowLabelExhaust					= 19,
	vcQcSealedHead						= 20,		//	78	= Manual No.
	vcLtLabelRecv						= 21,		//	79	= Manual No.
	vcRtLabelRecv						= 22,		//	80	= Manual No.
	vcRtCobotLabel						= 23,		//	81	= Manual No.
	vcRtCobotMbb						= 24,		//	82	= Manual No.
	vcRtCobotMbb2						= 25,		//	82	= Manual No.

	vacuumMAX,						
};

enum ConveyorList
{
	CV_FOAM_CONV						= 0,
	CV_TRAY_SHUTTLE						= 1,
	CV_FOAM_STACK						= 2,
	CV_QC								= 3,
	CV_MBB_FOLD							= 4,

	conveyorMAX,						
};	