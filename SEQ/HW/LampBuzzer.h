#pragma once
#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"

#define LAMP_TYPE_MAX 7

namespace HW
{
    typedef struct _EachConfig
    {
        UINT64 Red;
        UINT64 Yellow;
        UINT64 Green;
        UINT64 Buzzer;
    }EachConfig;

    typedef struct _LampConfig
    {
        EachConfig	state[LAMP_TYPE_MAX];
        UINT64	    isSkip;
        UINT64	    BuzzerOnTime;
    }LampConfig;

    class CLampBuzzer
    {
    private:
        //-------------------------------------------------------------------
        // Buzzer : 0(Off), 1~4(On)
        // Lamp   : 0(Off), 1(On), 2(Blink)
        enum Mode
        {
            STOP            = 0,
            RUN             = 1,
            ERR             = 2,
            CYCLE_RUN       = 3,
            UTIL_DOWN       = 4,
            EMPTY_FULL      = 5,
            ALARM           = 6, // warning
        };

        enum LampState
        {
            LAMP_OFF             = 0,
            LAMP_ON              = 1,
            LAMP_BLINK           = 2,
        };

        LampConfig*     m_pConfig;
        CBlinkTimer		m_tmBlink;

        int				m_oldState;

        enDO            m_oLampG;
        enDO            m_oLampY;
        enDO            m_oLampR;

        enDO            m_oBuzzer1;
        enDO            m_oBuzzer2;
        enDO            m_oBuzzer3;
        enDO            m_oBuzzer4;
        enDO            m_oBuzzer5;

        BOOL			m_isLampOn;

        int  GetState(void);
        void BuzzerOnOff(int nBuzzNo);

    public:
        BOOL			m_shouldBuzzerOff;
        CTimer			m_tmBuzzer;

        BOOL Init(LampConfig* pConfig, enDO oLampG, enDO oLampY, enDO oLampR, enDO oBuzzer1 = (enDO)NOT_DEFINED,
            enDO oBuzzer2 = (enDO)NOT_DEFINED, enDO oBuzzer3 = (enDO)NOT_DEFINED,
            enDO oBuzzer4 = (enDO)NOT_DEFINED, enDO oBuzzer5 = (enDO)NOT_DEFINED);
        void Run(void);
        void BuzzerOff(void);
        BOOL GetBlink(void);

        CLampBuzzer();
        virtual ~CLampBuzzer();
    };
}

extern HW::CLampBuzzer LAMPBUZZER;