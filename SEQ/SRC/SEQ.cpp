#include "..\Includes.h"

void MainSequence(void)
{
	bool onceCycle = false;
	int	cycleCnt = 0;
	double maxCycleTime = 0;
	CTimer tmOneTime = CTimer(TRUE);
	CTimer tmAvgTime = CTimer(TRUE);
	while (!CMain::isClose)
	{
		CTimer::Update();
		CSysTm::Update();
		OPR.isDryRun |= OPR.isNoDevice;
		OPR.isDebugMode = NV.use[useDebugMode];
		WMX.Update();
		LASERIO.Update();
		DryRunProcess();
		UpdateForMMI();

		for (int i = 0; i < robotMAX; i++)
		{
			RobotList robotNo = RobotList(i);
			ROBOT[robotNo].Update();
		}
		for (int i = 0; i < cylinderMAX; i++)
		{
			CylinderList cyno = CylinderList(i);
			CYL[cyno].Update();
		}
		for (int i = 0; i < VI_MAX; i++)
		{
			VisionList vino = VisionList(i);
			VI[vino].Update();
		}
		for (int i = 0; i < vacuumMAX; i++)
		{
			VacuumList vcno = VacuumList(i);
			VC[vcno].Update();
		}
		for (int i = 0; i < conveyorMAX; i++)
		{
			ConveyorList Cvno = ConveyorList(i);
			CONV[Cvno].Update();
		}
		for (int i = 0; i < laserMAX; i++)
		{
			LaserList lano = LaserList(i);
			if (/*OPR.isDryRun || */LAS[lano].isVirtual || NOT_DEFINED == LAS[lano].m_no)
				continue;

			if (NV.use[useLaserLabelPrint] && LAS[lano].IsError())
				LAS[lano].m_isErrorStop = true;
			else
				LAS[lano].m_isErrorStop = false;

			LAS[lano].Update();
		}
		for (int i = 0; i < antiCollisionMAX; i++)
		{
			AntiCollisionList acno = AntiCollisionList(i);
			ANTICOLLISION[acno].Update();
		}
		for (int i = 0; i < antiCollisionCircleMAX; i++)
		{
			AntiCollisionCircleList acno = AntiCollisionCircleList(i);
			ANTICOLLISIONCIRCLE[acno].Update();
		}

		SEALLIB.Update();

		AntiCollisionF();

		MSGNOTICE.Update();
		UPDATEMMI.Update();

		IPCMMI.Comm();
		IPCLASER.Comm();

		ErrorProcess();
		if (!OPR.isDebugMode)
		{
			LAMPBUZZER.Run();
			OPBTN.Run();
			TENKEYOPR.Jog();
			CalUPH();

			if (!OPR.isEmg && !OPR.isWrongParam)
			{
				CommonSequence();
				ManualSequence();
				AutoSequence();
			}
		}
		LotEndProcess();

		SMESSAGE.Send();
		for (int i = 0; i < logMAX; i++)
		{
			LogList lgno = LogList(i);
			LOG[lgno].Update();
		}

		NV_SHARED_MEM.Update();
		NV_MEMORY.Update();
		NV_TRACEABILITY.Update();
		NV_STAGE_DATA.Update();
		NV_STAGE_CAL.Update();
		NV_GLASS_CAL.Update();
		NV_TRAY_PART_DATA.Update();
		NV_COBOT_PART_DATA.Update();
		NV_FOAM_PART_DATA.Update();
		NV_LABEL_PART_DATA.Update();
		NV_MBB_PART_DATA.Update();
		NV_ROBOT.Update();
		NV_FIRE_DETECTOR_DATA.Update();
		NV_SEQ_VERSION.Update();

		Sleep(1);
		cycleCnt++;

		double dtmOneTime = tmOneTime.Elapsed();
		if (maxCycleTime < dtmOneTime)
			maxCycleTime = dtmOneTime;

		if (10000 <= cycleCnt)
		{
			double mSec = tmAvgTime.Elapsed() / 10000.;
			if (onceCycle)
			{
				LOG[logSYS].Message("main loop avg time " +	ToString(mSec, 3) +
					" msec, max time " + ToString(maxCycleTime, 3) + " msec, 1-loop time during 10000-loop");
			}

			onceCycle = 1;
			cycleCnt = 0;
			maxCycleTime = 0.;
			tmAvgTime.Reset();
		}
		tmOneTime.Reset();
		if (0)
		{
			SaveMotorRecipe();
		}
	}

	for (int i = 0; i < logMAX; i++)
	{
		LogList lgno = LogList(i);
		LOG[lgno].EndEvent();
	}

	if (NULL != CMain::Event)
		SetEvent(CMain::Event);

	CMain::isClose = true;
}

MesureTime MESURETIME[MAX_MESURETIME_CNT];

CNvm NV_TRACEABILITY = CNvm(L"TRACEABILITY", sizeof(TraceAbility));
CNvm NV_GLASS_CAL = CNvm(L"GLASS_CAL", sizeof(GlassCal) * IPC_MMI::maxLaserIndex);
CNvm NV_STAGE_CAL = CNvm(L"STAGE_CAL", sizeof(StageCal) * IPC_MMI::maxLaserIndex);
CNvm NV_STAGE_DATA = CNvm(L"STAGE_DATA", sizeof(StageData) * MAX_STAGE);
CNvm NV_TRAY_PART_DATA = CNvm(L"TRAY_PART_DATA", sizeof(IPC_MMI::TrayPartData) * nvTrayPartMax);
CNvm NV_COBOT_PART_DATA = CNvm(L"COBOT_PART_DATA", sizeof(IPC_MMI::CobotPartData) * nvCobotPartMax);
CNvm NV_FOAM_PART_DATA = CNvm(L"FOAM_PART_DATA", sizeof(IPC_MMI::FoamPartData) * nvFoamPartMax);
CNvm NV_LABEL_PART_DATA = CNvm(L"LABEL_PART_DATA", sizeof(IPC_MMI::LabelPartData) * nvLabelPartMax);
CNvm NV_MBB_PART_DATA = CNvm(L"MBB_PART_DATA", sizeof(IPC_MMI::MbbPartData) * nvMbbPartMax);
CNvm NV_ROBOT = CNvm(L"ROBOT_TABLE", sizeof(IPC_MMI::RobotTable) * 2);
CNvm NV_FIRE_DETECTOR_DATA = CNvm(L"FIRE_DETECTOR_DATA", sizeof(IPC_MMI::FireDetector) * nvFireDetectorMax);
CNvm NV_SEQ_VERSION = CNvm(L"SEQ_VERSION", sizeof(IPC_MMI::SeqVersion));

TraceAbility& TRACEABILITY = *(TraceAbility*)NV_TRACEABILITY.GetAddr();
GlassCal(&GLASS_CAL)[IPC_MMI::maxLaserIndex] = *(GlassCal(*)[IPC_MMI::maxLaserIndex])NV_GLASS_CAL.GetAddr();
StageCal(&STAGE_CAL)[IPC_MMI::maxLaserIndex] = *(StageCal(*)[IPC_MMI::maxLaserIndex])NV_STAGE_CAL.GetAddr();
StageData(&STAGE_DATA)[MAX_STAGE] = *(StageData(*)[MAX_STAGE])NV_STAGE_DATA.GetAddr();
IPC_MMI::TrayPartData(&TRAY_PART_DATA)[nvTrayPartMax] = *(IPC_MMI::TrayPartData(*)[nvTrayPartMax])NV_TRAY_PART_DATA.GetAddr();
IPC_MMI::CobotPartData(&COBOT_PART_DATA)[nvCobotPartMax] = *(IPC_MMI::CobotPartData(*)[nvCobotPartMax])NV_COBOT_PART_DATA.GetAddr();
IPC_MMI::FoamPartData(&FOAM_PART_DATA)[nvFoamPartMax] = *(IPC_MMI::FoamPartData(*)[nvFoamPartMax])NV_FOAM_PART_DATA.GetAddr();
IPC_MMI::LabelPartData(&LABEL_PART_DATA)[nvLabelPartMax] = *(IPC_MMI::LabelPartData(*)[nvLabelPartMax])NV_LABEL_PART_DATA.GetAddr();
IPC_MMI::MbbPartData(&MBB_PART_DATA)[nvMbbPartMax] = *(IPC_MMI::MbbPartData(*)[nvMbbPartMax])NV_MBB_PART_DATA.GetAddr();
IPC_MMI::RobotTable(&ROBOT_TABLE)[2] = *(IPC_MMI::RobotTable(*)[2])NV_ROBOT.GetAddr();
IPC_MMI::FireDetector(&FIRE_DETECTOR_DATA)[nvFireDetectorMax] = *(IPC_MMI::FireDetector(*)[nvFireDetectorMax])NV_FIRE_DETECTOR_DATA.GetAddr();
IPC_MMI::SeqVersion& SEQ_VERSION = *(IPC_MMI::SeqVersion*)NV_SEQ_VERSION.GetAddr();
