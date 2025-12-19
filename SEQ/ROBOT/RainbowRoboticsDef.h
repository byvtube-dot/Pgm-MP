#pragma once
#include "stdint.h"

#define RobotIp					L"10.0.2.7"
#define RobotPort				502

#define AddressDi				0
#define AddressDo				1
#define AddressState			50
#define AddressCurTcp			336
#define AddressCurJoint			262
#define AddressCmd				100
#define AddressUserCmdRd		128
#define AddressUserCmdWr		190

#define	MAX_INDEX_ROBOT			50

namespace RainbowRobotics
{
	enum RobotList
	{
		robotLt							= 0,
		robotRt							= 1,
		robotMAX,
	};

	enum PosOfLtCobot
	{
		P_LT_COBOT_READY								= 0,
		P_LT_COBOT_BTM_FOAM_PICK						= 1,
		P_LT_COBOT_BTM_FOAM_PICK_TO_READY				= 2,
		P_LT_COBOT_BTM_PARTIAL_FOAM_PICK				= 3,
		P_LT_COBOT_BTM_PARTIAL_FOAM_PICK_TO_READY		= 4,
		P_LT_COBOT_BTM_FOAM_PLACE						= 5,
		P_LT_COBOT_BTM_FOAM_PLACE_TO_READY				= 6,
		P_LT_COBOT_BTM_PARTIAL_FOAM_PLACE				= 7,
		P_LT_COBOT_BTM_PARTIAL_FOAM_PLACE_TO_READY		= 8,
		P_LT_COBOT_LT_LABEL_PICK_READY					= 9,	// Cylinder Act
		P_LT_COBOT_LT_LABEL_PICK						= 10,	// Vacuum Act
		P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY			= 11,
		P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY			= 12,	// Cylinder Action when moving
		P_LT_COBOT_RT_LABEL_PICK_READY					= 13,	// Cylinder Act
		P_LT_COBOT_RT_LABEL_PICK						= 14,	// Vacuum Act
		P_LT_COBOT_RT_LABEL_PICK_TO_PICK_READY			= 15,
		P_LT_COBOT_RT_LABEL_PICK_READY_TO_READY			= 16,	// Cylinder Action when moving
		P_LT_COBOT_LABEL_ATTACH							= 17,
		P_LT_COBOT_LABEL_ATTACH_TO_READY				= 18,
		P_LT_COBOT_TOP_FOAM_PICK						= 19,
		P_LT_COBOT_TOP_FOAM_PICK_TO_READY				= 20,
		P_LT_COBOT_TOP_FOAM_PLACE						= 21,
		P_LT_COBOT_TOP_FOAM_PLACE_TO_READY				= 22,
		P_LT_COBOT_FOAM_REJECT							= 23,
		P_LT_COBOT_FOAM_REJECT_TO_READY					= 24,

		P_LT_COBOT_MAX,
	};

	enum PosOfRtCobot
	{
		P_RT_COBOT_READY								= 0,
		P_RT_COBOT_LT_LABEL_PICK						= 1,
		P_RT_COBOT_LT_LABEL_PICK_TO_READY				= 2,
		P_RT_COBOT_RT_LABEL_PICK						= 3,
		P_RT_COBOT_RT_LABEL_PICK_TO_READY				= 4,
		P_RT_COBOT_LT_MBB_VI							= 5,
		P_RT_COBOT_LT_MBB_VI_TO_READY					= 6,
		P_RT_COBOT_LT_MBB_VI_TO_LABEL_ATTACH_READY		= 7,
		P_RT_COBOT_RT_MBB_VI							= 8,
		P_RT_COBOT_RT_MBB_VI_TO_READY					= 9,
		P_RT_COBOT_RT_MBB_VI_TO_LABEL_ATTACH_READY		= 10,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_READY			= 11,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH					= 12,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_READY			= 13,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_TO_ATTACH_VI		= 14,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_READY			= 15,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH					= 16,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_TO_READY			= 17,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_TO_ATTACH_VI		= 18,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI				= 19,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_READY		= 20,
		P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI_TO_PICK		= 21,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI				= 22,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI_TO_READY		= 23,
		P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI_TO_PICK		= 24,
		P_RT_COBOT_LT_MBB_PICK							= 25,
		P_RT_COBOT_LT_MBB_PICK_TO_READY					= 26,
		P_RT_COBOT_RT_MBB_PICK							= 27,
		P_RT_COBOT_RT_MBB_PICK_TO_READY					= 28,
		P_RT_COBOT_MBB_PLACE							= 29,
		P_RT_COBOT_MBB_PLACE_TO_READY					= 30,
		P_RT_COBOT_LABEL_REJECT							= 31,
		P_RT_COBOT_LABEL_REJECT_TO_READY				= 32,
		P_RT_COBOT_MBB_REJECT							= 33,
		P_RT_COBOT_MBB_REJECT_TO_READY					= 34,

		P_RT_COBOT_MAX,
	};

	enum RobotCmd
	{
		cmdIdle							= 0,

		cmdStartProgramOnce				= cmdIdle,
		cmdStartProgramRepeat			,
		cmdPauseProgram					,
		cmdStopProgram					,
		cmdResumefromPause				,
		cmdResumefromCollision			,
		cmdLoadDefaultProgram			,
		cmdRobotArmActivation			,
		cmdChangeToRealMode				,
		cmdPowerOffTheRobotArm			,
		cmdLoadProgramTable				,
		cmdResetSosFlag					,
		cmdSpeedBarChage				,

		cmdMax,
	};

	enum RobotUserCmd
	{
		userCmdIdle						= 0,

		userCmdReadTcpPzOffset     		= userCmdIdle,	// 128
		userCmdReadSpare01				,				// 129
		userCmdReadCurIdx				,				// 130
		userCmdReadError				,				// 131
		userCmdReadMax					,				

		userCmdWriteTcpPzOffset			= userCmdIdle,	// 190
		userCmdWriteSpare01				,				// 191
		userCmdWriteNxtIdx          	,				// 192
		userCmdWriteMove            	,				// 193
		userCmdWriteHome            	,				// 194
		userCmdWriteReset           	,				// 195
		userCmdWriteMax					,				
	};

	enum RobotDi
	{
		iRobot00						= 0,
		iRobot01						,
		iRobot02						,
		iRobot03						,
		iRobot04						,
		iRobot05						,
		iRobot06						,
		iRobot07						,
		iRobot08						,
		iRobot09						,
		iRobot10						,
		iRobot11						,
		iRobot12						,
		iRobot13						,
		iRobot14						,
		iRobot15						,

		robotDiMax,
	};
	enum RobotDo
	{
		oRobot00						 = 0,
		oRobot01						,
		oRobot02						,
		oRobot03						,
		oRobot04						,
		oRobot05						,
		oRobot06						,
		oRobot07						,
		oRobot08						,
		oRobot09						,
		oRobot10						,
		oRobot11						,
		oRobot12						,
		oRobot13						,
		oRobot14						,
		oRobot15						,

		robotDoMax,
	};

	enum RobotWordAddress
	{
		addressDi							= 0,

		addressActivated					= 50,
		addressRealMode						= 51,
		addressCollisionDetected			= 52,
		addressArmPowerEngaged				= 53,
		addressDirectTeachingMode			= 54,
		addressMoving						= 55,
		addressPauseState					= 56,
		addressTeachingPendantConnecred		= 57,
		addressProgramRun					= 58,
		addressNoArcModeOn					= 59,
		addressEmgButtonReleased			= 60,
		addressFirstProgramRun				= 61,
		addressLastProgramLoadDone			= 62,
		addressControlBoxAmpere				= 63,
		addressUnderActivation				= 64,
		addressCollisionState				= 65,
		addressCollisionThreshold			= 66,

		addressTcpMass						= 73,
		addressSystemSOS					= 74,
		addressHomeIsBegin					= 75,
		addressControlBoxHeartBeat			= 76,
		addressSpeedBarValue				= 77,

		// 100 배열에 256번지 부터 Read
		addressJointCurrent0				= 268 - 256,
		addressJointCurrent1				= 269 - 256,
		addressJointCurrent2				= 270 - 256,
		addressJointCurrent3				= 271 - 256,
		addressJointCurrent4				= 272 - 256,
		addressJointCurrent5				= 273 - 256,

		addressTCPCurrent0					= 336 - 256,
		addressTCPCurrent1					= 337 - 256,
		addressTCPCurrent2					= 338 - 256,
		addressTCPCurrent3					= 339 - 256,
		addressTCPCurrent4					= 340 - 256,
		addressTCPCurrent5					= 341 - 256,

		addressMax,
	};
}

