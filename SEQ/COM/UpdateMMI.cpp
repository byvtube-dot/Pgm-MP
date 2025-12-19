#include "..\Includes.h"

namespace COM
{
	void CUpdateMMI::Init(int* pMtNDm, int* pPkgNDm, int* pRcpNDm,
		ErrName nErMtTimeOver, ErrName nErMtWriteFail,
		ErrName nErPkgTimeOver, ErrName nErPkgWriteFail,
		ErrName nErRcpTimeOver, ErrName nErRcpWriteFail)
	{
		m_pMtSeq2Mmi = pMtNDm;
		m_pPkgSeq2Mmi = pPkgNDm;
		m_pRcpSeq2Mmi = pRcpNDm;
		m_nErMtTimeOver = nErMtTimeOver;
		m_nErMtWriteFail = nErMtWriteFail;
		m_nErPkgTimeOver = nErPkgTimeOver;
		m_nErPkgWriteFail = nErPkgWriteFail;
		m_nErRcpTimeOver = nErRcpTimeOver;
		m_nErRcpWriteFail = nErRcpWriteFail;

		InitMtData();
		InitMtBuffer();
		InitPkgData();
		InitPkgBuffer();
		InitRcpData();
		InitRcpBuffer();
		*m_pMtSeq2Mmi = COMM_IDLE;
		*m_pPkgSeq2Mmi = COMM_IDLE;
		*m_pRcpSeq2Mmi = COMM_IDLE;
	}

	void CUpdateMMI::InitMtData(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pMtUpdate.setData[i].setMtNo = NOT_DEFINED;
			m_pMtUpdate.setData[i].setIdxNo = NOT_DEFINED;
			m_pMtUpdate.setData[i].setPos = NOT_DEFINED;
			m_pMtUpdate.setData[i].setAcc = NOT_DEFINED;
			m_pMtUpdate.setData[i].setSpeed = NOT_DEFINED;
		}
		*m_pMtSeq2Mmi = COMM_IDLE;
	}

	void CUpdateMMI::InitMtBuffer(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pMtBuffer.setData[i].setMtNo = NOT_DEFINED;
			m_pMtBuffer.setData[i].setIdxNo = NOT_DEFINED;
			m_pMtBuffer.setData[i].setPos = NOT_DEFINED;
			m_pMtBuffer.setData[i].setAcc = NOT_DEFINED;
			m_pMtBuffer.setData[i].setSpeed = NOT_DEFINED;
		}
	}

	void CUpdateMMI::InitPkgData(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pPkgUpdate.setData[i].setPkgNo = NOT_DEFINED;
			m_pPkgUpdate.setData[i].setValue = NOT_DEFINED;
		}
		*m_pPkgSeq2Mmi = COMM_IDLE;
	}

	void CUpdateMMI::InitPkgBuffer(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pPkgBuffer.setData[i].setPkgNo = NOT_DEFINED;
			m_pPkgBuffer.setData[i].setValue = NOT_DEFINED;
		}
	}

	void CUpdateMMI::InitRcpData(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pRcpUpdate.setData[i].setPkgNo = NOT_DEFINED;
			m_pRcpUpdate.setData[i].setValue = NOT_DEFINED;
		}
		*m_pRcpSeq2Mmi = COMM_IDLE;
	}

	void CUpdateMMI::InitRcpBuffer(void)
	{
		for (int i = 0; i < 100; i++)
		{
			m_pRcpBuffer.setData[i].setPkgNo = NOT_DEFINED;
			m_pRcpBuffer.setData[i].setValue = NOT_DEFINED;
		}
	}

	bool CUpdateMMI::MtTimeOver(LONG tmDelay)
	{
		bool waitState = (COMM_IDLE < *m_pMtSeq2Mmi);

		if (!waitState)
			m_tmMtUpdateTime.Reset();
		else if (m_tmMtUpdateTime.TmOver(tmDelay))
		{
			ER.Save(m_nErMtTimeOver);
			m_tmMtUpdateTime.Reset();
			*m_pMtSeq2Mmi = COMM_IDLE;
			return (TRUE);
		}
		return (FALSE);
	}

	bool CUpdateMMI::PkgTimeOver(LONG tmDelay)
	{
		bool waitState = (COMM_IDLE < *m_pPkgSeq2Mmi);

		if (!waitState)
			m_tmPkgUpdateTime.Reset();
		else if (m_tmPkgUpdateTime.TmOver(tmDelay))
		{
			ER.Save(m_nErPkgTimeOver);
			m_tmPkgUpdateTime.Reset();
			*m_pPkgSeq2Mmi = COMM_IDLE;
			return (TRUE);
		}
		return (FALSE);
	}

	bool CUpdateMMI::RcpTimeOver(LONG tmDelay)
	{
		bool waitState = (COMM_IDLE < *m_pRcpSeq2Mmi);

		if (!waitState)
			m_tmRcpUpdateTime.Reset();
		else if (m_tmRcpUpdateTime.TmOver(tmDelay))
		{
			ER.Save(m_nErRcpTimeOver);
			m_tmRcpUpdateTime.Reset();
			*m_pRcpSeq2Mmi = COMM_IDLE;
			return (TRUE);
		}
		return (FALSE);
	}

	bool CUpdateMMI::IsRdy(void)
	{
		if (COMM_IDLE == *m_pMtSeq2Mmi && COMM_IDLE == *m_pPkgSeq2Mmi && COMM_IDLE == *m_pRcpSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsMtFail(void)
	{
		if (COMM_ERR == *m_pMtSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsPkgFail(void)
	{
		if (COMM_ERR == *m_pPkgSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsRcpFail(void)
	{
		if (COMM_ERR == *m_pRcpSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsMtCmp(void)
	{
		if (COMM_COMP == *m_pMtSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsPkgCmp(void)
	{
		if (COMM_COMP == *m_pPkgSeq2Mmi)
			return true;

		return false;
	}

	bool CUpdateMMI::IsRcpCmp(void)
	{
		if (COMM_COMP == *m_pRcpSeq2Mmi)
			return true;

		return false;
	}

	void CUpdateMMI::SetMtData(AxisList nMtNo, int nIdxNo, double dPos, double dSpeed, double dAcc)
	{
		if (!Between(nMtNo, 0, motorMAX - 1) || !Between(nIdxNo, 0, MAX_INDEX_MOTOR - 1))
			return;

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pMtBuffer.setData[i].setMtNo)
				continue;

			m_pMtBuffer.setData[i].setMtNo = nMtNo;
			m_pMtBuffer.setData[i].setIdxNo = nIdxNo;
			m_pMtBuffer.setData[i].setPos = dPos;

			if (0 > dSpeed)
				m_pMtBuffer.setData[i].setSpeed = MT[nMtNo].VelTable(nIdxNo);
			else
				m_pMtBuffer.setData[i].setSpeed = dSpeed;

			if (0 > dAcc)
				m_pMtBuffer.setData[i].setAcc = MT[nMtNo].AccTable(nIdxNo);
			else
				m_pMtBuffer.setData[i].setAcc = dAcc;

			LOG[logSEQ].Message("[UPDATE] MOTOR %d INDEX %d POS [%.3f -> %.3f]",
				nMtNo, nIdxNo, MT[nMtNo].PosTable(nIdxNo), m_pMtBuffer.setData[i].setPos);
			LOG[logSEQ].Message("[UPDATE] MOTOR %d INDEX %d VEL [%.3f -> %.3f]",
				nMtNo, nIdxNo, MT[nMtNo].VelTable(nIdxNo), m_pMtBuffer.setData[i].setSpeed);
			LOG[logSEQ].Message("[UPDATE] MOTOR %d INDEX %d ACC [%.3f -> %.3f]",
				nMtNo, nIdxNo, MT[nMtNo].AccTable(nIdxNo), m_pMtBuffer.setData[i].setAcc);

			break;
		}
	}

	void CUpdateMMI::SetPkgData(int nPkgNo, double dValue)
	{
		if (!Between(nPkgNo, 0, PKG_MAX - 1))
			return;

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pPkgBuffer.setData[i].setPkgNo)
				continue;

			m_pPkgBuffer.setData[i].setPkgNo = nPkgNo;
			m_pPkgBuffer.setData[i].setValue = dValue;

			LOG[logSEQ].Message("[UPDATE] PKG NO. %d [%.3f -> %.3f]",
				nPkgNo, NV.pkg[(PkgList)nPkgNo], dValue);

			break;
		}
	}

	void CUpdateMMI::SetRcpData(int nPkgNo, double dValue)
	{
		if (!Between(nPkgNo, 0, maxRCP - 1))
			return;

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pRcpBuffer.setData[i].setPkgNo)
				continue;

			m_pRcpBuffer.setData[i].setPkgNo = nPkgNo;
			m_pRcpBuffer.setData[i].setValue = dValue;

			LOG[logSEQ].Message("[UPDATE] RCP NO. %d [%.3f -> %.3f]",
				nPkgNo, NV.rcp[(RcpList)nPkgNo], dValue);

			break;
		}
	}

	void CUpdateMMI::Update(void)
	{
		if (MtTimeOver(5000) || PkgTimeOver(5000) || RcpTimeOver(5000))
		{
			if (MtTimeOver(5000))
			{
				InitMtData();
				InitMtBuffer();
			}
			if (PkgTimeOver(5000))
			{
				InitPkgData();
				InitPkgBuffer();
			}
			if (RcpTimeOver(5000))
			{
				InitRcpData();
				InitRcpBuffer();
			}
			return;
		}

		if (IsMtFail() || IsPkgFail() || IsRcpFail())
		{
			if (IsMtFail())
			{
				InitMtData();
				InitMtBuffer();
				ER.Save(m_nErMtWriteFail);
			}
			if (IsPkgFail())
			{
				InitPkgData();
				InitPkgBuffer();
				ER.Save(m_nErPkgWriteFail);
			}
			if (IsRcpFail())
			{
				InitRcpData();
				InitRcpBuffer();
				ER.Save(m_nErRcpWriteFail);
			}
			return;
		}

		if (IsMtCmp() || IsPkgCmp() || IsRcpCmp())
		{
			if (IsMtCmp())
				InitMtData();
			if (IsPkgCmp())
				InitPkgData();
			if (IsRcpCmp())
				InitRcpData();
		}

		if (!IsRdy())
			return;

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pMtBuffer.setData[i].setMtNo)
			{
				memcpy(&m_pMtUpdate, &m_pMtBuffer, sizeof(m_pMtUpdate));
				InitMtBuffer();
				*m_pMtSeq2Mmi = COMM_REQ;
				return;
			}
		}

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pPkgBuffer.setData[i].setPkgNo)
			{
				memcpy(&m_pPkgUpdate, &m_pPkgBuffer, sizeof(m_pPkgUpdate));
				InitPkgBuffer();
				*m_pPkgSeq2Mmi = COMM_REQ;
				return;
			}
		}

		for (int i = 0; i < 100; i++)
		{
			if (NOT_DEFINED != m_pRcpBuffer.setData[i].setPkgNo)
			{
				memcpy(&m_pRcpUpdate, &m_pRcpBuffer, sizeof(m_pRcpUpdate));
				InitRcpBuffer();
				*m_pRcpSeq2Mmi = COMM_REQ;
				return;
			}
		}
	}
}