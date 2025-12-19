#include "Parser.h"

void InitIndexNameAxis()
{
	SetIndexName<PosOfInPkX, P_IN_PK_X_MAX>(mtInPkX);
	SetIndexName<PosOfInPkZ, P_IN_PK_Z_MAX>(mtInPkZ);
	SetIndexName<PosOfTrayShuttleY, P_TRAY_SHUTTLE_Y_MAX>(mtTrayShuttleY);
	SetIndexName<PosOfTrayPusherX, P_TRAY_PUSHER_X_MAX>(mtTrayPusherX);
	SetIndexName<PosOfMbbOpenZ, P_MBB_OPEN_Z_MAX>(mtMbbOpenZ);
	SetIndexName<PosOfElevZ, P_ELEV_Z_MAX>(mtFoamSendElevZ);
	SetIndexName<PosOfFoamTurnT, P_FOAM_FLIP_T_MAX>(mtFoamFlipT);
	SetIndexName<PosOfElevZ, P_ELEV_Z_MAX>(mtFoamRecvElevZ);
	SetIndexName<PosOfElevZ, P_ELEV_Z_MAX>(mtFoamPartialElevZ);
	SetIndexName<PosOfFoamPkX, P_FOAM_PK_X_MAX>(mtFoamPkX);
	SetIndexName<PosOfFoamPkY, P_FOAM_PK_Y_MAX>(mtFoamPkY);
	SetIndexName<PosOfFoamPkY, P_FOAM_PK_Y_MAX>(mtFoamPkY2);
	SetIndexName<PosOfFoamPkZ, P_FOAM_PK_Z_MAX>(mtFoamPkZ);
	SetIndexName<PosOfMbbShuttleY, P_MBB_SHUTTLE_Y_MAX>(mtMbbShuttleY);
	SetIndexName<PosOfMbbShuttleConv, P_MBB_SHUTTLE_T_MAX>(mtMbbShuttleConv);
	SetIndexName<PosOfMbbQcViY, P_MBB_QC_VI_Y_MAX>(mtMbbQcViY);
	SetIndexName<PosOfMbbFoldPkX, P_MBB_FOLD_PK_X_MAX>(mtMbbFoldPkX);
	SetIndexName<PosOfMbbFoldPkZ, P_MBB_FOLD_PK_Z_MAX>(mtMbbFoldPkZ);
	SetIndexName<PosOfLabelShttleY, P_LABEL_SHUTTLE_Y_MAX>(mtLabelLtShuttleY);
	SetIndexName<PosOfLabelShttleY, P_LABEL_SHUTTLE_Y_MAX>(mtLabelRtShuttleY);
	SetIndexName<PosOfLabelLaserX, P_LASER_X_MAX>(mtLabelLaserX);
	SetIndexName<PosOfLabelFeed, P_LABEL_T_MAX>(mtLabelLtFeed);
	SetIndexName<PosOfLabelFeed, P_LABEL_T_MAX>(mtLabelRtFeed);
	SetIndexName<PosOfElevZ, P_ELEV_Z_MAX>(mtMbbLtElevZ);
	SetIndexName<PosOfElevZ, P_ELEV_Z_MAX>(mtMbbRtElevZ);
	SetIndexName<PosOfMbbQcX, P_MBB_QC_X_MAX>(mtMbbQcX);
}

void InitIndexNameRobot()
{
	SetIndexName<RainbowRobotics::PosOfLtCobot, RainbowRobotics::P_LT_COBOT_MAX>(RainbowRobotics::robotLt);
	SetIndexName<RainbowRobotics::PosOfRtCobot, RainbowRobotics::P_RT_COBOT_MAX>(RainbowRobotics::robotRt);

}
void InitIndexNameCylinder()
{
	SetIndexName<PosOfCylInPkOc, IN_PK_OC_MAX>(cylInPkOc);
	SetIndexName<PosOfCylBtmFoamTableUd, BTM_FOAM_TABLE_UD_MAX>(cylBtmFoamTableUd);
	SetIndexName<PosOfCylBtmFoamStopUd, BTM_FOAM_STOP_UD_MAX>(cylBtmFoamStopUd);
	SetIndexName<PosOfCylBtmFoamFtClampOc, BTM_FOAM_FT_CLAMP_OC_MAX>(cylBtmFoamFtClampOc);
	SetIndexName<PosOfCylBtmFoamRrClampOc, BTM_FOAM_RR_CLAMP_OC_MAX>(cylBtmFoamRrClampOc);
	SetIndexName<PosOfCylTopFoamStopUd, TOP_FOAM_STOP_UD_MAX>(cylTopFoamStopUd);
	SetIndexName<PosOfCylTrayShuttleNozzleUd, TRAY_SHUTTLE_NOZZLE_UD_MAX>(cylTrayShuttleNozzleUd);
	SetIndexName<PosOfCylTrayShuttleBridgeUd, TRAY_SHUTTLE_BRIDGE_UD_MAX>(cylTrayShuttleBridgeUd);
	SetIndexName<PosOfcylTrayShuttleBridgeFb, TRAY_SHUTTLE_BRIDGE_FB_MAX>(cylTrayShuttleBridgeFb);
	SetIndexName<PosOfCylTrayShuttleFtStopFb, TRAY_SHUTTLE_FT_STOP_FB_MAX>(cylTrayShuttleFtStopFb);
	SetIndexName<PosOfCylTrayShuttleRrStopFb, TRAY_SHUTTLE_RR_STOP_FB_MAX>(cylTrayShuttleRrStopFb);
	SetIndexName<PosOfCylTrayShuttleFtMaskUd, TRAY_SHUTTLE_FT_MASK_UD_MAX>(cylTrayShuttleFtMaskUd);
	SetIndexName<PosOfCylTrayShuttleRrMaskUd, TRAY_SHUTTLE_RR_MASK_UD_MAX>(cylTrayShuttleRrMaskUd);
	//SetIndexName<PosOfCylMbbOpenBtmStopUd, MBB_OPEN_BTM_STOP_UD_MAX>(cylMbbOpenBtmStopUd);
	SetIndexName<PosOfCylMbbOpenTopGuideFb, MBB_OPEN_TOP_GUIDE_FB_MAX>(cylMbbOpenTopGuideFb);
	SetIndexName<PosOfCylMbbOpenTopVacUd, MBB_OPEN_TOP_VAC_UD_MAX>(cylMbbOpenTopVacUd);
	SetIndexName<PosOfCylMbbOpenTopClampUd, MBB_OPEN_TOP_CLAMP_UD_MAX>(cylMbbOpenTopClampUd);
	SetIndexName<PosOfCylSealNozzleFb, SEAL_NOZZLE_FB_MAX>(cylSealNozzleFb);
	SetIndexName<PosOfCylSealMbbOpenGuideFtOc, SEAL_MBB_OPEN_GUIDE_FT_OC_MAX>(cylSealMbbOpenGuideFtOc);
	SetIndexName<PosOfCylSealMbbOpenGuideRrOc, SEAL_MBB_OPEN_GUIDE_RR_OC_MAX>(cylSealMbbOpenGuideRrOc);
	SetIndexName<PosOfCylSealNozzleMfcOc, SEAL_NOZZLE_MFC_OC_MAX>(cylSealNozzleMfcN2Oc);
	SetIndexName<PosOfCylSealBtmGuideUd, SEAL_BTM_GUIDE_UD_MAX>(cylSealBtmGuideUd);
	SetIndexName<PosOfCylSealerBtmUd, SEALER_BTM_UD_MAX>(cylSealerBtmUd);
	SetIndexName<PosOfCylSealerTopFtUd, SEALER_TOP_FT_UD_MAX>(cylSealerTopFtUd);
	SetIndexName<PosOfCylSealerTopRrUd, SEALER_TOP_RR_UD_MAX>(cylSealerTopRrUd);
	SetIndexName<PosOfCylSealTopGuideUd, SEAL_TOP_GUIDE_UD_MAX>(cylSealTopGuideUd);
	SetIndexName<PosOfCylLtCobotLabelPkUd, LT_COBOT_LABEL_PK_UD_MAX>(cylLtCobotLabelPkUd);
	SetIndexName<PosOfCylLtCobotLabelPkTurnFb, LT_COBOT_LABEL_PK_TURN_FB_MAX>(cylLtCobotLabelPkTurnFb);
	SetIndexName<PosOfCylFoamRecvElevLtAlignFb, FOAM_RECV_ELEV_LT_ALIGN_FB_MAX>(cylFoamRecvElevLtAlignFb);
	SetIndexName<PosOfCylFoamRecvElevRrAlignFb, FOAM_RECV_ELEV_RR_ALIGN_FB_MAX>(cylFoamRecvElevRrAlignFb);
	SetIndexName<PosOfCylFoamRecvElevLtStopFb, FOAM_RECV_ELEV_LT_STOP_FB_MAX>(cylFoamRecvElevLtStopFb);
	SetIndexName<PosOfCylFoamRecvElevRtStopFb, FOAM_RECV_ELEV_RT_STOP_FB_MAX>(cylFoamRecvElevRtStopFb);
	SetIndexName<PosOfCylFoamRrStackLiftUd, FOAM_RR_STACK_LIFT_UD_MAX>(cylFoamRrStackLiftUd);
	SetIndexName<PosOfCylFoamRrStackLtStopFb, FOAM_RR_STACK_LT_STOP_FB_MAX>(cylFoamRrStackLtStopFb);
	SetIndexName<PosOfCylFoamRrStackRtStopFb, FOAM_RR_STACK_RT_STOP_FB_MAX>(cylFoamRrStackRtStopFb);
	SetIndexName<PosOfCylFoamFtStackLiftUd, FOAM_FT_STACK_LIFT_UD_MAX>(cylFoamFtStackLiftUd);
	SetIndexName<PosOfCylFoamFtStackLtStopFb, FOAM_FT_STACK_LT_STOP_FB_MAX>(cylFoamFtStackLtStopFb);
	SetIndexName<PosOfCylFoamFtStackRtStopFb, FOAM_FT_STACK_RT_STOP_FB_MAX>(cylFoamFtStackRtStopFb);
	SetIndexName<PosOfCylFoamSendElevLtAlignFb, FOAM_SEND_ELEV_LT_ALIGN_FB_MAX>(cylFoamSendElevLtAlignFb);
	SetIndexName<PosOfCylFoamSendElevRtAlignFb, FOAM_SEND_ELEV_RT_ALIGN_FB_MAX>(cylFoamSendElevRtAlignFb);
	SetIndexName<PosOfCylFoamPartialElevLtAlignFb, FOAM_PARTIAL_ELEV_LT_ALIGN_FB_MAX>(cylFoamPartialElevLtAlignFb);
	SetIndexName<PosOfCylFoamPartialElevRrAlignFb, FOAM_PARTIAL_ELEV_FT_ALIGN_FB_MAX>(cylFoamPartialElevRrAlignFb);
	SetIndexName<PosOfCylMbbShuttleAlignFtFb, MBB_SHUTTLE_ALIGN_FT_FB_MAX>(cylMbbShuttleAlignFtFb);
	SetIndexName<PosOfCylMbbShuttleAlignRrFb, MBB_SHUTTLE_ALIGN_RR_FB_MAX>(cylMbbShuttleAlignRrFb);
	SetIndexName<PosOfCylMbbShuttleClampFtFb, MBB_SHUTTLE_CLAMP_FT_FB_MAX>(cylMbbShuttleClampFtFb);
	SetIndexName<PosOfCylMbbShuttleClampRrFb, MBB_SHUTTLE_CLAMP_RR_FB_MAX>(cylMbbShuttleClampRrFb);
	SetIndexName<PosOfCylQcSealedHeadBtmFtVacUd, QC_SEALED_HEAD_BTM_FT_VAC_UD_MAX>(cylQcSealedHeadBtmFtVacUd);
	SetIndexName<PosOfCylQcSealedHeadBtmRrVacUd, QC_SEALED_HEAD_BTM_RR_VAC_UD_MAX>(cylQcSealedHeadBtmRrVacUd);
	SetIndexName<PosOfCylQcSealedHeadTopFtClampUd, QC_SEALED_HEAD_TOP_FT_CLAMP_UD_MAX>(cylQcSealedHeadTopFtClampUd);
	SetIndexName<PosOfCylQcSealedHeadTopRrClampUd, QC_SEALED_HEAD_TOP_RR_CLAMP_UD_MAX>(cylQcSealedHeadTopRrClampUd);
	SetIndexName<PosOfCylQcSealedTrayStopUd, QC_SEALED_TRAY_STOP_UD_MAX>(cylQcSealedTrayStopUd);
	SetIndexName<PosOfCylQcSealedTrayAlignFtFb, QC_SEALED_TRAY_ALIGN_FT_FB_MAX>(cylQcSealedTrayAlignFtFb);
	SetIndexName<PosOfCylQcSealedTrayAlignRrFb, QC_SEALED_TRAY_ALIGN_RR_FB_MAX>(cylQcSealedTrayAlignRrFb);
	SetIndexName<PosOfCylMbbFoldAssistUd, MBB_FOLD_ASSIST_UD_MAX>(cylMbbFoldAssistUd);
	SetIndexName<PosOfCylMbbFoldAlignFtFb, MBB_FOLD_ALIGN_FT_FB_MAX>(cylMbbFoldAlignFtFb);
	SetIndexName<PosOfCylMbbFoldAlignRrFb, MBB_FOLD_ALIGN_RR_FB_MAX>(cylMbbFoldAlignRrFb);
	SetIndexName<PosOfCylMbbFoldRtFb, MBB_FOLD_RT_FB_MAX>(cylMbbFoldRtFb);
	SetIndexName<PosOfCylMbbFoldRtFtOc, MBB_FOLD_RT_FT_OC_MAX>(cylMbbFoldRtFtOc);
	SetIndexName<PosOfCylMbbFoldRtRrOc, MBB_FOLD_RT_RR_OC_MAX>(cylMbbFoldRtRrOc);
	SetIndexName<PosOfCylMbbFoldPkLtBlockTurnFb, MBB_FOLD_PK_LT_BLOCK_TURN_FB_MAX>(cylMbbFoldPkLtBlockTurnFb);
	SetIndexName<PosOfCylMbbFoldPkLtBlockFtOc, MBB_FOLD_PK_LT_BLOCK_FT_OC_MAX>(cylMbbFoldPkLtBlockFtOc);
	SetIndexName<PosOfCylMbbFoldPkLtBlockRrOc, MBB_FOLD_PK_LT_BLOCK_RR_OC_MAX>(cylMbbFoldPkLtBlockRrOc);
	SetIndexName<PosOfCylMbbFoldPkClampOc, MBB_FOLD_PK_CLAMP_OC_MAX>(cylMbbFoldPkClampOc);
	SetIndexName<PosOfCylLtLabelFeederFb, LT_LABEL_FEEDER_FB_MAX>(cylLtLabelFeederFb);
	SetIndexName<PosOfCylLtLabelFeederMountFb, LT_LABEL_FEEDER_MOUNT_FB_MAX>(cylLtLabelFeederMountFb);
	SetIndexName<PosOfCylLtLabelFeederLockFtFb, LT_LABEL_FEEDER_LOCK_FT_FB_MAX>(cylLtLabelFeederLockFtFb);
	SetIndexName<PosOfCylLtLabelFeederLockRrFb, LT_LABEL_FEEDER_LOCK_RR_FB_MAX>(cylLtLabelFeederLockRrFb);
	SetIndexName<PosOfCylLtLabelRecvUd, LT_LABEL_RECV_UD_MAX>(cylLtLabelRecvUd);
	SetIndexName<PosOfCylLtLabelRecvFb, LT_LABEL_RECV_FB_MAX>(cylLtLabelRecvFb);
	SetIndexName<PosOfCylLtLabelRecvEjectPinLtUd, LT_LABEL_RECV_EJECT_PIN_LT_UD_MAX>(cylLtLabelRecvEjectPinLtUd);
	SetIndexName<PosOfCylLtLabelRecvEjectPinRtUd, LT_LABEL_RECV_EJECT_PIN_RT_UD_MAX>(cylLtLabelRecvEjectPinRtUd);
	SetIndexName<PosOfCylRtLabelFeederFb, RT_LABEL_FEEDER_FB_MAX>(cylRtLabelFeederFb);
	SetIndexName<PosOfCylRtLabelFeederMountFb, RT_LABEL_FEEDER_MOUNT_FB_MAX>(cylRtLabelFeederMountFb);
	SetIndexName<PosOfCylRtLabelFeederLockFtFb, RT_LABEL_FEEDER_LOCK_FT_FB_MAX>(cylRtLabelFeederLockFtFb);
	SetIndexName<PosOfCylRtLabelFeederLockRrFb, RT_LABEL_FEEDER_LOCK_RR_FB_MAX>(cylRtLabelFeederLockRrFb);
	SetIndexName<PosOfCylRtLabelRecvUd, RT_LABEL_RECV_UD_MAX>(cylRtLabelRecvUd);
	SetIndexName<PosOfCylRtLabelRecvFb, RT_LABEL_RECV_FB_MAX>(cylRtLabelRecvFb);
	SetIndexName<PosOfCylRtLabelRecvEjectPinLtUd, RT_LABEL_RECV_EJECT_PIN_LT_UD_MAX>(cylRtLabelRecvEjectPinLtUd);
	SetIndexName<PosOfCylRtLabelRecvEjectPinRtUd, RT_LABEL_RECV_EJECT_PIN_RT_UD_MAX>(cylRtLabelRecvEjectPinRtUd);
	SetIndexName<PosOfCylPowerMeterOc, POWER_METER_OC_MAX>(cylPowerMeterOc);
	SetIndexName<PosOfCylLabelQcViFb, LABEL_QC_VI_FB_MAX>(cylLabelQcViFb);
	SetIndexName<PosOfCylRtCobotLabelPkUd, RT_COBOT_LABEL_PK_UD_MAX>(cylRtCobotLabelPkUd);
	SetIndexName<PosOfCylRtCobotLabelPkTurnFb, RT_COBOT_LABEL_PK_TURN_FB_MAX>(cylRtCobotLabelPkTurnFb);
	SetIndexName<PosOfCylLtMbbCstFb, LT_MBB_CST_FB_MAX>(cylLtMbbCstFb);
	SetIndexName<PosOfCylRtMbbCstFb, RT_MBB_CST_FB_MAX>(cylRtMbbCstFb);
	SetIndexName<PosOfcylMbbFoldPkLtAssistFb, MBB_FOLD_PK_LT_FB_ASSIST_MAX>(cylMbbFoldPkLtFtAssist);
	SetIndexName<PosOfcylMbbFoldPkLtAssistFb, MBB_FOLD_PK_LT_FB_ASSIST_MAX>(cylMbbFoldPkLtRrAssist);
	SetIndexName<PosOfcylSealMbbClampUd, SEAL_MBB_CLAMP_UD_MAX>(cylSealMbbClampUd);
	SetIndexName<PosOfcylSealMbbClampFb, SEAL_MBB_CLAMP_FB_MAX>(cylSealMbbClampFtFb);
	SetIndexName<PosOfcylSealMbbClampFb, SEAL_MBB_CLAMP_FB_MAX>(cylSealMbbClampRrFb);
	SetIndexName<PosOfcylSealMbbClampOc, SEAL_MBB_CLAMP_OC_MAX>(cylSealMbbClampFtOc);
	SetIndexName<PosOfcylSealMbbClampOc, SEAL_MBB_CLAMP_OC_MAX>(cylSealMbbClampRrOc);
	SetIndexName<PosOfcylMbbShuttleClampUd, MBB_SHUTTLE_CLAMP_UD_MAX>(cylMbbShuttleClampUd);
}

void InitIndexNameVacuum()
{
	SetIndexName<VacuumIndexMainAir, MAIN_AIR_MAX>(vcMainAir1);
	SetIndexName<VacuumIndexMainAir, MAIN_AIR_MAX>(vcMainAir2);
	SetIndexName<VacuumIndexMainAir, MAIN_AIR_MAX>(vcMainAir3);
	SetIndexName<VacuumIndexMainAir, MAIN_AIR_MAX>(vcMainAir4);
	SetIndexName<PosOfvcMbbOpenBtm, MBB_OPEN_BTM_MAX>(vcMbbOpenBtm);
	SetIndexName<PosOfvcMbbOpenTop, MBB_OPEN_TOP_MAX>(vcMbbOpenTop);
	SetIndexName<PosOfvcMbbOpenTop2, MBB_OPEN_TOP2_MAX>(vcMbbOpenTop2);
	SetIndexName<PosOfvcSealNozzle, SEAL_NOZZLE_MAX>(vcSealNozzle);
	SetIndexName<PosOfvcSealNozzleN2, SEAL_NOZZLE_N2_MAX>(vcSealNozzleN2);
	SetIndexName<PosOfvcLoadCellFt, LOADCELL_FT_MAX>(vcLoadCellFt);
	SetIndexName<PosOfvcLoadCellRr, LOADCELL_RR_MAX>(vcLoadCellRr);
	SetIndexName<PosOfvcLtCobotLabel, LT_COBOT_LABEL_MAX>(vcLtCobotLabel);
	SetIndexName<PosOfvcLtCobotFoam, LT_COBOT_FOAM_MAX>(vcLtCobotFoam);
	SetIndexName<PosOfvcLtCobotFoam2, LT_COBOT_FOAM2_MAX>(vcLtCobotFoam2);
	SetIndexName<PosOfvcFoamPkLaser, FOAM_LASER_MAX>(vcFoamPkLaser);
	SetIndexName<PosOfvcFoamFlip, FOAM_FLIP_MAX>(vcFoamFlip);
	SetIndexName<PosOfvcFoamFlip2, FOAM_FLIP2_MAX>(vcFoamFlip2);
	SetIndexName<PosOfvcFoamPk, FOAM_PK_MAX>(vcFoamPk);
	SetIndexName<PosOfvcFoamPk2, FOAM_PK2_MAX>(vcFoamPk2);
	SetIndexName<PosOfvcFlowLabelExhaust, FLOW_LABEL_EXHAUST_MAX>(vcFlowLabelExhaust);
	SetIndexName<PosOfvcQcSealedHead, QC_SEALED_HEAD_MAX>(vcQcSealedHead);
	SetIndexName<PosOfvcLtLabelRecv, LT_LABEL_RECV_MAX>(vcLtLabelRecv);
	SetIndexName<PosOfvcRtLabelRecv, RT_LABEL_RECV_MAX>(vcRtLabelRecv);
	SetIndexName<PosOfvcRtCobotLabel, RT_COBOT_LABEL_MAX>(vcRtCobotLabel);
	SetIndexName<PosOfvcRtCobotMbb, RT_COBOT_MBB_MAX>(vcRtCobotMbb);
	SetIndexName<PosOfvcRtCobotMbb2, RT_COBOT_MBB2_MAX>(vcRtCobotMbb2);
}

void RefactoringErrorCode(std::vector<std::pair<ErrName, std::string>> newErList,
	std::vector<std::pair<WrName, std::string>> newWrList)
{
	std::ofstream file;
	std::filesystem::path createPath = std::filesystem::current_path();
	std::filesystem::path createFile = createPath.parent_path() / "SEQ" / "DEF" / "ErrNameDef.h";
	file.open(createFile, std::ios::trunc);
	if (!file)
	{
		return;
	}
	file << "#pragma once\n";
	size_t maxNameLength = 0;
	for (const auto& item : newErList)
	{
		maxNameLength = std::max(maxNameLength, item.second.length());
	}
	for (const auto& item : newWrList)
	{
		maxNameLength = std::max(maxNameLength, item.second.length());
	}

	file << "class ErList\n";
	file << "{\n";
	file << "public:\n";
	file << "enum T\n";
	file << "{\n";
	for (const auto& item : newErList)
	{
		file << "    " << std::left << std::setw(maxNameLength + 2)  // 가장 긴 이름 + 여백
			<< item.second << "= " << item.first << ",\n";
	}
	file << "};\n";
	file << "};\n";
	file << "\n";
	file << "class WrList\n";
	file << "{\n";
	file << "public:\n";
	file << "enum T\n";
	file << "{\n";
	for (const auto& item : newWrList)
	{
		file << "    " << std::left << std::setw(maxNameLength + 2)  // 가장 긴 이름 + 여백
			<< item.second << "= " << item.first << ",\n";
	}
	file << "};\n";
	file << "};\n";
	file.close();
}