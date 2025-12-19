#include "..\Includes.h"

namespace COM
{
	CMsgNotice::CMsgNotice()
	{
		MsgClose();
	}

	CMsgNotice::~CMsgNotice()
	{

	}

	bool CMsgNotice::IsRdy()
	{
		bool MmiReady;
		MmiReady = (RECEIVE_CMD_POP != NV.ndm[msgMmi2Seq]);
		MmiReady &= (RECEIVE_CMD_BTN1 != NV.ndm[msgMmi2Seq]);
		MmiReady &= (RECEIVE_CMD_BTN2 != NV.ndm[msgMmi2Seq]);
		MmiReady &= (RECEIVE_CMD_BTN3 != NV.ndm[msgMmi2Seq]);
		MmiReady &= (RECEIVE_CMD_BTN4 != NV.ndm[msgMmi2Seq]);
		MmiReady &= (RECEIVE_CMD_CANCEL != NV.ndm[msgMmi2Seq]);

		if (MmiReady && Between(NV.ndm[msgSeq2Mmi], SEND_CMD_READY, SEND_CMD_CLOSE9))
			return (TRUE);

		return	(FALSE);
	}

	void CMsgNotice::Set(int MsgNo, const char* btn1, const char* btn2, const char* btn3, const char* btn4, const char* lpszTitle, ...)
	{
		va_list fmtList;
		char string[BUFF_LENGTH];
		char strBuffer[BUFF_LENGTH];
		ZeroMemory(strBuffer, sizeof(strBuffer));
		strcat(strBuffer, lpszTitle);
		va_start(fmtList, lpszTitle);

		vsprintf(string, strBuffer, fmtList);
		va_end(fmtList);

		if (MsgNo < MSG_NOTICE)
		{
			MsgClose();
			return;
		}

		if (MSG_CANMOVE == MsgNo)
		{
			if (!OPR.isCanMoveMsg)
				return;
		}

		if (!IsRdy())
			return;

		::memset(&m_strMsgTitle, NULL, sizeof(m_strMsgTitle));
		::memset(&m_strMsgBtn, NULL, sizeof(m_strMsgBtn));

		if (NULL != string)
			::memcpy(&m_strMsgTitle, string, sizeof(m_strMsgTitle));
		if (NULL != btn1)
			::memcpy(&m_strMsgBtn[0], btn1, sizeof(m_strMsgBtn[0]));
		if (NULL != btn2)
			::memcpy(&m_strMsgBtn[1], btn2, sizeof(m_strMsgBtn[1]));
		if (NULL != btn3)
			::memcpy(&m_strMsgBtn[2], btn3, sizeof(m_strMsgBtn[2]));
		if (NULL != btn4)
			::memcpy(&m_strMsgBtn[3], btn4, sizeof(m_strMsgBtn[3]));

		NV.ndm[msgSeq2Mmi] = MsgNo;
		NV.ndm[msgMmi2Seq] = RECEIVE_CMD_READY;

		m_tmMsgNoticeTime.Reset();
	}

	void CMsgNotice::MsgClose()
	{
		::memset(&m_strMsgTitle, NULL, sizeof(m_strMsgTitle));
		::memset(&m_strMsgBtn, NULL, sizeof(m_strMsgBtn));

		NV.ndm[msgSeq2Mmi] = SEND_CMD_READY;
		NV.ndm[msgMmi2Seq] = RECEIVE_CMD_READY;

		m_tmMsgNoticeTime.Reset();
	}

	bool CMsgNotice::TimeOver(LONG tmDelay)
	{
		bool waitState = (MSG_NOTICE <= NV.ndm[msgSeq2Mmi]);
		waitState &= (RECEIVE_CMD_POP != NV.ndm[msgMmi2Seq]);
		waitState &= (RECEIVE_CMD_BTN1 != NV.ndm[msgMmi2Seq]);
		waitState &= (RECEIVE_CMD_BTN2 != NV.ndm[msgMmi2Seq]);
		waitState &= (RECEIVE_CMD_BTN3 != NV.ndm[msgMmi2Seq]);
		waitState &= (RECEIVE_CMD_BTN4 != NV.ndm[msgMmi2Seq]);
		waitState &= (RECEIVE_CMD_CANCEL != NV.ndm[msgMmi2Seq]);
		waitState &= (!!NV.ndm[msgMmi2Seq]);

		if (!waitState)
		{
			m_tmMsgNoticeTime.Reset();
		}
		else if (m_tmMsgNoticeTime.TmOver(tmDelay))
		{
			ER.Save(ER_MSG_POP_TIMEOUT);
			m_tmMsgNoticeTime.Reset();
			return (TRUE);
		}

		return (FALSE);
	}

	void CMsgNotice::Update()
	{
		if (RECEIVE_CMD_POP == NV.ndm[msgMmi2Seq])
			return;

		if (RECEIVE_CMD_CANCEL == NV.ndm[msgMmi2Seq])
		{
			MsgClose();
			return;
		}

		if (TimeOver(30000))
		{
			MsgClose();
			return;
		}
			
		NoticeProcess();
			
		if (Between(NV.ndm[msgMmi2Seq], RECEIVE_CMD_BTN1, RECEIVE_CMD_BTN4))
		{
			MsgClose();
		}
	}
}