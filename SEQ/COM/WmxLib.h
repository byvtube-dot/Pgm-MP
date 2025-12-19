#pragma once
#include "..\Defines.h"
#include "..\..\BASE\BaseAll.h"

#include "..\HW\WMX\AdvancedMotionApi.h"
#include "..\HW\WMX\ApiBufferApi.h"
#include "..\HW\WMX\CompensationApi.h"
#include "..\HW\WMX\CoreMotionApi.h"
#include "..\HW\WMX\CyclicBufferApi.h"
#include "..\HW\WMX\EcApi.h"
#include "..\HW\WMX\EventApi.h"
#include "..\HW\WMX\IOApi.h"
#include "..\HW\WMX\LogApi.h"
#include "..\HW\WMX\PMMotionApi.h"
#include "..\HW\WMX\SimuApi.h"
#include "..\HW\WMX\UserMemoryApi.h"
#include "..\HW\WMX\WMX3Api.h"

using namespace wmx3Api;
using namespace ecApi;
using namespace std;
namespace COM
{
	static const UINT8 g_bitMask08[8] =
	{
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
	};

	static const UINT16 g_bitMask16[16] =
	{
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800,
		0x1000, 0x2000, 0x4000, 0x8000,
	};

	static const UINT32 g_bitMask32[32] =
	{
		0x00000001, 0x00000002, 0x00000004, 0x00000008,
		0x00000010, 0x00000020, 0x00000040, 0x00000080,
		0x00000100, 0x00000200, 0x00000400, 0x00000800,
		0x00001000, 0x00002000, 0x00004000, 0x00008000,
		0x00010000, 0x00020000, 0x00040000, 0x00080000,
		0x00100000, 0x00200000, 0x00400000, 0x00800000,
		0x01000000, 0x02000000, 0x04000000, 0x08000000,
		0x10000000, 0x20000000, 0x40000000, 0x80000000,
	};

	static const UINT64 g_bitMask64[64] =
	{
		0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008,
		0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080,
		0x0000000000000100, 0x0000000000000200, 0x0000000000000400, 0x0000000000000800,
		0x0000000000001000, 0x0000000000002000, 0x0000000000004000, 0x0000000000008000,
		0x0000000000010000, 0x0000000000020000, 0x0000000000040000, 0x0000000000080000,
		0x0000000000100000, 0x0000000000200000, 0x0000000000400000, 0x0000000000800000,
		0x0000000001000000, 0x0000000002000000, 0x0000000004000000, 0x0000000008000000,
		0x0000000010000000, 0x0000000020000000, 0x0000000040000000, 0x0000000080000000,
		0x0000000100000000, 0x0000000200000000, 0x0000000400000000, 0x0000000800000000,
		0x0000001000000000, 0x0000002000000000, 0x0000004000000000, 0x0000008000000000,
		0x0000010000000000, 0x0000020000000000, 0x0000040000000000, 0x0000080000000000,
		0x0000100000000000, 0x0000200000000000, 0x0000400000000000, 0x0000800000000000,
		0x0001000000000000, 0x0002000000000000, 0x0004000000000000, 0x0008000000000000,
		0x0010000000000000, 0x0020000000000000, 0x0040000000000000, 0x0080000000000000,
		0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000,
		0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000,
	};

	class CWmx
	{
	public:
		WMX3Api						api;
		CoreMotion					coremotion;
		ApiBuffer					apibuffer;
		Io							io;
		Compensation				compensation;
		CyclicBuffer				cyclicbuffer;
		AdvancedMotion				advancemotion;
		Ecat						ecat;
		UserMemory					usermemory;
		CoreMotionStatus			status;
		EcMasterInfo				masterInfo;

		CWmx()
		{
			coremotion = CoreMotion(&api);
			apibuffer = ApiBuffer(&api);
			io = Io(&api);
			compensation = Compensation(&api);
			cyclicbuffer = CyclicBuffer(&api);
			advancemotion = AdvancedMotion(&api);
			ecat = Ecat(&api);
			usermemory = UserMemory(&api);
		};
		~CWmx() {};
	};

	typedef enum
	{
		UR20_HUB,
		UR20_FBC_EC,
		WAGO_HUB,
		WAGO_750_354_COUPLER,
	}SLAVE_MODULE_TYPE;

	typedef enum
	{
		UR20_16DI_N,
		WAGO_750_1407_DI_16CH,
	}DIGITAL_INPUT_MODULE_TYPE;

	typedef enum
	{
		UR20_4AI_UI_16,
		WAGO_750_459_AI_4CH,
		WAGO_750_471_AI_4CH,
	}ANALOG_INPUT_MODULE_TYPE;

	typedef enum
	{
		UR20_16DO_N,
		WAGO_750_1505_DO_16CH,
	}DIGITAL_OUTPUT_MODULE_TYPE;

	typedef enum
	{
		UR20_4AO_UI_16,
		WAGO_750_564_AO_4CH,
	}ANALOG_OUTPUT_MODULE_TYPE;

	typedef enum
	{
		KOSES_8CH_VACUUM,
	}KOSES_MODULE_TYPE;

	typedef enum
	{
		DUMMY_MOTOR,
		EZI_SERVO_02,
		PANASONIC_SERVO,
		LS_MECAPION,
	}IO_MOTOR;

	typedef enum
	{
		SEALING,			// Sealing (88-24-0042-SEC_2 Only)
	}IO_SEALING;

	typedef enum
	{
		CMD_DONE,
		CMD_MODE_CHANGE,
		CMD_CURRENT_POS,
		CMD_ERR_READY,
		CMD_ERR_MODE,
		CMD_ERR_SOFTLIMIT,
		CMD_ERR_POS,
		CMD_ERR_VEL,
		CMD_ERR_ACC,
		CMD_ERR_PULSERATE,
		CMD_ERR_PARAM,
		CMD_ERR_MOTION_FAIL,
	}CommandResult;

	typedef enum
	{
		TYPE_COMPLETED_TIME,
		TYPE_REMAINING_TIME,
	}ReturnTimeType;

	enum enAxisDrv
	{
		C_HOME_START = 100,
		C_HOME_SEQ_START = C_HOME_START,
		C_HOME_SEQ_END,
		C_HOME_SEARCH_START,
		C_HOME_SEARCH_END,
		C_HOME_END = 199,

		C_POS_START = 200,
		C_POS_PAUSED,
		C_POS_END = 299,

		C_VEL_START = 300,
		C_VEL_END = 399,

		C_TRQ_START = 400,
		C_TRQ_END = 499,

		C_JOG_START = 500,
		C_JOG_END = 599,

		C_MOTION_BUSY = 600,
		C_POS_LINEAR = 601,
	};

	class MtCommand
	{
	public:
		class Profile
		{
		public:
			double		vel;
			double		acc;
			double		jerkAcc;
			double		jerkRatio;//?
			double		startingVelocity;
			double		endVelocity;
		};

		class Parameter
		{
		public:
			int			no;
			int			idx;
			double		pos;
			double		startPos;
			double		limitP;
			double		limitN;
			double		maxVel;//?
			double		maxAcc;//?
			double		maxJerkAcc;//?

			double		trq;//?
			double		rpm;//?

			double		pulseRate;
		};

		class SingleParamter : public Parameter
		{
		public:
			Profile		profile;
		};

		class InterpolationParamter
		{
		public:
			int			axisCount;
			double		pulseRate;
			Parameter	parameter[MAX_INTERPOLATION_MOTOR];
			Profile		profile;
		};
	};

	class CylCommand
	{
	public:
		class Profile
		{
		public:
			INT32		delayTimeA;
			INT32		delayTimeB;
		};

		class Parameter
		{
		public:
			int			no;
			int			idx;
		};

		class SingleParamter : public Parameter
		{
		public:
			Profile		profile;
		};
	};

	class VcCommand
	{
	public:
		class Profile
		{
		public:
			double		highOn;
			double		highOff;
			double		lowOn;
			double		lowOff;
			double		blowOn;
			INT32		timeOn;
			INT32		timeOff;
			INT32		timeBlow;
		};

		class Parameter
		{
		public:
			int			no;
			int			idx;
		};

		class SingleParamter : public Parameter
		{
		public:
			Profile		profile;
		};
	};

	class AcCommand
	{
	public:
		class Profile
		{
		public:
			INT32		delayTimeA;
			INT32		delayTimeB;
		};

		class Parameter
		{
		public:
			int			no;
			int			idx;
		};

		class SingleParamter : public Parameter
		{
		public:
			Profile		profile;
		};
	};

	class PdoData
	{
	private:
		BYTE	dataBuffer;

	public:
		PBYTE	dataAddress;
		UINT	byteAddress;

		PdoData()
		{
			ZeroMemory(this, sizeof(*this));
			dataAddress = &dataBuffer;
		};
		~PdoData() {};
	};

	class PdoSorting
	{
	public:
		enum
		{
			DIGITAL = 10000,
			ANALOGUE = 20000,
			MAX_DATA = 100,
		};

		BOOL isWago[MAX_DATA];
		UINT dataType[MAX_DATA];
		UINT channel[MAX_DATA];
		UINT rvaByte[MAX_DATA];
		UINT rvaMaxSize;
		UINT rvaWagoMaxSize;
		UINT dataCount;

		PdoSorting()
		{
			ZeroMemory(this, sizeof(*this));
		};
		~PdoSorting() {};
	};

	class CWmxLib : public CWmx
	{
	private:
		BYTE			m_iReadWriteData[2048];
		BYTE			m_oReadWriteData[2048];
		BYTE			m_oReadOnlyData[2048];
		BYTE			m_userMemReadWriteData[1024 * 248];
		BYTE			m_userMemReadOnlyData[1024 * 248];

		CTimer			tmConnectLoop;

		int				nStartCommCnt;
		int				nHotConnectCnt;
		int				nSlaveType[2048];
		int				nIoslaveCnt;
		int				nMtslaveCnt;
		int				nMtDummyCnt;

		INT16			m_iMaxDataSize;
		INT16			m_oMaxDataSize;

		PdoSorting		m_iPdoSorting;
		PdoSorting		m_oPdoSorting;

		CTimer			tiOn[MAX_CH][MAX_CONTACT];
		CTimer			tiOff[MAX_CH][MAX_CONTACT];

		CTimer			toOn[MAX_CH][MAX_CONTACT];
		CTimer			toOff[MAX_CH][MAX_CONTACT];

		CTimer			tusermemOn[MAX_CH][MAX_USERMEM_CONTACT];
		CTimer			tusermemOff[MAX_CH][MAX_USERMEM_CONTACT];

		UINT16			iBitMem[MAX_CH];
		UINT16			oBitMem[MAX_CH];
		UINT8			userBitMem[1024 * 248];

		BOOL			isIoConnected;
		BOOL			isMtConnected;
		BOOL			isPastConnect;

		int				panasonicChannelCount;

	public:
		BOOL			isConnected;

		PdoData			m_di[MAX_CH];
		PdoData			m_do[MAX_CH];
		PdoData			m_ai[MAX_CH];
		PdoData			m_ao[MAX_CH];
		PdoData			m_mt[MAX_CH];
		PdoData			m_si[MAX_CH];	// Sealing
		PdoData			m_so[MAX_CH];	// Sealing

	private:
		void WriteIO(void);
		void ReadIO(void);
		void Connection(void);

	public:
		void Update(void);

		BOOL Open(void);
		void InitPdoSorting(void);
		void InitPdoDevice(SLAVE_MODULE_TYPE module);
		void InitPdoDevice(DIGITAL_INPUT_MODULE_TYPE module, int channelNo);
		void InitPdoDevice(DIGITAL_OUTPUT_MODULE_TYPE module, int channelNo);
		void InitPdoDevice(ANALOG_INPUT_MODULE_TYPE module, int channelNo);
		void InitPdoDevice(ANALOG_OUTPUT_MODULE_TYPE module, int channelNo);
		void InitPdoDevice(KOSES_MODULE_TYPE module, int inputChannelNo1, int inputChannelNo2, int outputChannelNo);
		void InitPdoDevice(IO_MOTOR module, AxisList axisNo);
		void InitPdoDevice(IO_SEALING module, int channelNo);

		void OutDataClear();

		double MMToPulse(double umValue, double pulseRate);
		double PulseToMM(double pulseValue, double pulseRate);

		int CommandJog(MtCommand::SingleParamter command, BOOL isModeChange = TRUE);
		int CommandPos(MtCommand::SingleParamter command, BOOL isModeChange = TRUE);
		int CommandVel(MtCommand::SingleParamter command, BOOL isModeChange = TRUE);
		int CommandTrq(MtCommand::SingleParamter command, BOOL isModeChange = TRUE);
		int CommandPos(MtCommand::InterpolationParamter command, BOOL isModeChange = TRUE);
		int CommandSimulateTime(MtCommand::SingleParamter command, double specificPos, double& calculatedMoveTime, double& calculatedRemainTime);
		int CommandSimulatePosAndVel(MtCommand::SingleParamter command, ReturnTimeType returnType, double timeMilliseconds, double& calculatedPos, double& calculatedVel);
		int CommandSimulateTime(MtCommand::SingleParamter command, double& peakVelocity, double& totalTime, double& accelerationTime, double& cruiseTime, double& decelerationTime);

		BOOL AOn(enDI ioNo, DWORD dwDelayTime = 0);
		BOOL AOff(enDI ioNo, DWORD dwDelayTime = 0);
		void Set(enDI ioNo, BOOL isOn);
		BOOL IsOn(enDO ioNo, DWORD dwDelayTime = 0);
		BOOL IsOff(enDO ioNo, DWORD dwDelayTime = 0);
		BOOL IsReadOnly(enDO ioNo);
		BOOL IsBrakeOn(int no);
		void Set(enDO ioNo, BOOL isOn);
		void SetReadOnly(enDO ioNo, BOOL isOn);
		void SetBrake(int no, BOOL isOn);
		BOOL IsUserMemoryOn(enUSERMEM userMemNo, DWORD dwDelayTime = 0);
		BOOL IsUserMemoryOff(enUSERMEM userMemNo, DWORD dwDelayTime = 0);
		void UserMemorySet(enUSERMEM userMemNo, BOOL isOn);
		BOOL IsUserMemoryReadOnly(enUSERMEM userMemNo);
		void SetUserMemoryReadOnly(enUSERMEM userMemNo, BOOL isOn);

		CWmxLib();
		virtual ~CWmxLib();
	};
}