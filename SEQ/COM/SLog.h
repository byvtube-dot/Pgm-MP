#pragma once
#include "..\Defines.h"
#include "..\BASE\SLogBase.h"

namespace COM
{
	class SMessage
	{
	private:
		sloglib::SLogBase* m_pSLog;
		sloglib::SMemoryBase* m_pSMemoryBase;

	public:
		BOOL Open(SYSTEMTIME* pSystemTime = NULL);
		BOOL Recv(UINT32 milliSeconds = 0);
		BOOL Send(UINT32 milliSeconds = 0);
		void Process(const char* deviceId, const char* eventId, const char* status,
			const char* materialId, const char* lotId, const char* recipeId,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Transfer(const char* deviceId, const char* eventId, const char* status,
			const char* materialId, const char* materialType, const char* fromDevice, const char* toDevice,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Function(const char* deviceId, const char* eventId, const char* status,
			const char* materialId, const char* materialType,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Lot(const char* deviceId, const char* eventId, const char* lotId,
			const char* recipeId, const char* carrierId,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Configure(const char* deviceId, const char* cfgId,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Alarm(const char* deviceId, const char* eventId, const char* alarmCode, const char* status,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);
		void Comm(const char* deviceId, const char* eventId,
			const char* key1 = NULL, const char* value1 = NULL, const char* key2 = NULL, const char* value2 = NULL,
			const char* key3 = NULL, const char* value3 = NULL, const char* key4 = NULL, const char* value4 = NULL,
			const char* key5 = NULL, const char* value5 = NULL, const char* key6 = NULL, const char* value6 = NULL);

	public:
		SMessage();
		virtual ~SMessage();
	};

	struct PRC
	{
		string				deviceId;
		string				eventId;
		string				materialId;
		string				lotId;
		string				recipeId;
	};

	struct XFR
	{
		string				deviceId;
		string				eventId;
		string				materialId;
		string				materialType;
		string				fromDevice;
		string				toDevice;
	};

	class SBaseLog
	{
	private:
		PRC					m_prc;
		XFR					m_xfr;
		List<AxisList>		m_axisList;
		List<RobotList>		m_robotList;
		List<CylinderList>	m_cylinderList;
		List<VacuumList>	m_vacuumList;
		List<ConveyorList>	m_conveyorList;
		List<LaserList>		m_laserList;
		List<VisionList>	m_visionList;

		List<BOOL>			m_axisSelectiveRecording;
		List<BOOL>			m_robotSelectiveRecording;
		List<BOOL>			m_cylinderSelectiveRecording;
		List<BOOL>			m_vacuumSelectiveRecording;
		List<BOOL>			m_conveyorSelectiveRecording;
		List<BOOL>			m_laserSelectiveRecording;
		List<BOOL>			m_visionSelectiveRecording;
		
		// Sensor Log.
		BOOL				m_InputCheck[diLAST];
		BOOL				m_OutputCheck[doLAST];

		// LEH - Lot Event Log
		BOOL				m_LehCheck[10];
		// PRC - Process Log
		BOOL				m_PrcLogCheck[10];
	public:
		int					m_no;
		UINT64				m_uniqueXFRNumber;
		UINT64				m_uniquePRCNumber;

		static UINT64& UniqueNumber()
		{
			static UINT64 staticNumber;
			return staticNumber;
		}

		//void PRC(
		//	string prcEventId, string prcMaterialId, string prcLotId, string prcRecipeId,
		//	string xfrEventId, string xfrMaterialId, string xfrFromDevice, string xfrToDevice);
		void PRC(
			string prcEventId, string prcMaterialId, string prcLotId, string prcRecipeId);
		void XFR(string xfrEventId, string xfrMaterialId, string xfrFromDevice, string xfrToDevice);
		void Start(BOOL isPRC);
		void End(BOOL isPRC);
		void Clear();
		void SetPRCDeviceId(string deviceId);
		void SetPRCEventId(string eventId);
		void SetPRCMaterialId(string materialId);
		void SetPRCLotId(string lotId);
		void SetPRCRecipeId(string recipeId);
		void SetXFRDeviceId(string deviceId);
		void SetXFREventId(string eventId);
		void SetXFRMaterialId(string materialId);
		void SetXFRMaterialType(string materialType);
		void SetXFRFromDevice(string fromDevice);
		void SetXFRToDevice(string toDevice);
		string GetXFRDeviceId();
		string GetXFREventId();
		string GetXFRMaterialId();
		string GetXFRMaterialType();
		string GetXFRFromDevice();
		string GetXFRToDevice();
		void Add(AxisList axisNo, BOOL selectiveRecording = FALSE);
		void Add(RobotList axisNo, BOOL selectiveRecording = FALSE);
		void Add(ConveyorList conveyorNo, BOOL selectiveRecording = FALSE);
		void Add(CylinderList cylinderNo, BOOL selectiveRecording = FALSE);
		void Add(VacuumList vacuumNo, BOOL selectiveRecording = FALSE);
		void Add(LaserList laserNo, BOOL selectiveRecording = FALSE);
		void Add(VisionList visionNo, BOOL selectiveRecording = FALSE);
		int CheckUniqueXFRNumber();
		int CheckUniquePRCNumber(BOOL isPRC);
		BOOL IsRecording(AxisList axisNo);
		BOOL IsRecording(RobotList robotNo);
		BOOL IsRecording(ConveyorList conveyorNo);
		BOOL IsRecording(CylinderList cylinderNo);
		BOOL IsRecording(VacuumList vacuumNo);
		BOOL IsRecording(LaserList laserNo);
		BOOL IsRecording(VisionList visionNo);

		void RecordingOn(AxisList axisNo);
		void RecordingOn(RobotList robotNo);
		void RecordingOn(ConveyorList conveyorNo);
		void RecordingOn(CylinderList cylinderNo);
		void RecordingOn(VacuumList vacuumNo);
		void RecordingOn(LaserList laserNo);
		void RecordingOn(VisionList visionNo);

		void RecordingOff(AxisList axisNo);
		void RecordingOff(RobotList robotNo);
		void RecordingOff(ConveyorList conveyorNo);
		void RecordingOff(CylinderList cylinderNo);
		void RecordingOff(VacuumList vacuumNo);
		void RecordingOff(LaserList laserNo);
		void RecordingOff(VisionList visionNo);

		// ADD FNC Log (FNC: Function Log)
		void LogFncIn(string eventID, string startEnd, string materialId, string delayTime, int sensorNo, string actName);
		void LogFncOut(string eventID, string startEnd, string materialId, string delayTime, int sensorNo, string actName);

		// ADD LEH Log (LEH: Lot Event Log)
		// ADD PRC Log (PRC: Process Log)
		void LogPrc(string eventID, string startEnd, string materialId, string lotId, string recipeId, int bufNo = 0);

		SBaseLog();
		virtual ~SBaseLog();
	};

	class SLog : public SBaseLog
	{
	public:
		SBaseLog		axisLogStart[motorMAX > 0 ? motorMAX : 1];
		SBaseLog		robotLogStart[robotMAX > 0 ? robotMAX : 1];
		SBaseLog		cylinderLogStart[cylinderMAX > 0 ? cylinderMAX : 1];
		SBaseLog		vacuumLogStart[vacuumMAX > 0 ? vacuumMAX : 1];
		SBaseLog		laserLogStart[laserMAX > 0 ? laserMAX : 1];
		SBaseLog		visionLogStart[VI_MAX > 0 ? VI_MAX : 1];
		SBaseLog		conveyorLogStart[conveyorMAX > 0 ? conveyorMAX : 1];

		SBaseLog		axisLogEnd[motorMAX > 0 ? motorMAX : 1];
		SBaseLog		robotLogEnd[robotMAX > 0 ? robotMAX : 1];
		SBaseLog		cylinderLogEnd[cylinderMAX > 0 ? cylinderMAX : 1];
		SBaseLog		vacuumLogEnd[vacuumMAX > 0 ? vacuumMAX : 1];
		SBaseLog		laserLogEnd[laserMAX > 0 ? laserMAX : 1];
		SBaseLog		visionLogEnd[VI_MAX > 0 ? VI_MAX : 1];
		SBaseLog		conveyorLogEnd[conveyorMAX > 0 ? conveyorMAX : 1];

		static const int	maxDevice = (slogMAX > 0) ? slogMAX : 1;

	public:
		SLog() {};
		~SLog() {};
	};
}