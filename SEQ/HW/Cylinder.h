#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_MMI.h"

namespace HW
{
	class Cylinder
	{
	private:
		int										m_timeLimit;
		ErrName									m_errorCode;
		int										m_errorAct;
		BOOL									m_errorUse;
		CTimer									m_tmDelay;
		CTimer									m_tmError;

	public:
		enum
		{
			cylinderFWD		= 1,
			cylinderUP		= 1,
			cylinderOPEN	= 1,
			cylinderCW		= 1,
			cylinderBWD		= 2,
			cylinderDOWN	= 2,
			cylinderCLOSE	= 2,
			cylinderCCW		= 2,
			cylinderFREE	= 3,
		};

		static const int maxDevice = (cylinderMAX > 0) ? cylinderMAX : 1;
		int isVirtual;

		int										m_no;
		BOOL									m_isDriving;
		BOOL									m_isError;
		INT32									m_delayTime;
		INT32									m_index;
		string									m_name;
		List<string>							m_indexName;
		string									m_nameA;
		string									m_nameB;
		INT32									m_oCmd;
		IPC_MMI::CylState						m_state;
		IPC_MMI::CylTable						m_table;
		COM::CylCommand::SingleParamter			m_command;
		string									m_sLogFncAddStart;
		string									m_sLogFncAddEnd;

		Cylinder();
		virtual ~Cylinder();

	public:
		static BOOL DefaultCanMove(int dir, int index);
		static void CreateInitParameter();
		void SLogFncAddMessage(string message);
		void SLogStartEvent();
		void SLogEndEvent();
		COM::CylCommand::SingleParamter CommandTable(int idx);
		BOOL SetIO(enDI diA, enDI diB, enDO doA, enDO doB);
		BOOL SetParam(string nameA, string nameB);
		void SetErr(int timeLimit, ErrName errorCode, int errorAct, BOOL errorUse);
		void SetErr(int errorAct);
		int  GetErr();
		void Reset();
		void Update();

		void Fwd(INT32 index = 0);
		void Bwd(INT32 index = 0);
		void Open(INT32 index = 0);
		void Close(INT32 index = 0);
		void Up(INT32 index = 0);
		void Dn(INT32 index = 0);
		void Cw(INT32 index = 0);
		void CCw(INT32 index = 0);

		BOOL IsFwd();
		BOOL IsBwd();
		BOOL IsOpen();
		BOOL IsClose();
		BOOL IsUp();
		BOOL IsDn();
		BOOL IsCw();
		BOOL IsCCw();

		void Simulation(void);
		BOOL(*CanMove)(int, int) = nullptr;

		int  GetPos();
	private:
		void SetOut(BOOL isOnA, BOOL isOnB);

		void Actuate(int cmdDir);
	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Cylinder CYL[HW::Cylinder::maxDevice];
#else
extern CheckParser<HW::Cylinder, CylinderList, CylinderList(HW::Cylinder::maxDevice)> CYL;
#endif