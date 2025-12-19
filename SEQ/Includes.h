#pragma once

#ifdef _MAIN_CPP_
	#define EXTERN
#else
	#define EXTERN extern
#endif

#include "ROBOT/RainbowRoboticsDef.h"
#include "ROBOT/RainbowRobotics.h"

#include "Defines.h"
#include "..\BASE\Ini.h"
#include "..\BASE\BaseAll.h"

#include "COM\AntiCollision.h"
#include "COM\ApiBuffer.h"
#include "COM\CommonParam.h"
#include "COM\Error.h"
#include "COM\FSM.h"
#include "COM\Log.h"
#include "COM\Notice.h"
#include "COM\ProcessCapa.h"
#include "COM\RecipeCopy.h"
#include "COM\SharedIO.h"
#include "COM\SLog.h"
#include "COM\TenkeyOpr.h"
#include "COM\UpdateMMI.h"
#include "COM\WmxLib.h"

#include "HW\Cylinder.h"
#include "HW\Conveyor.h"
#include "HW\IO.h"
#include "HW\LampBuzzer.h"
#include "HW\Laser.h"
#include "HW\Motor.h"
#include "HW\Tenkey.h"
#include "HW\Vacuum.h"
#include "HW\Vision.h"
#include "HW\SealingLib.h"
#include "HW\Sealing.h"

using namespace HW;
using namespace COM;

#include "SRC\IPC_LASER.h"
#include "SRC\IPC_MMI.h"
#include "SRC\SEQ.h"
#include "SRC\PART\_01_InPk.h"
#include "SRC\PART\_02_FoamRail.h"
#include "SRC\PART\_03_FoamElev.h"
#include "SRC\PART\_04_FoamConv.h"
#include "SRC\PART\_05_FoamPk.h"
#include "SRC\PART\_06_FoamBox.h"
#include "SRC\PART\_07_TrayShuttle.h"
#include "SRC\PART\_08_MbbShuttle.h"
#include "SRC\PART\_09_MbbQc.h"
#include "SRC\PART\_10_BtmFoldConv.h"
#include "SRC\PART\_11_FoldPk.h"
#include "SRC\PART\_12_LabelFeeder.h"
#include "SRC\PART\_13_LabelLaser.h"
#include "SRC\PART\_14_LabelShuttle.h"
#include "SRC\PART\_15_MbbStack.h"
#include "SRC\PART\_16_LtCoBot.h"
#include "SRC\PART\_17_RtCoBot.h"

//#include "SRC\PART\_31_DustCollector.h"	// Unused

EXTERN IPC_MMI::CIPC				IPCMMI;
EXTERN IPC_LASER::CIPC				IPCLASER;
EXTERN UphTable						uphTable;
EXTERN COpBtn						OPBTN;

EXTERN CError<ErrName>				ER;
EXTERN CError<WrName>				WR;
EXTERN CMsgNotice					MSGNOTICE;
EXTERN SharedIO						LASERIO;
EXTERN CTenkeyOpr					TENKEYOPR;
EXTERN CUpdateMMI					UPDATEMMI;
EXTERN CWmxLib						WMX;
EXTERN OprFlag						OPR;
EXTERN FlowIfCondition				FLOWIFCONDITION[MAX_API_BUFFER_CONDITION];
EXTERN SMessage						SMESSAGE;

#ifndef CHECK_ACTUATOR_ARRAY
EXTERN AntiCollision				ANTICOLLISION[AntiCollision::maxDevice];
EXTERN AntiCollisionCircle			ANTICOLLISIONCIRCLE[AntiCollisionCircle::maxDevice];
EXTERN ApiBufferCommand				APIBUFFER[ApiBufferCommand::maxDevice];
EXTERN CFSM							FSM[fsmMAX];
EXTERN CLog							LOG[logMAX];
EXTERN SLog							SLOG[SLog::maxDevice];
#else
EXTERN CheckParser<AntiCollision, AntiCollisionList, AntiCollisionList(AntiCollision::maxDevice)>							ANTICOLLISION;
EXTERN CheckParser<AntiCollisionCircle, AntiCollisionCircleList, AntiCollisionCircleList(AntiCollisionCircle::maxDevice)>	ANTICOLLISIONCIRCLE;
EXTERN CheckParser<ApiBufferCommand, ApiBufferList, ApiBufferList(ApiBufferCommand::maxDevice)>								APIBUFFER;
EXTERN CheckParser<CFSM, FsmList, FsmList(fsmMAX)>																			FSM;
EXTERN CheckParser<CLog, LogList, LogList(logMAX)>																			LOG;
EXTERN CheckParser<SLog, SLogList, SLogList(SLog::maxDevice)>																SLOG;
#endif

EXTERN CTimer													tmUphReal;
EXTERN CTimer													tmUphIdle;
EXTERN bool														bMachineEmpty;
EXTERN CTimer													tmRunDown;
EXTERN CTimer													tmMachineEmpty;

EXTERN Property<INT32>											INDEX_LOCK;

//EXTERN NameParser<AxisList, motorMAX>							MOTORNAME;
//EXTERN NameParser<InterpolationMotorList, interpolationMAX>		INTERPOLATIONNAME;
//EXTERN NameParser<ConveyorList, conveyorMAX>					CONVEYORNAME;
//EXTERN NameParser<CylinderList, cylinderMAX>					CYLINDERNAME;
//EXTERN NameParser<VacuumList, vacuumMAX>						VACUUMNAME;
//EXTERN NameParser<VisionList, VI_MAX>							VISIONNAME;
//EXTERN NameParser<LaserList, laserMAX>							LASERNAME;
//EXTERN NameParser<enDI, diLAST>									DINAME;
//EXTERN NameParser<enDO, doLAST>									DONAME;
//EXTERN NameParser<enAI, aiLAST>									AINAME;
//EXTERN NameParser<enAO, aoLAST>									AONAME;
//EXTERN NameParser<PkgList, PKG_MAX>								PKGNAME;
//EXTERN NameParser<DDmList, DDM_MAX>								DDMNAME;
//EXTERN NameParser<NDmList, NDM_MAX>								NDMNAME;
//EXTERN NameParser<UseList, maxUSE>								USENAME;
//EXTERN NameParser<SysList, maxSYS>								SYSNAME;
//EXTERN NameParser<RcpList, maxRCP>								RCPNAME;
//EXTERN NameParser<ErrName, ER_MAX>								ERNAME;
//EXTERN NameParser<WrName, WR_MAX>								WRNAME;
//EXTERN NameParser<RobotList, robotMAX>							ROBOTNAME;