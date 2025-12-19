#include "..\Includes.h"

namespace HW
{
	CLampBuzzer::CLampBuzzer()
	{
		m_oBuzzer1 = (enDO)NOT_DEFINED;
		m_oBuzzer2 = (enDO)NOT_DEFINED;
		m_oBuzzer3 = (enDO)NOT_DEFINED;
		m_oBuzzer4 = (enDO)NOT_DEFINED;
		m_oBuzzer5 = (enDO)NOT_DEFINED;
	}

	CLampBuzzer::~CLampBuzzer()
	{

	}

	BOOL CLampBuzzer::Init(LampConfig* pConfig, enDO oLampG, enDO oLampY, enDO oLampR, enDO oBuzzer1,
		enDO oBuzzer2, enDO oBuzzer3, enDO oBuzzer4, enDO oBuzzer5)
	{
		m_pConfig = pConfig;
		m_shouldBuzzerOff = TRUE;

		m_oLampG = oLampG;
		m_oLampY = oLampY;
		m_oLampR = oLampR;

		m_oBuzzer1 = oBuzzer1;
		m_oBuzzer2 = oBuzzer2;
		m_oBuzzer3 = oBuzzer3;
		m_oBuzzer4 = oBuzzer4;
		m_oBuzzer5 = oBuzzer5;

		return (TRUE);
	}

	void CLampBuzzer::Run(void)
	{
		m_isLampOn = m_tmBlink.Blink(TRUE, 500, 500);

#ifdef _SAMSUNG_LAMP_
		m_pConfig->state[STOP].Red = LAMP_OFF;
		m_pConfig->state[STOP].Yellow = LAMP_ON;
		m_pConfig->state[STOP].Green = LAMP_OFF;
		m_pConfig->state[STOP].Buzzer = LAMP_OFF;

		m_pConfig->state[RUN].Red = LAMP_OFF;
		m_pConfig->state[RUN].Yellow = LAMP_OFF;
		m_pConfig->state[RUN].Green = LAMP_ON;
		m_pConfig->state[RUN].Buzzer = LAMP_OFF;

		m_pConfig->state[ERR].Red = LAMP_ON;
		m_pConfig->state[ERR].Yellow = LAMP_OFF;
		m_pConfig->state[ERR].Green = LAMP_OFF;
		m_pConfig->state[ERR].Buzzer = LAMP_ON;

		m_pConfig->state[CYCLE_RUN].Red = LAMP_ON;
		m_pConfig->state[CYCLE_RUN].Yellow = LAMP_OFF;
		m_pConfig->state[CYCLE_RUN].Green = LAMP_OFF;
		m_pConfig->state[CYCLE_RUN].Buzzer = LAMP_OFF;

		m_pConfig->state[UTIL_DOWN].Red = LAMP_ON;
		m_pConfig->state[UTIL_DOWN].Yellow = LAMP_OFF;
		m_pConfig->state[UTIL_DOWN].Green = LAMP_OFF;
		m_pConfig->state[UTIL_DOWN].Buzzer = LAMP_OFF;

		m_pConfig->state[EMPTY_FULL].Red = LAMP_OFF;
		m_pConfig->state[EMPTY_FULL].Yellow = LAMP_ON;
		m_pConfig->state[EMPTY_FULL].Green = LAMP_OFF;
		m_pConfig->state[EMPTY_FULL].Buzzer = LAMP_OFF;

		m_pConfig->state[ALARM].Red = LAMP_OFF;
		m_pConfig->state[ALARM].Yellow = LAMP_BLINK;
		m_pConfig->state[ALARM].Green = LAMP_ON;
		m_pConfig->state[ALARM].Buzzer = LAMP_OFF;

		m_pConfig->BuzzerOnTime = 3600;
		m_pConfig->isSkip = 0;
#endif

		int nCurState = GetState();

		if (m_oldState != nCurState)
		{
			m_tmBuzzer.Reset();
			m_oldState = nCurState;
			m_shouldBuzzerOff = FALSE;
		}

		if (m_pConfig->isSkip)
		{
			m_shouldBuzzerOff = TRUE;
		}

		if (LAMP_ON == m_pConfig->state[nCurState].Green)
			On(m_oLampG);
		else if (LAMP_OFF == m_pConfig->state[nCurState].Green)
			Off(m_oLampG);
		else
			Set(m_oLampG, m_isLampOn);

		if (LAMP_ON == m_pConfig->state[nCurState].Red)
			On(m_oLampR);
		else if (LAMP_OFF == m_pConfig->state[nCurState].Red)
			Off(m_oLampR);
		else
			Set(m_oLampR, m_isLampOn);

		if (LAMP_ON == m_pConfig->state[nCurState].Yellow)
			On(m_oLampY);
		else if (LAMP_OFF == m_pConfig->state[nCurState].Yellow)
			Off(m_oLampY);
		else
			Set(m_oLampY, m_isLampOn);

		if (m_shouldBuzzerOff)
		{
			m_tmBuzzer.Reset();
			BuzzerOnOff(0);
		}
		else
		{
			int buzzerOnTime = (int)(m_pConfig->BuzzerOnTime);
			BOOL isSkip = !!m_pConfig->isSkip;

			if (isSkip || (buzzerOnTime && m_tmBuzzer.TmOver(buzzerOnTime)))
				BuzzerOnOff(0);
			else
				BuzzerOnOff((int)m_pConfig->state[nCurState].Buzzer);
		}
	}

	int CLampBuzzer::GetState(void)
	{
		int nType = STOP;

		if (OPR.isAuto)
		{
			if (0 < WR.GetNo())
				nType = ALARM;
			else
				nType = RUN;
		}
		else
		{
			if (OPR.isCycleRun)
			{
				nType = CYCLE_RUN;
			}
			else if (0 < ER.GetNo())
			{
				nType = ERR;
			}
			else
			{
				nType = STOP;
			}
		}

		return nType;
	}

	void CLampBuzzer::BuzzerOnOff(int nBuzzNo)
	{
		if (NOT_DEFINED != m_oBuzzer1)
			Set(m_oBuzzer1, 1 == nBuzzNo);
		if (NOT_DEFINED != m_oBuzzer2)
			Set(m_oBuzzer2, 2 == nBuzzNo);
		if (NOT_DEFINED != m_oBuzzer3)
			Set(m_oBuzzer3, 3 == nBuzzNo);
		if (NOT_DEFINED != m_oBuzzer4)
			Set(m_oBuzzer4, 4 == nBuzzNo);
		if (NOT_DEFINED != m_oBuzzer5)
			Set(m_oBuzzer5, 5 == nBuzzNo);
	}

	void CLampBuzzer::BuzzerOff(void)
	{
		m_shouldBuzzerOff = TRUE;
	}

	BOOL CLampBuzzer::GetBlink(void)
	{
		return (m_isLampOn);
	}
}

HW::CLampBuzzer LAMPBUZZER;