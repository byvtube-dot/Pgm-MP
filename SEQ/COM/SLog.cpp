#include "..\Includes.h"

namespace COM
{
	SMessage::SMessage()
	{

	}

	SMessage::~SMessage()
	{
		if (NULL != this->m_pSLog)
			sloglib::DestroySLog(this->m_pSLog);
		if (NULL != this->m_pSMemoryBase)
			sloglib::DestroySMemory(this->m_pSMemoryBase);

		this->m_pSLog = NULL;
		this->m_pSMemoryBase = NULL;
	}

	BOOL SMessage::Open(SYSTEMTIME* pSystemTime)
	{
		this->m_pSLog = sloglib::CreateSLog(pSystemTime);
		this->m_pSMemoryBase = sloglib::CreateSMemory("NEGRETE");
		if (this->m_pSMemoryBase->open())
			return (TRUE);

		return (FALSE);
	}

	BOOL SMessage::Recv(UINT32 milliSeconds)
	{
		sloglib::Data data;
		if (this->m_pSMemoryBase->recv(data, milliSeconds))
			return (TRUE);

		return (FALSE);
	}

	BOOL SMessage::Send(UINT32 milliSeconds)
	{
		if (m_pSLog->logOverFlow())
		{

		}

		if (!m_pSLog->logData().empty())
		{
			if (this->m_pSMemoryBase->send(m_pSLog->logData(), milliSeconds))
				return (TRUE);
		}
		return (FALSE);
	}

	void SMessage::Process(const char* deviceId, const char* eventId, const char* status,
		const char* materialId, const char* lotId, const char* recipeId,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logProcess(deviceId, eventId, status,
			materialId, lotId, recipeId,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Transfer(const char* deviceId, const char* eventId, const char* status,
		const char* materialId, const char* materialType, const char* fromDevice, const char* toDevice,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logTransfer(deviceId, eventId, status,
			materialId, materialType, fromDevice, toDevice,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Function(const char* deviceId, const char* eventId, const char* status,
		const char* materialId, const char* materialType,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logFunction(deviceId, eventId, status, materialId, materialType,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Lot(const char* deviceId, const char* eventId, const char* lotId,
		const char* recipeId, const char* carrierId,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logLot(deviceId, eventId, lotId, recipeId, carrierId,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Configure(const char* deviceId, const char* cfgId,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logConfigure(deviceId, cfgId,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Alarm(const char* deviceId, const char* eventId, const char* alarmCode, const char* status,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logAlarm(deviceId, eventId, alarmCode, status,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	void SMessage::Comm(const char* deviceId, const char* eventId,
		const char* key1, const char* value1, const char* key2, const char* value2,
		const char* key3, const char* value3, const char* key4, const char* value4,
		const char* key5, const char* value5, const char* key6, const char* value6)
	{
		if (m_pSLog->logComm(deviceId, eventId,
			key1, value1, key2, value2, key3, value3, key4, value4, key5, value5, key6, value6))
		{

		}
	}

	SBaseLog::SBaseLog()
	{
		m_prc.deviceId = m_prc.eventId = m_prc.materialId = m_prc.lotId = m_prc.recipeId = "";
		m_xfr.deviceId = m_xfr.eventId = m_xfr.materialId = m_xfr.materialType = m_xfr.fromDevice = m_xfr.toDevice = "";
		m_uniqueXFRNumber = m_uniquePRCNumber = 0;
	}

	SBaseLog::~SBaseLog()
	{

	}

	// Original
	//void SBaseLog::PRC(
	//	string prcEventId, string prcMaterialId, string prcLotId, string prcRecipeId,
	//	string xfrEventId, string xfrMaterialId, string xfrFromDevice, string xfrToDevice)
	//{
	//	if ((prcEventId != m_prc.eventId || prcLotId != m_prc.lotId || prcRecipeId != m_prc.recipeId) &&
	//		(xfrEventId != m_xfr.eventId || xfrFromDevice != m_xfr.fromDevice || xfrToDevice != m_xfr.toDevice))
	//	{
	//		End(TRUE);
	//		SetPRCEventId(prcEventId);
	//		SetPRCMaterialId(prcMaterialId);
	//		SetPRCLotId(prcLotId);
	//		SetPRCRecipeId(prcRecipeId);
	//		SetXFREventId(xfrEventId);
	//		SetXFRMaterialId(xfrMaterialId);
	//		SetXFRFromDevice(xfrFromDevice);
	//		SetXFRToDevice(xfrToDevice);
	//		Start(TRUE);
	//	}
	//}

	void SBaseLog::PRC(
		string prcEventId, string prcMaterialId, string prcLotId, string prcRecipeId)
	{
		if (prcEventId != m_prc.eventId || prcLotId != m_prc.lotId || prcRecipeId != m_prc.recipeId)
		{
			End(TRUE);
			SetPRCEventId(prcEventId);
			SetPRCMaterialId(prcMaterialId);
			SetPRCLotId(prcLotId);
			SetPRCRecipeId(prcRecipeId);
			Start(TRUE);
		}
	}

	void SBaseLog::XFR(string xfrEventId, string xfrMaterialId, string xfrFromDevice, string xfrToDevice)
	{
		if (xfrEventId != m_xfr.eventId || xfrFromDevice != m_xfr.fromDevice || xfrToDevice != m_xfr.toDevice)
		{
			End(FALSE);
			SetXFREventId(xfrEventId);
			SetXFRMaterialId(xfrMaterialId);
			SetXFRFromDevice(xfrFromDevice);
			SetXFRToDevice(xfrToDevice);
			Start(FALSE);
		}
	}

	void SBaseLog::Start(BOOL isPRC)
	{
		if (SBaseLog::UniqueNumber() >= 18446744073709551615)
			SBaseLog::UniqueNumber() = 1;

		if (isPRC && !m_uniquePRCNumber &&
			"" != m_prc.deviceId && "" != m_prc.eventId &&
			"" != m_prc.materialId && "" != m_prc.lotId && "" != m_prc.recipeId)
		{
			SMESSAGE.Process(m_prc.deviceId.c_str(), m_prc.eventId.c_str(), "START",
				m_prc.materialId.c_str(), m_prc.lotId.c_str(), m_prc.recipeId.c_str());

			m_uniquePRCNumber = SBaseLog::UniqueNumber();
		}

		if (!m_uniqueXFRNumber &&
			"" != m_xfr.deviceId && "" != m_xfr.eventId && "" != m_xfr.materialId &&
			"" != m_xfr.materialType && "" != m_xfr.fromDevice && "" != m_xfr.toDevice)
		{
			SMESSAGE.Transfer(m_xfr.deviceId.c_str(), m_xfr.eventId.c_str(), "START",
				m_xfr.materialId.c_str(), m_xfr.materialType.c_str(), m_xfr.fromDevice.c_str(), m_xfr.toDevice.c_str());

			m_uniqueXFRNumber = SBaseLog::UniqueNumber();
		}

		if (m_uniqueXFRNumber)
		{
			SBaseLog::UniqueNumber()++;
			for (int i = 0; i < m_axisList.Count(); i++)
			{
				AxisList axis = m_axisList[i];			
				SLOG[SLogList(m_no)].axisLogStart[axis] = *this;
			}
			for (int i = 0; i < m_robotList.Count(); i++)
			{
				RobotList axis = m_robotList[i];
				SLOG[SLogList(m_no)].robotLogStart[axis] = *this;
			}
			for (int i = 0; i < m_cylinderList.Count(); i++)
			{
				CylinderList cylinder = m_cylinderList[i];
				SLOG[SLogList(m_no)].cylinderLogStart[cylinder] = *this;
			}
			for (int i = 0; i < m_vacuumList.Count(); i++)
			{
				VacuumList vacuum = m_vacuumList[i];
				SLOG[SLogList(m_no)].vacuumLogStart[vacuum] = *this;
			}
			for (int i = 0; i < m_laserList.Count(); i++)
			{
				LaserList laser = m_laserList[i];
				SLOG[SLogList(m_no)].laserLogStart[laser] = *this;
			}
			for (int i = 0; i < m_visionList.Count(); i++)
			{
				VisionList vision = m_visionList[i];
				SLOG[SLogList(m_no)].visionLogStart[vision] = *this;
			}
			for (int i = 0; i < m_conveyorList.Count(); i++)
			{
				ConveyorList conveyor = m_conveyorList[i];
				SLOG[SLogList(m_no)].conveyorLogStart[conveyor] = *this;
			}
		}
	}

	void SBaseLog::End(BOOL isPRC)
	{
		if (0 == CheckUniqueXFRNumber())
		{
			if ("" != m_xfr.deviceId && "" != m_xfr.eventId && "" != m_xfr.materialId &&
				"" != m_xfr.materialType && "" != m_xfr.fromDevice && "" != m_xfr.toDevice)
			{
				SMESSAGE.Transfer(m_xfr.deviceId.c_str(), m_xfr.eventId.c_str(), "END",
					m_xfr.materialId.c_str(), m_xfr.materialType.c_str(), m_xfr.fromDevice.c_str(), m_xfr.toDevice.c_str());
			}
		}
		if (0 == CheckUniquePRCNumber(isPRC))
		{
			if ("" != m_prc.deviceId && "" != m_prc.eventId &&
				"" != m_prc.materialId && "" != m_prc.lotId && "" != m_prc.recipeId)
			{
				SMESSAGE.Process(m_prc.deviceId.c_str(), m_prc.eventId.c_str(), "END",
					m_prc.materialId.c_str(), m_prc.lotId.c_str(), m_prc.recipeId.c_str());
			}
		}
	}

	void SBaseLog::Clear()
	{
		m_prc.deviceId = m_prc.eventId = m_prc.materialId = m_prc.lotId = m_prc.recipeId = "";
		m_xfr.deviceId = m_xfr.eventId = m_xfr.materialId = m_xfr.materialType = m_xfr.fromDevice = m_xfr.toDevice = "";
		m_uniqueXFRNumber = m_uniquePRCNumber = 0;
	}

	void SBaseLog::SetPRCDeviceId(string deviceId)
	{
		m_prc.deviceId = deviceId;
	}

	void SBaseLog::SetPRCEventId(string eventId)
	{
		m_prc.eventId = eventId;
	}

	void SBaseLog::SetPRCLotId(string lotId)
	{
		m_prc.lotId = lotId;
	}

	void SBaseLog::SetPRCMaterialId(string materialId)
	{
		m_prc.materialId = materialId;
	}

	void SBaseLog::SetPRCRecipeId(string recipeId)
	{
		m_prc.recipeId = recipeId;
	}

	void SBaseLog::SetXFRDeviceId(string deviceId)
	{
		m_xfr.deviceId = deviceId;
	}

	void SBaseLog::SetXFREventId(string eventId)
	{
		m_xfr.eventId = eventId;
	}

	void SBaseLog::SetXFRMaterialId(string materialId)
	{
		m_xfr.materialId = materialId;
	}

	void SBaseLog::SetXFRMaterialType(string materialType)
	{
		m_xfr.materialType = materialType;
	}

	void SBaseLog::SetXFRFromDevice(string fromDevice)
	{
		m_xfr.fromDevice = fromDevice;
	}

	void SBaseLog::SetXFRToDevice(string toDevice)
	{
		m_xfr.toDevice = toDevice;
	}

	string SBaseLog::GetXFRDeviceId()
	{
		return m_xfr.deviceId;
	}

	string SBaseLog::GetXFREventId()
	{
		return m_xfr.eventId;
	}

	string SBaseLog::GetXFRMaterialId()
	{
		return m_xfr.materialId;
	}

	string SBaseLog::GetXFRMaterialType()
	{
		return m_xfr.materialType;
	}

	string SBaseLog::GetXFRFromDevice()
	{
		return m_xfr.fromDevice;
	}

	string SBaseLog::GetXFRToDevice()
	{
		return m_xfr.toDevice;
	}

	void SBaseLog::Add(AxisList axisNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_axisList.Count(); i++)
		{
			if (axisNo == m_axisList[i])
				return;
		}
		m_axisList.Add(axisNo);
		m_axisSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(RobotList axisNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_robotList.Count(); i++)
		{
			if (axisNo == m_robotList[i])
				return;
		}
		m_robotList.Add(axisNo);
		m_robotSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(CylinderList cylinderNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_cylinderList.Count(); i++)
		{
			if (cylinderNo == m_cylinderList[i])
				return;
		}
		m_cylinderList.Add(cylinderNo);
		m_cylinderSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(VacuumList vacuumNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_vacuumList.Count(); i++)
		{
			if (vacuumNo == m_vacuumList[i])
				return;
		}
		m_vacuumList.Add(vacuumNo);
		m_vacuumSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(ConveyorList conveyorNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_conveyorList.Count(); i++)
		{
			if (conveyorNo == m_conveyorList[i])
				return;
		}
		m_conveyorList.Add(conveyorNo);
		m_conveyorSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(LaserList laserNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_laserList.Count(); i++)
		{
			if (laserNo == m_laserList[i])
				return;
		}
		m_laserList.Add(laserNo);
		m_laserSelectiveRecording.Add(selectiveRecording);
	}

	void SBaseLog::Add(VisionList visionNo, BOOL selectiveRecording)
	{
		for (int i = 0; i < m_visionList.Count(); i++)
		{
			if (visionNo == m_visionList[i])
				return;
		}
		m_visionList.Add(visionNo);
		m_visionSelectiveRecording.Add(selectiveRecording);
	}

	int SBaseLog::CheckUniqueXFRNumber()
	{
		if (!m_uniqueXFRNumber)
			return 1;

		UINT64 uniqueXFRNumber = m_uniqueXFRNumber;
		m_uniqueXFRNumber = 0;
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (uniqueXFRNumber == SLOG[no].m_uniqueXFRNumber)
				return 2;

			for (int axisNo = 0; axisNo < m_axisList.Count(); axisNo++)
			{
				AxisList axis = m_axisList[axisNo];
				if (uniqueXFRNumber != SLOG[no].axisLogEnd[axis].m_uniqueXFRNumber)
					continue;

				return 3;
			}
			for (int axisNo = 0; axisNo < m_robotList.Count(); axisNo++)
			{
				RobotList axis = m_robotList[axisNo];
				if (uniqueXFRNumber != SLOG[no].robotLogEnd[axis].m_uniqueXFRNumber)
					continue;

				return 33;
			}
			for (int cylinderNo = 0; cylinderNo < m_cylinderList.Count(); cylinderNo++)
			{
				CylinderList cylinder = m_cylinderList[cylinderNo];
				if (uniqueXFRNumber != SLOG[no].cylinderLogEnd[cylinder].m_uniqueXFRNumber)
					continue;

				return 4;
			}
			for (int vacuumNo = 0; vacuumNo < m_vacuumList.Count(); vacuumNo++)
			{
				VacuumList vacuum = m_vacuumList[vacuumNo];
				if (uniqueXFRNumber != SLOG[no].vacuumLogEnd[vacuum].m_uniqueXFRNumber)
					continue;

				return 5;
			}
			for (int laserNo = 0; laserNo < m_laserList.Count(); laserNo++)
			{
				LaserList laser = m_laserList[laserNo];
				if (uniqueXFRNumber != SLOG[no].laserLogEnd[laser].m_uniqueXFRNumber)
					continue;

				return 6;
			}
			for (int visionNo = 0; visionNo < m_visionList.Count(); visionNo++)
			{
				VisionList vision = m_visionList[visionNo];
				if (uniqueXFRNumber != SLOG[no].visionLogEnd[vision].m_uniqueXFRNumber)
					continue;

				return 7;
			}
			for (int conveyorNo = 0; conveyorNo < m_conveyorList.Count(); conveyorNo++)
			{
				ConveyorList conveyor = m_conveyorList[conveyorNo];
				if (uniqueXFRNumber != SLOG[no].conveyorLogEnd[conveyor].m_uniqueXFRNumber)
					continue;

				return 8;
			}
		}

		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			for (int axisNo = 0; axisNo < m_axisList.Count(); axisNo++)
			{
				AxisList axis = m_axisList[axisNo];
				if (uniqueXFRNumber != SLOG[no].axisLogStart[axis].m_uniqueXFRNumber)
					continue;

				SLOG[no].axisLogStart[axis].m_uniqueXFRNumber = 0;
			}
			for (int axisNo = 0; axisNo < m_robotList.Count(); axisNo++)
			{
				RobotList axis = m_robotList[axisNo];
				if (uniqueXFRNumber != SLOG[no].robotLogStart[axis].m_uniqueXFRNumber)
					continue;

				SLOG[no].axisLogStart[axis].m_uniqueXFRNumber = 0;
			}
			for (int cylinderNo = 0; cylinderNo < m_cylinderList.Count(); cylinderNo++)
			{
				CylinderList cylinder = m_cylinderList[cylinderNo];
				if (uniqueXFRNumber != SLOG[no].cylinderLogStart[cylinder].m_uniqueXFRNumber)
					continue;

				SLOG[no].cylinderLogStart[cylinder].m_uniqueXFRNumber = 0;
			}
			for (int vacuumNo = 0; vacuumNo < m_vacuumList.Count(); vacuumNo++)
			{
				VacuumList vacuum = m_vacuumList[vacuumNo];
				if (uniqueXFRNumber != SLOG[no].vacuumLogStart[vacuum].m_uniqueXFRNumber)
					continue;

				SLOG[no].vacuumLogStart[vacuum].m_uniqueXFRNumber = 0;
			}
			for (int laserNo = 0; laserNo < m_laserList.Count(); laserNo++)
			{
				LaserList laser = m_laserList[laserNo];
				if (uniqueXFRNumber != SLOG[no].laserLogStart[laser].m_uniqueXFRNumber)
					continue;

				SLOG[no].laserLogStart[laser].m_uniqueXFRNumber = 0;
			}
			for (int visionNo = 0; visionNo < m_visionList.Count(); visionNo++)
			{
				VisionList vision = m_visionList[visionNo];
				if (uniqueXFRNumber != SLOG[no].visionLogStart[vision].m_uniqueXFRNumber)
					continue;

				SLOG[no].visionLogStart[vision].m_uniqueXFRNumber = 0;
			}
			for (int conveyorNo = 0; conveyorNo < m_conveyorList.Count(); conveyorNo++)
			{
				ConveyorList conveyor = m_conveyorList[conveyorNo];
				if (uniqueXFRNumber != SLOG[no].conveyorLogStart[conveyor].m_uniqueXFRNumber)
					continue;

				SLOG[no].conveyorLogStart[conveyor].m_uniqueXFRNumber = 0;
			}
		}

		return 0;
	}

	int SBaseLog::CheckUniquePRCNumber(BOOL isPRC)
	{
		if (!isPRC || !m_uniquePRCNumber)
			return 1;

		UINT64 uniquePRCNumber = m_uniquePRCNumber;
		m_uniquePRCNumber = 0;
		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			if (uniquePRCNumber == SLOG[no].m_uniquePRCNumber)
				return 2;

			for (int axisNo = 0; axisNo < m_axisList.Count(); axisNo++)
			{
				AxisList axis = m_axisList[axisNo];
				if (uniquePRCNumber != SLOG[no].axisLogEnd[axis].m_uniquePRCNumber)
					continue;

				return 3;
			}
			for (int axisNo = 0; axisNo < m_robotList.Count(); axisNo++)
			{
				RobotList axis = m_robotList[axisNo];
				if (uniquePRCNumber != SLOG[no].robotLogEnd[axis].m_uniquePRCNumber)
					continue;

				return 3;
			}
			for (int cylinderNo = 0; cylinderNo < m_cylinderList.Count(); cylinderNo++)
			{
				CylinderList cylinder = m_cylinderList[cylinderNo];
				if (uniquePRCNumber != SLOG[no].cylinderLogEnd[cylinder].m_uniquePRCNumber)
					continue;

				return 4;
			}
			for (int vacuumNo = 0; vacuumNo < m_vacuumList.Count(); vacuumNo++)
			{
				VacuumList vacuum = m_vacuumList[vacuumNo];
				if (uniquePRCNumber != SLOG[no].vacuumLogEnd[vacuum].m_uniquePRCNumber)
					continue;

				return 5;
			}
			for (int laserNo = 0; laserNo < m_laserList.Count(); laserNo++)
			{
				LaserList laser = m_laserList[laserNo];
				if (uniquePRCNumber != SLOG[no].laserLogEnd[laser].m_uniquePRCNumber)
					continue;

				return 6;
			}
			for (int visionNo = 0; visionNo < m_visionList.Count(); visionNo++)
			{
				VisionList vision = m_visionList[visionNo];
				if (uniquePRCNumber != SLOG[no].visionLogEnd[vision].m_uniquePRCNumber)
					continue;

				return 7;
			}
		}

		for (int i = 0; i < slogMAX; i++)
		{
			SLogList no = SLogList(i);
			for (int axisNo = 0; axisNo < m_axisList.Count(); axisNo++)
			{
				AxisList axis = m_axisList[axisNo];
				if (uniquePRCNumber != SLOG[no].axisLogStart[axis].m_uniquePRCNumber)
					continue;

				SLOG[no].axisLogStart[axis].m_uniquePRCNumber = 0;
			}
			for (int axisNo = 0; axisNo < m_robotList.Count(); axisNo++)
			{
				RobotList axis = m_robotList[axisNo];
				if (uniquePRCNumber != SLOG[no].robotLogStart[axis].m_uniquePRCNumber)
					continue;

				SLOG[no].axisLogStart[axis].m_uniquePRCNumber = 0;
			}
			for (int cylinderNo = 0; cylinderNo < m_cylinderList.Count(); cylinderNo++)
			{
				CylinderList cylinder = m_cylinderList[cylinderNo];
				if (uniquePRCNumber != SLOG[no].cylinderLogStart[cylinder].m_uniquePRCNumber)
					continue;

				SLOG[no].cylinderLogStart[cylinder].m_uniquePRCNumber = 0;
			}
			for (int vacuumNo = 0; vacuumNo < m_vacuumList.Count(); vacuumNo++)
			{
				VacuumList vacuum = m_vacuumList[vacuumNo];
				if (uniquePRCNumber != SLOG[no].vacuumLogStart[vacuum].m_uniquePRCNumber)
					continue;

				SLOG[no].vacuumLogStart[vacuum].m_uniquePRCNumber = 0;
			}
			for (int laserNo = 0; laserNo < m_laserList.Count(); laserNo++)
			{
				LaserList laser = m_laserList[laserNo];
				if (uniquePRCNumber != SLOG[no].laserLogStart[laser].m_uniquePRCNumber)
					continue;

				SLOG[no].laserLogStart[laser].m_uniquePRCNumber = 0;
			}
			for (int visionNo = 0; visionNo < m_visionList.Count(); visionNo++)
			{
				VisionList vision = m_visionList[visionNo];
				if (uniquePRCNumber != SLOG[no].visionLogStart[vision].m_uniquePRCNumber)
					continue;

				SLOG[no].visionLogStart[vision].m_uniquePRCNumber = 0;
			}
			for (int conveyorNo = 0; conveyorNo < m_conveyorList.Count(); conveyorNo++)
			{
				ConveyorList conveyor = m_conveyorList[conveyorNo];
				if (uniquePRCNumber != SLOG[no].conveyorLogStart[conveyor].m_uniquePRCNumber)
					continue;

				SLOG[no].conveyorLogStart[conveyor].m_uniquePRCNumber = 0;
			}
		}
		
		return 0;
	}

	BOOL SBaseLog::IsRecording(AxisList axisNo)
	{
		for (int i = 0; i < m_axisList.Count(); i++)
		{
			if (axisNo != m_axisList[i])
				continue;

			return m_axisSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(RobotList robotNo)
	{
		for (int i = 0; i < m_robotList.Count(); i++)
		{
			if (robotNo != m_robotList[i])
				continue;

			return m_robotSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(CylinderList cylinderNo)
	{
		for (int i = 0; i < m_cylinderList.Count(); i++)
		{
			if (cylinderNo != m_cylinderList[i])
				continue;

			return m_cylinderSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(VacuumList vacuumNo)
	{
		for (int i = 0; i < m_vacuumList.Count(); i++)
		{
			if (vacuumNo != m_vacuumList[i])
				continue;

			return m_vacuumSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(ConveyorList conveyorNo)
	{
		for (int i = 0; i < m_conveyorList.Count(); i++)
		{
			if (conveyorNo != m_conveyorList[i])
				continue;

			return m_conveyorSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(LaserList laserNo)
	{
		for (int i = 0; i < m_laserList.Count(); i++)
		{
			if (laserNo != m_laserList[i])
				continue;

			return m_laserSelectiveRecording[i];
		}
		return FALSE;
	}

	BOOL SBaseLog::IsRecording(VisionList visionNo)
	{
		for (int i = 0; i < m_visionList.Count(); i++)
		{
			if (visionNo != m_visionList[i])
				continue;

			return m_visionSelectiveRecording[i];
		}
		return FALSE;
	}

	void SBaseLog::RecordingOn(AxisList axisNo)
	{
		for (int i = 0; i < m_axisList.Count(); i++)
		{
			if (axisNo != m_axisList[i])
				continue;

			m_axisSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(RobotList robotNo)
	{
		for (int i = 0; i < m_robotList.Count(); i++)
		{
			if (robotNo != m_robotList[i])
				continue;

			m_robotSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(ConveyorList conveyorNo)
	{
		for (int i = 0; i < m_conveyorList.Count(); i++)
		{
			if (conveyorNo != m_conveyorList[i])
				continue;

			m_conveyorSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(CylinderList cylinderNo)
	{
		for (int i = 0; i < m_cylinderList.Count(); i++)
		{
			if (cylinderNo != m_cylinderList[i])
				continue;

			m_cylinderSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(VacuumList vacuumNo)
	{
		for (int i = 0; i < m_vacuumList.Count(); i++)
		{
			if (vacuumNo != m_vacuumList[i])
				continue;

			m_vacuumSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(LaserList laserNo)
	{
		for (int i = 0; i < m_laserList.Count(); i++)
		{
			if (laserNo != m_laserList[i])
				continue;

			m_laserSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOn(VisionList visionNo)
	{
		for (int i = 0; i < m_visionList.Count(); i++)
		{
			if (visionNo != m_visionList[i])
				continue;

			m_visionSelectiveRecording[i] = TRUE;
		}
	}

	void SBaseLog::RecordingOff(AxisList axisNo)
	{
		for (int i = 0; i < m_axisList.Count(); i++)
		{
			if (axisNo != m_axisList[i])
				continue;

			m_axisSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(RobotList robotNo)
	{
		for (int i = 0; i < m_robotList.Count(); i++)
		{
			if (robotNo != m_robotList[i])
				continue;

			m_robotSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(ConveyorList conveyorNo)
	{
		for (int i = 0; i < m_conveyorList.Count(); i++)
		{
			if (conveyorNo != m_conveyorList[i])
				continue;

			m_conveyorSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(CylinderList cylinderNo)
	{
		for (int i = 0; i < m_cylinderList.Count(); i++)
		{
			if (cylinderNo != m_cylinderList[i])
				continue;

			m_cylinderSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(VacuumList vacuumNo)
	{
		for (int i = 0; i < m_vacuumList.Count(); i++)
		{
			if (vacuumNo != m_vacuumList[i])
				continue;

			m_vacuumSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(LaserList laserNo)
	{
		for (int i = 0; i < m_laserList.Count(); i++)
		{
			if (laserNo != m_laserList[i])
				continue;

			m_laserSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::RecordingOff(VisionList visionNo)
	{
		for (int i = 0; i < m_visionList.Count(); i++)
		{
			if (visionNo != m_visionList[i])
				continue;

			m_visionSelectiveRecording[i] = FALSE;
		}
	}

	void SBaseLog::LogFncIn(string eventID, string startEnd, string materialId, string delayTime, int sensorNo, string actName)
	{
		// ex) materialId: $ or ID
		// ex) actName: FWD, ON, etc

		char buffer[100];
		sprintf(buffer, "'SEN_X%04d'", sensorNo);
		string sensorName(buffer);

		if ((startEnd == "START" && m_InputCheck[sensorNo]) || (startEnd == "END" && !m_InputCheck[sensorNo]))
			return;

		SMESSAGE.Function(m_xfr.deviceId.c_str(), eventID.c_str(), startEnd.c_str(), materialId.c_str(), m_xfr.materialType.c_str(),
			"'TYPE'", "'SENSOR'", "'DELAYTIME'", delayTime.c_str(), sensorName.c_str(), actName.c_str());

		if (startEnd == "START")
			m_InputCheck[sensorNo] = TRUE;
		else
			m_InputCheck[sensorNo] = FALSE;

	}

	void SBaseLog::LogFncOut(string eventID, string startEnd, string materialId, string delayTime, int sensorNo, string actName)
	{
		// ex) materialId: $ or ID
		// ex) actName: FWD, ON, etc

		char buffer[100];
		sprintf(buffer, "'SEN_Y%04d'", sensorNo);
		string sensorName(buffer);

		if ((startEnd == "START" && m_OutputCheck[sensorNo]) || (startEnd == "END" && !m_OutputCheck[sensorNo]))
			return;

		SMESSAGE.Function(m_xfr.deviceId.c_str(), eventID.c_str(), startEnd.c_str(), materialId.c_str(), m_xfr.materialType.c_str(),
			"'TYPE'", "'SENSOR'", "'DELAYTIME'", delayTime.c_str(), sensorName.c_str(), actName.c_str());

		if (startEnd == "START")
			m_OutputCheck[sensorNo] = TRUE;
		else
			m_OutputCheck[sensorNo] = FALSE;
	}

	void SBaseLog::LogPrc(string eventID, string startEnd, string materialId, string lotId, string recipeId, int bufNo)
	{
		if ((startEnd == "START" && m_PrcLogCheck[bufNo]) || (startEnd == "END" && !m_PrcLogCheck[bufNo]))
			return;

		SMESSAGE.Process(m_xfr.deviceId.c_str(), eventID.c_str(), startEnd.c_str(), materialId.c_str(), lotId.c_str(),	recipeId.c_str());

		if (startEnd == "START")
			m_PrcLogCheck[bufNo] = TRUE;
		else
			m_PrcLogCheck[bufNo] = FALSE;
	}
}