#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <memory>

#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\HW\IO.h"
#include "..\HW\Motor.h"

#define MAX_API_BUFFER_CHANNEL		30
#define MAX_API_BUFFER_AXIS			30
#define MAX_API_BUFFER_COMMAND		30
#define MAX_API_BUFFER_FUNCTION		30
#define MAX_API_BUFFER_DIO			30
#define MAX_API_BUFFER_POS			30
#define MAX_API_BUFFER_CONDITION	100

namespace COM
{
	typedef enum
	{
		fncTypeNone = 0,
		fncTypeAxisIsReady,
		fncTypeAxisIsOverrideReady,
		fncTypeAxisGetCompletedTime,
		fncTypeAxisGetCompletedDistance,
		fncTypeAxisGetRemainingTime,
		fncTypeAxisGetRemainingDistance,
		fncTypeAxisPosMove1,
		fncTypeAxisPosMove2,
		fncTypeAxisPosMove3,
		fncTypeLinearInterpolationMove,
		fncTypeAxisTwoSpeedMove,
		fncTypeAxisSemiTwoSpeedMove,

		fncTypeCylinderFwd,
		fncTypeCylinderBwd,
		fncTypeCylinderIsFwd,
		fncTypeCylinderIsBwd,

		fncTypeVacuumOn,
		fncTypeVacuumOff,
		fncTypeVacuumBlowOn,
		fncTypeVacuumBlowOff,
		fncTypeVacuumAOn,
		fncTypeVacuumAOff,
		fncTypeVacuumIsOn,
		fncTypeVacuumIsOff,

		fncTypeIoAOn,
		fncTypeIoAOff,
		fncTypeIoIsOn,
		fncTypeIoIsOff,
		fncTypeIoOn,
		fncTypeIoOff,

		fncTypeUserMemoryIsOn,
		fncTypeUserMemoryIsOff,
		fncTypeUserMemoryOn,
		fncTypeUserMemoryOff,

		fncTypeControlFlowIf,
		fncTypeControlFlowElseIf,
		fncTypeControlFlowElse,
		fncTypeControlFlowEndIf,
		fncTypeControlSleep,
	}FunctionType;

	typedef enum
	{
		cmdTypeNone = 0,
		cmdTypeFlowStartIf,
		cmdTypeFlowIf,
		cmdTypeFlowElseIf,
		cmdTypeFlowElse,
		cmdTypeFlowEndIf,
		cmdTypeCondition,
		cmdTypeAxis,
		cmdTypeLine,
		cmdTypeIo,
		cmdTypeUserMemory,
		cmdTypeSleep,
		cmdTypeUserCondition,
	}CommandType;

	typedef enum
	{
		cylinderStateFwd = 0,
		cylinderStateBwd,
		cylinderStateIsFwd,
		cylinderStateIsBwd,
	}CylinderState;

	typedef enum
	{
		vacuumStateOn = 0,
		vacuumStateOff,
		vacuumStateBlowOn,
		vacuumStateBlowOff,
		vacuumStateAOn,
		vacuumStateAOff,
		vacuumStateIsOn,
		vacuumStateIsOff,
	}VacuumState;

	typedef enum
	{
		ERR_NONE = 0,
		ERR_ACTIVED_CYCLE,
		ERR_INACTIVED_CYCLE,
		ERR_ACTIVED_CODING_MODE,
		ERR_INACTIVED_CODING_MODE,
		ERR_ALLOCATED_CHANNEL,
		ERR_UNALLOCATED_CHANNEL,
		ERR_POS_INDEX_RANGE_OUT,
		ERR_1ST_START_POS_RANGE_OUT,
		ERR_UNALLOCATED_AXIS,
		ERR_ALLOCATED_OVERLAP_AXIS,
		ERR_ALLOCATED_AXIS_RANGE_OUT,
		ERR_ALLOCATED_COMMAND_RANGE_OUT,
		ERR_ALLOCATED_FUNCTION_RANGE_OUT,
		ERR_ALLOCATED_POS_RANGE_OUT,
		ERR_NO_READY_AXIS,
		ERR_POS_VALUE,
		ERR_VEL_VALUE,
		ERR_ACC_VALUE,
		ERR_UNALLOCATED_LINE_AXIS,
		ERR_OPERATION_CONDITION,
		ERR_WRONG_OVERRIDE_COMMAND,
		ERR_API_BUFFER_COMMAND,

		ERR_UNALLOCATED_CYLINDER,
		ERR_UNALLOCATED_VACUUM,
		ERR_UNALLOCATED_BUFFER,
		ERR_UNALLOCATED_FUNCTION,
		ERR_WRONG_ALLOCATED_BUFFER,
	}ErrorApiBuffer;

	enum
	{
		CMD_START = 100,
		CMD_BUSY,
		CMD_END,
	};

	class SeqBufferConditionType
	{
	public:
		enum T
		{
			Cylinder,
			Vacuum,
			Function,
		};
	};

	class SeqBufferConditionLogic
	{
	public:
		enum T
		{
			NONE,
			AND,
			OR,
		};
	};

	typedef BOOL(*FunctionSeqeunce)();
	static CTimer tmBufferChannel[MAX_API_BUFFER_CHANNEL];

	static bool& BufferChannel(int channel)
	{
		static bool setBufferChannel[MAX_API_BUFFER_CHANNEL];
		if (Between(channel, 0, MAX_API_BUFFER_CHANNEL - 1))
			return setBufferChannel[channel];

		static bool dummyBufferChannel;
		return dummyBufferChannel;
	}

	static bool& UserMemoryBufferCount(int index)
	{
		static bool userMemoryBufferCount[MAX_USERMEM_BUFFER_COUNT];
		if (Between(index, 0, MAX_USERMEM_BUFFER_COUNT - 1))
			return userMemoryBufferCount[index];

		static bool dummyBufferCount;
		return dummyBufferCount;
	}

	// 공통 기반 클래스 정의
	class Base
	{
	public:
		string name = { 0 };
		//virtual void PrintInfo() const = 0;
		virtual ~Base() = default;
	};

	class AxisInfo
	{
	private:
		typedef struct
		{
			int				cnt;
			double			startPos[MAX_API_BUFFER_POS];
		}INFO;

	public:
		int					axisCnt;
		int					axis[MAX_API_BUFFER_AXIS];
		int					idx[MAX_API_BUFFER_AXIS];
		INFO				info[MAX_API_BUFFER_AXIS];
	};

	class IoInfo
	{
	public:
		int					cnt;
		int					num[MAX_API_BUFFER_DIO];
	};

	union SeqBufferConditionArguments
	{
		SeqBufferConditionArguments() : function(nullptr) {}
		struct Cylinder
		{
			int		no;
			int		state;
		}cylinder;

		struct Vacuum
		{
			int		no;
			int		state;
		}vacuum;

		FunctionSeqeunce function;
	};

	class SeqBufferParameters
	{
	public:
		SeqBufferParameters() {};
		SeqBufferConditionType::T						type;
		SeqBufferConditionArguments						arg;
	};

	typedef TreeNode<SeqBufferConditionLogic::T, List<SeqBufferParameters>> UserCondition;

	class SeqBufferCondition 
	{
	public:
		std::shared_ptr<UserCondition>					userCondition;
		int												userMemNo;
		int												userMemCount;
		SeqBufferCondition()
		{
			userCondition = std::make_shared<UserCondition>();
		};
	};

	class FlowBufferCondition
	{
	public:
		std::shared_ptr<List<SeqBufferCondition>>		userCondition;
		int												userMemNo;
		int												userMemCount;
		FlowBufferCondition()
		{
			userCondition = std::make_shared<List<SeqBufferCondition>>();
		};
	};

	typedef struct
	{
		CommandType										type;
		MtCommand::SingleParamter						mtProfile;
		MtCommand::InterpolationParamter				lnProfile;
		ApiBufferCondition								apiBufferCondition;
		SeqBufferCondition								seqBufferCondition;
		FlowBufferCondition								flowBufferCondition;
		UINT											sleepTime;
	}Command;

	class SeqCylinderParameter
	{
	private:
		std::shared_ptr<UserCondition> layer;
		SeqBufferParameters parameter;

	public:
		SeqCylinderParameter() {};
		SeqCylinderParameter(std::shared_ptr<UserCondition> newLayer, CylinderList cylinderList)
		{
			layer = newLayer;
			parameter.type = SeqBufferConditionType::Cylinder;
			parameter.arg.cylinder.no = cylinderList;
		};
		~SeqCylinderParameter() {};
		void IsFwd()
		{
			if (NULL == layer || !Between(parameter.arg.cylinder.no, 0, cylinderMAX - 1))
				return;

			parameter.arg.cylinder.state = cylinderStateIsFwd;
			layer->list.Add(parameter);
		}
		void IsUp()
		{
			IsFwd();
		}
		void IsOpen()
		{
			IsFwd();
		}
		void IsBwd()
		{
			if (NULL == layer || !Between(parameter.arg.cylinder.no, 0, cylinderMAX - 1))
				return;

			parameter.arg.cylinder.state = cylinderStateIsBwd;
			layer->list.Add(parameter);
		}
		void IsDn()
		{
			IsBwd();
		}
		void IsClose()
		{
			IsBwd();
		}
	};

	class SeqCylinderArray
	{
	private:
		std::shared_ptr<UserCondition> layer;
		SeqCylinderParameter parameter;

	public:
		SeqCylinderArray() {};
		SeqCylinderArray(std::shared_ptr<UserCondition> newLayer)
		{
			layer = newLayer;
		};
		~SeqCylinderArray() {};
		SeqCylinderParameter& operator[](CylinderList index)
		{
			if (!Between(index, 0, cylinderMAX - 1))
				throw std::out_of_range("Index out of range");

			parameter = SeqCylinderParameter(layer, index);
			return parameter;
		}
	};

	class SeqVacuumParameter
	{
	private:
		std::shared_ptr<UserCondition> layer;
		SeqBufferParameters parameter;

	public:
		SeqVacuumParameter() {};
		SeqVacuumParameter(std::shared_ptr<UserCondition> newLayer, VacuumList vacuumList)
		{
			layer = newLayer;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = vacuumList;
		};
		~SeqVacuumParameter() 
		{

		};
		void AOn()
		{
			if (NULL == layer || !Between(parameter.arg.vacuum.no, 0, vacuumMAX - 1))
				return;

			parameter.arg.vacuum.state = vacuumStateAOn;
			layer->list.Add(parameter);
		}
		void AOff()
		{
			if (NULL == layer || !Between(parameter.arg.vacuum.no, 0, vacuumMAX - 1))
				return;

			parameter.arg.vacuum.state = vacuumStateAOff;
			layer->list.Add(parameter);
		}
		void IsOn()
		{
			if (NULL == layer || !Between(parameter.arg.vacuum.no, 0, vacuumMAX - 1))
				return;

			parameter.arg.vacuum.state = vacuumStateIsOn;
			layer->list.Add(parameter);
		}
		void IsOff()
		{
			if (NULL == layer || !Between(parameter.arg.vacuum.no, 0, vacuumMAX - 1))
				return;

			parameter.arg.vacuum.state = vacuumStateIsOff;
			layer->list.Add(parameter);
		}
	};

	class SeqVacuumArray
	{
	private:
		std::shared_ptr<UserCondition> layer;
		SeqVacuumParameter parameter;

	public:
		SeqVacuumArray() {};
		SeqVacuumArray(std::shared_ptr<UserCondition> newLayer)
		{
			layer = newLayer;
		};
		~SeqVacuumArray()
		{

		};
		SeqVacuumParameter& operator[](VacuumList index)
		{
			if (!Between(index, 0, vacuumMAX - 1))
				throw std::out_of_range("Index out of range");

			parameter = SeqVacuumParameter(layer, index);
			return parameter;
		}
	};

	class SeqFunctionParameter
	{
	private:
		std::shared_ptr<UserCondition> layer;
		SeqBufferParameters parameter;

	public:
		SeqFunctionParameter() {};
		SeqFunctionParameter(std::shared_ptr<UserCondition> newLayer)
		{
			layer = newLayer;
		};
		~SeqFunctionParameter()
		{

		};
		void Add(FunctionSeqeunce funcSequence)
		{
			if (NULL == layer || nullptr == funcSequence)
				return;
			
			parameter.type = SeqBufferConditionType::Function;
			parameter.arg.function = funcSequence;
			layer->list.Add(parameter);
		}
	};

	class FlowIfCondition
	{
	private:
		std::shared_ptr<UserCondition>				layer;

		void Clear()
		{
			layer = std::make_shared<UserCondition>();
			cylinder = SeqCylinderArray(layer);
			vacuum = SeqVacuumArray(layer);
			function = SeqFunctionParameter(layer);
		}

	public:
		SeqCylinderArray							cylinder;
		SeqVacuumArray								vacuum;
		SeqFunctionParameter						function;

		FlowIfCondition()
		{
			Clear();
		};
		~FlowIfCondition()
		{

		};

		void SetLogic(SeqBufferConditionLogic::T Logic)
		{
			Clear();
			layer->logic = Logic;
		}

		void AddCondition(FlowIfCondition addCondition)
		{
			if (nullptr == layer)
				return;

			layer->Add(addCondition.layer);
		}

		const std::shared_ptr<UserCondition>& Nodes()
		{
			return layer;
		}
	};

	union FunctionArguments
	{
		FunctionArguments() {};
		~FunctionArguments() {};
		struct AxisIsReady
		{
			AxisList					axisNo;
		}axisIsReady;

		struct AxisIsOverrideReady
		{
			AxisList					axisNo;
		}axisIsOverrideReady;

		struct AxisGetCompletedTime
		{
			AxisList					axisNo;
			double						time;
		}axisGetCompletedTime;

		struct AxisGetCompletedDistance
		{
			AxisList					axisNo;
			double						distance;
		}axisGetCompletedDistance;

		struct AxisGetRemainingTime
		{
			AxisList					axisNo;
			double						time;
		}axisGetRemainingTime;

		struct AxisGetRemainingDistance
		{
			AxisList					axisNo;
			double						distance;
		}axisGetRemainingDistance;

		struct AxisPosMove
		{
			AxisList					axisNo;
			int							posIdx;
			double						pos;
			int							velRate;
			int							accRate;
			MtCommand::SingleParamter	command;
		}axisPosMove;

		struct LinearInterpolationMove
		{
			InterpolationMotorList		lineNo;
		}linearInterpolationMove;

		struct AxisTwoSpeedMove
		{
			AxisList					axisNo;
			int							idx01;
			int							idx02;
		}axisTwoSpeedMove;

		struct AxisSemiTwoSpeedMove
		{
			AxisList					axisNo;
			int							idx;
			double						slowDist;
		}axisSemiTwoSpeedMove;

		struct CylinderFwd
		{
			CylinderList				cylinderNo;
		}cylinderFwd;

		struct CylinderBwd
		{
			CylinderList				cylinderNo;
		}cylinderBwd;

		struct CylinderIsFwd
		{
			CylinderList				cylinderNo;
		}cylinderIsFwd;

		struct CylinderIsBwd
		{
			CylinderList				cylinderNo;
		}cylinderIsBwd;

		struct VacuumOn
		{
			VacuumList					vacuumNo;
		}vacuumOn;

		struct VacuumOff
		{
			VacuumList					vacuumNo;
		}vacuumOff;

		struct VacuumBlowOn
		{
			VacuumList					vacuumNo;
		}vacuumBlowOn;

		struct VacuumBlowOff
		{
			VacuumList					vacuumNo;
		}vacuumBlowOff;

		struct VacuumAOn
		{
			VacuumList					vacuumNo;
		}vacuumAOn;

		struct VacuumAOff
		{
			VacuumList					vacuumNo;
		}vacuumAOff;

		struct VacuumIsOn
		{
			VacuumList					vacuumNo;
		}vacuumIsOn;

		struct VacuumIsOff
		{
			VacuumList					vacuumNo;
		}vacuumIsOff;

		struct IoAOn 
		{
			enDI						ioNo;
		}ioAOn;

		struct IoAOff
		{
			enDI						ioNo;
		}ioAOff;
		
		struct IoIsOn
		{
			enDO						ioNo;
		}ioIsOn;

		struct IoIsOff
		{
			enDO						ioNo;
		}ioIsOff;

		struct IoOn
		{
			enDO						ioNo;
		}ioOn;

		struct IoOff
		{
			enDO						ioNo;
		}ioOff;

		struct UserMemoryIsOn
		{
			enUSERMEM					usermemoryNo;
		}userMemoryIsOn;

		struct UserMemoryIsOff
		{
			enUSERMEM					usermemoryNo;
		}userMemoryIsOff;

		struct UserMemoryOn
		{
			enUSERMEM					usermemoryNo;
		}userMemoryOn;

		struct UserMemoryOff
		{
			enUSERMEM					usermemoryNo;
		}userMemoryOff;

		struct ControlFlowIf
		{
			FlowIfCondition				condition;
			BOOL						waitFlow;
		}controlFlowIf;

		struct ControlFlowElseIf
		{
			FlowIfCondition				condition;
		}controlFlowElseIf;

		struct ControlFlowElse
		{
		}controlFlowElse;

		struct ControlFlowEndIf
		{
		}controlFlowEndIf;

		struct ControlSleep
		{
			UINT						milliseconds;
		}controlSleep;
	};

	typedef struct
	{
		FunctionType				type;
		FunctionArguments			args;
	}Function;

	class Buffer
	{
	public:
		bool						useCoding;
		int							channel;
		int							count;
		Command						command[MAX_API_BUFFER_COMMAND];

		int							functionCount;
		Function					function[MAX_API_BUFFER_COMMAND];
		FunctionSeqeunce			funcSequence[MAX_API_BUFFER_COMMAND];

		Buffer()
		{
			ZeroMemory(this, sizeof(*this));
		}
	};

	struct ErrorBufferArguments
	{
		ErrorApiBuffer	code;
		string			message;
	};

	class ErrorBuffer
	{
	private:
		CQueue<ErrorBufferArguments> alarm;

	public:
		ErrorBuffer()
		{
			Clear();
		};
		~ErrorBuffer() {};
		ErrorApiBuffer GetLastError();
		void PrintErrorMessage();
		ErrorApiBuffer Push(string functionName, ErrorApiBuffer errorApiBuffer);
		void Clear();
	};

	class AxisBuffer
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		AxisInfo*				m_pAxisInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		AxisList				m_no;

		AxisBuffer(){};
		AxisBuffer(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pAxisInfo = pAxisInfo;
			m_pErrorBuffer = pErrorBuffer;
		};

		~AxisBuffer(){};

		ErrorApiBuffer IsReady();
		ErrorApiBuffer IsOverrideReady();
		ErrorApiBuffer GetCompletedTime(double time);
		ErrorApiBuffer GetCompletedDistance(double distance);
		ErrorApiBuffer GetRemainingTime(double time);
		ErrorApiBuffer GetRemainingDistance(double distance);
		ErrorApiBuffer Move(int posIdx);
		ErrorApiBuffer Move(int posIdx, double pos, int velRate = 0, int accRate = 0);
		ErrorApiBuffer Move(MtCommand::SingleParamter command);
		ErrorApiBuffer TwoSpeedMove(int idx01, int idx02);
		ErrorApiBuffer SemiTwoSpeedMove(int idx, double slowDist = 5);
		ErrorApiBuffer SetStartPos(double startPos);
	};

	class LineBuffer
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		AxisInfo*				m_pAxisInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		InterpolationMotorList	m_no;

		LineBuffer(){};
		LineBuffer(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pAxisInfo = pAxisInfo;
			m_pErrorBuffer = pErrorBuffer;
		};

		~LineBuffer(){};

		ErrorApiBuffer LinearInterpolationMove();
	};

	class IoBuffer : public Base
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoInfo*					m_pIoInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		IoBuffer()
		{
			name = "class IoBuffer";
		};
		IoBuffer(CFSM* pFsm, Buffer* pBuffer, IoInfo* pIoInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pIoInfo = pIoInfo;
			m_pErrorBuffer = pErrorBuffer;
		};

		~IoBuffer(){};

		ErrorApiBuffer AOn(enDI ioNo);
		ErrorApiBuffer AOff(enDI ioNo);
		ErrorApiBuffer BOn(enDI ioNo);
		ErrorApiBuffer BOff(enDI ioNo);

		ErrorApiBuffer IsOn(enDO ioNo);
		ErrorApiBuffer IsOff(enDO ioNo);
		ErrorApiBuffer On(enDO ioNo);
		ErrorApiBuffer Off(enDO ioNo);
	};

	class CylinderBuffer
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		CylinderList			m_no;

		CylinderBuffer() {};
		CylinderBuffer(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pErrorBuffer = pErrorBuffer;
		};

		~CylinderBuffer(){};

		ErrorApiBuffer Fwd();
		ErrorApiBuffer Up();
		ErrorApiBuffer Open();

		ErrorApiBuffer Bwd();
		ErrorApiBuffer Dn();
		ErrorApiBuffer Close();

		ErrorApiBuffer IsFwd();
		ErrorApiBuffer IsUp();
		ErrorApiBuffer IsOpen();

		ErrorApiBuffer IsBwd();
		ErrorApiBuffer IsDn();
		ErrorApiBuffer IsClose();
	};

	class VacuumBuffer
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		VacuumList				m_no;

		VacuumBuffer() {};
		VacuumBuffer(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pErrorBuffer = pErrorBuffer;
		};

		~VacuumBuffer(){};

		ErrorApiBuffer On();
		ErrorApiBuffer Off();
		ErrorApiBuffer BlowOn();
		ErrorApiBuffer BlowOff();
		ErrorApiBuffer AOn();
		ErrorApiBuffer AOff();
		ErrorApiBuffer IsOn();
		ErrorApiBuffer IsOff();
	};

	class UserMemoryBuffer
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoInfo*					m_pIoInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		UserMemoryBuffer() {};
		UserMemoryBuffer(CFSM* pFsm, Buffer* pBuffer, IoInfo* pIoInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pIoInfo = pIoInfo;
			m_pErrorBuffer = pErrorBuffer;
		};
		~UserMemoryBuffer(){};

		ErrorApiBuffer IsOn(enUSERMEM userMemNo);
		ErrorApiBuffer IsOff(enUSERMEM userMemNo);
		ErrorApiBuffer On(enUSERMEM userMemNo);
		ErrorApiBuffer Off(enUSERMEM userMemNo);
	};

	class BufferParameter : public Base
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		AxisInfo*				m_pAxisInfo;
		IoInfo*					m_pIoInfo;
		IoInfo*					m_pUsermemoryInfo;
		ErrorBuffer*			m_pErrorBuffer;
		FlowBufferCondition*	m_pBufferCondition;

		AxisBuffer				axisBuffer;
		LineBuffer				lineBuffer;
		CylinderBuffer			cylinderBuffer;
		VacuumBuffer			vacuumBuffer;
		UserMemoryBuffer		userMemoryBuffer;

		ErrorApiBuffer FlowStartIf(BOOL waitFlow);

	public:
		BufferParameter()
		{
			name = "class BufferParameter";
		};
		BufferParameter(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, IoInfo* pIoInfo, IoInfo* pUsermemoryInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pAxisInfo = pAxisInfo;
			m_pIoInfo = pIoInfo;
			m_pUsermemoryInfo = pUsermemoryInfo;
			m_pErrorBuffer = pErrorBuffer;
			axisBuffer = AxisBuffer(pFsm, pBuffer, pAxisInfo, pErrorBuffer);
			lineBuffer = LineBuffer(pFsm, pBuffer, pAxisInfo, pErrorBuffer);
			cylinderBuffer = CylinderBuffer(pFsm, pBuffer, pErrorBuffer);
			vacuumBuffer = VacuumBuffer(pFsm, pBuffer, pErrorBuffer);
			userMemoryBuffer = UserMemoryBuffer(pFsm, pBuffer, pUsermemoryInfo, pErrorBuffer);
			ZeroMemory(m_pIoInfo, sizeof(*m_pIoInfo));
			ZeroMemory(m_pUsermemoryInfo, sizeof(*m_pUsermemoryInfo));
			ZeroMemory(m_pAxisInfo, sizeof(*m_pAxisInfo));
			m_pBufferCondition = nullptr;
		};
		~BufferParameter() {};
		AxisBuffer& operator[](AxisList index)
		{
			if (!Between(index, 0, motorMAX - 1))
				throw std::out_of_range("Index out of range");

			axisBuffer.m_no = AxisList(index);
			return axisBuffer;
		}
		LineBuffer& operator[](InterpolationMotorList index)
		{
			if (!Between(index, 0, interpolationMAX - 1))
				throw std::out_of_range("Index out of range");

			lineBuffer.m_no = InterpolationMotorList(index);
			return lineBuffer;
		}
		CylinderBuffer& operator[](CylinderList index)
		{
			if (!Between(index, 0, cylinderMAX - 1))
				throw std::out_of_range("Index out of range");

			cylinderBuffer.m_no = CylinderList(index);
			return cylinderBuffer;
		}
		VacuumBuffer& operator[](VacuumList index)
		{
			if (!Between(index, 0, vacuumMAX - 1))
				throw std::out_of_range("Index out of range");

			vacuumBuffer.m_no = VacuumList(index);
			return vacuumBuffer;
		}

		ErrorApiBuffer AOn(enDI ioNo);
		ErrorApiBuffer AOff(enDI ioNo);
		ErrorApiBuffer BOn(enDI ioNo);
		ErrorApiBuffer BOff(enDI ioNo);

		ErrorApiBuffer IsOn(enDO ioNo);
		ErrorApiBuffer IsOff(enDO ioNo);
		ErrorApiBuffer On(enDO ioNo);
		ErrorApiBuffer Off(enDO ioNo);

		ErrorApiBuffer IsOn(enUSERMEM userMemNo);
		ErrorApiBuffer IsOff(enUSERMEM userMemNo);
		ErrorApiBuffer On(enUSERMEM userMemNo);
		ErrorApiBuffer Off(enUSERMEM userMemNo);

		ErrorApiBuffer FlowIf(FlowIfCondition condition, BOOL waitFlow = FALSE);
		ErrorApiBuffer FlowElseIf(FlowIfCondition condition);
		ErrorApiBuffer FlowElse();
		ErrorApiBuffer FlowEndIf();
		ErrorApiBuffer Sleep(UINT milliseconds);
		void CommandClear();
	};

	class AxisCommand
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		AxisInfo*				m_pAxisInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		AxisList				m_no;

		AxisCommand() {};
		AxisCommand(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pAxisInfo = pAxisInfo;
			m_pErrorBuffer = pErrorBuffer;
		};

		~AxisCommand() {};

		ErrorApiBuffer IsReady();
		ErrorApiBuffer IsOverrideReady();
		ErrorApiBuffer GetCompletedTime(double time);
		ErrorApiBuffer GetCompletedDistance(double distance);
		ErrorApiBuffer GetRemainingTime(double time);
		ErrorApiBuffer GetRemainingDistance(double distance);
		ErrorApiBuffer PosMove(int posIdx);
		ErrorApiBuffer PosMove(int posIdx, double pos, int velRate = 0, int accRate = 0);
		ErrorApiBuffer PosMove(MtCommand::SingleParamter command);
		ErrorApiBuffer LinearInterpolationMove(InterpolationMotorList line);
		ErrorApiBuffer TwoSpeedMove(int idx01, int idx02);
		ErrorApiBuffer SemiTwoSpeedMove(int idx, double slowDist = 5);
	};

	class AxisCommandArray : public Base
	{
	private:
		AxisCommand axisBuffer;

	public:
		AxisCommandArray()
		{
			name = "class AxisArray";
		};
		AxisCommandArray(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, ErrorBuffer* pErrorBuffer)
		{
			axisBuffer = AxisCommand(pFsm, pBuffer, pAxisInfo, pErrorBuffer);
		};
		~AxisCommandArray() {};
		AxisCommand& operator[](AxisList index)
		{
			if (!Between(index, 0, motorMAX - 1))
				throw std::out_of_range("Index out of range");

			axisBuffer.m_no = (AxisList)index;
			return axisBuffer;
		}
	};

	class CylinderCommand
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoBuffer*				m_pIoBuffer;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		CylinderList			m_no;

		CylinderCommand() {};
		CylinderCommand(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pErrorBuffer = pErrorBuffer;
		};

		~CylinderCommand() {};

		ErrorApiBuffer Fwd();
		ErrorApiBuffer Up();
		ErrorApiBuffer Open();

		ErrorApiBuffer Bwd();
		ErrorApiBuffer Dn();
		ErrorApiBuffer Close();

		ErrorApiBuffer IsFwd();
		ErrorApiBuffer IsUp();
		ErrorApiBuffer IsOpen();

		ErrorApiBuffer IsBwd();
		ErrorApiBuffer IsDn();
		ErrorApiBuffer IsClose();
	};

	class CylinderCommandArray : public Base
	{
	private:
		CylinderCommand cylinderCommand;

	public:
		CylinderCommandArray()
		{
			name = "class CylinderCommandArray";
		};
		CylinderCommandArray(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			cylinderCommand = CylinderCommand(pFsm, pBuffer, pErrorBuffer);
		};
		~CylinderCommandArray() {};
		CylinderCommand& operator[](CylinderList index)
		{
			if (!Between(index, 0, cylinderMAX - 1))
				throw std::out_of_range("Index out of range");

			cylinderCommand.m_no = (CylinderList)index;
			return cylinderCommand;
		}
	};

	class VacuumCommand
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoBuffer*				m_pIoBuffer;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		VacuumList				m_no;

		VacuumCommand() {};
		VacuumCommand(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pErrorBuffer = pErrorBuffer;
		};

		~VacuumCommand() {};

		ErrorApiBuffer On();
		ErrorApiBuffer Off();
		ErrorApiBuffer BlowOn();
		ErrorApiBuffer BlowOff();
		ErrorApiBuffer AOn();
		ErrorApiBuffer AOff();
	};

	class VacuumCommandArray : public Base
	{
	private:
		VacuumCommand vacuumCommand;

	public:
		VacuumCommandArray()
		{
			name = "class VacuumCommandArray";
		};
		VacuumCommandArray(CFSM* pFsm, Buffer* pBuffer, ErrorBuffer* pErrorBuffer)
		{
			vacuumCommand = VacuumCommand(pFsm, pBuffer, pErrorBuffer);
		};
		~VacuumCommandArray() {};
		VacuumCommand& operator[](VacuumList index)
		{
			if (!Between(index, 0, vacuumMAX - 1))
				throw std::out_of_range("Index out of range");

			vacuumCommand.m_no = (VacuumList)index;
			return vacuumCommand;
		}
	};

	class IoCommand : public Base
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoInfo*					m_pIoInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		IoCommand()
		{
			name = "class IoBuffer";
		};
		IoCommand(CFSM* pFsm, Buffer* pBuffer, IoInfo* pIoInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pIoInfo = pIoInfo;
			m_pErrorBuffer = pErrorBuffer;
		};

		~IoCommand() {};

		ErrorApiBuffer AOn(enDI ioNo);
		ErrorApiBuffer AOff(enDI ioNo);
		ErrorApiBuffer BOn(enDI ioNo);
		ErrorApiBuffer BOff(enDI ioNo);

		ErrorApiBuffer IsOn(enDO ioNo);
		ErrorApiBuffer IsOff(enDO ioNo);
		ErrorApiBuffer On(enDO ioNo);
		ErrorApiBuffer Off(enDO ioNo);
	};

	class UserMemoryCommand : public Base
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		IoInfo*					m_pIoInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		UserMemoryCommand() {};
		UserMemoryCommand(CFSM* pFsm, Buffer* pBuffer, IoInfo* pIoInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pIoInfo = pIoInfo;
			m_pErrorBuffer = pErrorBuffer;
		};
		~UserMemoryCommand() {};

		ErrorApiBuffer IsOn(enUSERMEM userMemNo);
		ErrorApiBuffer IsOff(enUSERMEM userMemNo);
		ErrorApiBuffer On(enUSERMEM userMemNo);
		ErrorApiBuffer Off(enUSERMEM userMemNo);
	};

	class ControlCommand
	{
	private:
		CFSM*					m_pFsm;
		Buffer*					m_pBuffer;
		AxisInfo*				m_pAxisInfo;
		IoInfo*					m_pIoInfo;
		IoInfo*					m_pUsermemInfo;
		ErrorBuffer*			m_pErrorBuffer;

	public:
		ControlCommand()
		{

		}

		ControlCommand(CFSM* pFsm, Buffer* pBuffer, AxisInfo* pAxisInfo, IoInfo* pIoInfo, IoInfo* pUsermemInfo, ErrorBuffer* pErrorBuffer)
		{
			m_pFsm = pFsm;
			m_pBuffer = pBuffer;
			m_pAxisInfo = pAxisInfo;
			m_pIoInfo = pIoInfo;
			m_pUsermemInfo = pUsermemInfo;
			m_pErrorBuffer = pErrorBuffer;
			ZeroMemory(m_pIoInfo, sizeof(*m_pIoInfo));
			ZeroMemory(m_pUsermemInfo, sizeof(*m_pUsermemInfo));
			ZeroMemory(m_pAxisInfo, sizeof(*m_pAxisInfo));
		};

		~ControlCommand()
		{

		}

		ErrorApiBuffer FlowIf(FlowIfCondition condition, BOOL waitFlow = FALSE);
		ErrorApiBuffer FlowElseIf(FlowIfCondition condition);
		ErrorApiBuffer FlowElse();
		ErrorApiBuffer FlowEndIf();
		ErrorApiBuffer Sleep(UINT milliseconds);
	};

	class ApiBufferCommand
	{
	private:
		CFSM					m_fsm;
		AxisInfo				m_axisInfo;
		IoInfo					m_ioInfo;
		IoInfo					m_usermemoryInfo;
		Buffer					m_buffer;
		DWORD					m_dwTimeLimit;
		BufferParameter			bufferParameter;

	public:
		AxisCommandArray		axis;
		CylinderCommandArray	cylinder;
		VacuumCommandArray		vacuum;
		IoCommand				io;
		UserMemoryCommand		usermemory;
		ControlCommand			control;
		ErrorBuffer				error;

		static const int	maxDevice = (apibufferMAX > 0) ? apibufferMAX : 1;

		ApiBufferCommand()
		{
			bufferParameter = BufferParameter(&m_fsm, &m_buffer, &m_axisInfo, &m_ioInfo, &m_usermemoryInfo, &error);
			
			axis = AxisCommandArray(&m_fsm, &m_buffer, &m_axisInfo, &error);
			cylinder = CylinderCommandArray(&m_fsm, &m_buffer, &error);
			vacuum = VacuumCommandArray(&m_fsm, &m_buffer, &error);
			io = IoCommand(&m_fsm, &m_buffer, &m_ioInfo, &error);
			usermemory = UserMemoryCommand(&m_fsm, &m_buffer, &m_usermemoryInfo, &error);
			control = ControlCommand(&m_fsm, &m_buffer, &m_axisInfo, &m_ioInfo, &m_usermemoryInfo, &error);

			m_buffer.channel = NOT_DEFINED;
			m_buffer.count = 0;
			m_buffer.functionCount = 0;
			ZeroMemory(m_buffer.function, sizeof(m_buffer.function));
			ZeroMemory(m_buffer.funcSequence, sizeof(m_buffer.funcSequence));

			ZeroMemory(&m_ioInfo, sizeof(m_ioInfo));
			ZeroMemory(&m_usermemoryInfo, sizeof(m_usermemoryInfo));
			ZeroMemory(&m_axisInfo, sizeof(m_axisInfo));
		}
		~ApiBufferCommand(){}

		ErrorApiBuffer Run(void);
		ErrorApiBuffer Stop(void);
		void Clear();
		void BitClear(void);
		int  Channel(void);
		BOOL IsBusy(void);
		void CommandCycle(void);
		ErrorApiBuffer Start(DWORD dwTimeLimit = 300000);
		ErrorApiBuffer End(void);
	};

	template<typename T1, typename T2>
	BOOL CheckNodeListCondition(const std::shared_ptr<TreeNode<T1, T2>>& node);
	template<typename T1, typename T2>
	BOOL IsNodeCondition(const std::shared_ptr<TreeNode<T1, T2>>& node);
};