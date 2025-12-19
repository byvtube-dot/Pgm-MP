#pragma once
#include "..\COM\FSM.h"

namespace COM
{
	class CUpdateMMI
	{
	protected:
		int*					m_pMtSeq2Mmi;
		int*					m_pPkgSeq2Mmi;
		int*					m_pRcpSeq2Mmi;
		ErrName					m_nErMtTimeOver;
		ErrName					m_nErMtWriteFail;
		ErrName					m_nErPkgTimeOver;
		ErrName					m_nErPkgWriteFail;
		ErrName					m_nErRcpTimeOver;
		ErrName					m_nErRcpWriteFail;

	public:
		IPC_MMI::MtUpdateMmi	m_pMtUpdate;
		IPC_MMI::MtUpdateMmi	m_pMtBuffer;
		IPC_MMI::PkgUpdateMmi	m_pPkgUpdate;
		IPC_MMI::PkgUpdateMmi	m_pPkgBuffer;
		IPC_MMI::PkgUpdateMmi	m_pRcpUpdate;
		IPC_MMI::PkgUpdateMmi	m_pRcpBuffer;
		CTimer					m_tmMtUpdateTime;
		CTimer					m_tmPkgUpdateTime;
		CTimer					m_tmRcpUpdateTime;

		void Init(int* pMtNDm, int* pPkgNDm, int* pRcpNDm,
			ErrName nErMtTimeOver, ErrName nErMtWriteFail,
			ErrName nErPkgTimeOver, ErrName nErPkgWriteFail,
			ErrName nErRcpTimeOver, ErrName nErRcpWriteFail);

		void InitMtData(void);
		void InitMtBuffer(void);
		void InitPkgData(void);
		void InitPkgBuffer(void);
		void InitRcpData(void);
		void InitRcpBuffer(void);
		bool MtTimeOver(LONG tmDelay);
		bool PkgTimeOver(LONG tmDelay);
		bool RcpTimeOver(LONG tmDelay);
		bool IsRdy(void);
		bool IsMtCmp(void);
		bool IsPkgCmp(void);
		bool IsRcpCmp(void);
		bool IsMtFail(void);
		bool IsPkgFail(void);
		bool IsRcpFail(void);
		void SetMtData(AxisList nMtNo, int nIdxNo, double dPos, double dSpeed = (double)NOT_DEFINED, double dAcc = (double)NOT_DEFINED);
		void SetPkgData(int nPkgNo, double dValue);
		void SetRcpData(int nPkgNo, double dValue);
		void Update();

		CUpdateMMI() {}
		virtual ~CUpdateMMI() {}
	};
}


