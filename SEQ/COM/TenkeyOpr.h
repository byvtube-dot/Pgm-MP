#pragma once
#include "..\COM\FSM.h"
#include "..\HW\Tenkey.h"

namespace COM
{
	class CTenkeyOpr
	{
	public:
		enum JogCmd
		{
			C_JOGWAIT = 100,
			C_JOGMOVE_N,
			C_JOGMOVE_P,
			C_JOGSTOP,
		};

	public:
		CTenkeyOpr() {}
		virtual ~CTenkeyOpr() {}

		CFSM m_fsmJog;

		void Init(PUSHORT pInAddr, PUSHORT pOutAddr);
		void SetUnitofHundred(BOOL isUnitofHundred);
		int GetTenkeyNo(void);
		void Jog(void);

	private:
		HW::CTenkey			m_tenkey;
		BOOL				m_isDisable;

	};
}
