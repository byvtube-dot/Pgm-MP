#pragma once
enum enDI
{
//CH 00
	iTenkeyCol1								= 0,
	iTenkeyCol2								= 1,
	iTenkeyCol3								= 2,
	iTenkeyRow1								= 3,
	iTenkeyRow2								= 4,
	iTenkeyRow3								= 5,
	iTenkeyRow4								= 6,
	iTenkeyEmo								= 7,
	iSwTeach								= 8,
	iSwStart								= 9,
	iSwStop									= 10,
	iSwReset								= 11,
	iSwRrStart								= 12,
	iSwRrStop								= 13,
	iSwRrReset								= 14,
	iPowerOn								= 15,
												  
//CH 01											  
	iDoorLock01								= 100,	// FRONT LEFT
	iDoorLock02								= 101,
	iDoorLock03								= 102,
	iBEmo01									= 103,
	iBEmo02									= 104,
	iBEmo03									= 105,
	iBFoamRader								= 106,
	iSwFoamMutting							= 107,
	iBFoamSafetyLightCurtain				= 108,
	iDoorCloseBtm01							= 109,
	iDoorCloseBtm02							= 110,
	iDoorCloseBtm03							= 111,
	iBEmo04									= 112,	// ADD
	iBEmo05									= 113,	// ADD
	iBEmo08									= 114,	// ADD 25.09.11
												  
//CH 02											  
	iCylInPkOpen							= 200,
	iCylInPkClose							= 201,
	iInPkExist								= 202,

	iFanAlarmTop01							= 208,
	iFanAlarmTop02							= 209,
	iFanAlarmTop03							= 210,
	iFanAlarmTop04							= 211,
	iFanAlarmTop05							= 212,
	iFanAlarmTop06							= 213,
	iFanAlarmTop07							= 214,
	iFanAlarmTop08							= 215,
												  
//CH 03											  
	iBtmFoamExistIn							= 300,
	iBtmFoamExistEnd						= 301,
	iCylBtmFoamTableUp						= 302,
	iCylBtmFoamTableDn						= 303,
	iCylBtmFoamStopUp						= 304,
	iCylBtmFoamStopDn						= 305,
	iCylBtmFoamClampFtOp					= 306,
	iCylBtmFoamClampFtCl					= 307,
	iCylBtmFoamClampRrOp					= 308,
	iCylBtmFoamClampRrCl					= 309,
	iTopFoamExistIn							= 310,
	iTopFoamExistEnd						= 311,
	iCylTopFoamStopUp						= 312,
	iCylTopFoamStopDn						= 313,
									  
//CH 04								  
	iTrayShuttleExistIn						= 400,
	iTrayShuttleExistEnd					= 401,	
	iTrayShuttleLtColorSensor				= 402,	
	iTrayShuttleRtColorSensor				= 403,	
	iCylTrayShuttleStopFtFwd				= 404,
	iCylTrayShuttleStopFtBwd				= 405,
	iCylTrayShuttleStopRrFwd				= 406,
	iCylTrayShuttleStopRrBwd				= 407,
	iCylTrayShuttleMaskFtUp					= 408,	// Sensor Total 3EA (Up 2EA, Down 1EA)
	iCylTrayShuttleMaskDn					= 409,
	iCylTrayShuttleMaskRrUp					= 410,

//CH 05
	iCylTrayShuttleAirBlowUp				= 502,
	iCylTrayShuttleAirBlowDn				= 503,
	iCylTrayShuttleBridgeUp					= 504,
	iCylTrayShuttleBridgeDn					= 505,
	iCylTrayShuttleBridgeFwd				= 506,
	iCylTrayShuttleBridgeBwd				= 507,
	
//CH 06
	iMbbOpenBtmBlockExistFt					= 600,
	iMbbOpenBtmBlockExistRr					= 601,
	iCylMbbOpenBtmStopUp					= 602,	// unused
	iCylMbbOpenBtmStopDn					= 603,	// unused

	iCylMbbOpenTopGuideFwd					= 608,
	iCylMbbOpenTopGuideBwd					= 609,
	iCylMbbOpenTopVacUp						= 610,
	iCylMbbOpenTopVacDn						= 611,
	iCylMbbOpenTopClampUp					= 612,
	iCylMbbOpenTopClampDn					= 613,

//CH 07 
	iCylSealNozzleFwd						= 700,
	iCylSealNozzleBwd						= 701,
	iCylSealMbbOpenGuideFtOp				= 702,
	iCylSealMbbOpenGuideFtCl				= 703,
	iCylSealMbbOpenGuideRrOp				= 704,
	iCylSealMbbOpenGuideRrCl				= 705,

	iBSealcontactDetected					= 707,		// CC_100
	iCylSealBtmGuideUp						= 708,
	iCylSealBtmGuideDn						= 709,
	iCylSealerBtmUp							= 710,		// Anvil
	iCylSealerBtmDn							= 711,		// Anvil

//CH 08
	iCylSealMBBClampUp						= 800,		// 25.10.31 ADD
	iCylSealMBBClampDn						= 801,		// 25.10.31 ADD
	iCylSealMBBClampFtFwd					= 802,		// 25.10.31 ADD
	iCylSealMBBClampFtBwd					= 803,		// 25.10.31 ADD
	iCylSealMBBClampRrFwd					= 804,		// 25.10.31 ADD
	iCylSealMBBClampRrBwd					= 805,		// 25.10.31 ADD
	iCylSealMBBClampFtClose					= 806,		// 25.10.31 ADD
	iCylSealMBBClampRrClose					= 807,		// 25.10.31 ADD

	iCylSealerTopFtUp						= 810,
	iCylSealerTopFtDn						= 811,
	iCylSealerTopRrUp						= 812,
	iCylSealerTopRrDn						= 813,
	iCylSealTopGuideUp						= 814,
	iCylSealTopGuideDn						= 815,

//CH 09
	iCylLtCobotLabelPkUp					= 900,
	iCylLtCobotLabelPkDn					= 901,
	iCylLtCobotLabelPkTurnFwd				= 902,
	iCylLtCobotLabelPkTurnBwd				= 903,

	iFoamRecvElevExistBtm					= 906,
	iFoamRecvElevExistTop					= 907,
	iFoamRrStackExistBtm					= 908,
	iFoamRrStackExistTop					= 909,
	iFoamFtStackExistBtm					= 910,
	iFoamFtStackExistTop					= 911,
	iFoamSendElevExistBtm					= 912,
	iFoamSendElevExistTop					= 913,
	iFoamFlipExistLt						= 914,
	iFoamFlipExistRt						= 915,

//CH 10
	iCylFoamRecvElevLtAlignFwd				= 1000,
	iCylFoamRecvElevLtAlignBwd				= 1001,
	iCylFoamRecvElevRrAlignFwd				= 1002,
	iCylFoamRecvElevRrAlignBwd				= 1003,
	iCylFoamRecvElevLtStopFwd				= 1004,
	iCylFoamRecvElevLtStopBwd				= 1005,
	iCylFoamRecvElevRtStopFwd				= 1006,
	iCylFoamRecvElevRtStopBwd				= 1007,
	iCylFoamRrStackLiftUp					= 1008,
	iCylFoamRrStackLiftDn					= 1009,
	iCylFoamRrStackLtStopFwd				= 1010,
	iCylFoamRrStackLtStopBwd				= 1011,
	iCylFoamRrStackRtStopFwd				= 1012,
	iCylFoamRrStackRtStopBwd				= 1013,
	iCylFoamFtStackLiftUp					= 1014,
	iCylFoamFtStackLiftDn					= 1015,

//CH 11
	iCylFoamFtStackLtStopFwd				= 1100,
	iCylFoamFtStackLtStopBwd				= 1101,
	iCylFoamFtStackRtStopFwd				= 1102,
	iCylFoamFtStackRtStopBwd				= 1103,
	iCylFoamSendElevLtAlignFwd				= 1104,
	iCylFoamSendElevLtAlignBwd				= 1105,
	iCylFoamSendElevRtAlignFwd				= 1106,
	iCylFoamSendElevRtAlignBwd				= 1107,
	iCylFoamPartialElevLtAlignFwd			= 1108,
	iCylFoamPartialElevLtAlignBwd			= 1109,
	iCylFoamPartialElevRrAlignFwd			= 1110,
	iCylFoamPartialElevRrAlignBwd			= 1111,
	iLabelRejectLtCount						= 1112,
	iLabelRejectLtFull						= 1113,
	iFoamPartialRecvElevExistBtm			= 1114,
	iFoamPartialRecvElevExistTop			= 1115,

//CH 12
	iFoamPkExist							= 1200,
	iFoamLtCstFoamExist						= 1201,
	iFoamLtCstDetectLtFt					= 1202,
	iFoamLtCstDetectRtFt					= 1203,
	iFoamLtCstDetectLtRr					= 1204,
	iFoamLtCstDetectRtRr					= 1205,

	iFoamRtCstFoamExist						= 1207,
	iFoamRtCstDetectLtFt					= 1208,
	iFoamRtCstDetectRtFt					= 1209,
	iFoamRtCstDetectLtRr					= 1210,
	iFoamRtCstDetectRtRr					= 1211,

 	iSmema01Request							= 1212,
	iSmema02Comp							= 1213,
	iSmema03								= 1214,

//CH 13
	iFoamLtStackPioValId					= 1300,
	iFoamLtStackPioCs0						= 1301,
	iFoamLtStackPioCs1						= 1302,
	iFoamLtStackPioCs2						= 1303,
	iFoamLtStackPioCs3						= 1304,
	iFoamLtStackPioTrReq					= 1305,
	iFoamLtStackPioBusy						= 1306,
	iFoamLtStackPioComp						= 1307,
	iFoamRtStackPioValId					= 1308,
	iFoamRtStackPioCs0						= 1309,
	iFoamRtStackPioCs1						= 1310,
	iFoamRtStackPioCs2						= 1311,
	iFoamRtStackPioCs3						= 1312,
	iFoamRtStackPioTrReq					= 1313,
	iFoamRtStackPioBusy						= 1314,
	iFoamRtStackPioComp						= 1315,
//CH 14
	iSpare1400								= 1400,

//CH 15		/**** RIGHT PART ***/
	iDoorLock04								= 1500,
	iDoorLock05								= 1501,
	iDoorLock06								= 1502,
	iBEmo06									= 1503,
	iBEmo07									= 1504,
	iBMbbCCTV								= 1505,
	iBMbbRader								= 1506,
	iSwMbbMutting							= 1507,
	iBMbbSafetyLightCurtain					= 1508,
	iSmema11Receive							= 1509,
	iSmema12Comp							= 1510,
	iSmema13Safety							= 1511,
	iSmema14Auto							= 1512,

	iBEmo09									= 1515,		// ADD 25.09.11

//CH 16
	iDoorCloseBtm04							= 1600,
	iDoorCloseBtm05							= 1601,
	iDoorCloseBtm06							= 1602,
	iDoorCloseBtm07							= 1603,
	iDoorCloseBtm08							= 1604,
	iDoorCloseBtm09							= 1605,
	iDoorCloseBtm10							= 1606,

	iFanAlarmTop09							= 1608,
	iFanAlarmTop10							= 1609,
	iFanAlarmTop11							= 1610,
	iFanAlarmTop12							= 1611,
	iFanAlarmTop13							= 1612,
	iFanAlarmTop14							= 1613,
	iFanAlarmTop15							= 1614,
	iFanAlarmTop16							= 1615,
			
//CH 17
	iFanAlarmTop17							= 1700,		
	iFanAlarmTop18							= 1701,		
	iFanAlarmTop19							= 1702,		
	iFanAlarmTop20							= 1703,		
	iFanAlarmTop21							= 1704,		
	iFanAlarmTop22							= 1705,		
	iFanAlarmTop23							= 1706,		
	iFanAlarmTop24							= 1707,
	iFanAlarmBtm01							= 1708,
	iFanAlarmBtm02							= 1709,
	iFanAlarmBtm03							= 1710,
	iFanAlarmBtm04							= 1711,
	iFanAlarmBtm05							= 1712,
	iFanAlarmBtm06							= 1713,

//CH 18
	iMbbShuttleConvExist					= 1800,

	iCylMbbShuttleAlignFtFwd				= 1802,
	iCylMbbShuttleAlignFtBwd				= 1803,
	iCylMbbShuttleAlignRrFwd				= 1804,
	iCylMbbShuttleAlignRrBwd				= 1805,
	iCylMbbShuttleClampFtFwd				= 1806,
	iCylMbbShuttleClampFtBwd				= 1807,
	iCylMbbShuttleClampRrFwd				= 1808,
	iCylMbbShuttleClampRrBwd				= 1809,
	iCylMbbShuttleClampUp					= 1810,		// 25.10.31 ADD
	iCylMbbShuttleClampDn					= 1811,		// 25.10.31 ADD

// CH 19
	iQcConvExistIn							= 1900,
	iQcConvExistEnd							= 1901,
	iCylQcSealedHeadBtmVacFtUp				= 1902,
	iCylQcSealedHeadBtmVacFtDn				= 1903,
	iCylQcSealedHeadBtmVacRrUp				= 1904,
	iCylQcSealedHeadBtmVacRrDn				= 1905,
	iCylQcSealedHeadTopClampFtUp			= 1906,
	iCylQcSealedHeadTopClampFtDn			= 1907,
	iCylQcSealedHeadTopClampRrUp			= 1908,
	iCylQcSealedHeadTopClampRrDn			= 1909,
	iCylQcSealedTrayStopUp					= 1910,
	iCylQcSealedTrayStopDn					= 1911,
	iCylQcSealedTrayAlignFtFwd				= 1912,
	iCylQcSealedTrayAlignFtBwd				= 1913,
	iCylQcSealedTrayAlignRrFwd				= 1914,
	iCylQcSealedTrayAlignRrBwd				= 1915,

// CH 20
	iMbbFoldConvExist						= 2000,

	iCylMbbFoldAssistUp						= 2002,
	iCylMbbFoldAssistDn						= 2003,
	iCylMbbFoldAlignFtFwd					= 2004,
	iCylMbbFoldAlignFtBwd					= 2005,
	iCylMbbFoldAlignRrFwd					= 2006,
	iCylMbbFoldAlignRrBwd					= 2007,
	iCylMbbFoldRtFwd						= 2008,
	iCylMbbFoldRtBwd						= 2009,
	iCylMbbFoldRtFtOp						= 2010,
	iCylMbbFoldRtFtCl						= 2011,
	iCylMbbFoldRtRrOp						= 2012,
	iCylMbbFoldRtRrCl						= 2013,

// CH 21
	iMbbFoldPkLtExist						= 2100,
	iMbbFoldPkRtExist						= 2101,
	iCylMbbFoldPkLtTurnFwd					= 2102,
	iCylMbbFoldPkLtTurnBwd					= 2103,
	iCylMbbFoldPkLtFtOp						= 2104,
	iCylMbbFoldPkLtFtCl						= 2105,
	iCylMbbFoldPkLtRrOp						= 2106,
	iCylMbbFoldPkLtRrCl						= 2107,
	iCylMbbFoldPkClampOp					= 2108,
	iCylMbbFoldPkClampCl					= 2109,
	iCylMbbFoldPkLtFtAssistFwd				= 2110,
	iCylMbbFoldPkLtFtAssistBwd				= 2111,
	iCylMbbFoldPkLtRrAssistFwd				= 2112,
	iCylMbbFoldPkLtRrAssistBwd				= 2113,

// CH 22
	iLtLabelRecvExist						= 2200,

	iCylLtLabelRecvUp						= 2202,
	iCylLtLabelRecvDn						= 2203,
	iCylLtLabelRecvFwd						= 2204,
	iCylLtLabelRecvBwd						= 2205,
	iCylLtLabelRecvEjectPinLtUp				= 2206,
	iCylLtLabelRecvEjectPinLtDn				= 2207,
	iCylLtLabelRecvEjectPinRtUp				= 2208,
	iCylLtLabelRecvEjectPinRtDn				= 2209,

// CH_23
	iRtLabelRecvExist						= 2300,

	iCylRtLabelRecvUp						= 2302,
	iCylRtLabelRecvDn						= 2303,
	iCylRtLabelRecvFwd						= 2304,
	iCylRtLabelRecvBwd						= 2305,
	iCylRtLabelRecvEjectPinLtUp				= 2306,
	iCylRtLabelRecvEjectPinLtDn				= 2307,
	iCylRtLabelRecvEjectPinRtUp				= 2308,
	iCylRtLabelRecvEjectPinRtDn				= 2309,

// CH 24
	iLtLabelFeederLabelExist				= 2400,
	iLtLabelFeederDetectFt					= 2401,
	iCylLtLabelFeederFwd					= 2402,
	iCylLtLabelFeederBwd					= 2403,
	iCylLtLabelFeederMountFwd				= 2404,
	iCylLtLabelFeederMountBwd				= 2405,
	iCylLtLabelFeederLockFtFwd				= 2406,
	iCylLtLabelFeederLockFtBwd				= 2407,
	iCylLtLabelFeederLockRrFwd 				= 2408,
	iCylLtLabelFeederLockRrBwd 				= 2409,
	iLtLabelFeederDetectRr					= 2410,
	
// CH 25
	iRtLabelFeederLabelExist				= 2500,
	iRtLabelFeederDetectFt					= 2501,
	iCylRtLabelFeederFwd					= 2502,
	iCylRtLabelFeederBwd					= 2503,
	iCylRtLabelFeederMountFwd				= 2504,
	iCylRtLabelFeederMountBwd				= 2505,
	iCylRtLabelFeederLockFtFwd				= 2506,
	iCylRtLabelFeederLockFtBwd				= 2507,
	iCylRtLabelFeederLockRrFwd				= 2508,
	iCylRtLabelFeederLockRrBwd				= 2509,
	iRtLabelFeederDetectRr					= 2510,
	
// CH 26
	iLaserLabelPos							= 2600,

	iCylPowerMeterOp						= 2602,
	iCylPowerMeterCl						= 2603,
	iCylLabelQcViLt							= 2604,
	iCylLabelQcViRt							= 2605,

	iCylRtCobotLabelPkUp					= 2608,
	iCylRtCobotLabelPkDn					= 2609,	
	iCylRtCobotLabelPkTurnFwd				= 2610,
	iCylRtCobotLabelPkTurnBwd				= 2611,

// CH 27 
	iLtMbbCstExist							= 2700,
	iLtMbbCstDetectLt						= 2701,
	iLtMbbCstDetectRt						= 2702,
	iLtMbbCstFtLevel						= 2703,
	iLtMbbCstMidLevel						= 2704,
	iLtMbbCstRrLevel						= 2705,
	iCylLtMbbCstFwd							= 2706,
	iCylLtMbbCstBwd							= 2707,
	iRtMbbCstExist							= 2708,
	iRtMbbCstDetectLt						= 2709,
	iRtMbbCstDetectRt						= 2710,
	iRtMbbCstFtLevel						= 2711,
	iRtMbbCstMidLevel						= 2712,
	iRtMbbCstRrLevel						= 2713,
	iCylRtMbbCstFwd							= 2714,
	iCylRtMbbCstBwd							= 2715,

// CH 28
	iLabelLtFeederPioValId					= 2800,
	iLabelLtFeederPioCs0					= 2801,
	iLabelLtFeederPioCs1					= 2802,
	iLabelLtFeederPioCs2					= 2803,
	iLabelLtFeederPioCs3					= 2804,
	iLabelLtFeederPioTrReq					= 2805,
	iLabelLtFeederPioBusy					= 2806,
	iLabelLtFeederPioComp					= 2807,
	iLabelRtFeederPioValId					= 2808,
	iLabelRtFeederPioCs0					= 2809,
	iLabelRtFeederPioCs1					= 2810,
	iLabelRtFeederPioCs2					= 2811,
	iLabelRtFeederPioCs3					= 2812,
	iLabelRtFeederPioTrReq					= 2813,
	iLabelRtFeederPioBusy					= 2814,
	iLabelRtFeederPioComp					= 2815,

// CH 29
	iMbbLtCstPioValId						= 2900,
	iMbbLtCstPioCs0							= 2901,
	iMbbLtCstPioCs1							= 2902,
	iMbbLtCstPioCs2							= 2903,
	iMbbLtCstPioCs3							= 2904,
	iMbbLtCstPioTrReq						= 2905,
	iMbbLtCstPioBusy						= 2906,
	iMbbLtCstPioComp						= 2907,
	iMbbRtCstPioValId						= 2908,
	iMbbRtCstPioCs0							= 2909,
	iMbbRtCstPioCs1							= 2910,
	iMbbRtCstPioCs2							= 2911,
	iMbbRtCstPioCs3							= 2912,
	iMbbRtCstPioTrReq						= 2913,
	iMbbRtCstPioBusy						= 2914,
	iMbbRtCstPioComp						= 2915,

// CH 30
	iViAuto									= 3000,
	iViLabelQcBusy							= 3001,
	iViLabelFeederPrsBusy					= 3002,
	iViLtCobotBusy							= 3003,
	iViInPkBusy								= 3004,
	iViRtCobotBusy							= 3005,
	iViMbbQcBusy							= 3006,

	iViLabelQcMode1							= 3008,
	iViLabelQcMode2							= 3009,
	iViLabelQcMode3							= 3010,
	iViLabelQcMode4							= 3011,
	iViLabelFeederMode1						= 3012,
	iViLabelFeederMode2						= 3013,
	iViLabelFeederMode3						= 3014,
	iViLabelFeederMode4						= 3015,

// CH 31
	iViLtCobotMode1							= 3100,
	iViLtCobotMode2							= 3101,
	iViLtCobotMode3							= 3102,
	iViLtCobotMode4							= 3103,
	iViInPkMode1							= 3104,
	iViInPkMode2							= 3105,
	iViInPkMode3							= 3106,
	iViInPkMode4							= 3107,
	iViRtCobotMode1							= 3108,
	iViRtCobotMode2							= 3109,
	iViRtCobotMode3							= 3110,
	iViRtCobotMode4							= 3111,
	iViMbbQcMode1							= 3112,
	iViMbbQcMode2							= 3113,
	iViMbbQcMode3							= 3114,
	iViMbbQcMode4							= 3115,
// CH 32
	
// CH 33
	iTrashCanExist							= 3300,
	iLabelRejectRtCount						= 3301,
	iLabelRejectRtFull						= 3302,
	
	diLAST,
};

enum enDO							  
{									  
//CH 00										
	oTenkeyLowA								= 0,
	oTenkeyLowB								= 1,
	oTenkeyLowC								= 2,
	oTenkeyLowD								= 3,
	oTenkeyMidA								= 4,
	oTenkeyMidB								= 5,
	oTenkeyMidC								= 6,
	oTenkeyMidD								= 7,
	oTenkeyHigh								= 8,
	oSwLampStart							= 9,
	oSwLampStop								= 10,
	oSwLampReset							= 11,
	oSwLampRrStart							= 12,
	oSwLampRrStop							= 13,
	oSwLampRrReset							= 14,
												  
//CH 01											 	
	oDoorLock01								= 100,
	oDoorLock02								= 101,
	oDoorLock03								= 102,
	oMotorPwr01								= 103, 
	oMotorPwr02								= 104, 
	oIlluminator1							= 105,
	oFoamSafetyPlcMutting					= 106,	// (SF PLC전달용)
	oFoamMuttingLamp						= 107,
	oTowerLampGreen							= 108,
	oTowerLampYellow						= 109,
	oTowerLampRed							= 110,
	oVoiceBuzzer01							= 111,
	oVoiceBuzzer02							= 112,
	oVoiceBuzzer03							= 113,
	oVoiceBuzzer04							= 114,
	oVoiceBuzzer05							= 115,
												  
//CH 03											 	
	oCylInPkOpen							= 200,
	oCylInPkClose							= 201,
	oCylBtmFoamTableUp						= 202,
	oCylBtmFoamTableDn						= 203,
	oCylBtmFoamStopUp						= 204,
	oCylBtmFoamStopDn						= 205,
	oCylBtmFoamClampOp						= 206,
	oCylBtmFoamClampCl						= 207,
	oAcFoamConvRun							= 208,
	oAcFoamConvDir							= 209,

	oCylTopFoamStopUp						= 212,
	oCylTopFoamStopDn						= 213,

//CH 03											  
	oAcTrayShuttleConvRun					= 300,
	oAcTrayShuttleConvDir					= 301,

	oCylTrayShuttleStopFwd					= 304,
	oCylTrayShuttleStopBwd					= 305,

	oCylTrayShuttleMaskUp					= 308,
	oCylTrayShuttleMaskDn					= 309,

//CH 04								  
	oTrayShuttleAirBlowOn					= 400,

	oCylTrayShuttleAirBlowUp				= 402,
	oCylTrayShuttleAirBlowDn				= 403,
	oCylTrayShuttleBridgeUp					= 404,
	oCylTrayShuttleBridgeDn					= 405,
	oCylTrayShuttleBridgeFwd				= 406,
	oCylTrayShuttleBridgeBwd				= 407,
	oCylSealMBBClampUp						= 408,
	oCylSealMBBClampDn						= 409,
	oCylSealMBBClampFwd						= 410,
	oCylSealMBBClampBwd						= 411,

	oCylSealMBBClampOpen					= 414,
	oCylSealMBBClampClose					= 415,

//CH 05								  
	oVacMbbOpenBtmOff						= 500,
	oVacMbbOpenBtmBlow						= 501,	// unused
	oCylMbbOpenBtmStopUp					= 502,	// unused
	oCylMbbOpenBtmStopDn					= 503,	// unused

	oCylMbbOpenTopGuideFwd					= 508,
	oCylMbbOpenTopGuideBwd					= 509,
	oCylMbbOpenTopVacUp						= 510,
	oCylMbbOpenTopVacDn						= 511,
	oCylMbbOpenTopClampUp					= 512,
	oCylMbbOpenTopClampDn					= 513,
	oVacMbbOpenTopOn						= 514,
	oVacMbbOpenTopBlow						= 515,

//CH 06								 	
	oCylSealNozzleFwd						= 600,
	oCylSealNozzleBwd						= 601,
	oCylSealMbbOpenGuideOp					= 602,
	oCylSealMbbOpenGuideCl					= 603,
	oVacSealNozzleOff						= 604,
	oCylSealNozzleMfcN2Op					= 605,
	oVacSealNozzleN2On						= 606,

	oCylSealBtmGuideUp						= 608,
	oCylSealBtmGuideDn						= 609,
	oCylSealerBtmUp							= 610,
	oCylSealerBtmDn							= 611,
	oCylSealerTopUp							= 612,
	oCylSealerTopDn							= 613,
	oCylSealTopGuideUp						= 614,
	oCylSealTopGuideDn						= 615,

//CH 07
	oCylLtCobotLabelPkUp					= 700,
	oCylLtCobotLabelPkDn					= 701,
	oVacLtCobotLabelOn						= 702,
	oVacLtCobotLabelBlow					= 703,
	oVacLtCobotFoamOn						= 704,
	oVacLtCobotFoamBlow						= 705,
	oCylLtCobotLabelPkTurnFwd				= 706,
	oCylLtCobotLabelPkTurnBwd				= 707,
	oAcFoamStackConvRun						= 708,
	oAcFoamStackConvDir						= 709,

	oVacFoamFlipOn							= 714,
	oVacFoamFlipBlow						= 715,

//CH 08
	oCylFoamRecvElevLtAlignFwd				= 800,
	oCylFoamRecvElevLtAlignBwd				= 801,
	oCylFoamRecvElevRrAlignFwd				= 802,
	oCylFoamRecvElevRrAlignBwd				= 803,
	oCylFoamRecvElevStopFwd					= 804,
	oCylFoamRecvElevStopBwd					= 805,

	oCylFoamRrStackLiftUp					= 808,
	oCylFoamRrStackLiftDn					= 809,
	oCylFoamRrStackStopFwd					= 810,
	oCylFoamRrStackStopBwd					= 811,

	oCylFoamFtStackLiftUp					= 814,
	oCylFoamFtStackLiftDn					= 815,

//CH 09
	oCylFoamFtStackStopFwd					= 900,
	oCylFoamFtStackStopBwd					= 901,

	oCylFoamSendElevLtAlignFwd				= 904,
	oCylFoamSendElevLtAlignBwd				= 905,
	oCylFoamSendElevRtAlignFwd				= 906,
	oCylFoamSendElevRtAlignBwd				= 907,
	oCylFoamPartialElevLtAlignFwd			= 908,
	oCylFoamPartialElevLtAlignBwd			= 909,
	oCylFoamPartialElevRrAlignFwd			= 910,
	oCylFoamPartialElevRrAlignBwd			= 911,

//CH 10
	oVacFoamPkOn							= 1000,
	oVacFoamPkBlow							= 1001,
	oFoamRaderMode0							= 1002,
	oFoamRaderMode1							= 1003,
	oFoamRaderMode2							= 1004,
	oFoamRaderMode3							= 1005,

	oSmema01Recv							= 1012,	// '자재를 받는중' Cycle중에는 On
	oSmema02Comp							= 1013, // Pickup & Data Recv Comp
	oSmema03Safety							= 1014,	// Picker Z Ready
	oSmema04Auto							= 1015,	// Autorun + 자재 받을수 있는상태(Picker Empty) + Picker Z Ready

//CH 11
	oFoamLtStackPioLdReq					= 1100,
	oFoamLtStackPioUldReq					= 1101,
	oFoamLtStackPioAbort					= 1102,
	oFoamLtStackPioRdy						= 1103,
	oFoamLtStackPioSpare05					= 1104,
	oFoamLtStackPioSpare06					= 1105,
	oFoamLtStackPioSpare07					= 1106,
	oFoamLtStackPioSpare08					= 1107,
	oFoamRtStackPioLdReq					= 1108,
	oFoamRtStackPioUldReq					= 1109,
	oFoamRtStackPioAbort					= 1110,
	oFoamRtStackPioRdy						= 1111,
	oFoamRtStackPioSpare05					= 1112,
	oFoamRtStackPioSpare06					= 1113,
	oFoamRtStackPioSpare07					= 1114,
	oFoamRtStackPioSpare08					= 1115,
	
//CH 12		/**** RIGHT PART ***/
 	oDoorLock04								= 1200,
	oDoorLock05								= 1201,
	oDoorLock06								= 1202,	  
	oMotorPwr03								= 1203, 
	oMotorPwr04								= 1204, 
	oIlluminator2							= 1205,
	oMbbSafetyPlcMutting					= 1206,	// (SF PLC전달용)
	oMbbMuttingLamp							= 1207,
	
	oSmema11Req								= 1209,	// 2-3 Smema // Tray 
	oSmema12Comp							= 1210,
	oSmema13Safety							= 1211,
	oBuzzer01								= 1212,
	oBuzzer02								= 1213,
	oBuzzer03								= 1214,
	oBuzzer04								= 1215,

//CH 13

	oCylMbbShuttleAlignFwd					= 1302,
	oCylMbbShuttleAlignBwd					= 1303,

	oCylMbbShuttleClampFwd					= 1306,
	oCylMbbShuttleClampBwd					= 1307,
	oCylMbbShuttleClampUp					= 1308,		// 25.10.31 ADD
	oCylMbbShuttleClampDn					= 1309,		// 25.10.31 ADD

	oVoiceBuzzer11							= 1311,
	oVoiceBuzzer12							= 1312,
	oVoiceBuzzer13							= 1313,
	oVoiceBuzzer14							= 1314,
	oVoiceBuzzer15							= 1315,

//CH 14
	oAcQcConvRun							= 1400,
	oAcQcConvDir							= 1401,
	oCylQcSealedHeadBtmVacUp				= 1402,
	oCylQcSealedHeadBtmVacDn				= 1403,
	oVacQcSealedHeadOff						= 1404,
	oVacQcSealedHeadBlow					= 1405,
	oCylQcSealedHeadTopClampUp				= 1406,
	oCylQcSealedHeadTopClampDn				= 1407,

	oCylQcSealedTrayStopUp					= 1410,
	oCylQcSealedTrayStopDn					= 1411,
	oCylQcSealedTrayAlignFwd				= 1412,
	oCylQcSealedTrayAlignBwd				= 1413,

// CH 15
	oAcMbbFoldConvRun						= 1500,
	oAcMbbFoldConvDir						= 1501,
	oCylMbbFoldAssistUp						= 1502,
	oCylMbbFoldAssistDn						= 1503,
	oCylMbbFoldAlignFwd						= 1504,
	oCylMbbFoldAlignBwd						= 1505,

	oCylMbbFoldRtFwd						= 1508,
	oCylMbbFoldRtBwd						= 1509,
	oCylMbbFoldRtOp							= 1510,
	oCylMbbFoldRtCl							= 1511,
	
// CH 16

	oCylMbbFoldPkLtTurnFwd					= 1602,
	oCylMbbFoldPkLtTurnBwd					= 1603,
	oCylMbbFoldPkLtOp						= 1604,
	oCylMbbFoldPkLtCl						= 1605,

	oCylMbbFoldPkClampOp					= 1608,
	oCylMbbFoldPkClampCl					= 1609,
	oCylMbbFoldPkLtAssistFwd				= 1610,
	oCylMbbFoldPkLtAssistBwd				= 1611,

// CH 17
	oVacLtLabelRecvOn						= 1700,
	oVacLtLabelRecvBlow						= 1701,
	oCylLtLabelRecvUp						= 1702,
	oCylLtLabelRecvDn						= 1703,
	oCylLtLabelRecvFwd						= 1704,
	oCylLtLabelRecvBwd						= 1705,
	oCylLtLabelRecvEjectPinUp				= 1706,
	oCylLtLabelRecvEjectPinDn				= 1707,

// CH 18
	oVacRtLabelRecvOn						= 1800,
	oVacRtLabelRecvBlow						= 1801,
	oCylRtLabelRecvUp						= 1802,
	oCylRtLabelRecvDn						= 1803,
	oCylRtLabelRecvFwd						= 1804,
	oCylRtLabelRecvBwd						= 1805,
	oCylRtLabelRecvEjectPinUp				= 1806,
	oCylRtLabelRecvEjectPinDn				= 1807,

// CH 19

	oCylLtLabelFeederFwd					= 1902,
	oCylLtLabelFeederBwd					= 1903,
	oCylLtLabelFeederMountFwd				= 1904,
	oCylLtLabelFeederMountBwd				= 1905,
	oCylLtLabelFeederLockFwd				= 1906,
	oCylLtLabelFeederLockBwd				= 1907,

	oCylRtLabelFeederFwd					= 1910,
	oCylRtLabelFeederBwd					= 1911,
	oCylRtLabelFeederMountFwd				= 1912,
	oCylRtLabelFeederMountBwd				= 1913,
	oCylRtLabelFeederLockFwd				= 1914,
	oCylRtLabelFeederLockBwd				= 1915,

// CH 20
	oSolLaserExpenderAirBlow				= 2000,
	oSolLaserScannerAirBlow					= 2001,
	oCylPowerMeterOp						= 2002,
	oCylPowerMeterCl						= 2003,
	oCylLabelQcViLt							= 2004,
	oCylLabelQcViRt							= 2005,

	oCylRtCobotLabelPkUp					= 2008,
	oCylRtCobotLabelPkDn					= 2009,
	oCylRtCobotLabelPkTurnFwd				= 2010,
	oCylRtCobotLabelPkTurnBwd				= 2011,
	oVacRtCobotLabelOn						= 2012,
	oVacRtCobotLabelBlow					= 2013,
	oVacRtCobotMbbOn						= 2014,
	oVacRtCobotMbbBlow						= 2015,

// CH 21
	oSolLtMbbCstAirBlow						= 2100,

	oMbbRaderMode0							= 2102,
	oMbbRaderMode1							= 2103,
	oMbbRaderMode2							= 2104,
	oMbbRaderMode3							= 2105,
	oCylLtMbbCstFwd							= 2106,
	oCylLtMbbCstBwd							= 2107,
	oSolRtMbbCstAirBlow						= 2108,

	oCylRtMbbCstFwd							= 2114,
	oCylRtMbbCstBwd							= 2115,

// CH 22
	oLabelLtFeederPioLdReq					= 2200,
	oLabelLtFeederPioUldReq					= 2201,
	oLabelLtFeederPioAbort					= 2202,
	oLabelLtFeederPioRdy					= 2203,
	oLabelLtFeederPioSpare05				= 2204,
	oLabelLtFeederPioSpare06				= 2205,
	oLabelLtFeederPioSpare07				= 2206,
	oLabelLtFeederPioSpare08				= 2207,
	oLabelRtFeederPioLdReq					= 2208,
	oLabelRtFeederPioUldReq					= 2209,
	oLabelRtFeederPioAbort					= 2210,
	oLabelRtFeederPioRdy					= 2211,
	oLabelRtFeederPioSpare05				= 2212,
	oLabelRtFeederPioSpare06				= 2213,
	oLabelRtFeederPioSpare07				= 2214,
	oLabelRtFeederPioSpare08				= 2215,

// CH 23
	oMbbLtCstPioLdReq						= 2300,
	oMbbLtCstPioUldReq						= 2301,
	oMbbLtCstPioAbort						= 2302,
	oMbbLtCstPioRdy							= 2303,
	oMbbLtCstPioSpare05						= 2304,
	oMbbLtCstPioSpare06						= 2305,
	oMbbLtCstPioSpare07						= 2306,
	oMbbLtCstPioSpare08						= 2307,
	oMbbRtCstPioLdReq						= 2308,
	oMbbRtCstPioUldReq						= 2309,
	oMbbRtCstPioAbort						= 2310,
	oMbbRtCstPioRdy							= 2311,
	oMbbRtCstPioSpare05						= 2312,
	oMbbRtCstPioSpare06						= 2313,
	oMbbRtCstPioSpare07						= 2314,
	oMbbRtCstPioSpare08						= 2315,

// CH 24
	oViLabelQcTrig							= 2400,
	oViLabelFeederTrig						= 2401,
	oViLtCobotTrig							= 2402,
	oViInPkTrig								= 2403,
	oViRtCobotTrig							= 2404,
	oViMbbQcTrig							= 2405,

	oViLabelQcReset							= 2408,
	oViLabelFeederReset						= 2409,
	oViLtCobotReset							= 2410,
	oViInPkReset							= 2411,
	oViRtCobotReset							= 2412,
	oViMbbQcReset							= 2413,

// CH 25
	oViLabelQcMode1							= 2500,
	oViLabelQcMode2							= 2501,
	oViLabelQcMode3							= 2502,
	oViLabelQcMode4							= 2503,
	oViLabelFeederMode1						= 2504,
	oViLabelFeederMode2						= 2505,
	oViLabelFeederMode3						= 2506,
	oViLabelFeederMode4						= 2507,
	oViLtCobotMode1							= 2508,
	oViLtCobotMode2							= 2509,
	oViLtCobotMode3							= 2510,
	oViLtCobotMode4							= 2511,
	oViInPkMode1							= 2512,
	oViInPkMode2							= 2513,
	oViInPkMode3							= 2514,
	oViInPkMode4							= 2515,

// CH 26
	oViRtCobotMode1							= 2600,
	oViRtCobotMode2							= 2601,
	oViRtCobotMode3							= 2602,
	oViRtCobotMode4							= 2603,
	oViMbbQcMode1							= 2604,
	oViMbbQcMode2							= 2605,
	oViMbbQcMode3							= 2606,
	oViMbbQcMode4							= 2607,

	doLAST,
};

enum enAI							  
{									  
	// Left Part
	aiMainAir1								= 0,
	aiMainAir2								= 1,	
	aiMainAir3								= 2,	
	aiMainAir4								= 3,

	aiVacMbbOpenBtm							= 100,
	aiVacMbbOpenTop							= 101,	
	aiVacMbbOpenTop2						= 102,	// ADD

	aiVacSealNozzle							= 200,
	aiAirSealNozzleN2						= 201,	// Digital_300B
	aiLoadCellSealFt						= 202,
	aiLoadCellSealRr						= 203,

	aiVacLtCobotLabel						= 300,
	aiVacLtCobotFoam						= 301,
	aiVacLtCobotFoam2						= 302,
	aiFoamPkLaser							= 303,	// ADD IL-100

	aiVacFoamFlip							= 400,
	aiVacFoamFlip2							= 401,
	aiVacFoamPk								= 402,
	aiVacFoamPk2							= 403,

	// Right Part
	aiFlowLabelExhaust						= 500,
	aiVacQcSealedHead						= 501,
	aiVacLtLabelRecv						= 502,
	aiVacRtLabelRecv						= 503,

	aiVacRtCobotLabel						= 601,
	aiVacRtCobotMbb							= 602,
	aiVacRtCobotMbb2						= 603,

	aiSpare									= 701,
	aiLAST,
};									  
									  
enum enAO							  
{									  
	aoAirSealNozzleN2						= 0,	// Digital_300B
	aoLoadCellSeal							= 1,	// Average

	aoPressureRegulator						= 3,	// Average
	aoLAST,
};