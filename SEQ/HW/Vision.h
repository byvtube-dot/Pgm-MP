#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\SRC\IPC_MMI.h"

namespace HW
{
	class Vision
	{
	//private:
	public:
		enDI						m_iAuto;
		enDI						m_iBusy;
		enDI						m_iPass;
		enDI						m_iFail;
		enDI						m_iStop;
		enDI						m_iMode1;
		enDI						m_iMode2;
		enDI						m_iMode3;
		enDI						m_iMode4;
		enDI						m_iMode5;

		enDO						m_oStart;
		enDO						m_oReset;
		enDO						m_oLive;
		enDO						m_oMode1;
		enDO						m_oMode2;
		enDO						m_oMode3;
		enDO						m_oMode4;
		enDO						m_oMode5;


	private:
		bool						m_isReservedReset;
		CTimer						m_tmReservedReset;
		static void CompEvent(bool oldValue, bool newValue, LPVOID lpParam);
		static void SLogEvent(bool oldValue, bool newValue, LPVOID lpParam);

	public:
		static const int			maxDevice = (VI_MAX > 0) ? VI_MAX : 1;
		int							isVirtual;

		int							m_no;

		IPC_MMI::PrsViData			m_prsData;
		IPC_MMI::QcViData			m_qcData;
		IPC_MMI::BcdViData			m_unitData;
		Property<bool>				m_isWaitData;
		Property<bool>				m_isRcvComp;

		string						m_deviceId;
		string						m_materialType;
		string						m_name;
		//List<string>				m_indexName;

		Vision();
		virtual ~Vision();

		static void CreateInitParameter();
		void SLogStartEvent();
		void SLogEndEvent();
		BOOL SetIO(enDI iAuto, enDI iBusy,
			enDI iMode1, enDI iMode2, enDI iMode3, enDI iMode4, enDI iMode5,
			enDO oStart, enDO oReset, enDO oLive,
			enDO oMode1, enDO oMode2, enDO oMode3, enDO oMode4, enDO oMode5);
		BOOL SetIO(enDI iAuto, enDI iBusy, enDI iPass, enDI iFail, enDI iStop,
			enDI iMode1, enDI iMode2, enDI iMode3, enDI iMode4, enDI iMode5,
			enDO oStart, enDO oReset, enDO oLive,
			enDO oMode1, enDO oMode2, enDO oMode3, enDO oMode4, enDO oMode5);
		void Update();
		BOOL IsAuto();
		BOOL IsStart();
		BOOL Start(int mode);
		BOOL IsBusy();
		void Reset(BOOL isReservedReset = TRUE);
		void Clear();
		void SetMode(UINT mode);
		BOOL IsModeOn(UINT mode, DWORD dDelay = 0);
		BOOL IsMode(UINT mode);
		BOOL IsLive();
		bool LiveOn(int mode);
		void LiveOff();
	};
}

#ifndef CHECK_ACTUATOR_ARRAY
extern HW::Vision VI[HW::Vision::maxDevice];
#else
extern CheckParser<HW::Vision, VisionList, VisionList(HW::Vision::maxDevice)> VI;
#endif