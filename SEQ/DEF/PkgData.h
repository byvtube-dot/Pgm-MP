#pragma once
enum PkgList
{
	unitCnt									= 1,
	
	productTargetCount						= 900,	// MMI
	productMontlyTargetCount				= 901,	// MMI

	PKG_MAX									= 1000,
};

enum DDmList
{
	uphReal									= 0,
	uphIdle									= 1,
												  
	seqProgramVer							= 10,
											  
	machineRunTime							= 15,
	machineStopTime							= 16,
	machineErrorTime						= 17,
									
	sealingPressureRegulator				= 39,		// Sealing PressureRegulator

	foamPkLaserHeightTolerance				= 56,		// 
	prsDataToleranceX						= 57,		// 
	prsDataToleranceY						= 58,		// 

	prsDataLtLabelSetX						= 60,		// 
	prsDataLtLabelSetY						= 61,		// 
	prsDataLtLabelSetT						= 62,		// 
	prsDataRtLabelSetX						= 63,		// 
	prsDataRtLabelSetY						= 64,		// 
	prsDataRtLabelSetT						= 65,		// 

	foamPkLaserDisplacement					= 100,
	sealingloadcell1						= 101, 
	sealingloadcell2						= 102, 
	sealingloadcellOutputValue				= 103, 
	mfcValuemilliLiter						= 104, 
	setmfcValueAnalogOutput					= 105,

	foamPkOffsetPitch						= 700,		// 25.09.10

	// Auto Screen UPH : 800 ~ 809
	scrUph01								= 800,
	scrUph02								= 801,
	scrUph03								= 802,
	scrUph04								= 803,
	scrUph05								= 804,
	scrUph06								= 805,
	scrUph07								= 806,
	scrUph08								= 807,
	scrUph09								= 808,
	scrUph10								= 809,

	// Auto Screen Yield : 810 ~ 819
	scrYield01								= 810,
	scrYield02								= 811,
	scrYield03								= 812,
	scrYield04								= 813,
	scrYield05								= 814,
	scrYield06								= 815,
	scrYield07								= 816,
	scrYield08								= 817,
	scrYield09								= 818,
	scrYield10								= 819,

	// Auto Screen Production : 820 ~ 849
	scrProduct01							= 820,
	scrProduct02							= 821,
	scrProduct03							= 822,
	scrProduct04							= 823,
	scrProduct05							= 824,
	scrProduct06							= 825,
	scrProduct07							= 826,
	scrProduct08							= 827,
	scrProduct09							= 828,
	scrProduct10							= 829,
	scrProduct11							= 830,
	scrProduct12							= 831,
	scrProduct13							= 832,
	scrProduct14							= 833,
	scrProduct15							= 834,
	scrProduct16							= 835,
	scrProduct17							= 836,
	scrProduct18							= 837,
	scrProduct19							= 838,
	scrProduct20							= 839,
	scrProduct21							= 840,
	scrProduct22							= 841,
	scrProduct23							= 842,
	scrProduct24							= 843,
	scrProduct25							= 844,
	scrProduct26							= 845,
	scrProduct27							= 846,
	scrProduct28							= 847,
	scrProduct29							= 848,
	scrProduct30							= 849,

	// Cycle Time : 850 ~ 899
	cycleTime01								= 850,
	cycleTime02								= 851,
	cycleTime03								= 852,
	cycleTime04								= 853,
	cycleTime05								= 854,
	cycleTime06								= 855,
	cycleTime07								= 856,
	cycleTime08								= 857,
	cycleTime09								= 858,
	cycleTime10								= 859,
	cycleTime11								= 860,
	cycleTime12								= 861,
	cycleTime13								= 862,
	cycleTime14								= 863,
	cycleTime15								= 864,
	cycleTime16								= 865,
	cycleTime17								= 866,
	cycleTime18								= 867,
	cycleTime19								= 868,
	cycleTime20								= 869,
	cycleTime21								= 870,
	cycleTime22								= 871,
	cycleTime23								= 872,
	cycleTime24								= 873,
	cycleTime25								= 874,
	cycleTime26								= 875,
	cycleTime27								= 876,
	cycleTime28								= 877,
	cycleTime29								= 878,
	cycleTime30								= 879,
	cycleTime31								= 880,
	cycleTime32								= 881,
	cycleTime33								= 882,
	cycleTime34								= 883,
	cycleTime35								= 884,
	cycleTime36								= 885,
	cycleTime37								= 886,
	cycleTime38								= 887,
	cycleTime39								= 888,
	cycleTime40								= 889,
	cycleTime41								= 880,
	cycleTime42								= 881,
	cycleTime43								= 882,
	cycleTime44								= 883,
	cycleTime45								= 884,
	cycleTime46								= 885,
	cycleTime47								= 886,
	cycleTime48								= 887,
	cycleTime49								= 888,
	cycleTime50								= 889,

	// Life Time : 900 ~ 929
	curLifeTime01							= 900,
	curLifeTime02							= 901,
	curLifeTime03							= 902,
	curLifeTime04							= 903,
	curLifeTime05							= 904,
	curLifeTime06							= 905,
	curLifeTime07							= 906,
	curLifeTime08							= 907,
	curLifeTime09							= 908,
	curLifeTime10							= 909,
	curLifeTime11							= 910,
	curLifeTime12							= 911,
	curLifeTime13							= 912,
	curLifeTime14							= 913,
	curLifeTime15							= 914,
	curLifeTime16							= 915,
	curLifeTime17							= 916,
	curLifeTime18							= 917,
	curLifeTime19							= 918,
	curLifeTime20							= 919,
	curLifeTime21							= 920,
	curLifeTime22							= 921,
	curLifeTime23							= 922,
	curLifeTime24							= 923,
	curLifeTime25							= 924,
	curLifeTime26							= 925,
	curLifeTime27							= 926,
	curLifeTime28							= 927,
	curLifeTime29							= 928,
	curLifeTime30							= 929,

	// Set Life Time : 900 ~ 929
	resetLifeTime01							= 930,
	resetLifeTime02							= 931,
	resetLifeTime03							= 932,
	resetLifeTime04							= 933,
	resetLifeTime05							= 934,
	resetLifeTime06							= 935,
	resetLifeTime07							= 936,
	resetLifeTime08							= 937,
	resetLifeTime09							= 938,
	resetLifeTime10							= 939,
	resetLifeTime11							= 940,
	resetLifeTime12							= 941,
	resetLifeTime13							= 942,
	resetLifeTime14							= 943,
	resetLifeTime15							= 944,
	resetLifeTime16							= 945,
	resetLifeTime17							= 946,
	resetLifeTime18							= 947,
	resetLifeTime19							= 948,
	resetLifeTime20							= 949,
	resetLifeTime21							= 950,
	resetLifeTime22							= 951,
	resetLifeTime23							= 952,
	resetLifeTime24							= 953,
	resetLifeTime25							= 954,
	resetLifeTime26							= 955,
	resetLifeTime27							= 956,
	resetLifeTime28							= 957,
	resetLifeTime29							= 958,
	resetLifeTime30							= 959,

	// Start Life Time : 960 ~ 989
	startLifeTime01							= 960,
	startLifeTime02							= 961,
	startLifeTime03							= 962,
	startLifeTime04							= 963,
	startLifeTime05							= 964,
	startLifeTime06							= 965,
	startLifeTime07							= 966,
	startLifeTime08							= 967,
	startLifeTime09							= 968,
	startLifeTime10							= 969,
	startLifeTime11							= 970,
	startLifeTime12							= 971,
	startLifeTime13							= 972,
	startLifeTime14							= 973,
	startLifeTime15							= 974,
	startLifeTime16							= 975,
	startLifeTime17							= 976,
	startLifeTime18							= 977,
	startLifeTime19							= 978,
	startLifeTime20							= 979,
	startLifeTime21							= 980,
	startLifeTime22							= 981,
	startLifeTime23							= 982,
	startLifeTime24							= 983,
	startLifeTime25							= 984,
	startLifeTime26							= 985,
	startLifeTime27							= 986,
	startLifeTime28							= 987,
	startLifeTime29							= 988,
	startLifeTime30							= 989,

	DDM_MAX									= 1000,
};

enum NDmList
{
    // <<< 0~199 : 설비 공통 >>>
	deviceCurGrpNo							= 0,
    deviceCurJobNo							= 1,
	changeJob								= 2,	/// 180211
   											  
	//enum enMachineState					  
	//{										  
	//	MC_STATE_STOP	  =0,				  
	//	MC_STATE_AUTO	  =1,				  
	//	MC_STATE_ERROR	  =2,				  
	//	MC_STATE_CYCLE	  =3,				  
	//	MC_STATE_EMG	  =4,				  
	//	MC_STATE_RUN_DOWN =5,				  
	//};									  
	scrMcState								= 3,
	startBeforeJobCheck						= 4,
											  
	// << Lot Start/End >>					  
											  
	lotState								= 10,	// 0: idle, 1: lot start, 2: pre end, 3: end
	lotStartEvent							= 11,	// mmi 기입.  Lot InPut
	lotEndCmd								= 12,	// mmi 기입.  화면 클릭
	lotEndEvent								= 13,	// seq 기입.  Auto LOTEND
	lotStartNum								= 14,	// mmi 기입.
	lotInsertCnt							= 15,	// mmi 기입.
	lotInCnt								= 16,	// seq 기입.
	lotOutCnt								= 17,	// seq 기입.
	lotUnitCnt								= 18,	// seq 기입.
	lotUseSkipCHECK				     		= 19,	// MMI
	//30~49 : 화면 버튼						    
	scrBtnStart								= 30,
	scrBtnStop								= 31,
	scrBtnReset								= 32,
	scrLampStart							= 33,
	scrLampStop								= 34,
	scrLampReset							= 35,
													  
	scrStopLoading							= 40,
	startSeq								= 64,
	scrAllHomeComp							= 65,
	scrLight								= 66,
										  
	bypassDoorClose							= 80,
	bypassDoorLock							= 81,

    jobEnable								= 96,
    jogAxisNo								= 97,
    jogSpeed								= 98,
    scrTenkeyNo								= 99,
										  
	// 100~113 : 각 파트별 자재 유/무 표시    
	// 1 : 자재 있음					   
	// 0 : 자재 없음					   
    sealInReady 							= 100,
    sealInError								= 101,
    sealInGood								= 102,
    sealInBad								= 103,
    sealInEnable							= 104,
    sealInStart								= 105,
    sealInReset								= 106,

	sealenOutAbort							= 120,
    sealenOutStart							= 121,
    sealenOutCalib							= 122,
    sealenOutShakeOff						= 123,
    sealenOutInit							= 124,
    sealenOutStop							= 125,
    sealenOutQuit							= 126,

	// 200 ~ 220 Cobot Safety Condition
    ltCobotInPkCanMove						= 200,	// Btm Place 
    ltCobotSendElevCanMove					= 201,
    ltCobotPartialElevCanMove				= 202,
    ltCobotFlipCanMove						= 203,
    ltCobotFoamPkCanMove					= 204,
    ltCobotLabelShuttleCanMove				= 205,
    ltCobotRtCobotMbbPlaceCanMove			= 206,
    ltCobotRtCobotRejectCanMove				= 207,

    rtCobotMbbShuttleCanMove				= 210,
    rtCobotLabelShuttleCanMove				= 211,
    rtCobotMbbElevCanMove					= 212,
    rtCobotLtCobotLabelPickCanMove			= 213,
    rtCobotLtCobotRejectCanMove				= 214,
	
	// 230 ~ Cobot Label Request
	reqLtCobotLabel							= 230,
	reqRtCobotLabel							= 231,
	
	// 250 ~ : MMI --> SEQ Comm State
	recvPreMcData							= 250,	// MMI -> SEQ Receive data From The first equipment

	// 300 ~ : SEQ --> MMI Comm State
    reqTrayShuttle2DRead					= 300,
    reqNextMcDataSend						= 301,

	// Vacuum Data // 제외되는 항목 제거는 추후 고민
    reqVacDataMainAir1						= 310,
    reqVacDataMainAir2						= 311,
    reqVacDataMainAir3						= 312,
    reqVacDataMainAir4						= 313,
    reqVacDataMbbOpenBtm					= 314,
    reqVacDataMbbOpenTop					= 315,
    reqVacDataMbbOpenTop2					= 316,
    reqVacDataSealNozzle					= 317,
    reqVacDataSealNozzleN2					= 318,
    reqVacDataLoadCellFt					= 319,
    reqVacDataLoadCellRr					= 320,
    reqVacDataLtCobotLabel					= 321,
    reqVacDataLtCobotFoam					= 322,
    reqVacDataLtCobotFoam2					= 323,
    reqVacDataFoamPkLaser					= 324,
    reqVacDataFoamFlip						= 325,
    reqVacDataFoamFlip2						= 326,
    reqVacDataFoamPk						= 327,
    reqVacDataFoamPk2						= 328,
    reqVacDataFlowLabelExhaust				= 329,
    reqVacDataQcSealedHead					= 330,
    reqVacDataLtLabelRecv					= 331,
    reqVacDataRtLabelRecv					= 332,
    reqVacDataRtCobotLabel					= 333,
    reqVacDataRtCobotMbb					= 334,
    reqVacDataRtCobotMbb2					= 335,

    reqLabelDataSend						= 350,

    reqViDataSendTrayPk						= 360,	// Mode0,1	: PP Band Inspection
    reqViDataSendInPkTrayVi					= 361,	// Mode0	: HIC,DESICCANT Exsit Inspection
    reqViDataSendInPkLabel					= 362,	// Mode3	: Label Inspection
    reqViDataSendRtCobotLtMbbStackMode1		= 363,	// Mode1	: Lt MBB Stack Label Inspection (Left)
    reqViDataSendRtCobotRtMbbStackMode1		= 364,	// Mode2	: Rt MBB Stack Label Inspection (Right)
    reqViDataSendLabelPrs					= 365,	
    reqViDataSendLabelQc					= 366,	
    reqViDataSendMbbSealing					= 367,	// MBB Sealing


	XGemStatus								= 500,

	commLotDisplay							= 510,
	commLotCloseCheck						= 511,
	commSboxLabelInfo						= 512,

	commEqpFoamLtBoxLoaderEmptyreq     		= 513,
	commEqpFoamRtBoxLoaderEmptyreq     	    = 514,
	commEqpLabelLtFeederLoaderEmptyreq      = 515,
	commEqpLabelRtFeederLoaderEmptyreq      = 516,
	commEqpLtMbbStackLoaderEmptyreq         = 517,
	commEqpRtMbbStackLoaderEmptyreq         = 518,


	commEqpFoamLtBoxFullCarreirMovereq  	= 519,
	commEqpFoamRtBoxFullCarreirMovereq 		= 520,
	commEqpLabelLtFeederFullCarreirMovereq  = 521,
	commEqpLabelRtFeederFullCarreirMovereq  = 522,
	commEqpLtMbbStackFullCarreirMovereq     = 523,
	commEqpRtMbbStackFullCarreirMovereq     = 524,


	commEqpFoamLtBoxPortStatusChange   	    = 525,
	commEqpFoamRtBoxPortStatusChange   	    = 526,
	commEqpLabelLtFeederPortStatusChange    = 527,
	commEqpLabelRtFeederPortStatusChange    = 528,
	commEqpLtMbbStackPortStatusChange       = 529,
	commEqpRtMbbStacPortStatusChange        = 530,


	commEqpFoamLtBoxRfidRead         		= 531,
	commEqpFoamRtBoxRfidRead         	    = 532,
	commEqpLabelLtFeederRfidRead            = 533,
	commEqpLabelRtFeederRfidRead            = 534,
	commEqpLtMbbStackRfidRead               = 535,
	commEqpRtMbbStackRfidRead               = 536,
	//PROCDESS
	commLotIn						    	= 540,
	commLotOut						    	= 541,

	commTrayTransferStart                   = 542,
	commTrayTransferEnd                     = 543,

	commFoamPadTransferStart                = 544,
	commFoamPadTransferEnd                  = 545,
	commLabelAttachStart                    = 546,
	commLabelAttachEnd                      = 547,
	commMbbLabelAttachStart                 = 548,
	commMbbLabelAttachEnd                   = 549,
	commMbbTransferStart                    = 550,
	commMbbTransferEnd                      = 551,
	commMbbSealingStart                     = 552,
	commMbbSealingEnd                       = 553,
	commMbbFoldingStart                     = 554,
	commMbbFoldingEnd                       = 555,
	commTrayOutStart                        = 556,
	commTrayOutEnd                          = 557,

	//FDC
	commmMbbOpenBottomVacEnd                = 560,
	commmMbbOpenTop1VacEnd                  = 561,
	commmMbbOpenTop2VacEnd                  = 562,
	commSealNozzleVacEnd                    = 563,
	commSealNozzle2VacEnd                   = 564,
	commFrontSealLoadCellVacEnd             = 565,
	commRearSealLoadCellVacEnd              = 566,
	commLeftCobotLabelVacEnd                = 567,
	commLeftCobotFoamPad1VacEnd             = 568,
	commLeftCobotFoamPad2VacEnd             = 569,
	commFoamFlip1VacEnd                     = 570,
	commFoamFlip2VacEnd                     = 571,
	commFoamPk1VacEnd                       = 572,
	commFoamPk2VacEnd                       = 573,
	commQcSealedHeadVacEnd                  = 574,
	commLeftLabelReciveVacEnd               = 575,
	commRightLabelReceiveVacEnd             = 576,
	commRightCobotLabelVacEnd               = 577,
	commRightCobotMbb1VacEnd                = 578,
	commRightCobotMbb2VacEnd                = 579,
	commLabelVisionResult                   = 580,
	commProductIn                           = 581,
	commProductOut                          = 582,

	dailyProductCnt							= 600,

	// 800 ~ debugging
    cycletimecheck0							= 800,
    cycletimecheck1							= 801,
    cycletimecheck2							= 802,
    cycletimecheck3							= 803,
    cycletimecheck4							= 804,
    cycletimecheck5							= 805,
    cycletimecheck6							= 806,
    cycletimecheck7							= 807,
    cycletimecheck8							= 808,
    cycletimecheck9							= 809,
    cycletimecheck10						= 810,
    cycletimecheck11						= 811,
    cycletimecheck12						= 812,
    cycletimecheck13						= 813,
    cycletimecheck14						= 814,
    cycletimecheck15						= 815,
    cycletimecheck16						= 816,
    cycletimecheck17						= 817,
    cycletimecheck18						= 818,
    cycletimecheck19						= 819,

	simulationDebug0						= 820,
	simulationDebug1						= 821,
	simulationDebug2						= 822,
	simulationDebug3						= 823,
	simulationDebug4						= 824,
	simulationDebug5						= 825,
	simulationDebug6						= 826,
	simulationDebug7						= 827,
	simulationDebug8						= 828,
	simulationDebug9						= 829,

	ltLabelPrintTest						= 840,
	rtLabelPrintTest						= 841,

	ltCobotModbusCycleTimeCheck				= 890,
	rtCobotModbusCycleTimeCheck				= 891,

	msgSeq2Mmi								= 900,	// SEQ -> MMI= 단 0은 Ready로 봄= 1~9 : 예외적인 트리거(강제종료 등..., = 값 10부터는 CMD NUMBER로 보면됨= 
	msgMmi2Seq								= 901,	// MMI -> SEQ  1: 팝업 중= 2 : 버튼 0 눌렀을때= 3 : 버튼 1 눌렀을때= 4 : 버튼 2 눌렀을때= 5 : 버튼 3 눌렀을때 
	updateMtSeq2Mmi							= 902,
	updatePkgSeq2Mmi						= 903,
	updateRepSeq2Mmi						= 904,

	ConnectedVision1						= 910,
	ConnectedVision2						= 911,
	ConnectedVision3						= 912,
	ConnectedVision4						= 913,
	ConnectedVision5						= 914,
	ConnectedVision6						= 915,
	ConnectedInline1				   		= 920,
	ConnectedInline2				   		= 921,
	ConnectedRfid1							= 925,
	ConnectedRfid2							= 926,
	ConnectedRfid3							= 927,
	ConnectedBarcode1						= 930,
	ConnectedLaser1							= 932,
	ConnectedSecsgem 				   		= 940,

   	NDM_MAX									= 1000,
};

enum SysList
{
	machineSpeed							= 0,

	SafetyLightCurtainAlarmTime				= 1,
	
	setScanRotAngle							= 21,
	setDrawRotAngle							= 22,

	// << Scanner auto calibration >	> 		  
	scanCalPointCnt							= 31,
	scanCalFovX								= 32,
	scanCalFovY								= 33,
	scanCalPitchX							= 34,
	scanCalPitchY							= 35,

	// << Vi 2D calibration >> 
	glassCalCntX							= 41,
	glassCalCntY							= 42,
	glassCalPitchX							= 43,
	glassCalPitchY							= 44,

	stageCalCntX							= 61,
	stageCalCntY							= 62,
	stageCalPitchX							= 63,
	stageCalPitchY							= 64,
	
	crossHairXCnt							= 81,
	crossHairYCnt							= 82,
	crossHairXPitch							= 83,
	crossHairYPitch							= 84,

	// Lamp&Buzzer Setting : 970 ~ 999
	twrLampR1								= 970,
	twrLampY1								= 971,
	twrLampG1								= 972,
	buzzer1									= 973,
	twrLampR2								= 974,
	twrLampY2								= 975,
	twrLampG2								= 976,
	buzzer2									= 977,
	twrLampR3								= 978,
	twrLampY3								= 979,
	twrLampG3								= 980,
	buzzer3									= 981,
	twrLampR4								= 982,
	twrLampY4								= 983,
	twrLampG4								= 984,
	buzzer4									= 985,
	twrLampR5								= 986,
	twrLampY5								= 987,
	twrLampG5								= 988,
	buzzer5									= 989,
	twrLampR6								= 990,
	twrLampY6								= 991,
	twrLampG6								= 992,
	buzzer6									= 993,
	twrLampR7								= 994,
	twrLampY7								= 995,
	twrLampG7								= 996,
	buzzer7									= 997,
	buzzerOff								= 998,
	buzzerOnTime							= 999,

	maxSYS									= 1000,
};

enum UseList
{
	useLotInput								= 0,
	useSecsGem								= 1,

	useLaserLabelPrint						= 3,

	useInPkBandingInsp						= 6,
	useFoamPkMixFlipInsp					= 7,
	useTrayShuttle2D						= 8,
	useTrayShuttleAirBlow					= 9,
	useMbbQc								= 10,

													  
	useCheckLaserPower						= 20,
	useAutoCrossHair						= 21,
	
	useFoamBoxAmr							= 30,	// AMR
	useLabelFeederAmr						= 31,	// AMR
	useMbbCstAmr							= 32,	// AMR

	useLtCobotContinueSeq					= 41,	// Label Attach, Foam Pick continuous operation
	useRtCobotContinueSeq					= 42,	// MBB VI, Label Attach, Label Attach Vi, MBB Pick continuous operation

	useFoamFlip								= 45,	// Use as Skip only for testing.

	useFanAlarmCheck						= 51,

	useStandalone							= 55,

	useMasterKeyMode						= 58,
	useDebugMode							= 59,

	
	maxUSE									= 1000,
};

enum RcpList
{
	trayMaxCount							= 0,		// 
	labelMaxCount							= 1,		// 
	mbbMaxCount								= 2,		// 
	sealing1stPressureRegulator				= 3,		// Sealing PressureRegulator
	sealing2ndPressureRegulator				= 4,		// Sealing PressureRegulator

	sendElevLevelDnDelay					= 10,		// Delay Defaul = ms
	recvElevLevelDnDelay					= 11,		//
	partialElevLevelDnDelay					= 12,		//
	sendElevLevelUpDelay					= 13,		//
	recvElevLevelUpDelay					= 14,		//
	partialElevLevelUpDelay					= 15,		//

	foamStackConvStopDelay					= 16,		//
	mbbConvStopArrivalDelay					= 17,		//
	mbbFoldConvStopArrivalDelay				= 18,		//
	labelArrivalDelay						= 19,		//
	laserDelay								= 20,		//

	insertTrq								= 30,		//
	insert1stTrqMaxVel						= 31,		//
	insert2ndTrqMaxVel						= 32,		//
	mbbOpenAirBlowOnTime					= 33,		// ms
	mbbOpenAirBlowOffTime					= 34,		// ms
	mbbOpenAirBlowCnt						= 35,		// Count
	setNozzleN2AoSetValue					= 36,		// Setting N2 Analog Output Value
	setNozzleN2SetMilliLiter				= 37,		// Setting N2 Result (Milli Liter)
	mbbElevLevelDnDelay						= 38,		// Delay Defaul = ms
	mbbElevLevelUpDelay						= 39,		// Delay Defaul = ms

	qcViInspCnt								= 40,		// Sealed MBB QC Vision Inspection Count
	qcViInspYPitch							= 41,		// Sealed MBB QC Vision Inspection Pitch
	mbbQcRetryCnt							= 42,		// 
	labelPrsRetryCnt						= 43,		// 
	inPkViRetryCnt							= 44,		// 
	rtCobotlabelViRetryCnt					= 45,		// 

	normalFoamMaxCnt						= 50,		// Normal = 120ea
	partialFoamMaxCnt						= 51,		// Partial = 64ea
	normalFoamThickness						= 52,		// Normal = 30mm 
	partialFoamThickness					= 53,		// Partial = 55mm
	normalFoamBtmGap						= 54,		// Normal Btm Gap = 10mm 
	partialFoamBtmGap						= 55,		// Partial Btm Gap = 40mm
	foamPkLaserFoamBaseHeight				= 56,		// 100mm	Foam Base IL-100 Setting Data = 75mm ~ 130mm
	foamPkLaserFoamWallHeight				= 57,		// 80mm		Foam Wall IL-100 Setting Data = 75mm ~ 130mm
	foamPkLaserPartialFoamBaseHeight		= 58,		// 100mm	Foam Base IL-100 Setting Data = 75mm ~ 130mm
	foamPkLaserPartialFoamWallHeight		= 59,		// 90mm		Foam Wall IL-100 Setting Data = 75mm ~ 130mm

	FoamBoxUnitCntX							= 60,		// 2
	FoamBoxUnitCntY							= 61,		// 4
	FoamBoxUnitPitchX						= 62,
	FoamBoxUnitPitchY						= 63,
	FoamBoxBlockCntX						= 64,
	FoamBoxBlockCntY						= 65,

	FoamBoxStartEdge						= 67,
	FoamBoxDirection						= 68,
	FoamBoxZigZag							= 69,
	
	FoamBoxBlock1EndCntX					= 70,
	FoamBoxBlock2EndCntX					= 71,
	FoamBoxBlock3EndCntX					= 72,
	FoamBoxBlock4EndCntX					= 73,
	
	FoamBoxBlock1EndCntY					= 75,
	FoamBoxBlock2EndCntY					= 76,
	FoamBoxBlock3EndCntY					= 77,
	FoamBoxBlock4EndCntY					= 78,
	
	FoamBoxBlock1OffsetX					= 80,
	FoamBoxBlock2OffsetX					= 81,
	FoamBoxBlock3OffsetX					= 82,
	FoamBoxBlock4OffsetX					= 83,
	
	FoamBoxBlock1OffsetY					= 85,
	FoamBoxBlock2OffsetY					= 86,
	FoamBoxBlock3OffsetY					= 87,
	FoamBoxBlock4OffsetY					= 88,
	
	labelEdgeCoordLtX						= 102,		// -60.0
	labelEdgeCoordLtY						= 103,		//  25.0
	labelEdgeCoordRtX						= 104,		//  60.0
	labelEdgeCoordRtY						= 105,		//  25.0
	calCrossHairCoordLtX					= 106,		// -58.0
	calCrossHairCoordLtY					= 107,		//  23.0
	calCrossHairCoordRtX					= 108,		//  58.0
	calCrossHairCoordRtY					= 109,		//  23.0

	usrLtLabelOffsetX						= 200,
	usrLtLabelOffsetY						= 201,
	usrLtLabelOffsetT						= 202,
	
	usrRtLabelOffsetX						= 210,
	usrRtLabelOffsetY						= 211,
	usrRtLabelOffsetT						= 212,

	// Set Life Time : 900 ~ 929
	setLifeTime01							= 900,
	setLifeTime02							= 901,
	setLifeTime03							= 902,
	setLifeTime04							= 903,
	setLifeTime05							= 904,
	setLifeTime06							= 905,
	setLifeTime07							= 906,
	setLifeTime08							= 907,
	setLifeTime09							= 908,
	setLifeTime10							= 909,
	setLifeTime11							= 910,
	setLifeTime12							= 911,
	setLifeTime13							= 912,
	setLifeTime14							= 913,
	setLifeTime15							= 914,
	setLifeTime16							= 915,
	setLifeTime17							= 916,
	setLifeTime18							= 917,
	setLifeTime19							= 918,
	setLifeTime20							= 919,
	setLifeTime21							= 920,
	setLifeTime22							= 921,
	setLifeTime23							= 922,
	setLifeTime24							= 923,
	setLifeTime25							= 924,
	setLifeTime26							= 925,
	setLifeTime27							= 926,
	setLifeTime28							= 927,
	setLifeTime29							= 928,
	setLifeTime30							= 929,

	// Life Time  Type: 930 ~ 989
	// 0 : Count= 1 : Time
	typeLifeTime01							= 930,
	typeLifeTime02							= 931,
	typeLifeTime03							= 932,
	typeLifeTime04							= 933,
	typeLifeTime05							= 934,
	typeLifeTime06							= 935,
	typeLifeTime07							= 936,
	typeLifeTime08							= 937,
	typeLifeTime09							= 938,
	typeLifeTime10							= 939,
	typeLifeTime11							= 940,
	typeLifeTime12							= 941,
	typeLifeTime13							= 942,
	typeLifeTime14							= 943,
	typeLifeTime15							= 944,
	typeLifeTime16							= 945,
	typeLifeTime17							= 946,
	typeLifeTime18							= 947,
	typeLifeTime19							= 948,
	typeLifeTime20							= 949,
	typeLifeTime21							= 950,
	typeLifeTime22							= 951,
	typeLifeTime23							= 952,
	typeLifeTime24							= 953,
	typeLifeTime25							= 954,
	typeLifeTime26							= 955,
	typeLifeTime27							= 956,
	typeLifeTime28							= 957,
	typeLifeTime29							= 958,
	typeLifeTime30							= 959,

	maxRCP									= 1000,
};