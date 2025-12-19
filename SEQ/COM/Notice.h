#pragma once
#include "..\COM\FSM.h"

#define BUFF_LENGTH		512

namespace COM
{
	class CMsgNotice
	{
	public:
		char	m_strMsgTitle[200];
		char	m_strMsgBtn[4][10];
		CTimer  m_tmMsgNoticeTime;

		bool IsRdy();
		void Set(int MsgNo, const char* btn1, const char* btn2, const char* btn3, const char* btn4, const char* lpszTitle, ...);
		void MsgClose();
		bool TimeOver(LONG tmDelay);
		void Update();

		CMsgNotice();
		virtual ~CMsgNotice();
	};
}

