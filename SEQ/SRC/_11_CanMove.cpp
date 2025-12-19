#include "..\Includes.h"

// LEFT PART
BOOL LtCobotCanMove(int cmd)
{
	if (!Motor::DefaultCanMove(cmd))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (cmd == P_LT_COBOT_READY)
		{
			//for (int nPos = P_LT_COBOT_LT_LABEL_PICK; nPos <= P_LT_COBOT_RT_LABEL_PICK; nPos++)
			//{
			//	if (ROBOT[robotLt].IsRdy(nPos))
			//		return FALSE;
			//}
			//for (int nPos = P_LT_COBOT_LABEL_REJECT; nPos <= P_LT_COBOT_FOAM_REJECT; nPos++)
			//{
			//	if (ROBOT[robotLt].IsRdy(nPos))
			//		return FALSE;
			//}
		}
		else
		{
			{
				//if (!ROBOT[robotLt].TcpInPos(P_LT_COBOT_READY) && !ROBOT[robotLt].JointInPos(P_LT_COBOT_READY))
				//	return FALSE;
			}
		}
		
		if (cmd == P_LT_COBOT_BTM_FOAM_PICK || cmd == P_LT_COBOT_BTM_FOAM_PICK_TO_READY ||
			cmd == P_LT_COBOT_LABEL_ATTACH || cmd == P_LT_COBOT_LABEL_ATTACH_TO_READY ||
			cmd == P_LT_COBOT_TOP_FOAM_PICK || cmd == P_LT_COBOT_TOP_FOAM_PICK_TO_READY)
		{
			if (!MT[mtFoamFlipT].InPos(P_FOAM_FLIP_T_SEND))
				return FALSE;
		}
		else if (cmd == P_LT_COBOT_BTM_FOAM_PLACE || cmd == P_LT_COBOT_BTM_PARTIAL_FOAM_PLACE)
		{
			double anticollisionPos = MT[mtInPkX].PosTable(P_IN_PK_X_READY) - 1.;
			if (MT[mtInPkX].m_state.realCnt < anticollisionPos)
				return FALSE;
		}
		else if (cmd == P_LT_COBOT_BTM_PARTIAL_FOAM_PICK || cmd == P_LT_COBOT_BTM_PARTIAL_FOAM_PICK_TO_READY)
		{
			double anticollisionPos = MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY);
			if (MT[mtFoamPkY].m_state.realCnt > anticollisionPos || MT[mtFoamPkY].m_state.cmdCnt > anticollisionPos)
				return FALSE;
		}
		else if (cmd == P_LT_COBOT_LT_LABEL_PICK_READY || cmd == P_LT_COBOT_LT_LABEL_PICK ||
			cmd == P_LT_COBOT_LT_LABEL_PICK_TO_PICK_READY || cmd == P_LT_COBOT_LT_LABEL_PICK_READY_TO_READY)
		{
			if (!MT[mtLabelLtShuttleY].InPos(P_LABEL_SHUTTLE_Y_SEND))
				return FALSE;

			double anticollisionPos = MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY);
			if (MT[mtFoamPkY].m_state.realCnt > anticollisionPos || MT[mtFoamPkY].m_state.cmdCnt > anticollisionPos)
				return FALSE;

			if (!NV.ndm[rtCobotLtCobotLabelPickCanMove])
				return FALSE;
		}
		else if (cmd == P_LT_COBOT_RT_LABEL_PICK_READY || cmd == P_LT_COBOT_RT_LABEL_PICK ||
			cmd == P_LT_COBOT_RT_LABEL_PICK_TO_PICK_READY || cmd == P_LT_COBOT_RT_LABEL_PICK_READY_TO_READY)
		{
			if (!MT[mtLabelRtShuttleY].InPos(P_LABEL_SHUTTLE_Y_SEND))
				return FALSE;

			double anticollisionPos = MT[mtFoamPkY].PosTable(P_FOAM_PK_Y_SAFETY);
			if (MT[mtFoamPkY].m_state.realCnt > anticollisionPos || MT[mtFoamPkY].m_state.cmdCnt > anticollisionPos)
				return FALSE;

			if (!NV.ndm[rtCobotLtCobotLabelPickCanMove])
				return FALSE;
		}
	}
	else
	{

	}
	return TRUE;
}

BOOL InPkXCanMove(int cmd)
{
	AxisList mtNo = mtInPkX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (cmd == P_IN_PK_X_BTM_FOAM_VI_1ST || cmd == P_IN_PK_X_BTM_FOAM_VI_2ND)
		{
			if (!IsMtSafetyPos(mtInPkZ, P_IN_PK_Z_READY, P_IN_PK_Z_BTM_FOAM_VI_NORMAL, P_IN_PK_Z_BTM_FOAM_VI_PARTIAL))
				return FALSE;
		}
		else if (cmd == P_IN_PK_X_BAND_VI_1ST || cmd == P_IN_PK_X_BAND_VI_2ND || cmd == P_IN_PK_X_BAND_VI_3RD)
		{
			if (!IsMtSafetyPos(mtInPkZ, P_IN_PK_Z_READY, P_IN_PK_Z_BAND_VI))
				return FALSE;
		}
		else
		{
			if (!IsMtSafetyPos(mtInPkZ, P_IN_PK_Z_READY))
				return FALSE;
		}

		if (cmd != P_IN_PK_X_READY || cmd != P_IN_PK_X_PICK)
		{
			if (!NV.ndm[ltCobotInPkCanMove])
				return FALSE;
		}
	}
	else
	{
		if (!IsMtSafetyPos(mtInPkZ, P_IN_PK_Z_READY))
			return FALSE;

		// TODO: Cobot Safety Check
		//if (!ROBOT[robotLt].IsRdy(rtCobotReady))
		//	return FALSE;
	}

	return TRUE;
}

BOOL InPkZCanMove(int cmd)
{
	AxisList mtNo = mtInPkZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_IN_PK_Z_READY != cmd)
		{
			if (!IsMtRdy(mtInPkX))
				return FALSE;
		}

		if (P_IN_PK_Z_PICK == cmd)
		{
			if (!IsMtSafetyPos(mtInPkX, P_IN_PK_X_PICK))
				return FALSE;
		}
		else if (P_IN_PK_Z_BAND_VI == cmd)
		{
			if (!IsMtSafetyPos(mtInPkX, P_IN_PK_X_BAND_VI_1ST, P_IN_PK_X_BAND_VI_2ND, P_IN_PK_X_BAND_VI_3RD))
				return FALSE;
		}
		else if (P_IN_PK_Z_PLACE_NORMAL == cmd)
		{
			if (!IsMtSafetyPos(mtInPkX, P_IN_PK_X_PLACE))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL TrayShuttleYCanMove(int cmd)
{
	AxisList mtNo = mtTrayShuttleY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (!IsMtSafetyPos(mtTrayPusherX, P_TRAY_PUSHER_X_READY) ||
		!IsMtSafetyPos(mtMbbOpenZ, P_MBB_OPEN_Z_READY))
		return FALSE;

	if (!IsCylSafetyPos(cylTrayShuttleBridgeFb, Cylinder::cylinderBWD))
		return FALSE;

	if (IsSensorAOn(iTrayShuttleExistIn) ||
		IsSensorAOn(iTrayShuttleLtColorSensor) || IsSensorAOn(iTrayShuttleRtColorSensor))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}
	
	return TRUE;
}

BOOL TrayPushXCanMove(int cmd)
{
	AxisList mtNo = mtTrayPusherX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (!IsCylSafetyPos(cylTrayShuttleNozzleUd, Cylinder::cylinderDOWN) ||
		!IsCylSafetyPos(cylTrayShuttleBridgeUd, Cylinder::cylinderDOWN))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_TRAY_PUSHER_X_1ST_END == cmd || P_TRAY_PUSHER_X_2ND_END == cmd)
		{
			if (!IsMtSafetyPos(mtMbbOpenZ, P_MBB_OPEN_Z_READY))
				return FALSE;

			if (!IsMtSafetyPos(mtTrayShuttleY, P_TRAY_SHUTTLE_Y_SEND))
				return FALSE;
		}
		else if (P_TRAY_PUSHER_X_END_TOLERANCE == cmd)
			return FALSE;

		if (!IsCylSafetyPos(cylTrayShuttleFtStopFb, Cylinder::cylinderBWD) ||
			!IsCylSafetyPos(cylTrayShuttleRrStopFb, Cylinder::cylinderBWD))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbOpenZCanMove(int cmd)
{
	AxisList mtNo = mtMbbOpenZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (!IsMtSafetyPos(mtTrayPusherX, P_TRAY_PUSHER_X_READY))
			return FALSE;

		if (P_MBB_OPEN_Z_PICK == cmd)
		{
			if (!IsCylSafetyPos(cylMbbOpenTopGuideFb, Cylinder::cylinderBWD) ||
				!IsCylSafetyPos(cylMbbOpenTopClampUd, Cylinder::cylinderUP))
				return FALSE;
		}
		else if (P_MBB_OPEN_Z_SLOW_UP_OFFSET == cmd || P_MBB_OPEN_Z_SLOW_DN_OFFSET == cmd)
			return FALSE;
	}

	return TRUE;
}

BOOL FoamSendElevZCanMove(int cmd)
{
	AxisList mtNo = mtFoamSendElevZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_ELEV_Z_TOP == cmd || P_ELEV_Z_PITCH == cmd)
		{
			if (!IsMtSafetyPos(mtFoamFlipT, P_FOAM_FLIP_T_SEND))
				return FALSE;

			if (!IsCylSafetyPos(cylFoamSendElevLtAlignFb, Cylinder::cylinderBWD) ||
				!IsCylSafetyPos(cylFoamSendElevRtAlignFb, Cylinder::cylinderBWD))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL FoamTurnTCanMove(int cmd)
{
	AxisList mtNo = mtFoamFlipT;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (!NV.ndm[ltCobotFlipCanMove])
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (P_FOAM_FLIP_T_PICK == cmd)
		{
			if (!IsMtRdy(mtNo) || !IsMtRdy(mtFoamSendElevZ))
				return FALSE;

			if (!IsCylSafetyPos(cylFoamSendElevLtAlignFb, Cylinder::cylinderFWD) ||
				!IsCylSafetyPos(cylFoamSendElevRtAlignFb, Cylinder::cylinderFWD))
				return FALSE;
		}
		else if (P_FOAM_FLIP_T_SEND == cmd)
		{
			if (!IsCylSafetyPos(cylFoamSendElevLtAlignFb, Cylinder::cylinderBWD) ||
				!IsCylSafetyPos(cylFoamSendElevRtAlignFb, Cylinder::cylinderBWD))
				return FALSE;
		}
	}

	return TRUE;
}

BOOL FoamRecvElevZCanMove(int cmd)
{
	AxisList mtNo = mtFoamRecvElevZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_ELEV_Z_TOP == cmd || P_ELEV_Z_PITCH == cmd)
		{
			if (!MT[mtFoamPkX].m_state.isHome || !MT[mtFoamPkY].m_state.isHome ||
				(MT[mtFoamPkX].InPos(P_FOAM_PK_X_LT_ELEV_PLACE) && MT[mtFoamPkY].InPos(P_FOAM_PK_Y_LT_ELEV_PLACE)))
			{
				if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL FoamPartialElevCanMove(int cmd)
{
	AxisList mtNo = mtFoamPartialElevZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;
	
	// TODO: Cobot Safety Check
	//if (!NV.ndm[ltCobotPartialElevCanMove])
	//	return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_ELEV_Z_TOP == cmd || P_ELEV_Z_PITCH == cmd)
		{
			if (!MT[mtFoamPkX].m_state.isHome || !MT[mtFoamPkY].m_state.isHome ||
				(MT[mtFoamPkX].InPos(P_FOAM_PK_X_RT_ELEV_PLACE) && MT[mtFoamPkY].InPos(P_FOAM_PK_Y_RT_ELEV_PLACE)))
			{
				if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL FoamPkXCanMove(int cmd)
{
	AxisList mtNo = mtFoamPkX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_FOAM_PK_X_LT_CST_LT_CHECK_MIX_FLIP == cmd || P_FOAM_PK_X_LT_CST_RT_CHECK_MIX_FLIP == cmd)
		{
			if (!IsMtSafetyIdx(mtFoamPkZ, P_FOAM_PK_Z_READY, P_FOAM_PK_Z_LT_CST_CHECK_MIX_FLIP))
				return FALSE;
		}
		if (P_FOAM_PK_X_RT_CST_LT_CHECK_MIX_FLIP == cmd || P_FOAM_PK_X_RT_CST_RT_CHECK_MIX_FLIP == cmd)
		{
			if (!IsMtSafetyIdx(mtFoamPkZ, P_FOAM_PK_Z_READY, P_FOAM_PK_Z_RT_CST_CHECK_MIX_FLIP))
				return FALSE;
		}
	}
	else
	{
		if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
			return FALSE;
	}

	return TRUE;
}

BOOL FoamPkYCanMove(int cmd)
{
	AxisList mtNo = mtFoamPkY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_FOAM_PK_Y_LT_CST_LT_CHECK_MIX_FLIP == cmd || P_FOAM_PK_Y_LT_CST_RT_CHECK_MIX_FLIP == cmd)
		{
			if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY, P_FOAM_PK_Z_LT_CST_CHECK_MIX_FLIP))
				return FALSE;
		}
		else if (P_FOAM_PK_Y_RT_CST_LT_CHECK_MIX_FLIP == cmd || P_FOAM_PK_Y_RT_CST_RT_CHECK_MIX_FLIP == cmd)
		{
			if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY, P_FOAM_PK_Z_RT_CST_CHECK_MIX_FLIP))
				return FALSE;
		}
		else if (P_FOAM_PK_Y_LT_ELEV_PLACE == cmd || P_FOAM_PK_Y_RT_ELEV_PLACE == cmd)
		{
			if (!NV.ndm[ltCobotFoamPkCanMove])
				return FALSE;

			if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
				return FALSE;
		}
		else
		{
			if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
				return FALSE;
		}
	}
	else
	{
		if (!IsMtSafetyPos(mtFoamPkZ, P_FOAM_PK_Z_READY))
			return FALSE;
	}

	return TRUE;
}

BOOL FoamPkZCanMove(int cmd)
{
	AxisList mtNo = mtFoamPkZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		// TODO: Needs more thought
		//if (P_FOAM_PK_Z_LT_CST_PICK == cmd)
		//{
		//	if (!IsMtSafetyIdx(mtFoamPkX, P_FOAM_PK_X_LT_CST_PICK,
		//		P_FOAM_PK_X_LT_CST_LT_CHECK_MIX_FLIP, P_FOAM_PK_X_LT_CST_RT_CHECK_MIX_FLIP) ||
		//		!IsMtSafetyIdx(mtFoamPkY, P_FOAM_PK_Y_LT_CST_PICK,
		//		P_FOAM_PK_Y_LT_CST_LT_CHECK_MIX_FLIP, P_FOAM_PK_Y_LT_CST_RT_CHECK_MIX_FLIP))
		//		return FALSE;
		//}
		//else if (P_FOAM_PK_Z_RT_CST_PICK == cmd)
		//{
		//	if (!IsMtSafetyIdx(mtFoamPkX, P_FOAM_PK_X_RT_CST_PICK,
		//		P_FOAM_PK_X_RT_CST_LT_CHECK_MIX_FLIP, P_FOAM_PK_X_RT_CST_RT_CHECK_MIX_FLIP) ||
		//		!IsMtSafetyIdx(mtFoamPkY, P_FOAM_PK_Y_RT_CST_PICK,
		//			P_FOAM_PK_Y_RT_CST_LT_CHECK_MIX_FLIP, P_FOAM_PK_Y_RT_CST_RT_CHECK_MIX_FLIP))
		//		return FALSE;
		//}
	}

	return TRUE;
}

// RIGHT PART
BOOL RtCobotCanMove(int cmd)
{
	if (!Motor::DefaultCanMove(cmd))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (cmd == P_RT_COBOT_READY)
		{
			
		}
		else if (cmd == P_RT_COBOT_LT_LABEL_PICK)
		{
			if (!MT[mtLabelLtShuttleY].InPos(P_LABEL_SHUTTLE_Y_SEND))
				return FALSE;
			// TODO: RT COBOT SAFETY SENSOR
		}
		else if (cmd == P_RT_COBOT_RT_LABEL_PICK)
		{
			if (!MT[mtLabelRtShuttleY].InPos(P_LABEL_SHUTTLE_Y_SEND))
				return FALSE;
			// TODO: RT COBOT SAFETY SENSOR
		}
		else if (cmd == P_RT_COBOT_LT_MBB_VI || cmd == P_RT_COBOT_LT_MBB_LABEL_ATTACH_VI)
		{
			
		}
		else if (cmd == P_RT_COBOT_RT_MBB_VI || cmd == P_RT_COBOT_RT_MBB_LABEL_ATTACH_VI)
		{
			
		}
		else if (cmd == P_RT_COBOT_LT_MBB_LABEL_ATTACH || cmd == P_RT_COBOT_LT_MBB_PICK)
		{
			if (!MT[mtMbbLtElevZ].IsRdy())
				return FALSE;
		}
		else if (cmd == P_RT_COBOT_RT_MBB_LABEL_ATTACH || cmd == P_RT_COBOT_RT_MBB_PICK)
		{
			if (!MT[mtMbbRtElevZ].IsRdy())
				return FALSE;
		}
	}
	else
	{

	}

	return TRUE;
}

BOOL MbbShuttleYCanMove(int cmd)
{
	AxisList mtNo = mtMbbShuttleY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (!IsMtSafetyPos(mtMbbOpenZ, P_MBB_OPEN_Z_READY))
		return FALSE;

	if (!IsMtRdy(mtMbbShuttleConv))
		return FALSE;

	if (!IsCylSafetyPos(cylSealBtmGuideUd, Cylinder::cylinderDOWN) ||
		!IsCylSafetyPos(cylSealTopGuideUd, Cylinder::cylinderUP) ||
		!IsCylSafetyPos(cylSealerBtmUd, Cylinder::cylinderDOWN) ||
		!IsCylSafetyPos(cylSealerTopFtUd, Cylinder::cylinderUP) ||
		!IsCylSafetyPos(cylSealerTopRrUd, Cylinder::cylinderUP))
		return FALSE;

	// TODO: Cobot Safety Check
	if (!NV.ndm[rtCobotMbbShuttleCanMove])
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}
BOOL MbbShuttleConvCanMove(int cmd)
{
	AxisList mtNo = mtMbbShuttleConv;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo) || !IsMtRdy(mtMbbShuttleY))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbQcYCanMove(int cmd)
{
	AxisList mtNo = mtMbbQcViY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbFoldXCanMove(int cmd)
{
	AxisList mtNo = mtMbbFoldPkX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (P_MBB_FOLD_PK_X_ALIGN == cmd || P_MBB_FOLD_PK_X_CLAMP == cmd || P_MBB_FOLD_PK_X_LEFT_TURN_CLAMP == cmd)
		{
			if (!IsMtSafetyPos(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY, P_MBB_FOLD_PK_Z_PICK))
				return FALSE;
		}
		else if (P_MBB_FOLD_PK_X_RIGHT_ATTACH_START == cmd || P_MBB_FOLD_PK_X_RIGHT_ATTACH_END == cmd || P_MBB_FOLD_PK_X_RIGHT_CLAMP == cmd)
		{
			if (!IsMtSafetyPos(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY, P_MBB_FOLD_PK_Z_RIGHT_ATTACH))
				return FALSE;
		}
		else
		{
			if (!IsMtSafetyPos(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
				return FALSE;
		}
	}
	else
	{
		if (!IsMtSafetyPos(mtMbbFoldPkZ, P_MBB_FOLD_PK_Z_READY))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbFoldZCanMove(int cmd)
{
	AxisList mtNo = mtMbbFoldPkZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo) || !IsMtRdy(mtMbbFoldPkX))
			return FALSE;

		if (P_MBB_FOLD_PK_Z_PICK == cmd)
		{
			if (!IsMtSafetyPos(mtMbbFoldPkX, P_MBB_FOLD_PK_X_PICK, P_MBB_FOLD_PK_X_LEFT_TURN))
				return FALSE;
		}
		else if (P_MBB_FOLD_PK_Z_PLACE == cmd)
		{
			if (!IsMtSafetyPos(mtMbbFoldPkX, P_MBB_FOLD_PK_X_PLACE))
				return FALSE;

			if (IsSensorAOff(iSmema13Safety))
				return FALSE;
		}
		else if (P_MBB_FOLD_PK_Z_SLOW_PICK_OFFSET == cmd ||
			P_MBB_FOLD_PK_Z_SLOW_PLACE_OFFSET == cmd)
			return FALSE;
	}

	return TRUE;
}

BOOL LabelLtShuttleYCanMove(int cmd)
{
	AxisList mtNo = mtLabelLtShuttleY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	// TODO: Cobot Safety Check
	if (IsNdmOff(ltCobotLabelShuttleCanMove) || IsNdmOff(rtCobotLabelShuttleCanMove))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL LabelRtShuttleYCanMove(int cmd)
{
	AxisList mtNo = mtLabelRtShuttleY;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	// TODO: Cobot Safety Check
	if (IsNdmOff(ltCobotLabelShuttleCanMove) || IsNdmOff(rtCobotLabelShuttleCanMove))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL LabelLaserXCanMove(int cmd)
{
	AxisList mtNo = mtLabelLaserX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL LabelLtFeedCanMove(int cmd)
{
	AxisList mtNo = mtLabelLtFeed;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL LabelRtFeedCanMove(int cmd)
{
	AxisList mtNo = mtLabelRtFeed;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbLtElevZCanMove(int cmd)
{
	AxisList mtNo = mtMbbLtElevZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	// TODO: Cobot Safety Check
	//if (IsNdmOff(rtCobotMbbElevCanMove))
	//	return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (!IsCylSafetyPos(cylLtMbbCstFb, Cylinder::cylinderBWD))
			return FALSE;
	}
	
	return TRUE;
}

BOOL MbbRtElevZCanMove(int cmd)
{
	AxisList mtNo = mtMbbRtElevZ;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	//if (!ROBOT[robotLt].m_isHome)
	//	return FALSE;

	// TODO: Cobot Safety Check
	//if (IsNdmOff(rtCobotMbbElevCanMove))
	//	return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;

		if (!IsCylSafetyPos(cylRtMbbCstFb, Cylinder::cylinderBWD))
			return FALSE;
	}

	return TRUE;
}

BOOL MbbMbbQcXCanMove(int cmd)
{
	AxisList mtNo = mtMbbQcX;

	if (!Motor::DefaultCanMove(cmd) || !CommonCanMove(mtNo))
		return FALSE;

	if (HOME_CANMOVE != cmd)
	{
		if (!IsMtRdy(mtNo))
			return FALSE;
	}

	return TRUE;
}

// Cylinder
BOOL cylInPkOcCanMove(int dir, int index)
{
	return TRUE;
}


// Program Optimization (프로그램 최적화) // Message PopUp & Check
BOOL CommonCanMove(int mtNo)
{
	if (MT[mtNo].isVirtualSW || MT[mtNo].isVirtualHW)
		return TRUE;

	if (!MT[mtNo].m_state.isServoOn)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%d : %s] Motor is Not Servo On.",
			MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL IsMtRdy(int mtNo)
{
	if (!MT[mtNo].IsRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%02d : %s]\n Not Ready.", MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL IsMtSafetyPos(int mtNo, int cmd1, int cmd2, int cmd3, int cmd4, int cmd5, int cmd6, int cmd7, int cmd8, int cmd9, int cmd10)
{
	if (!MT[mtNo].IsRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%02d : %s]\n Not Ready.", MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	bool bSafetyPos = false;
	if (cmd10 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3) ||
			MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5) || MT[mtNo].InPos(cmd6) ||
			MT[mtNo].InPos(cmd7) || MT[mtNo].InPos(cmd8) || MT[mtNo].InPos(cmd9) ||
			MT[mtNo].InPos(cmd10))
			bSafetyPos = true;
	}
	else if (cmd9 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3) ||
			MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5) || MT[mtNo].InPos(cmd6) ||
			MT[mtNo].InPos(cmd7) || MT[mtNo].InPos(cmd8) || MT[mtNo].InPos(cmd9))
			bSafetyPos = true;
	}
	else if (cmd8 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3) ||
			MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5) || MT[mtNo].InPos(cmd6) ||
			MT[mtNo].InPos(cmd7) || MT[mtNo].InPos(cmd8))
			bSafetyPos = true;
	}
	else if (cmd7 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3) ||
			MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5) || MT[mtNo].InPos(cmd6) ||
			MT[mtNo].InPos(cmd7))
			bSafetyPos = true;
	}
	else if (cmd6 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3) ||
			MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5) || MT[mtNo].InPos(cmd6))
			bSafetyPos = true;
	}
	else if (cmd5 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) ||
			MT[mtNo].InPos(cmd3) || MT[mtNo].InPos(cmd4) || MT[mtNo].InPos(cmd5))
			bSafetyPos = true;
	}
	else if (cmd4 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) ||
			MT[mtNo].InPos(cmd3) || MT[mtNo].InPos(cmd4))
			bSafetyPos = true;
	}
	else if (cmd3 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2) || MT[mtNo].InPos(cmd3))
			bSafetyPos = true;
	}
	else if (cmd2 != -1)
	{
		if (MT[mtNo].InPos(cmd1) || MT[mtNo].InPos(cmd2))
			bSafetyPos = true;
	}
	else if (cmd1 != -1)
	{
		if (MT[mtNo].InPos(cmd1))
			bSafetyPos = true;
	}

	if (!bSafetyPos)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%02d : %s]\n Not Safety Position.",
			MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL IsMtSafetyIdx(int mtNo, int cmd1, int cmd2, int cmd3, int cmd4,
	int cmd5, int cmd6, int cmd7, int cmd8, int cmd9, int cmd10)
{
	if (!MT[mtNo].IsRdy())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%02d : %s]\n Not Ready.", MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	bool bSafetyPos = false;
	if (MT[mtNo].IsRdy(cmd1) || MT[mtNo].IsRdy(cmd2) || MT[mtNo].IsRdy(cmd3) ||
		MT[mtNo].IsRdy(cmd4) || MT[mtNo].IsRdy(cmd5) || MT[mtNo].IsRdy(cmd6) ||
		MT[mtNo].IsRdy(cmd7) || MT[mtNo].IsRdy(cmd8) || MT[mtNo].IsRdy(cmd9) ||
		MT[mtNo].IsRdy(cmd10))
		bSafetyPos = true;

	if (!bSafetyPos)
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%02d : %s]\n Not Safety Position.",
			MT[mtNo].m_no, MT[mtNo].m_name.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL IsCylSafetyPos(int pmNo, int pmPos)
{
	if (pmPos != CYL[pmNo].GetPos())
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"[%s]\n Not Safety Position.", CYL[pmNo].m_name.c_str());
		return FALSE;
	}

	return TRUE;
}

BOOL IsSensorAOn(INT32 iono, DWORD dwDelayTime)
{
	if (AOn((enDI)iono, dwDelayTime))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"A-Contact Sensor\n [X%04d] is On.", iono);
		return TRUE;
	}

	return FALSE;
}

BOOL IsSensorAOff(INT32 iono, DWORD dwDelayTime)
{
	if (AOff((enDI)iono, dwDelayTime))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"A-Contact Sensor\n [X%04d] is Off.", iono);
		return TRUE;
	}

	return FALSE;
}

BOOL IsSensorBOn(INT32 iono, DWORD dwDelayTime)
{
	if (BOn((enDI)iono, dwDelayTime))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"B-Contact Sensor\n [X%04d] is On.", iono);
		return TRUE;
	}

	return FALSE;
}

BOOL IsSensorBOff(INT32 iono, DWORD dwDelayTime)
{
	if (BOff((enDI)iono, dwDelayTime))
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"B-Contact Sensor\n [X%04d] is Off.", iono);
		return TRUE;
	}

	return FALSE;
}

BOOL IsNdmOn(NDmList ndmno)
{
	if (NV.ndm[ndmno])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"NDM \n [X%04d] is On.", ndmno);
		return TRUE;
	}

	return FALSE;
}

BOOL IsNdmOff(NDmList ndmno)
{
	if (!NV.ndm[ndmno])
	{
		MSGNOTICE.Set(MSG_CANMOVE, "CLOSE", NULL, NULL, NULL,
			"NDM\n [X%04d] is Off.", ndmno);
		return TRUE;
	}

	return FALSE;
}