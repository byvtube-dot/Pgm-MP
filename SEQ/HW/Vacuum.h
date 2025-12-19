#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_MMI.h"

namespace HW
{
	class Vacuum
	{
	private:
		double	m_dDeviceLevelH;	// Analog To Digital Convertor level	(ampare or voltage)
		double	m_dDeviceLevelL;	// Analog To Digital Convertor level	(ampare or voltage)
		double	m_dSensorLevelH;	// Sensor Signal level					(ampare or voltage)
		double	m_dSensorLevelL;	// Sensor Signal level					(ampare or voltage)
		double	m_dDisplayValueH;	// Sensor Display value (kpa, N, mm, etc..)
		double	m_dDisplayValueL;	// Sensor Display value (kpa, N, mm, etc..)
		double	m_dResolution;		// Data Resolution (S5, S7, Koses Vac)

	public:
		static const int maxDevice = (vacuumMAX > 0) ? vacuumMAX : 1;
		int isVirtual;
		enum
		{
			vacuumON	= 1,
			vacuumOFF	= 2,
			vacuumFREE	= 3,
		};

		int										m_no;
		BOOL									m_isDriving;
		BOOL									m_isError;
		INT32									m_delayTime;
		bool									m_bAnalogueInput;
		ErrName									m_errorCode;
		LONG									m_timeLimit;
		int										m_errorAct;
		BOOL									m_errorUse;
		enAI									m_aiNo;
		IPC_MMI::VacState						m_state;
		IPC_MMI::VacTable						m_table;
		IPC_MMI::VacFdcData						m_fdcData;			/** FDC VACUUM DATA **/
		COM::VcCommand::SingleParamter			m_command;
		CTimer									m_tmHighOn;
		CTimer									m_tmLowOff;
		CTimer									m_tmBlowOff;
		string									m_deviceId;
		string									m_materialType;
		string									m_name;
		List<string>							m_indexName;
		string									m_nameA;
		string									m_nameB;
		INT32									m_iDir;
		INT32									m_oDir;
		INT32									m_oCmd;
		CTimer									m_tmDelay;
		CTimer									m_tmError;
		string									m_sLogFncAddStart;
		string									m_sLogFncAddEnd;

		// MFC 적산유량 계산 25.08.26
		bool m_bMfcSensor;
		CTimer m_tmRefreshMfcSensor;
		double m_LastUpdateTime;

		// FDC Data Average
		CTimer m_tmFdcRefresh;
		double m_FdcLastUpdateTime;
		double m_FdcAverageData;
		double m_FdcAverageDataCnt;

		Vacuum();
		virtual ~Vacuum();

	public:
		static void CreateInitParameter();
		void SLogFncAddMessage(string message);
		void SLogStartEvent();
		void SLogEndEvent();
		COM::VcCommand::SingleParamter CommandTable(int idx);
		BOOL SetIO(enAI ioNo, enDO oOnNo, enDO oOffNo, enDO oBlowNo);
		BOOL SetIO(enAI ioNo, enDI iHighNo, enDI iLowNo, enDO oOnNo, enDO oOffNo, enDO oBlowNo);
		void SetParam(double dLevelH, double dLevelL, double dSensorH, double dSensorL, double dValueH, double dValueL, double dResolution);
		void SetErr(LONG timeLimit, ErrName errorCode, int errorAct, BOOL errorUse);
		void SetErr(int errorAct);
		void Reset();
		void Update(void);
		void Clear(void);
		void On(int index = 0);
		void Off(int index = 0, bool bBlowUse = true);
		void BlowOn(int index = 0);
		void BlowOff(void);
		bool AOn();
		bool AOff();
		bool IsOn(LONG lDelayTime = 0);
		bool IsOff(LONG lDelayTime = 0);
		bool IsBlowOn(LONG lDelayTime = 0);
		bool IsBlowOff(LONG lDelayTime = 0);
	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Vacuum VC[HW::Vacuum::maxDevice];
#else
extern CheckParser<HW::Vacuum, VacuumList, VacuumList(HW::Vacuum::maxDevice)> VC;
#endif
