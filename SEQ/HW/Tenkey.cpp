#include "..\Includes.h"

namespace HW
{
	CTenkey::CTenkey()
	{
		m_nCount = 0;
		m_isUnitofHundred = TRUE;
	}

	CTenkey::~CTenkey()
	{

	}

	void CTenkey::SetIO(PUSHORT pInAddr, PUSHORT pOutAddr)
	{
		if (!Between(m_nCount, 0, MAX_TENKEY_CNT - 1))
		{
			throw CException(CException::INIT_FAIL, __LINE__, __FUNCTION__);
		}

		m_pKeyPad[m_nCount] = (KEY_PAD*)pInAddr;
		m_pSegmentOutput[m_nCount] = (SEGMENT*)pOutAddr;
		m_nCount++;
	}

	void CTenkey::SetUnitofHundred(BOOL isUnitofHundred)
	{
		m_isUnitofHundred = isUnitofHundred;
	}

	int CTenkey::GetTenkeyNo(void)
	{
		if (!Between(m_nCount, 1, MAX_TENKEY_CNT))
		{
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);
		}

		int nTenkeyNo = NO_KEY;
		int nKeyPadNo = GetKeyPadNo();

		for (int i = 0; i < m_nCount; i++)
		{
			if (m_isUnitofHundred)
				m_pSegmentOutput[i]->hDigit = m_segment.hDigit;

			m_pSegmentOutput[i]->mDigit = ~m_segment.mDigit;
			m_pSegmentOutput[i]->lDigit = ~m_segment.lDigit;
		}
		Display();

		switch (m_fsmKey.Get())
		{
		case C_INIT_TENKEY:
			m_segment.hDigit = m_segment.mDigit = m_segment.lDigit = 0;
			m_fsmKey.Set(C_KEY_PRESSED);
			break;

		case C_KEY_PRESSED:
			if (NO_KEY == nKeyPadNo || m_fsmBlink.IsBusy())
			{
				m_fsmKey.RstDelay();
				break;
			}

			if (!m_fsmKey.Delay(50))
				break;

			if (SET_KEY == nKeyPadNo)
			{
				nTenkeyNo = GetSegNo();
				m_fsmKey.Set(C_KEY_RELEASE);
				m_fsmBlink.Set(C_BLINK_START);
			}
			else if (CLR_KEY == nKeyPadNo)
			{
				nTenkeyNo = GetSegNo() + 1000;
				m_fsmKey.Set(C_KEY_RELEASE);
				m_fsmBlink.Set(C_BLINK_START);
			}
			else
			{
				if (m_isUnitofHundred)
				{
					m_segment.hDigit = 0;
					if (1 == m_segment.mDigit)
						m_segment.hDigit = 1;
				}

				m_segment.mDigit = m_segment.lDigit;
				m_segment.lDigit = nKeyPadNo;

				if (m_bRst)
					m_segment.hDigit = m_segment.mDigit = 0;

				m_bRst = FALSE;
				m_fsmKey.Set(C_KEY_RELEASE);
			}
			break;

		case C_KEY_RELEASE:
			if (NO_KEY != nKeyPadNo)
			{
				m_fsmKey.RstDelay();
			}
			else if (m_fsmKey.Delay(100))
			{
				m_fsmKey.Set(C_KEY_PRESSED);
			}
			break;

		case C_KEY_DISABLE:
			m_segment.hDigit = 0;
			m_segment.mDigit = m_segment.lDigit = 0xf;
			m_fsmKey.Set(C_IDLE);
			break;
		}

		return (nTenkeyNo);
	}

	void CTenkey::Enable(void)
	{
		m_fsmKey.Set(C_INIT_TENKEY);
	}

	void CTenkey::Disable(void)
	{
		m_fsmKey.Set(C_KEY_DISABLE);
	}

	void CTenkey::Display(void)
	{
		switch (m_fsmBlink.Get())
		{
		case C_BLINK_START:
			m_tmpSegment = m_segment;
			m_fsmBlink.Set(C_BLINK_OFF);
			break;

		case C_BLINK_OFF:
			m_segment.hDigit = 0;
			m_segment.mDigit = m_segment.lDigit = 0xf;

			if (FALSE == m_fsmBlink.Delay(300))
				break;

			m_fsmBlink.Set(C_BLINK_ON);
			break;

		case C_BLINK_ON:
			m_segment = m_tmpSegment;
			m_fsmBlink.Set(C_IDLE);
			break;
		}
	}

	int	CTenkey::GetKeyPadNo(void)
	{
		int nKeyPadNo = NO_KEY;
		BOOL isUsedKeyPad = FALSE;
		for (int i = 0; i < m_nCount; i++)
		{
			if (NO_KEY != GetKeyPadNo(i))
			{
				if (isUsedKeyPad)
					nKeyPadNo = NO_KEY;
				else
					nKeyPadNo = GetKeyPadNo(i);

				isUsedKeyPad = TRUE;
			}
		}
		return nKeyPadNo;
	}

	int	CTenkey::GetKeyPadNo(UINT nCount)
	{
		if (!Between(nCount, 0, MAX_TENKEY_CNT - 1))
		{
			throw CException(CException::OUT_OF_RANGE, __LINE__, __FUNCTION__);
		}

		int nRow = ConvHexToDec(m_pKeyPad[nCount]->row);
		int nCol = ConvHexToDec(m_pKeyPad[nCount]->col);

		if (NO_KEY == nRow || NO_KEY == nCol)
			return (NO_KEY);

		int nKeypadMask[_ROW_CNT_][_COL_CNT_] =
		{
			{1,       2,       3},
			{4,       5,       6},
			{7,       8,       9},
			{CLR_KEY, 0, SET_KEY}
		};

		int nRet = nKeypadMask[nRow][nCol];
		return (nRet);
	}

	int  CTenkey::ConvHexToDec(WORD wVal)
	{
		int nRet = NO_KEY;

		if (0x0001 == wVal)
		{
			nRet = 0;
		}
		else if (0x0002 == wVal)
		{
			nRet = 1;
		}
		else if (0x0004 == wVal)
		{
			nRet = 2;
		}
		else if (0x0008 == wVal)
		{
			nRet = 3;
		}

		return (nRet);
	}

	int CTenkey::GetSegNo(void)
	{
		int segmentVal = (m_segment.lDigit) + (m_segment.mDigit * 10);
		if (m_segment.hDigit)
			segmentVal = segmentVal + 100;

		return segmentVal;
	}
}