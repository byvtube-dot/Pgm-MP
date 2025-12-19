#pragma once
#include "IO.h"
#include "..\COM\SharedIO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_LASER.h"

namespace HW
{
	class Laser
	{
	private:
		seqInput						m_iAuto;
		seqInput						m_iReady;
		seqInput						m_iBusy;
		seqInput						m_iError;
		seqInput						m_iRecvCoordData;

		seqOutput						m_oTrig;
		seqOutput						m_oReset;
		seqOutput						m_oSendCoordData;
		static void SLogEvent(bool oldValue, bool newValue, LPVOID lpParam);

	public:
		static const int maxDevice = (laserMAX > 0) ? laserMAX : 1;
		int isVirtual;

		INT32							m_no;
		CTimer							m_tmWaitCoordData;
		ErrName							m_codeErrorAuto;
		ErrName							m_codeErrorReady;
		ErrName							m_codeErrorBusy;
		ErrName							m_codeErrorDataWait;
		ErrName							m_codeErrorCoordDataWait;
		ErrName							m_codeErrorStop;
		ErrName							m_codeErrorMode;

		int								m_iMode;

		bool							m_isError;
		Property<bool>					m_isReadData;
		Property<bool>					m_isWaitData;
		bool							m_isWaitCoordData1;
		bool							m_isWaitCoordData2;

		bool							m_isErrorMode;
		bool							m_isErrorReady;
		bool							m_isErrorBusy;
		bool							m_isErrorCoordDataWait;
		bool							m_isErrorStop;
		bool							m_isErrorAuto;

		IPC_LASER::ScannerData			m_scanData;

		string							m_deviceId;
		string							m_materialType;
		string							m_name;
		//List<string>					m_indexName;

		Laser();
		virtual ~Laser();

		static void CreateInitParameter();
		void SLogStartEvent();
		void SLogEndEvent();
		void SetIO(seqInput iAuto, seqInput iReady, seqInput iBusy, seqInput iError, seqInput iRecvCoordData,
			seqOutput oTrig, seqOutput oReset, seqOutput oSendCoordData);
		void SetErr(ErrName codeErrorAuto, ErrName codeErrorReady, ErrName codeErrorBusy, ErrName codeErrorDataWait,
			ErrName codeErrorCoordDataWait, ErrName codeErrorStop, ErrName codeErrorMode);
		void Update();
		bool SendCoordData();
		BOOL IsError();
		BOOL Start();
		BOOL IsBusy();
		void Reset();
		void Clear();
		void SetMode(enLaserMode iMode);
		int IsMode();
		bool IsWaitCoordData();
		IPC_LASER::ScanCal RotateCalData(IPC_LASER::ScanCal* pOrg, int angle, int markCnt);
		IPC_LASER::ScannerCoord RotateCoordData(IPC_LASER::ScannerCoord* pOrg, int scanAngle, int drawAngle);
	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Laser LAS[HW::Laser::maxDevice];
#else
extern CheckParser<HW::Laser, LaserList, LaserList(HW::Laser::maxDevice)> LAS;
#endif