#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_MMI.h"

namespace HW
{
	class Conveyor
	{
	private:
		CTimer									m_tmDelay;

	public:
		typedef struct _ConvState
		{
			INT32	doA;			// Display when defined
			INT32	doB;			// Display when defined
			INT32	iDir;			// (1:FWD,UP,OPEN,CW), (2:BWD,DOWN,CLOSE,CCW)
			INT32	oDir;			// (1:FWD,UP,OPEN,CW), (2:BWD,DOWN,CLOSE,CCW) 
		}ConvState;

		enum
		{
			conveyorFWD		= 1,
			conveyorBWD		= 2,
			conveyorSTOP	= 3,
		};

		static const int maxDevice = (conveyorMAX > 0) ? conveyorMAX : 1;
		int isVirtual;

		int										m_no;
		BOOL									m_isDriving;
		string									m_name;
		string									m_nameA;
		string									m_nameB;
		string									m_nameC;
		INT32									m_oCmd;
		ConvState								m_state;
		string									m_sLogFncAddStart;
		string									m_sLogFncAddEnd;

		Conveyor();
		virtual ~Conveyor();

	public:
		static BOOL DefaultCanMove(int dir, int index);
		static void CreateInitParameter();
		void SLogFncAddMessage(string message);
		void SLogStartEvent();
		void SLogEndEvent();
		BOOL SetIO(enDO doDir, enDO doRun);
		BOOL SetParam(string nameA, string nameB, string nameC);
		void Update();

		void Fwd(INT32 index = 0);
		void Bwd(INT32 index = 0);
		void Stop(INT32 index = 0);

		BOOL IsFwd(INT32 nDelay = 0);
		BOOL IsBwd(INT32 nDelay = 0);
		BOOL IsStop(INT32 nDelay = 0);

		void Simulation(void);
		BOOL(*CanMove)(int, int) = nullptr;

	private:
		void SetOut(BOOL isOnA, BOOL isOnB);

		void Actuate(int cmdDir);

	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Conveyor CONV[HW::Conveyor::maxDevice];
#else
extern CheckParser<HW::Conveyor, ConveyorList, ConveyorList(HW::Conveyor::maxDevice)> CONV;
#endif