#include "..\Includes.h"

void ManualSequence()
{
	int tenkeyNo = TENKEYOPR.GetTenkeyNo();
	if (NOT_DEFINED == tenkeyNo)
		return;

	OPR.isCanMoveMsg = TRUE;
	switch (tenkeyNo)
	{
		case HOME(1):
		{
			CYL[cylInPkOc].Open();
			break;
		}
		case STEP(1):
		{
			CYL[cylInPkOc].Close();
			break;
		}
		case HOME(2):
		{
            CYL[cylBtmFoamTableUd].Dn();
            break;
		}
		case STEP(2):
        {
            CYL[cylBtmFoamTableUd].Up();
            break;
        }
        case HOME(3):
        {
            CYL[cylBtmFoamStopUd].Up();
            break;
        }
        case STEP(3):
        {
            CYL[cylBtmFoamStopUd].Dn();
            break;
        }
        case HOME(4):
        {
            CYL[cylBtmFoamFtClampOc].Open();
            CYL[cylBtmFoamRrClampOc].Open();
            break;
        }
        case STEP(4):
        {
            CYL[cylBtmFoamFtClampOc].Close();
            CYL[cylBtmFoamRrClampOc].Close();
            break;
        }
        case HOME(5):
        {
            CYL[cylTopFoamStopUd].Up();
            break;            
        }
        case STEP(5):
        {
            CYL[cylTopFoamStopUd].Dn();
            break;
        }
        case HOME(6):
        {
            CYL[cylTrayShuttleNozzleUd].Dn();
            break;
        }
        case STEP(6):
        {
            CYL[cylTrayShuttleNozzleUd].Up();
            break;
        }
        case HOME(7):
        {
            CYL[cylTrayShuttleBridgeUd].Dn();
            break;
        }
        case STEP(7):
        {
            CYL[cylTrayShuttleBridgeUd].Up();
            break;
        }
        case HOME(8):
        {
            CYL[cylTrayShuttleBridgeFb].Bwd();
            break;
        }
        case STEP(8):
        {
            CYL[cylTrayShuttleBridgeFb].Fwd();
            break;
        }
        case HOME(9):
        {
            CYL[cylTrayShuttleFtStopFb].Fwd();
            CYL[cylTrayShuttleRrStopFb].Fwd();
            break;
        }
        case STEP(9):
        {
            CYL[cylTrayShuttleFtStopFb].Bwd();
            CYL[cylTrayShuttleRrStopFb].Bwd();
            break;
        }
        case HOME(10):
        {
            CYL[cylTrayShuttleFtMaskUd].Up();
            CYL[cylTrayShuttleRrMaskUd].Up();
            break;
        }
        case STEP(10):
        {
            CYL[cylTrayShuttleFtMaskUd].Dn();
            CYL[cylTrayShuttleRrMaskUd].Dn();
            break;
        }
        case HOME(11):
        {
            break;
        }
        case STEP(11):
        {
            break;
        }
        case HOME(12):
        {
            CYL[cylMbbOpenTopGuideFb].Bwd();
            break;
        }
        case STEP(12):
        {
            CYL[cylMbbOpenTopGuideFb].Fwd();
            break;
        }
        case HOME(13):
        {
            CYL[cylMbbOpenTopVacUd].Up();
            break;
        }
        case STEP(13):
        {
            CYL[cylMbbOpenTopVacUd].Dn();
            break;
        }
        case HOME(14):
        {
            CYL[cylMbbOpenTopClampUd].Up();
            break;
        }
        case STEP(14):
        {
            CYL[cylMbbOpenTopClampUd].Dn();
            break;
        }
        case HOME(15):
        {
            CYL[cylSealNozzleFb].Bwd();
            break;
        }
        case STEP(15):
        {
            CYL[cylSealNozzleFb].Fwd();
            break;
        }
        case HOME(16):
        {
            CYL[cylSealMbbOpenGuideFtOc].Open();
            CYL[cylSealMbbOpenGuideRrOc].Open();
            break;
        }
        case STEP(16):
        {
            CYL[cylSealMbbOpenGuideFtOc].Close();
            CYL[cylSealMbbOpenGuideRrOc].Close();
            break;
        }
        case HOME(17):
        {
            CYL[cylSealNozzleMfcN2Oc].Open();
            break;
        }
        case STEP(17):
        {
            CYL[cylSealNozzleMfcN2Oc].Close();
            break;
        }
        case HOME(18):
        {
            CYL[cylSealBtmGuideUd].Dn();
            break;
        }
        case STEP(18):
        {
            CYL[cylSealBtmGuideUd].Up();
            break;
        }
        case HOME(19):
        {
            CYL[cylSealTopGuideUd].Up();
            break;
        }
        case STEP(19):
        {
            CYL[cylSealTopGuideUd].Dn();
            break;
        }
        case HOME(20):
        {
            CYL[cylSealerBtmUd].Dn();
            break;
        }
        case STEP(20):
        {
            CYL[cylSealerBtmUd].Up();
            break;
        }
        case HOME(21):
        {
            CYL[cylSealerTopFtUd].Up();
            CYL[cylSealerTopRrUd].Up();
            break;
        }
        case STEP(21):
        {
            CYL[cylSealerTopFtUd].Dn();
            CYL[cylSealerTopRrUd].Dn();
            break;
        }
        case HOME(22): //
        {
            CYL[cylLtCobotLabelPkUd].Up();
            break;
        }
        case STEP(22):
        {
            CYL[cylLtCobotLabelPkUd].Dn();
            break;
        }
        case HOME(23):
        {
            CYL[cylLtCobotLabelPkTurnFb].Fwd();
            break;
        }
        case STEP(23):
        {
            CYL[cylLtCobotLabelPkTurnFb].Bwd();
            break;
        }
        case HOME(24):
        {
            CYL[cylFoamRecvElevLtAlignFb].Bwd();
            break;
        }
        case STEP(24):
        {
            CYL[cylFoamRecvElevLtAlignFb].Fwd();
            break;
        }
        case HOME(25):
        {
            CYL[cylFoamRecvElevRrAlignFb].Bwd();
            break;
        }
        case STEP(25):
        {
            CYL[cylFoamRecvElevRrAlignFb].Fwd();
            break;
        }
        case HOME(26):
        {
            CYL[cylFoamRecvElevLtStopFb].Fwd();
            CYL[cylFoamRecvElevRtStopFb].Fwd();
            break;
        }
        case STEP(26):
        {
            CYL[cylFoamRecvElevLtStopFb].Bwd();
            CYL[cylFoamRecvElevRtStopFb].Bwd();
            break;
        }
        case HOME(27):
        {
            CYL[cylFoamRrStackLiftUd].Dn();
            break;
        }
        case STEP(27):
        {
            CYL[cylFoamRrStackLiftUd].Up();
            break;
        }
        case HOME(28):
        {
            CYL[cylFoamRrStackLtStopFb].Fwd();
            CYL[cylFoamRrStackRtStopFb].Fwd();
            break;
        }
        case STEP(28):
        {
            CYL[cylFoamRrStackLtStopFb].Bwd();
            CYL[cylFoamRrStackRtStopFb].Bwd();
            break;
        }
        case HOME(29):
        {
            CYL[cylFoamFtStackLiftUd].Dn();
            break;
        }
        case STEP(29):
        {
            CYL[cylFoamFtStackLiftUd].Up();
            break;
        }
        case HOME(30):
        {
            CYL[cylFoamFtStackLtStopFb].Fwd();
            CYL[cylFoamFtStackRtStopFb].Fwd();
            break;
        }
        case STEP(30):
        {
            CYL[cylFoamFtStackLtStopFb].Bwd();
            CYL[cylFoamFtStackRtStopFb].Bwd();
            break;
        }
        case HOME(31):
        {
            CYL[cylFoamSendElevLtAlignFb].Bwd();
            break;
        }
        case STEP(31):
        {
            CYL[cylFoamSendElevLtAlignFb].Fwd();
            break;
        }
        case HOME(32):
        {
            CYL[cylFoamSendElevRtAlignFb].Bwd();
            break;
        }
        case STEP(32):
        {
            CYL[cylFoamSendElevRtAlignFb].Fwd();
            break;
        }
        case HOME(33):
        {
            CYL[cylFoamPartialElevLtAlignFb].Bwd();
            break;
        }
        case STEP(33):
        {
            CYL[cylFoamPartialElevLtAlignFb].Fwd();
            break;
        }
        case HOME(34):
        {
            CYL[cylFoamPartialElevRrAlignFb].Bwd();
            break;
        }
        case STEP(34):
        {
            CYL[cylFoamPartialElevRrAlignFb].Fwd();
            break;
        }
        case HOME(35):
        {
            CYL[cylMbbShuttleAlignFtFb].Bwd();
            CYL[cylMbbShuttleAlignRrFb].Bwd();
            break;
        }
        case STEP(35):
        {
            CYL[cylMbbShuttleAlignFtFb].Fwd();
            CYL[cylMbbShuttleAlignRrFb].Fwd();
            break;
        }
        case HOME(36):
        {
            CYL[cylMbbShuttleClampFtFb].Bwd();
            CYL[cylMbbShuttleClampRrFb].Bwd();
            break;
        }
        case STEP(36):
        {
            CYL[cylMbbShuttleClampFtFb].Fwd();
            CYL[cylMbbShuttleClampRrFb].Fwd();
            break;
        }
        case HOME(37):
        {
            CYL[cylQcSealedHeadBtmFtVacUd].Dn();
            CYL[cylQcSealedHeadBtmRrVacUd].Dn();
            break;
        }
        case STEP(37):
        {
            CYL[cylQcSealedHeadBtmFtVacUd].Up();
            CYL[cylQcSealedHeadBtmRrVacUd].Up();
            break;
        }
        case HOME(38):
        {
            CYL[cylQcSealedHeadTopFtClampUd].Up();
            CYL[cylQcSealedHeadTopRrClampUd].Up();
            break;
        }
        case STEP(38):
        {
            CYL[cylQcSealedHeadTopFtClampUd].Dn();
            CYL[cylQcSealedHeadTopRrClampUd].Dn();
            break;
        }
        case HOME(39):
        {
            CYL[cylQcSealedTrayStopUd].Up();
            break;
        }
        case STEP(39):
        {
            CYL[cylQcSealedTrayStopUd].Dn();
            break;
        }
        case HOME(40):
        {
            CYL[cylQcSealedTrayAlignFtFb].Bwd();
            CYL[cylQcSealedTrayAlignRrFb].Bwd();
            break;
        }
        case STEP(40):
        {
            CYL[cylQcSealedTrayAlignFtFb].Fwd();
            CYL[cylQcSealedTrayAlignRrFb].Fwd();
            break;
        }
        case HOME(41):
        {
            CYL[cylMbbFoldAssistUd].Dn();
            break;
        }
        case STEP(41):
        {
            CYL[cylMbbFoldAssistUd].Up();
            break;
        }
        case HOME(42):
        {
            CYL[cylMbbFoldPkLtBlockTurnFb].Bwd();
            break;
        }
        case STEP(42):
        {
            CYL[cylMbbFoldPkLtBlockTurnFb].Fwd();
            break;
        }
        case HOME(43):
        {
            CYL[cylMbbFoldAlignFtFb].Bwd();
            CYL[cylMbbFoldAlignRrFb].Bwd();
            break;
        }
        case STEP(43):
        { // TODO: RENAME
            CYL[cylMbbFoldAlignFtFb].Fwd();
            CYL[cylMbbFoldAlignRrFb].Fwd();
            break;
        }
        case HOME(44):
        {
            CYL[cylMbbFoldRtFb].Bwd();
            break;
        }
        case STEP(44):
        {
            CYL[cylMbbFoldRtFb].Fwd();
            break;
        }
        case HOME(45):
        {
            CYL[cylMbbFoldRtFtOc].Open();
            CYL[cylMbbFoldRtRrOc].Open();
            break;
        }
        case STEP(45):
        {
            CYL[cylMbbFoldRtFtOc].Close();
            CYL[cylMbbFoldRtRrOc].Close();
            break;
        }
        case HOME(46):
        {
            CYL[cylMbbFoldPkLtBlockFtOc].Open();
            CYL[cylMbbFoldPkLtBlockRrOc].Open();
            break;
        }
        case STEP(46):
        {
            CYL[cylMbbFoldPkLtBlockFtOc].Close();
            CYL[cylMbbFoldPkLtBlockRrOc].Close();
            break;
        }
        case HOME(47):
        {
            CYL[cylMbbFoldPkClampOc].Open();
            break;
        }
        case STEP(47):
        {
            CYL[cylMbbFoldPkClampOc].Close();
            break;
        }
        case HOME(48):
        {
            CYL[cylLtLabelFeederMountFb].Bwd();
            CYL[cylLtLabelFeederFb].Bwd();
            break;
        }
        case STEP(48):
        {
            CYL[cylLtLabelFeederFb].Fwd();
            break;
        }
        case HOME(49):
        {
            CYL[cylLtLabelFeederMountFb].Bwd();
            break;
        }
        case STEP(49):
        {
            if (!LABEL_FEEDER[enlabelLt].FeederTrqZeroStop())
                break;

            CYL[cylLtLabelFeederMountFb].Fwd();
            break;
        }
        case HOME(50):
        {
            CYL[cylLtLabelFeederLockFtFb].Bwd();
            CYL[cylLtLabelFeederLockRrFb].Bwd();
            break;
        }
        case STEP(50):
        {
            CYL[cylLtLabelFeederLockFtFb].Fwd();
            CYL[cylLtLabelFeederLockRrFb].Fwd();
            break;
        }
        case HOME(51):
        {
            CYL[cylLtLabelRecvUd].Dn();
            break;
        }
        case STEP(51):
        {
            CYL[cylLtLabelRecvUd].Up();
            break;
        }
        case HOME(52):
        {
            CYL[cylLtLabelRecvFb].Bwd();
            break;
        }
        case STEP(52):
        {
            CYL[cylLtLabelRecvFb].Fwd();
            break;
        }
        case HOME(53):
        {
            CYL[cylLtLabelRecvEjectPinLtUd].Dn();
            CYL[cylLtLabelRecvEjectPinRtUd].Dn();
            break;
        }
        case STEP(53):
        {
            CYL[cylLtLabelRecvEjectPinLtUd].Up();
            CYL[cylLtLabelRecvEjectPinRtUd].Up();
            break;
        }
        case HOME(54):
        {
            CYL[cylRtLabelFeederMountFb].Bwd();

            CYL[cylRtLabelFeederFb].Bwd();
            break;
        }
        case STEP(54):
        {
            CYL[cylRtLabelFeederFb].Fwd();
            break;
        }
        case HOME(55):
        {
            CYL[cylRtLabelFeederMountFb].Bwd();
            break;
        }
        case STEP(55):
        {
            if (!LABEL_FEEDER[enlabelRt].FeederTrqZeroStop())
                break;

            CYL[cylRtLabelFeederMountFb].Fwd();
            break;
        }
        case HOME(56):
        {
            CYL[cylRtLabelFeederLockFtFb].Bwd();
            CYL[cylRtLabelFeederLockRrFb].Bwd();
            break;
        }
        case STEP(56):
        {
            CYL[cylRtLabelFeederLockFtFb].Fwd();
            CYL[cylRtLabelFeederLockRrFb].Fwd();
            break;
        }
        case HOME(57):
        {
            CYL[cylRtLabelRecvUd].Dn();
            break;
        }
        case STEP(57):
        {
            CYL[cylRtLabelRecvUd].Up();
            break;
        }
        case HOME(58):
        {
            CYL[cylRtLabelRecvFb].Bwd();
            break;
        }
        case STEP(58):
        {
            CYL[cylRtLabelRecvFb].Fwd();
            break;
        }
        case HOME(59):
        {
            CYL[cylRtLabelRecvEjectPinLtUd].Dn();
            CYL[cylRtLabelRecvEjectPinRtUd].Dn();
            break;
        }
        case STEP(59):
        {
            CYL[cylRtLabelRecvEjectPinLtUd].Up();
            CYL[cylRtLabelRecvEjectPinRtUd].Up();
            break;
        }
        case HOME(60):
        {
            CYL[cylPowerMeterOc].Open();
            break;
        }
        case STEP(60):
        {
            CYL[cylPowerMeterOc].Close();
            break;
        }
        case HOME(61):
        {
            CYL[cylLabelQcViFb].Fwd();
            break;
        }
        case STEP(61):
        {
            CYL[cylLabelQcViFb].Bwd();
            break;
        }
        case HOME(62):
        {
            CYL[cylRtCobotLabelPkUd].Up();
            break;
        }
        case STEP(62):
        {
            CYL[cylRtCobotLabelPkUd].Dn();
            break;
        }
        case HOME(63):
        {
            CYL[cylRtCobotLabelPkTurnFb].Fwd();
            break;
        }
        case STEP(63):
        {
            CYL[cylRtCobotLabelPkTurnFb].Bwd();
            break;
        }
        case HOME(64):
        {
            if (!MT[mtMbbLtElevZ].InPos(P_ELEV_Z_BTM))
            {
                MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
                    "[%02d : %s]\n Not Safety Position.",
                    MT[mtMbbLtElevZ].m_no, MT[mtMbbLtElevZ].m_name.c_str());
                break;
            }
            CYL[cylLtMbbCstFb].Bwd();
            break;
        }
        case STEP(64):
        {
            if (!MT[mtMbbLtElevZ].InPos(P_ELEV_Z_BTM))
            {
                MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
                    "[%02d : %s]\n Not Safety Position.",
                    MT[mtMbbLtElevZ].m_no, MT[mtMbbLtElevZ].m_name.c_str());
                break;
            }
            CYL[cylLtMbbCstFb].Fwd();
            break;
        }
        case HOME(65):
        {
            if (!MT[mtMbbRtElevZ].InPos(P_ELEV_Z_BTM))
            {
                MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
                    "[%02d : %s]\n Not Safety Position.",
                    MT[mtMbbRtElevZ].m_no, MT[mtMbbRtElevZ].m_name.c_str());
                break;
            }
            CYL[cylRtMbbCstFb].Bwd();
            break;
        }
        case STEP(65):
        {
            if (!MT[mtMbbRtElevZ].InPos(P_ELEV_Z_BTM))
            {
                MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
                    "[%02d : %s]\n Not Safety Position.",
                    MT[mtMbbRtElevZ].m_no, MT[mtMbbRtElevZ].m_name.c_str());
                break;
            }
            CYL[cylRtMbbCstFb].Fwd();
            break;
        }
        case HOME(66):
        {
            CYL[cylMbbFoldPkLtFtAssist].Bwd();
            CYL[cylMbbFoldPkLtRrAssist].Bwd();
            break;
        }
        case STEP(66):
        {
            CYL[cylMbbFoldPkLtFtAssist].Fwd();
            CYL[cylMbbFoldPkLtRrAssist].Fwd();
            break;
        }
        case HOME(67):
        {
            break;
        }
        case STEP(67):
        {
            break;
        }
        case HOME(68):
        {
            break;
        }
        case STEP(68):
        {
            break;
        }
        case HOME(69):
        {
            break;
        }
        case STEP(69):
        {
            break;
        }
        case HOME(70):
        {
            VC[vcMbbOpenBtm].BlowOn();
            break;
        }
        case STEP(70):
        {
            VC[vcMbbOpenBtm].On();
            break;
        }
        case HOME(71):
        {
            VC[vcMbbOpenTop].BlowOn();
            VC[vcMbbOpenTop2].BlowOn();
            break;
        }
        case STEP(71):
        {
            VC[vcMbbOpenTop].On();
            VC[vcMbbOpenTop2].On();
            break;
        }
        case HOME(72):
        {
            CYL[cylSealNozzleMfcN2Oc].Open();
            VC[vcSealNozzle].BlowOn();
            break;
        }
        case STEP(72):
        {
            CYL[cylSealNozzleMfcN2Oc].Close();
            VC[vcSealNozzle].On();
            break;
        }
        case HOME(73):
        {
            CYL[cylSealNozzleMfcN2Oc].Open();
            VC[vcSealNozzleN2].BlowOn();
            break;
        }
        case STEP(73):
        {
            CYL[cylSealNozzleMfcN2Oc].Open();
            VC[vcSealNozzleN2].On();
            break;
        }
        case HOME(74):
        {
            VC[vcLtCobotLabel].BlowOn();
            break;
        }
        case STEP(74):
        {
            VC[vcLtCobotLabel].On();
            break;
        }
        case HOME(75):
        {
            VC[vcLtCobotFoam].BlowOn();
            VC[vcLtCobotFoam2].BlowOn();
            break;
        }
        case STEP(75):
        {
            VC[vcLtCobotFoam].On();
            VC[vcLtCobotFoam2].On();
            break;
        }
        case HOME(76):
        {
            VC[vcFoamFlip].BlowOn();
            VC[vcFoamFlip2].BlowOn();
            break;
        }
        case STEP(76):
        {
            VC[vcFoamFlip].On();
            VC[vcFoamFlip2].On();
            break;
        }
        case HOME(77):
        {
            VC[vcFoamPk].BlowOn();
            VC[vcFoamPk2].BlowOn();
            break;
        }
        case STEP(77):
        {
            VC[vcFoamPk].On();
            VC[vcFoamPk2].On();
            break;
        }
        case HOME(78):
        {
            VC[vcQcSealedHead].BlowOn();
            break;
        }
        case STEP(78):
        {
            VC[vcQcSealedHead].On();
            break;
        }
        case HOME(79):
        {
            VC[vcLtLabelRecv].BlowOn();
            break;
        }
        case STEP(79):
        {
            VC[vcLtLabelRecv].On();
            break;
        }
        case HOME(80):
        {
            VC[vcRtLabelRecv].BlowOn();
            break;
        }
        case STEP(80):
        {
            VC[vcRtLabelRecv].On();
            break;
        }
        case HOME(81):
        {
            VC[vcRtCobotLabel].BlowOn();
            break;
        }
        case STEP(81):
        {
            VC[vcRtCobotLabel].On();
            break;
        }
        case HOME(82):
        {
            VC[vcRtCobotMbb].BlowOn();
            VC[vcRtCobotMbb2].BlowOn();
            break;
        }
        case STEP(82):
        {
            VC[vcRtCobotMbb].On();
            VC[vcRtCobotMbb2].On();
            break;
        }
        case HOME(83):
        {
            break;
        }
        case STEP(83):
        {
            break;
        }
        case HOME(84):
        {
            break;
        }
        case STEP(84):
        {
            break;
        }
        case HOME(85):
        {
            break;
        }
        case STEP(85):
        {
            break;
        }
        case HOME(86):
        {
            break;
        }
        case STEP(86):
        {
            break;
        }
        case HOME(87):
        {
            break;
        }
        case STEP(87):
        {
            break;
        }
        case HOME(88):
        {
            break;
        }
        case STEP(88):
        {
            break;
        }
        case HOME(89):
        {
            break;
        }
        case STEP(89):
        {
            break;
        }
        case HOME(90):
        {
            break;
        }
        case STEP(90):
        {
            break;
        }
        case HOME(91):
        {
            CYL[cylSealMbbClampUd].Up();
            break;
        }
        case STEP(91):
        {
            CYL[cylSealMbbClampUd].Dn();
            break;
        }
        case HOME(92):
        {
            CYL[cylSealMbbClampFtFb].Bwd();
            CYL[cylSealMbbClampRrFb].Bwd();
            break;
        }
        case STEP(92):
        {
            CYL[cylSealMbbClampFtFb].Fwd();
            CYL[cylSealMbbClampRrFb].Fwd();
            break;
        }
        case HOME(93):
        {
            CYL[cylSealMbbClampFtOc].Open();
            CYL[cylSealMbbClampRrOc].Open();
            break;
        }
        case STEP(93):
        {
            CYL[cylSealMbbClampFtOc].Close();
            CYL[cylSealMbbClampRrOc].Close();
            break;
        }
        case HOME(94):
        {
            CYL[cylMbbShuttleClampUd].Up();
            break;
        }
        case STEP(94):
        {
            CYL[cylMbbShuttleClampUd].Dn();
            break;
        }
        case HOME(95):
        {
            break;
        }
        case STEP(95):
        {
            break;
        }
        case HOME(96):
        {
            break;
        }
        case STEP(96):
        {
            break;
        }
        case HOME(97):
        {
            break;
        }
        case STEP(97):
        {
            break;
        }
        case HOME(98):
        {
            OPR.isDryRun = FALSE;
            break;
        }
        case STEP(98):
        {
            OPR.isDryRun = TRUE;
            break;
        }


		case HOME(99):
		{
			LOG[logSEQ].Message("Tenkey 99 + Home");
			for (int mtno = 0; mtno < motorMAX; mtno++)
			{
				//if (MT[mtno].m_DummyMotor) continue;
				MT[AxisList(mtno)].CancelHomeSearch();
			}

			OPR.isAllHome = FALSE;

			if (!FSM[fsmSEQ].IsBusy())
				FSM[fsmSEQ].Set(C_ALLHOME_START);

			NV.use[useMasterKeyMode] = FALSE;
			break;
		}

        case STEP(101):
        {
            LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExistPrinted;
            if (!LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelRecv())
                break;

            LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_RECV_START);
            break;
        }
        case HOME(101):
        {
            if (!LABEL_SHUTTLE[enlabelLt].Exist())
                MtPosMove(mtLabelLtShuttleY, P_LABEL_SHUTTLE_Y_RECV);
            //LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExistPrinted;
            LABEL_SHUTTLE[enlabelLt].Exist() = false;
            LABEL_SHUTTLE[enlabelLt].m_pVacLabelRecv->BlowOn();
            break;
        }
        case STEP(102):
        {
            if (!LABEL_SHUTTLE[enlabelRt].IsCanCycleLabelRecv())
                break;

            LABEL_SHUTTLE[enlabelRt].m_pFsm->Set(LABEL_SHUTTLE[enlabelRt].C_LABEL_RECV_START);
            break;
        }
        case HOME(102):
        {
            if (!LABEL_SHUTTLE[enlabelRt].Exist())
                MtPosMove(mtLabelRtShuttleY, P_LABEL_SHUTTLE_Y_RECV);
            LABEL_PART_DATA[nvLabelRtFeeder].exist = enLabelExistPrinted;
            LABEL_SHUTTLE[enlabelRt].Exist() = false;
            LABEL_SHUTTLE[enlabelRt].m_pVacLabelRecv->BlowOn();
            break;
        }
        //case HOME(102):
        //{
        //    if (!LABEL_LASER.IsCanCyclePrs())
        //        break;

        //    LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PRS_VI_START, nvLabelLtFeeder);
        //    break;
        //}
        case HOME(103):
        {
            LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExistPrs;
            if (!LABEL_LASER.IsCanCycleLaser())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_START, nvLabelLtFeeder);
            break;
        }
        case HOME(104):
        {
            if (!LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelQc())
                break;

            LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_QC_VI_START);
            break;
        }
        case HOME(105):
        {
            LABEL_SHUTTLE[enlabelLt].Exist() = false;
            LABEL_SHUTTLE[enlabelLt].m_pVacLabelRecv->BlowOn();
            OPR.isCanMoveMsg = FALSE;
            NV.ndm[ltLabelPrintTest] = FALSE;
            break;
        }
        case STEP(105):
        {
            OPR.isCanMoveMsg = FALSE;
            NV.ndm[ltLabelPrintTest] = TRUE;
            break;
        }
        case HOME(106):
        {
            LABEL_SHUTTLE[enlabelRt].Exist() = false;
            LABEL_SHUTTLE[enlabelRt].m_pVacLabelRecv->BlowOn();
            OPR.isCanMoveMsg = FALSE;
            NV.ndm[rtLabelPrintTest] = FALSE;
            break;
        }
        case STEP(106):
        {
            OPR.isCanMoveMsg = FALSE;
            NV.ndm[rtLabelPrintTest] = TRUE;
            break;
        }

        case STEP(107):
        {
            strcpy(LABEL_PART_DATA[nvLabelLtFeeder].labelInfo.sBoxId, "3_AMBARELLA SBOX");
            strcpy(LABEL_PART_DATA[nvLabelLtFeeder].labelInfo.sBoxLabel, "^XA^LH000,000^FS^FO520,020^AD^FD22/06/11^FS^FO555,043^AD^FD00:39^FS^FO025,155^AD^FDDVC: ^FS^FO025,085^BY2,2,56^B3,,,N^FDA8-B0-P             ^FS^FO085,150^AD,30,20^FDA8-B0-P ^FS^FO025,180^BY2,2,76^B3,,,N^FDNP9DKMTC  004002212AH^FS^FO025,265^AD^FDLOT: ^FS^FO085,260^AF^FDNP9DKMTC^FS^FO265,265^AD^FDQ'TY: ^FS^FO322,260^AD,30,20^FD400^FS^FO450,265^AD^FDW/W:^FS^FO510,260^AF^FD2212 AH^FS^FO025,290^AD^FDOPTION:          ^FS^FO510,290^AF^FD^FS^FO160,305^AD^FD00182081MPP^FS^FO320,305^AD^FD^FS^FO025,330^AB,31,15^FDASSEMBLED IN KOREA FROM COMPONENTS OF KOREA^FS~DGROHSMARK,3500,48,0000001FFFFF00000000000000000000000000000000000,0000007FFFFFC0000000000000000000000000000000000,000003FFFFFFF8000000000000000000000000000000000,00000FFFFFFFFE000000000000000000000000000000000,00007FFFFFFFFFE00000000000000000000000000000000,0000FFFFFFFFFFE00000000000000000000000000000000,0007FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC,0007FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE,000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF,001FFFFFFFFFFFFF0000000000000000000000000000007,003FFFFFFFFFFFFF8000000000000000000000000000003,007FFFFFFFFFFFFFC000000000000000000000000000001,00FFFFFFFFFFFFFFE000000000000000000000000000001,01FFFFFFFFFFFFFFF000000000000000000000000000001,03FFFFFFFFFFFFFFF000000000000000000000000000001,03FFE3FFFFFFFFFFF000000000000000000000000000001,000FE3FFFFFFFFFFF800000000000000000000000000001,0001E3FFFFFFFFFFCC00000000000000000000000000003,09F0E3FFFFFFFFFFCE00000000000000000000000000003,09F8E3FFFFFFFFFFCE00000000000000000000000000003,19FCE3FFFFFFFFFFCF00000000000000000000000000007,19FCE3E1FE1FFC3F0000FF800078383E000000000000007,39FCE3C07C00F0060000FFE0007838FFC00000000000006,39FCE3FF3CF8F3E7CFC0FFF0007839E7C00000000000006,39F1E3FF3CFCE7E3CFC0F071F87839E3C00000000000006,7801E3E03CFCE7E3CFC0F073FC7839E0000000000000006,7803E3803CFCE7E3CFC0F0F3FE7FF8F800000000000000E,F9FFE31F3CFCE003CFE0FFC30F7FF87FC0000000000000C,F9FFE31F3CFCE007CFE0FFE30F7FF83FE0000000000001C,F9FFE31F3CFCE7FFCFE0F1E30F783807E0000000000001C,F9FFE31F3CFCE7FFCFE0F0F30F7839C1E0000000000001C,F9FFE31F3CFCE3FFCFE0F0730F7839C1E0000000000001C,F9FFE31F3CFCF3FFCFF0F0730F7839E3E0000000000003C,F9FFE3C03CFCF80FF070F073FE7838FFC00000000000038,FFFFFFFFFFFFFFFFFFF00000F000003C000000000000038,FFFFFFFFFFFFFFFFFFF0000000000000000000000000038,FFFFFFFFFFFFFFFFFFF0000000000000000000000000030,FFFFFFDFFFFFFFFFFFF0000000000000000000000000070,FFF8038FFFFFFFFFFFF0000000000000000000000000060,FFF8018FFFFFFFFFFFF0000000000000000000000000060,FFF8018FFFFFFFFFFFF00000000000000000000000000E0,FFF9FFFFFFFFFFBFFFF00000000000000000000000000E0,FFF9FFFFFFFFFFBFFFF00F8000000000018C000000001C0,FFF9FFCF00E00C00FFE03FE000000000018C000001C01C0,FFF9FFCF00E00C00FFE07FF000000000018C000001C01C0,FFF9FFCF7CE3C7BFFFE07DF0000000000180000001C03C0,FFF803CF7C63C7BFFFE0F078000000000180000001C03C0,FFF803CF7E63CFBFFFE0F079FC7BDF3BE18C3E1FE3F0380,7FF9FFCF7FE0FFBFFFC1F003FF7FFFBFF18CF79FF3F0780,7FF9FFCF7FF01FBFFFC1F0038F7CE3BC318CE39E79C0780,3FF9FFCF7FF80FBFFFC1E00387F8E3B8198C039C79C0700,3FF9FFCF7FFF07BFFFC1F00307F8E3B8198C3F9C79C0700,3FF9FFCF7FFFC7FFFFC1F07B87F8E3B8198CFF9C79C0700,1FF9FFCF7FE7C7CFFF0078F38F78E3BC318CC39C79C0E00,1FF9FFCF7FE1C7C0FF007FF3FF78E3BFF18CE79C79F0E00,0FF9FFCF7FE007E0FE003FE3FE78E3BFF18CFF9C79F0C00,0FF9FFDF7FF00FE0FE000F81FC78E3BBE18C7F9C79F0C00,0FFFFFFFFFFFFFFFFC00060000000038000000000000C00,07FFFFFFFFFFFFFFF800000000000038000000000000800,03FFFFFFFFFFFFFFF000000000000038000000000000800,03FFFFFFFFFFFFFFF000000000000038000000000001800,01FFFFFFFFFFFFFFF000000000000000000000000001800,01FFFFFFFFFFFFFFF000000000000000000000000007800,007FFFFFFFFFFFFFC00000000000000000000000000F000,003FFFFFFFFFFFFF800000000000000000000000001E000,001FFFFFFFFFFFFF000000000000000000000000007C000,000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8000,0007FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000,0003FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000,0000FFFFFFFFFFE00000000000000000000000000000000,00007FFFFFFFFFE00000000000000000000000000000000,00003FFFFFFFFF800000000000000000000000000000000,0000007FFFFFC0000000000000000000000000000000000,0000001FFFFF00000000000000000000000000000000000,00000001FFE000000000000000000000000000000000000^FO635,165^XGROHSMARK,1,1^FS^PQ1^XZ");
            
            break;
        }
        case STEP(110):
        {
            if (!FOAM_PK.IsCanCyclePickUp(FALSE))
                return;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PICK_UP_START, FOAM_PK.msgLtFoamBox);
            break;
        }
        case STEP(111):
        {
            FOAM_PART_DATA[enFoamRecvElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamRecvElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamRecvElev].m_pFsm->Set(FOAM_ELEV[enFoamRecvElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(112):
        {
            if (!FOAM_PK.IsCanCyclePlace())
                break;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.msgFoamRecvElev);
            break;
        }

        case STEP(115):
        {
            if (!FOAM_PK.IsCanCyclePickUp(TRUE))
                return;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PICK_UP_START, FOAM_PK.msgRtFoamBox);
            break;
        }
        case STEP(116):
        {
            FOAM_PART_DATA[nvFoamPartialElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamPartialElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamPartialElev].m_pFsm->Set(FOAM_ELEV[enFoamPartialElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(117):
        {
            if (!FOAM_PK.IsCanCyclePlace())
                break;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.msgFoamPartialElev);
            break;
        }

        case STEP(120):
        {
            FOAM_PART_DATA[nvFoamSendElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamSendElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamSendElev].m_pFsm->Set(FOAM_ELEV[enFoamSendElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(121):
        {
            if (!FOAM_ELEV[enFoamSendElev].IsCanCycleFlip())
                break;

            FOAM_ELEV[enFoamSendElev].m_pFsm->Set(FOAM_ELEV[enFoamSendElev].C_FOAM_FLIP_START);
            break;
        }
        case HOME(121):
        {
            FOAM_PART_DATA[nvFoamFlip].exist = false;
            VC[vcFoamFlip].BlowOn(FOAM_FLIP_OFF);
            VC[vcFoamFlip2].BlowOn(FOAM_FLIP_OFF);

        }
        case HOME(130):
        {
            MT[mtLabelLtFeed].Stop();
            break;
        }
        case STEP(130):
        {
            MT[mtLabelLtFeed].VelFwd(P_LABEL_T_FEED);
            break;
        }
        case STEP(132):
        {
            LABEL_FEEDER[enlabelLt].Exist() = enEmpty;
            if (!LABEL_FEEDER[enlabelLt].IsCanCycleLabelFeed())
                break;

            LABEL_FEEDER[enlabelLt].m_pFsm->Set(LABEL_FEEDER[enlabelLt].C_LABEL_FEED_START);
            break;
        }
        case STEP(140):
        {
            LABEL_FEEDER[enlabelRt].Exist() = enEmpty;
            if (!LABEL_FEEDER[enlabelRt].IsCanCycleLabelFeed())
                break;

            LABEL_FEEDER[enlabelRt].m_pFsm->Set(LABEL_FEEDER[enlabelRt].C_LABEL_FEED_START);
            break;
        }

        case STEP(145):
        {
            if (!MBB_STACK[enMbbCstLt].IsCanCycleLevelCheck())
                break;

            MBB_STACK[enMbbCstLt].m_pFsm->Set(MBB_STACK[enMbbCstLt].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(148):
        {
            if (!MBB_STACK[enMbbCstRt].IsCanCycleLevelCheck())
                break;

            MBB_STACK[enMbbCstRt].m_pFsm->Set(MBB_STACK[enMbbCstRt].C_LEVEL_CHECK_START);
            break;
        }
        
        case STEP(150):
        {
            MBB_SHUTTLE.Exist() = enExistSealed;
            MBB_QC.Exist() = enEmpty;
            if (!MBB_SHUTTLE.IsCanCycleMbbPackSend())
                break;

            MBB_SHUTTLE.m_pFsm->Set(MBB_SHUTTLE.C_MBB_PACK_SEND_START);
            break;
        }
        case STEP(151):
        {
            MBB_QC.Exist() = enExistSealed;
            if (!MBB_QC.IsCanQcCycle())
                break;

            MBB_QC.m_pFsm->Set(MBB_QC.C_MBB_QC_VI_START);
            break;
        }
        case STEP(152):
        {
            if (!MBB_QC.IsCanSendCycle())
                break;

            MBB_QC.m_pFsm->Set(MBB_QC.C_MBB_QC_SEND_START);
            break;
        }
        case STEP(153):
        {
            if (!FOLD_PK.IsCanCyclePickUp())
                break;

            //FOLD_PK.m_pFsm->Set(FOLD_PK.C_FOLD_PK_PICK_UP_START);
            break;
        }
        case HOME(199):
        {
            NV.ddm[mfcValuemilliLiter] = 0;
            NV.rcp[setNozzleN2AoSetValue] = 0;
            VC[vcSealNozzleN2].Off();
            Off(oCylSealNozzleMfcN2Op);
            NV.ndm[simulationDebug3] = 0;
            break;
        }
        case STEP(199):
        {
            NV.ddm[mfcValuemilliLiter] = 0;
            NV.rcp[setNozzleN2AoSetValue] = 100;
            NV.ndm[simulationDebug3] = TRUE;
            break;
        }

        case STEP(200):
        {
            if (!IN_PK.IsCanCyclePickUp())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_TRAY_PICK_UP_START);
            break;
        }
        case STEP(201):
        {
            if (!IN_PK.IsCanCycleBandingVi())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_BANDING_VI_START);
            break;
        }
        case STEP(202):
        {
            if (!IN_PK.IsCanCyclePlace())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_TRAY_PLACE_START);
            break;
        }
        case STEP(203):
        {
            if (!FOAM_RAIL.IsCanCycleBtmFoamOpen())
                break;

            FOAM_RAIL.m_pFsm->Set(FOAM_RAIL.C_BTM_FOAM_OPEN_START);
            break;
        }
        case STEP(204):
        {
            if (!FOAM_RAIL.IsCanCycleBtmFoamSend())
                break;

            FOAM_RAIL.m_pFsm->Set(FOAM_RAIL.C_BTM_FOAM_SEND_START);
            break;
        }
        case STEP(205):
        {
            if (!FOAM_RAIL.IsCanCycleTopFoamSend())
                break;

            FOAM_RAIL.m_pFsm->Set(FOAM_RAIL.C_TOP_FOAM_SEND_START);
            break;
        }
        case STEP(206):
        {
            FOAM_PART_DATA[nvFoamRecvElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamRecvElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamRecvElev].m_pFsm->Set(FOAM_ELEV[enFoamRecvElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(207):
        {
            FOAM_PART_DATA[nvFoamPartialElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamPartialElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamPartialElev].m_pFsm->Set(FOAM_ELEV[enFoamPartialElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(208):
        {
            FOAM_PART_DATA[nvFoamSendElev].flag.levelDid = false;
            if (!FOAM_ELEV[enFoamSendElev].IsCanCycleLevelCheck())
                break;

            FOAM_ELEV[enFoamSendElev].m_pFsm->Set(FOAM_ELEV[enFoamSendElev].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(209):
        {
            if (!FOAM_ELEV[enFoamSendElev].IsCanCycleFlip())
                break;

            FOAM_ELEV[enFoamSendElev].m_pFsm->Set(FOAM_ELEV[enFoamSendElev].C_FOAM_FLIP_START);
            break;
        }
        case STEP(210):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift1To2);
            break;
        }
        case STEP(211):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift1To3);
            break;
        }
        case STEP(212):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift1To4);
            break;
        }
        case STEP(213):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift2To3);
            break;
        }
        case STEP(214):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift2To4);
            break;
        }
        case STEP(215):
        {
            if (!FOAM_STACK_CONV.IsCanCycleFoamMove())
                break;

            FOAM_STACK_CONV.m_pFsm->Set(FOAM_STACK_CONV.C_FOAM_MOVE_START, FOAM_STACK_CONV.msgMzMoveLift3To4);
            break;
        }
        case STEP(216):
        {
            if (!FOAM_PK.IsCanCyclePickUp(FALSE))
                return;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PICK_UP_START, FOAM_PK.msgLtFoamBox);
            break;
        }
        case STEP(217):
        {
            if (!FOAM_PK.IsCanCyclePickUp(TRUE))
                return;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PICK_UP_START, FOAM_PK.msgRtFoamBox);
            break;
        }
        case STEP(218):
        {
            if (!FOAM_PK.IsCanCyclePlace())
                break;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.msgFoamRecvElev);
            break;
        }
        case STEP(219):
        {
            if (!FOAM_PK.IsCanCyclePlace())
                break;

            FOAM_PK.m_pFsm->Set(FOAM_PK.C_FOAM_PLACE_START, FOAM_PK.msgFoamPartialElev);
            break;
        }
        case STEP(220):
        {
            if (!FOAM_CST[enFoamBoxLt].IsCanCycleFoamBoxLoad())
                break;

            FOAM_CST[enFoamBoxLt].m_pFsm->Set(FOAM_CST[enFoamBoxLt].C_FOAM_CST_LD_START);
            break;
        }
        case STEP(221):
        {
            if (!FOAM_CST[enFoamBoxLt].IsCanCycleFoamBoxUnload())
                break;

            FOAM_CST[enFoamBoxLt].m_pFsm->Set(FOAM_CST[enFoamBoxLt].C_FOAM_CST_ULD_START);
            break;
        }
        case STEP(222):
        {
            if (!FOAM_CST[enFoamBoxRt].IsCanCycleFoamBoxLoad())
                break;

            FOAM_CST[enFoamBoxRt].m_pFsm->Set(FOAM_CST[enFoamBoxRt].C_FOAM_CST_LD_START);
            break;
        }
        case STEP(223):
        {
            if (!FOAM_CST[enFoamBoxRt].IsCanCycleFoamBoxUnload())
                break;

            FOAM_CST[enFoamBoxRt].m_pFsm->Set(FOAM_CST[enFoamBoxRt].C_FOAM_CST_ULD_START);
            break;
        }
        case STEP(224):
        {
            if (!TRAY_SHUTTLE.IsCanCycleFoamPackSend())
                break;

            TRAY_SHUTTLE.m_pFsm->Set(TRAY_SHUTTLE.C_FOAM_PACK_SEND_START);
            break;
        }
        case STEP(225):
        {
            if (!MBB_SHUTTLE.IsCanCycleMbbOpen())
                break;

            MBB_SHUTTLE.m_pFsm->Set(MBB_SHUTTLE.C_MBB_OPEN_START);
            break;
        }
        case STEP(226):
        {
            if (!MBB_SHUTTLE.IsCanCycleMbbSealing())
                break;

            MBB_SHUTTLE.m_pFsm->Set(MBB_SHUTTLE.C_MBB_SEALING_START);
            break;
        }
        case STEP(227):
        {
            if (!MBB_SHUTTLE.IsCanCycleMbbPackSend())
                break;

            MBB_SHUTTLE.m_pFsm->Set(MBB_SHUTTLE.C_MBB_PACK_SEND_START);
            break;
        }
        case STEP(228):
        {
            MBB_QC.Exist() = enExistSealed;
            if (!MBB_QC.IsCanQcCycle())
                break;

            MBB_QC.m_pFsm->Set(MBB_QC.C_MBB_QC_VI_START);
            break;
        }
        case STEP(229):
        {
            if (!MBB_QC.IsCanSendCycle())
                break;

            MBB_QC.m_pFsm->Set(MBB_QC.C_MBB_QC_SEND_START);
            break;
        }
        case STEP(230):
        {
            if (!FOLD_PK.IsCanCyclePickUp())
                break;

            FOLD_PK.m_pFsm->Set(FOLD_PK.C_PICK_UP_START);
            break;
        }
        case STEP(231):
        {
            if (!FOLD_PK.IsCanCyclePlace())
                break;

            FOLD_PK.m_pFsm->Set(FOLD_PK.C_PLACE_START);
            break;
        }
        case STEP(232):
        {
            if (!LABEL_FEEDER[enlabelLt].IsCanCycleLabelFeed())
                break;

            LABEL_FEEDER[enlabelLt].m_pFsm->Set(LABEL_FEEDER[enlabelLt].C_LABEL_FEED_START);
            break;
        }
        case STEP(233):
        {
            if (!LABEL_FEEDER[enlabelLt].IsCanCycleLabelCstLoad())
                break;

            LABEL_FEEDER[enlabelLt].m_pFsm->Set(LABEL_FEEDER[enlabelLt].C_LABEL_CST_LD_START);
            break;
        }
        case STEP(234):
        {
            if (!LABEL_FEEDER[enlabelLt].IsCanCycleLabelCstUnload())
                break;

            LABEL_FEEDER[enlabelLt].m_pFsm->Set(LABEL_FEEDER[enlabelLt].C_LABEL_CST_ULD_START);
            break;
        }
        case STEP(235):
        {
            if (!LABEL_FEEDER[enlabelRt].IsCanCycleLabelFeed())
                break;

            LABEL_FEEDER[enlabelRt].m_pFsm->Set(LABEL_FEEDER[enlabelRt].C_LABEL_FEED_START);
            break;
        }
        case STEP(236):
        {
            if (!LABEL_FEEDER[enlabelRt].IsCanCycleLabelCstLoad())
                break;

            LABEL_FEEDER[enlabelRt].m_pFsm->Set(LABEL_FEEDER[enlabelRt].C_LABEL_CST_LD_START);
            break;
        }
        case STEP(237):
        {
            if (!LABEL_FEEDER[enlabelRt].IsCanCycleLabelCstUnload())
                break;

            LABEL_FEEDER[enlabelRt].m_pFsm->Set(LABEL_FEEDER[enlabelRt].C_LABEL_CST_ULD_START);
            break;
        }
        case HOME(238):
        {
            LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExist;
            if (!LABEL_LASER.IsCanCyclePrs())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PRS_VI_START, nvLabelLtFeeder);
            break;
        }
        case STEP(238):
        {
            LABEL_PART_DATA[nvLabelRtFeeder].exist = enLabelExist;
            if (!LABEL_LASER.IsCanCyclePrs())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PRS_VI_START, nvLabelRtFeeder);
            break;
        }
        case HOME(239):
        {
            LABEL_PART_DATA[nvLabelLtFeeder].exist = enLabelExistPrs;
            if (!LABEL_LASER.IsCanCycleLaser())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_START, nvLabelLtFeeder);
            break;
        }
        case STEP(239):
        {
            LABEL_PART_DATA[nvLabelRtFeeder].exist = enLabelExistPrs;
            if (!LABEL_LASER.IsCanCycleLaser())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_START, nvLabelRtFeeder);
            break;
        }

        case HOME(240):
        {
            LABEL_LASER.m_PowerMeterDid = false;
            if (!LABEL_LASER.IsCanCyclePowerMeter())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PWR_METER_START, MODE_PWR_CHECK);
            break;
        }
        case STEP(240):
        {
            LABEL_LASER.m_PowerMeterDid = false;
            if (!LABEL_LASER.IsCanCyclePowerMeter())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_PWR_METER_START, MODE_PWR_TABLE);
            break;
        }
        case STEP(241):
        {
            if (!LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelRecv())
                break;

            LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_RECV_START);
            break;
        }
        case STEP(242):
        {
            if (!LABEL_SHUTTLE[enlabelLt].IsCanCycleLabelQc())
                break;

            LABEL_SHUTTLE[enlabelLt].m_pFsm->Set(LABEL_SHUTTLE[enlabelLt].C_LABEL_QC_VI_START);
            break;
        }
        case STEP(243):
        {
            if (!LABEL_SHUTTLE[enlabelRt].IsCanCycleLabelRecv())
                break;

            LABEL_SHUTTLE[enlabelRt].m_pFsm->Set(LABEL_SHUTTLE[enlabelRt].C_LABEL_RECV_START);
            break;
        }
        case STEP(244):
        {
            if (!LABEL_SHUTTLE[enlabelRt].IsCanCycleLabelQc())
                break;

            LABEL_SHUTTLE[enlabelRt].m_pFsm->Set(LABEL_SHUTTLE[enlabelRt].C_LABEL_QC_VI_START);
            break;
        }
        case STEP(245):
        {
            if (!MBB_STACK[enMbbCstLt].IsCanCycleLevelCheck())
                break;

            MBB_STACK[enMbbCstLt].m_pFsm->Set(MBB_STACK[enMbbCstLt].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(246):
        {
            if (!MBB_STACK[enMbbCstLt].IsCanCycleMbbCstLoad())
                break;

            MBB_STACK[enMbbCstLt].m_pFsm->Set(MBB_STACK[enMbbCstLt].C_MBB_CST_LD_START);
            break;
        }
        case STEP(247):
        {
            if (!MBB_STACK[enMbbCstLt].IsCanCycleMbbCstUnload())
                break;

            MBB_STACK[enMbbCstLt].m_pFsm->Set(MBB_STACK[enMbbCstLt].C_MBB_CST_ULD_START);
            break;
        }
        case STEP(248):
        {
            if (!MBB_STACK[enMbbCstRt].IsCanCycleLevelCheck())
                break;

            MBB_STACK[enMbbCstRt].m_pFsm->Set(MBB_STACK[enMbbCstRt].C_LEVEL_CHECK_START);
            break;
        }
        case STEP(249):
        {
            if (!MBB_STACK[enMbbCstRt].IsCanCycleMbbCstLoad())
                break;

            MBB_STACK[enMbbCstRt].m_pFsm->Set(MBB_STACK[enMbbCstRt].C_MBB_CST_LD_START);
            break;
        }
        case STEP(250):
        {
            if (!MBB_STACK[enMbbCstRt].IsCanCycleMbbCstUnload())
                break;

            MBB_STACK[enMbbCstRt].m_pFsm->Set(MBB_STACK[enMbbCstRt].C_MBB_CST_ULD_START);
            break;
        }

        case STEP(251):
        {
            if (!LT_COBOT.IsCanCycleBtmFoamPick(FALSE))
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_BTM_FOAM_PICK_START, _NORMAL);
            break;
        }
        case STEP(252):
        {
            if (!LT_COBOT.IsCanCycleBtmFoamPick(TRUE))
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_BTM_FOAM_PICK_START, _PARTIAL);
            break;
        }
        case STEP(253):
        {
            if (!LT_COBOT.IsCanCycleBtmFoamPlace())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_BTM_FOAM_PLACE_START, LT_COBOT.Exist(_FOAM));
            break;
        }
        case STEP(254):
        {
            if (OPR.isDryRun)
            {
                TRAY_PART_DATA[3].exist = 8;
                TRAY_PART_DATA[3].flag.ltCobotLabelPickup = false;
                TRAY_PART_DATA[3].flag.ltCobotLabelPrinted = true;
            }

            if (!LT_COBOT.IsCanCycleLabelPick())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_LABEL_PICK_START, _LT);
            break;
        }
        case STEP(255):
        {
            if (OPR.isDryRun)
            {
                TRAY_PART_DATA[3].exist = 8;
                TRAY_PART_DATA[3].flag.ltCobotLabelPickup = false;
                TRAY_PART_DATA[3].flag.ltCobotLabelPrinted = true;
            }

            if (!LT_COBOT.IsCanCycleLabelPick())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_LABEL_PICK_START, _RT);
            break;
        }
        case STEP(256):
        {
            if (!LT_COBOT.IsCanCycleLabelAttach())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_LABEL_ATTACH_START);
            break;
        }
        case STEP(257):
        {
            if (!LT_COBOT.IsCanCycleTopFoamPick())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_TOP_FOAM_PICK_START);
            break;
        }
        case STEP(258):
        {
            if (!LT_COBOT.IsCanCycleTopFoamPlace())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_TOP_FOAM_PLACE_START);
            break;
        }
        case STEP(259):
        {
            if (!LT_COBOT.IsCanCycleFoamReject())
                break;

            LT_COBOT.m_pFsm->Set(LT_COBOT.C_FOAM_REJECT_START);
            break;
        }
        case STEP(264):
        {
            //if (OPR.isDryRun)
            //{
            //    TRAY_PART_DATA[nvTrayShuttle].exist = 8;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;
            //}
			TRAY_PART_DATA[nvTrayShuttle].exist = 9;
			TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
			TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
			TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
			TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;
            if (!RT_COBOT.IsCanCycleLabelPick())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_PICK_START, _LT);
            break;
        }
        case STEP(265):
        {
            //if (OPR.isDryRun)
            //{
            //    TRAY_PART_DATA[nvTrayShuttle].exist = 8;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
            //    TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;
            //}
			TRAY_PART_DATA[nvTrayShuttle].exist = 9;
			TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPickup = true;
			TRAY_PART_DATA[nvTrayShuttle].flag.ltCobotLabelPrinted = true;
			TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPickup = false;
			TRAY_PART_DATA[nvTrayShuttle].flag.rtCobotLabelPrinted = true;
            if (!RT_COBOT.IsCanCycleLabelPick())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_PICK_START, _RT);
            break;
        }
        case STEP(266):
        {
            if (!RT_COBOT.IsCanCycleMbbVi())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_VI_START, _LT);
            break;
        }
        case STEP(267):
        {
            if (!RT_COBOT.IsCanCycleMbbVi())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_VI_START, _RT);
            break;
        }
        case STEP(268):
        {
            if (!RT_COBOT.IsCanCycleLabelAttach())
                break;

            int nMsgDir;
            if (MBB_PART_DATA[nvLtMbbStack].exist == enMbbExistQc)
                nMsgDir = 0;
            else if (MBB_PART_DATA[nvRtMbbStack].exist == enMbbExistQc)
                nMsgDir = 1;
            else
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_ATTACH_START, nMsgDir);
            break;
        }
        case STEP(269):
        {
            if (!RT_COBOT.IsCanCycleLabelAttachVi())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_ATTACH_VI_START, _LT);
            break;
        }
        case STEP(270):
        {
            if (!RT_COBOT.IsCanCycleLabelAttachVi())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_ATTACH_VI_START, _RT);
            break;
        }
        case HOME(271):
        {
            COBOT_PART_DATA[nvRtCobot].exist = enEmpty;
            MBB_PART_DATA[nvLtMbbStack].exist = enMbbExistLabelQc;
            VC[vcRtCobotMbb].Off(RT_COBOT_MBB_OFF);
            break;
        }
        case STEP(271):
        {
            if (!RT_COBOT.IsCanCycleMbbPick())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_PICK_START, 0);  // Left
            break;
        }
        case STEP(272):
        {
            if (!RT_COBOT.IsCanCycleMbbPick())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_PICK_START, 1);  // Left
            break;
        }
        case STEP(273):
        {
            if (!RT_COBOT.IsCanCycleMbbPlace())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_PLACE_START);
            break;
        }
        case STEP(274):
        {
            if (!RT_COBOT.IsCanCycleLabelReject())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_LABEL_REJECT_START);
            break;
        }
        case STEP(275):
        {
            if (!RT_COBOT.IsCanCycleMbbReject())
                break;

            RT_COBOT.m_pFsm->Set(RT_COBOT.C_MBB_REJECT_START);
            break;
        }


        case HOME(276):
        {
            if (!LABEL_LASER.IsCanCycleLaser())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_1POINT_CAL_VI_START, 0);
            break;
        }
        case STEP(276):
        {
            if (!LABEL_LASER.IsCanCycleLaser())
                break;

            LABEL_LASER.m_pFsm->Set(LABEL_LASER.C_LASER_1POINT_CAL_VI_START, 1);
            break;
        }
        case HOME(277):
        {
            break;
        }
        case STEP(277):
        {
            FOAM_RAIL.Exist(_BTM_FOAM_) = enExistBtmFoamOpen;
            if (!IN_PK.IsCanCycleBtmFoamVi())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_BTM_FOAM_VI_START);
            break;
        }
        case HOME(278):
        {
            break;
        }
        case STEP(278):
        {
            FOAM_RAIL.Exist(_BTM_FOAM_) = enExistBtmFoamTray;
            if (!IN_PK.IsCanCycleBtmFoamTrayVi())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_BTM_FOAM_TRAY_VI_START);
            break;
        }
        case HOME(279):
        {
            break;
        }
        case STEP(279):
        {
            FOAM_RAIL.Exist(_TOP_FOAM_) = enExistTrayTopFoam;
            if (!IN_PK.IsCanCycleTopFoamVi())
                break;

            IN_PK.m_pFsm->Set(IN_PK.C_LABEL_ATTACH_VI_START);
            break;
        }
        case HOME(280):
        {
            if (ROBOT[robotLt].CanMove(P_LT_COBOT_READY))
                ROBOT[robotLt].Move(P_LT_COBOT_READY);

            break;
        }
        case STEP(280):
        {
            break;
        }
        case HOME(281):
        {
            break;
        }
        case STEP(281):
        {
            break;
        }
        case HOME(282):
        {
            break;
        }
        case STEP(282):
        {
            break;
        }
        case HOME(283):
        {
            break;
        }
        case STEP(283):
        {
            break;
        }
        case HOME(284):
        {
            break;
        }
        case STEP(284):
        {
            break;
        }
        case HOME(285):
        {
            break;
        }
        case STEP(285):
        {
            break;
        }
        case HOME(286):
        {
            break;
        }
        case STEP(286):
        {
            break;
        }
        case HOME(287):
        {
            break;
        }
        case STEP(287):
        {
            break;
        }
        case HOME(288):
        {
            break;
        }
        case STEP(288):
        {
            break;
        }
        case HOME(289):
        {
            break;
        }
        case STEP(289):
        {
            break;
        }
        case HOME(290):
        {
            if (ROBOT[robotRt].CanMove(P_RT_COBOT_READY))
                ROBOT[robotRt].Move(P_RT_COBOT_READY);
            break;
        }
        case STEP(290):
        {
            break;
        }
        case HOME(291):
        {
            break;
        }
        case STEP(291):
        {
            break;
        }
        case HOME(292):
        {
            break;
        }
        case STEP(292):
        {
            break;
        }
        case HOME(293):
        {
            break;
        }
        case STEP(293):
        {
            break;
        }
        case HOME(294):
        {
            break;
        }
        case STEP(294):
        {
            break;
        }
        case HOME(295):
        {
            break;
        }
        case STEP(295):
        {
            break;
        }
        case HOME(296):
        {
            break;
        }
        case STEP(296):
        {
            break;
        }
        case HOME(297):
        {
            break;
        }
        case STEP(297):
        {
            break;
        }
        case HOME(298):
        {
            break;
        }
        case STEP(298):
        {
            break;
        }
        case HOME(299):
        {
            break;
        }
        case STEP(299):
        {
            break;
        }
        case STEP(300):
        {
            SEAL[sealMbb].Start();
            break;
        }
        case STEP(301):
        {
            SEAL[sealMbb].SetInit();
            break;
        }
        case STEP(302):
        {
            SEAL[sealMbb].Stop();
            break;
        }

        case STEP(303):
        {
            SEAL[sealMbb].Quit();
            break;
        }
        case STEP(310):
        {
            MBB_SHUTTLE.m_pFsm->Set(MBB_SHUTTLE.C_MBB_TEST_SEALING_START);
			break;
        }
        case STEP(320):
        {
            TRAY_PART_DATA[nvTopFoam].exist = enExistTrayTopFoamQc;
            TRAY_PART_DATA[nvMbbShuttle].exist = enExistMbb;
            break;
        }
        case STEP(321):
        {
            TRAY_SHUTTLE.CylInit();
            MBB_SHUTTLE.CylInit();
            break;
        }
        case HOME(400):
        {
            ROBOT[robotLt].StartHomeCommand();
            break;
        }
        case HOME(401):
        {
            ROBOT[robotRt].StartHomeCommand();
            break;
        }
        case STEP(555):
        {
            MT[mtLabelLtFeed].Stop();
            break;
        }
        case HOME(666):
        {
            break;
        }
        case STEP(666):
        {
            ROBOT[robotLt].Move(P_LT_COBOT_READY);

            break;
        }

        case STEP(667):
        {
            break;
        }

        case STEP(668):
        {
            break;
        }
        case STEP(669):
        {
            break;
        }
        case STEP(777):
        {
            ROBOT[robotRt].Move(P_RT_COBOT_READY);

            break;
        }
        case STEP(888):
        {
            ROBOT[robotLt].m_isHome = true;
            ROBOT[robotRt].m_isHome = true;
            
            break;
        }
        
        case STEP(900):
        {
            break;
            //int nTarget = 0;

            //for (int i = 0; i < 15; i++)
            //{
            //    if (i == 0) nTarget = 0;
            //    else if (i == 1) nTarget = 3;
            //    else if (i == 2) nTarget = 5;
            //    else if (i == 3) nTarget = 8;
            //    else if (i == 4) nTarget = 11;
            //    else if (i == 5) nTarget = 14;
            //    else if (i == 6) nTarget = 16;
            //    else if (i == 7) nTarget = 18;
            //    else if (i == 8) nTarget = 20;
            //    else if (i == 9) nTarget = 22;
            //    else if (i == 10) nTarget = 23;
            //    else if (i == 11) nTarget = 25;
            //    else if (i == 12) nTarget = 28;
            //    else if (i == 13) nTarget = 30;
            //    else if (i == 14) nTarget = 31;

            //    ROBOT_TABLE[robotLt].joint[i].joint_0 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_0 * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].joint[i].joint_1 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_1 * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].joint[i].joint_2 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_2 * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].joint[i].joint_3 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_3 * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].joint[i].joint_4 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_4 * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].joint[i].joint_5 = round(ROBOT_TABLE[robotLt].joint[nTarget].joint_5 * 10.) / 10.;

            //    ROBOT_TABLE[robotLt].tcp[i].tcpPx = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpPx * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].tcp[i].tcpPy = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpPy * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].tcp[i].tcpPz = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpPz * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].tcp[i].tcpRx = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpRx * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].tcp[i].tcpRy = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpRy * 10.) / 10.;
            //    ROBOT_TABLE[robotLt].tcp[i].tcpRz = round(ROBOT_TABLE[robotLt].tcp[nTarget].tcpRz * 10.) / 10.;
            //}

            //for (int i = 0; i < 14; i++)
            //{
            //    if (i == 0) nTarget = 0;
            //    else if (i == 1) nTarget = 2;
            //    else if (i == 2) nTarget = 4;
            //    else if (i == 3) nTarget = 6;
            //    else if (i == 4) nTarget = 8;
            //    else if (i == 5) nTarget = 10;
            //    else if (i == 6) nTarget = 12;
            //    else if (i == 7) nTarget = 14;
            //    else if (i == 8) nTarget = 16;
            //    else if (i == 9) nTarget = 19;
            //    else if (i == 10) nTarget = 22;
            //    else if (i == 11) nTarget = 25;
            //    else if (i == 12) nTarget = 26;
            //    else if (i == 13) nTarget = 38;

            //    ROBOT_TABLE[robotRt].joint[i].joint_0 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_0 * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].joint[i].joint_1 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_1 * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].joint[i].joint_2 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_2 * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].joint[i].joint_3 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_3 * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].joint[i].joint_4 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_4 * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].joint[i].joint_5 = round(ROBOT_TABLE[robotRt].joint[nTarget].joint_5 * 10.) / 10.;

            //    ROBOT_TABLE[robotRt].tcp[i].tcpPx = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpPx * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].tcp[i].tcpPy = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpPy * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].tcp[i].tcpPz = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpPz * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].tcp[i].tcpRx = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpRx * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].tcp[i].tcpRy = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpRy * 10.) / 10.;
            //    ROBOT_TABLE[robotRt].tcp[i].tcpRz = round(ROBOT_TABLE[robotRt].tcp[nTarget].tcpRz * 10.) / 10.;
            //}
            break;
        }
        case STEP(901):
        {
            for (int i = 0; i < 50; i++)
                ROBOT_TABLE[robotLt].nVel[i] = 30;

            break;
        }
        case STEP(902):
        {
            for (int i = 0; i < 50; i++)
                ROBOT_TABLE[robotRt].nVel[i] = 100;

            break;
        }
        case STEP(990):
        {
            double dX = 10;
            double dY = 2;
            double dT = GetTheta(dX, dY);
            LOG[logSEQ].Message("GetTheta [%.3f]", dT);
            break;
        }
		case HOME(999):
        {
            for (int nFoamNum = 0; nFoamNum <= nvFoamPartialElev; nFoamNum++)
            {
                if (nFoamNum == nvFoamFlip)
                    continue;
                FOAM_PART_DATA[nFoamNum].exist = true;
                FOAM_PART_DATA[nFoamNum].cnt = 100;
            }
            LABEL_PART_DATA[nvLabelLtFeeder].flag.existCst = true;
            LABEL_PART_DATA[nvLabelRtFeeder].flag.existCst = true;
            MBB_PART_DATA[nvLtMbbStack].flag.existCst = true;
            MBB_PART_DATA[nvRtMbbStack].flag.existCst = true;
			break;
        }
		case STEP(999):
		{
			if (OPR.isNoDevice || OPR.isDryRun)
			{
				for (int nVcNum = 0; nVcNum < vacuumMAX; nVcNum++)
					VC[nVcNum].Off();

				for (int nTrayNum = 0; nTrayNum < nvTrayPartMax; nTrayNum++)
					ZeroMemory(&TRAY_PART_DATA[nTrayNum], sizeof(IPC_MMI::TrayPartData));

				for (int nCobotNum = 0; nCobotNum < nvCobotPartMax; nCobotNum++)
					ZeroMemory(&COBOT_PART_DATA[nCobotNum], sizeof(IPC_MMI::CobotPartData));

				for (int nFoamNum = 0; nFoamNum < nvFoamPartMax; nFoamNum++)
					ZeroMemory(&FOAM_PART_DATA[nFoamNum], sizeof(IPC_MMI::FoamPartData));

				for (int nLabelNum = 0; nLabelNum < nvLabelPartMax; nLabelNum++)
					ZeroMemory(&LABEL_PART_DATA[nLabelNum], sizeof(IPC_MMI::LabelPartData));

				for (int nMbbNum = 0; nMbbNum < nvMbbPartMax; nMbbNum++)
					ZeroMemory(&MBB_PART_DATA[nMbbNum], sizeof(IPC_MMI::MbbPartData));

                FOAM_PART_DATA[nvFoamSendElev].exist = true;
                FOAM_PART_DATA[nvFoamSendElev].cnt = 14;

                FOAM_PART_DATA[nvFoamFtStack].exist = true;
                FOAM_PART_DATA[nvFoamFtStack].cnt = 14;


				//FOAM_PART_DATA[nvFoamLtCst].exist = true;
				//FOAM_PART_DATA[nvFoamRtCst].exist = true;

				//LABEL_PART_DATA[nvLabelLtFeeder].flag.existCst = true;
				//LABEL_PART_DATA[nvLabelRtFeeder].flag.existCst = true;

				//MBB_PART_DATA[nvLtMbbStack].flag.existStack = true;
				//MBB_PART_DATA[nvRtMbbStack].flag.existStack = true;

				// Etc.
				NV.ndm[simulationDebug0] = 0;
                NV.ndm[recvPreMcData] = 0;
			}
			break;
		}
	}

	OPR.isCanMoveMsg = FALSE;

}