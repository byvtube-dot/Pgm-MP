#ifndef _SEALING_H_
#define _SEALING_H_

#include <Windows.h>
#include "..\..\BASE\BaseAll.h"
#include "SealingLib.h"

enum SealingList
{
	sealMbb = 0,
	SEAL_MAX,
};

// Write Register
enum enContorlFlag
{
	enOutAbort = 0,				// Bit 0: Abort running process ( 0 -> 1 = abort running process -> error message will be displayed after abort) 
	enOutStart,					// Bit 1: Start US-Process ( 0 -> 1 = start ultrasonic process) 
	enOutCalib,					// Bit 2: Start Calib-pulse ( 0 -> 1 = start calibration pulse for lose power check) 
	enOutShakeOff,				// Bit 3: Start Shake-off pulse ( 0 -> 1 = start shake off pulse to clean sonotrode after ultrasonic process) 
	enOutInit,					// Bit 4: Start Init ( 0 -> 1 = start initialisation after switch on or equipment change) 
	enOutStop,					// Bit 5: US-Stop: ( 0 -> 1 = regular US off in ultrasonic process-> error message will not be displayed after stop)
	enOutQuit,					// Bit 6: Quit Error (0 -> 1 = quit last error / Reset)
	enContorlFlagMax,
};

enum GeneralCommand
{
	NoCommand					= 0,
	AbortProcess				= 1,
	StartUSProcess				= 2,
	StartCalibPulse				= 3,
	StartShakeOffPulse			= 4,
	StartInit					= 5,
	USStop						= 6,
	QuitError					= 10,
	StartFrequencyScan			= 11,
	RefDistanceSensor			= 20,
	ChangeToProductionMode		= 100,
	ChangeToSetupMode			= 101,
	ChangeToSampleMode			= 103,
	BlockSystem					= 104,
	SetParameterFeed			= 150,
	SetParameterResults			= 200,
};

enum ParameterSetting
{
	LoadDataset					= 1,
	ProcessStartTrigger			= 5,
	ProcessStartForceSetting	= 7,
	USOnStartCondition			= 10,
	USAmplitudeSetting			= 12,
	USForceSetting				= 13,
	USCondition2				= 14,
	USAmplitude2Setting			= 16,
	USForce2Setting				= 17,
	USCondition3				= 18,
	USAmplitude3Setting			= 20,
	USForce3Setting				= 21,
	USOnUntilCondition			= 30,
	HoldTime					= 40,
	HoldForceSetting			= 41
};

enum LimitSetting
{
    TriggerTimeMinMax			= 50,
    TriggerDistanceMinMax		= 51,
    TriggerForceMinMax			= 52,
    USOnEnergyMinMax			= 53,
    USOnMaxPowerMinMax			= 54,
    USOnMaxForceMinMax			= 55,
    USOnTimeMinMax				= 56,
    USOnDistanceMinMax			= 57,
    USOnWeldDistanceMinMax		= 58
};

enum CalibrationSetting
{
	CalibrationPulseDuration	= 70,
	ActuatorForceSetting		= 75,
	ShakeOffPulseDuration		= 80
};

enum NetworkSetting
{
	EthernetIPAddress			= 180		// Not Used
};

// Read Register
enum enIOState
{
	enInReady = 0,		// Bit 0: Ready ( 0= system not ready for start, 1= system is ready for start) ; during welding process Ready=0 
	enInError,			// Bit 1: Error ( 0= Error, 1= no Error) 
	enInGood,			// Bit 2: Good (last limit check result), is set until the next weld cycle ; 1= Good 0= not Good 
	enInBad,			// Bit 3: Bad (last limit check result), is set until the next weld cycle ; 1= Bad 0= not Bad
	enInEnable,
	enInStart,
	enInReset,
	enInStateMax,
};

class CSealing
{
public:
	CSealing()
	{

	};
	~CSealing() { };

	const UINT16 BIT_MASK[16] =
	{
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800,
		0x1000, 0x2000, 0x4000, 0x8000,
	};

	char m_strName[400];
	int	m_no;

	CTimer m_tmCtrlFlag[enContorlFlagMax];
	CTimer m_tmCommand;

	SEALDATA::ReadRegister* m_pReadData;
	SEALDATA::WriteRegister* m_pWriteData;

	INT32 m_noDeviceMovingTime;

	void Init(int nIndex, SEALDATA::ReadRegister* pInputArray, SEALDATA::WriteRegister* pOutputArray);
	void Update(void);
	void GetState(void);
	int GetResult(void);
	int GetError(void);
	void SetMMIDisplay(void);

	// Control-Flag
	void SetCtrlFlagOn(int bit, bool isOn = true);
	BOOL IsSetCtrlFlagOn(int bit, bool isOn = true);
	void ResetCtrlFlagOn(int bit);

	void ResetProcess(void);		// Alarm Clear

	void Abort(void);
	BOOL CanStart(void);
	void Start(void);
	void Calib(void);
	void Shakeoff(void);
	void SetInit(void);
	void Stop(void);
	BOOL CanQuit(void);
	void Quit(void);

	void SetInReady(bool isOn);
	void SetInError(bool isOn);
	void SetInGood(bool isOn);
	void SetInBad(bool isOn);
	void SetInEnable(bool isOn);
	void SetInStart(bool isOn);
	void SetInReset(bool isOn);
	
};

EXTERN CSealing SEAL[SEAL_MAX];

#endif