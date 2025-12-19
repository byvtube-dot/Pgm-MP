#include "..\Includes.h"

namespace COM
{
	ErrorApiBuffer ErrorBuffer::GetLastError()
	{
		if (0 < alarm.size())
		{
			ErrorBufferArguments arguments = alarm.back();
			return arguments.code;
		}
		return (ERR_NONE);
	};

	void ErrorBuffer::PrintErrorMessage()
	{
		if (0 < alarm.size())
		{
			CQueue<ErrorBufferArguments> queue = alarm;
			size_t maxCount = queue.size();
			for (int i = 0; i < maxCount; i++)
			{
				if (queue.empty())
					break;

				ErrorBufferArguments arguments = queue.pop();
				LOG[logSEQ].Message("%s", arguments.message.c_str());
			}
		}
	};

	ErrorApiBuffer ErrorBuffer::Push(string functionName, ErrorApiBuffer errorApiBuffer)
	{
		char buffer[256] = { 0 };
		snprintf(buffer, sizeof(buffer), "[ApiBuffer],[%s],[Errcode],[%.3d]", functionName.c_str(), errorApiBuffer);
		ErrorBufferArguments bufferArguments;
		bufferArguments.code = errorApiBuffer;
		bufferArguments.message = string(buffer);
		alarm.push(bufferArguments);
		return errorApiBuffer;
	};

	void ErrorBuffer::Clear()
	{
		alarm.clear();
	};

	ErrorApiBuffer AxisBuffer::IsReady()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::OpState;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.opState.axis = m_no;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.opState.opState = OperationState::Idle;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::IsOverrideReady()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::MotionStartedOverrideReady;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.motionStartedOverrideReady.axis = m_no;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::GetCompletedTime(double time)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::CompletedTime;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.completedTime.axis = m_no;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.completedTime.timeMilliseconds = time;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::GetCompletedDistance(double distance)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::CompletedDistance;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.completedDistance.axis = m_no;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.completedDistance.distance = distance;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::GetRemainingTime(double time)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::RemainingTime;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.remainingTime.axis = m_no;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.remainingTime.timeMilliseconds = time;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::GetRemainingDistance(double distance)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::RemainingDistance;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.remainingDistance.axis = m_no;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.remainingDistance.distance = distance;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::Move(int posIdx)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeAxis;
		m_pBuffer->command[m_pBuffer->count].mtProfile = MT[m_no].CommandTable(posIdx);

		BOOL isExist = (FALSE);
		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			if (m_pAxisInfo->axis[axisCnt] == m_pBuffer->command[m_pBuffer->count].mtProfile.no)
			{
				int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
				if (posCnt >= (MAX_API_BUFFER_POS - 1))
				{
					m_pFsm->Set(C_ERROR);
					return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
				}

				m_pAxisInfo->idx[axisCnt] = m_pBuffer->command[m_pBuffer->count].mtProfile.idx;
				m_pAxisInfo->info[axisCnt].startPos[posCnt + 1] = m_pBuffer->command[m_pBuffer->count].mtProfile.pos;
				m_pBuffer->command[m_pBuffer->count].mtProfile.startPos = m_pAxisInfo->info[axisCnt].startPos[posCnt];
				posCnt++;
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::Move(int posIdx, double pos, int velRate, int accRate)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			if (0 == (int)velRate)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_VEL_VALUE);
			}
			if (0 == (int)accRate)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACC_VALUE);
			}
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeAxis;

		ZeroMemory(&m_pBuffer->command[m_pBuffer->count].mtProfile, sizeof(m_pBuffer->command[m_pBuffer->count].mtProfile));
		m_pBuffer->command[m_pBuffer->count].mtProfile.no = m_no;
		m_pBuffer->command[m_pBuffer->count].mtProfile.idx = posIdx;
		m_pBuffer->command[m_pBuffer->count].mtProfile.pos = pos;
		m_pBuffer->command[m_pBuffer->count].mtProfile.profile.vel = (double)round(m_pBuffer->command[m_pBuffer->count].mtProfile.maxVel * (velRate / 100.) * (NV.sys[machineSpeed] / 100.));
		m_pBuffer->command[m_pBuffer->count].mtProfile.profile.acc = (double)round(m_pBuffer->command[m_pBuffer->count].mtProfile.maxAcc * (accRate / 100.) * (NV.sys[machineSpeed] / 100.));
		if (0 == (int)velRate)
			m_pBuffer->command[m_pBuffer->count].mtProfile.profile.vel = MT[m_no].CommandTable(posIdx).profile.vel;
		if (0 == (int)accRate)
			m_pBuffer->command[m_pBuffer->count].mtProfile.profile.acc = MT[m_no].CommandTable(posIdx).profile.acc;

		m_pBuffer->command[m_pBuffer->count].mtProfile.profile.jerkAcc = (double)m_pBuffer->command[m_pBuffer->count].mtProfile.maxJerkAcc;
		m_pBuffer->command[m_pBuffer->count].mtProfile.profile.jerkRatio = MT[m_no].m_jerkRatio;
		m_pBuffer->command[m_pBuffer->count].mtProfile.limitP = MT[m_no].m_limitP;
		m_pBuffer->command[m_pBuffer->count].mtProfile.limitN = MT[m_no].m_limitN;

		BOOL isExist = (FALSE);
		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			if (m_pAxisInfo->axis[axisCnt] == m_pBuffer->command[m_pBuffer->count].mtProfile.no)
			{
				int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
				if (posCnt >= (MAX_API_BUFFER_POS - 1))
				{
					m_pFsm->Set(C_ERROR);
					return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
				}

				m_pAxisInfo->idx[axisCnt] = m_pBuffer->command[m_pBuffer->count].mtProfile.idx;
				m_pAxisInfo->info[axisCnt].startPos[posCnt + 1] = m_pBuffer->command[m_pBuffer->count].mtProfile.pos;
				m_pBuffer->command[m_pBuffer->count].mtProfile.startPos = m_pAxisInfo->info[axisCnt].startPos[posCnt];
				posCnt++;
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::Move(MtCommand::SingleParamter command)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeAxis;
		m_pBuffer->command[m_pBuffer->count].mtProfile = command;

		BOOL isExist = (FALSE);
		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			if (m_pAxisInfo->axis[axisCnt] == m_pBuffer->command[m_pBuffer->count].mtProfile.no)
			{
				int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
				if (posCnt >= (MAX_API_BUFFER_POS - 1))
				{
					m_pFsm->Set(C_ERROR);
					return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
				}

				m_pAxisInfo->idx[axisCnt] = m_pBuffer->command[m_pBuffer->count].mtProfile.idx;
				m_pAxisInfo->info[axisCnt].startPos[posCnt + 1] = m_pBuffer->command[m_pBuffer->count].mtProfile.pos;
				m_pBuffer->command[m_pBuffer->count].mtProfile.startPos = m_pAxisInfo->info[axisCnt].startPos[posCnt];
				posCnt++;
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::TwoSpeedMove(int idx01, int idx02)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(idx01, 0, MAX_INDEX_MOTOR - 1) || !Between(idx02, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}
		if (MT[m_no].CommandTable(idx01).profile.vel == MT[m_no].CommandTable(idx02).profile.vel)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_VEL_VALUE);
		}

		double pos1st = (double)MT[m_no].PosTable(idx01);
		double pos2nd = (double)MT[m_no].PosTable(idx02);
		double posCur = 0;

		BOOL isExist = (FALSE);
		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
			if (posCnt >= (MAX_API_BUFFER_POS - 1))
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
			}

			if (m_pAxisInfo->axis[axisCnt] == m_no)
			{
				posCur = m_pAxisInfo->info[axisCnt].startPos[posCnt];
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}

		double distance = NOT_DEFINED;
		if (posCur < pos2nd)
			distance = pos1st - posCur;
		else if (posCur > pos2nd)
			distance = posCur - pos1st;

		if (0 >= distance)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_VALUE);
		}

		MtCommand::SingleParamter mov1st = MT[m_no].CommandTable(idx01);
		MtCommand::SingleParamter mov2nd = MT[m_no].CommandTable(idx02);
		mov2nd.startPos = mov1st.pos;

		BOOL isGood = FALSE;
		double peakVelocity = 0; double totalTime = 0;
		double accelerationTime = 0; double cruiseTime = 0; double decelerationTime = 0;

		if (mov1st.profile.vel < mov2nd.profile.vel)
		{
			mov1st.profile.endVelocity = mov1st.profile.vel;
			for (int i = 0; i < 10; i++)
			{
				peakVelocity = totalTime = accelerationTime = cruiseTime = decelerationTime = 0;
				WMX.CommandSimulateTime(mov1st, peakVelocity,
					totalTime, accelerationTime, cruiseTime, decelerationTime);

				peakVelocity = (round(peakVelocity) * 1000.) / 1000.;

				if (mov1st.profile.vel == peakVelocity)
				{
					isGood = TRUE;
					break;
				}
				else
				{
					mov1st.profile.vel = peakVelocity;
					mov1st.profile.endVelocity = mov1st.profile.vel;
				}
			}

			if (!isGood || 0 >= peakVelocity)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
			}

			mov1st.profile.startingVelocity = 0;
			mov1st.profile.endVelocity = mov1st.profile.vel;

			mov2nd.profile.startingVelocity = mov1st.profile.vel;
		}
		else
		{
			mov2nd.profile.endVelocity = mov2nd.profile.vel;
			for (int i = 0; i < 10; i++)
			{
				peakVelocity = totalTime = accelerationTime = cruiseTime = decelerationTime = 0;
				WMX.CommandSimulateTime(mov2nd, peakVelocity,
					totalTime, accelerationTime, cruiseTime, decelerationTime);

				peakVelocity = (round(peakVelocity) * 1000.) / 1000.;

				if (mov2nd.profile.vel == peakVelocity)
				{
					isGood = TRUE;
					break;
				}
				else
				{
					mov2nd.profile.vel = peakVelocity;
					mov2nd.profile.endVelocity = mov2nd.profile.vel;
				}
			}

			if (!isGood || 0 >= peakVelocity)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
			}

			mov2nd.profile.startingVelocity = mov2nd.profile.vel;
			mov2nd.profile.endVelocity = 0;

			mov1st.profile.endVelocity = mov2nd.profile.vel;
		}

		long isError = FALSE;
		isError |= IsReady();
		isError |= Move(mov1st);
		isError |= GetRemainingDistance(1);
		isError |= Move(mov2nd);

		if (isError)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
		}

		return (ERR_NONE);
	}


	ErrorApiBuffer AxisBuffer::SemiTwoSpeedMove(int idx, double slowDist)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(idx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}
		double posCur = 0;

		BOOL isExist = (FALSE);
		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
			if (posCnt >= (MAX_API_BUFFER_POS - 1))
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
			}

			if (m_pAxisInfo->axis[axisCnt] == m_no)
			{
				posCur = m_pAxisInfo->info[axisCnt].startPos[posCnt];
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		MtCommand::SingleParamter mov1st = MT[m_no].CommandTable(idx);
		MtCommand::SingleParamter mov2nd = MT[m_no].CommandTable(idx);

		if (posCur < mov2nd.pos)
		{
			mov2nd.profile.acc = mov1st.profile.acc * 0.1;
			mov2nd.profile.vel = mov1st.profile.vel * 0.1;
			mov1st.pos = mov2nd.pos - slowDist;

			if (mov2nd.profile.acc < 1)
			{
				mov2nd.profile.acc = 1;
			}
			if (mov2nd.profile.vel < 1)
			{
				mov2nd.profile.vel = 1;
			}
		}
		else if (posCur > mov2nd.pos)
		{
			mov1st.profile.acc = mov2nd.profile.acc * 0.1;
			mov1st.profile.vel = mov2nd.profile.vel * 0.1;
			mov1st.pos = posCur - slowDist;

			if (mov1st.profile.acc < 1)
			{
				mov1st.profile.acc = 1;
			}
			if (mov1st.profile.vel < 1)
			{
				mov1st.profile.vel = 1;
			}
		}

		double pos1st = mov1st.pos;
		double pos2nd = mov2nd.pos;

		double distance = NOT_DEFINED;
		if (posCur < pos2nd)
			distance = pos1st - posCur;
		else if (posCur > pos2nd)
			distance = posCur - pos1st;

		if (0 >= distance)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_VALUE);
		}

		mov2nd.startPos = mov1st.pos;

		BOOL isGood = FALSE;
		double peakVelocity = 0; double totalTime = 0;
		double accelerationTime = 0; double cruiseTime = 0; double decelerationTime = 0;

		if (mov1st.profile.vel < mov2nd.profile.vel)
		{
			mov1st.profile.endVelocity = mov1st.profile.vel;
			for (int i = 0; i < 10; i++)
			{
				peakVelocity = totalTime = accelerationTime = cruiseTime = decelerationTime = 0;
				WMX.CommandSimulateTime(mov1st, peakVelocity,
					totalTime, accelerationTime, cruiseTime, decelerationTime);

				peakVelocity = (round(peakVelocity) * 1000.) / 1000.;

				if (mov1st.profile.vel == peakVelocity)
				{
					isGood = TRUE;
					break;
				}
				else
				{
					mov1st.profile.vel = peakVelocity;
					mov1st.profile.endVelocity = mov1st.profile.vel;
				}
			}

			if (!isGood || 0 >= peakVelocity)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
			}

			mov1st.profile.startingVelocity = 0;
			mov1st.profile.endVelocity = mov1st.profile.vel;

			mov2nd.profile.startingVelocity = mov1st.profile.vel;
		}
		else
		{
			mov2nd.profile.endVelocity = mov2nd.profile.vel;
			for (int i = 0; i < 10; i++)
			{
				peakVelocity = totalTime = accelerationTime = cruiseTime = decelerationTime = 0;
				WMX.CommandSimulateTime(mov2nd, peakVelocity,
					totalTime, accelerationTime, cruiseTime, decelerationTime);

				peakVelocity = (round(peakVelocity) * 1000.) / 1000.;

				if (mov2nd.profile.vel == peakVelocity)
				{
					isGood = TRUE;
					break;
				}
				else
				{
					mov2nd.profile.vel = peakVelocity;
					mov2nd.profile.endVelocity = mov2nd.profile.vel;
				}
			}

			if (!isGood || 0 >= peakVelocity)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
			}

			mov2nd.profile.startingVelocity = mov2nd.profile.vel;
			mov2nd.profile.endVelocity = 0;

			mov1st.profile.endVelocity = mov2nd.profile.vel;
		}

		long isError = FALSE;
		isError |= IsReady();
		isError |= Move(mov1st);
		isError |= GetRemainingDistance(1);
		isError |= Move(mov2nd);

		if (isError)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_OPERATION_CONDITION);
		}

		return (ERR_NONE);
	}

	ErrorApiBuffer AxisBuffer::SetStartPos(double startPos)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		if (m_pAxisInfo->axisCnt >= (MAX_API_BUFFER_AXIS - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}

		for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
		{
			if (m_pAxisInfo->axis[axisCnt] != m_no)
				continue;

			return (ERR_NONE);
		}
		m_pAxisInfo->axis[m_pAxisInfo->axisCnt] = m_no;
		m_pAxisInfo->info[m_pAxisInfo->axisCnt].startPos[0] = startPos;
		m_pAxisInfo->axisCnt++;
		return (ERR_NONE);
	}

	ErrorApiBuffer LineBuffer::LinearInterpolationMove()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (1 > LN[m_no].m_command.axisCount)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_LINE_AXIS);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeLine;
		m_pBuffer->command[m_pBuffer->count].lnProfile = LN[m_no].m_command;

		for (int lineCnt = 0; lineCnt < m_pBuffer->command[m_pBuffer->count].lnProfile.axisCount; lineCnt++)
		{
			BOOL isExist = (FALSE);
			for (int axisCnt = 0; axisCnt < m_pAxisInfo->axisCnt; axisCnt++)
			{
				if (m_pAxisInfo->axis[axisCnt] == m_pBuffer->command[m_pBuffer->count].lnProfile.parameter[lineCnt].no)
				{
					int& posCnt = m_pAxisInfo->info[axisCnt].cnt;
					if (posCnt >= (MAX_API_BUFFER_POS - 1))
					{
						m_pFsm->Set(C_ERROR);
						return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_POS_RANGE_OUT);
					}

					m_pAxisInfo->idx[axisCnt] = m_pBuffer->command[m_pBuffer->count].lnProfile.parameter[lineCnt].idx;
					m_pAxisInfo->info[axisCnt].startPos[posCnt + 1] = m_pBuffer->command[m_pBuffer->count].lnProfile.parameter[lineCnt].pos;
					m_pBuffer->command[m_pBuffer->count].lnProfile.parameter[lineCnt].startPos = m_pAxisInfo->info[axisCnt].startPos[posCnt];
					posCnt++;
					isExist = (TRUE);
					break;
				}
			}
			if (!isExist)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
			}
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::AOn(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOInput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress = WMX.m_di[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.invert = (FALSE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::AOff(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOInput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress = WMX.m_di[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.invert = (TRUE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::BOn(enDI ioNo)
	{
		return AOff(ioNo);
	}

	ErrorApiBuffer IoBuffer::BOff(enDI ioNo)
	{
		return AOn(ioNo);
	}

	ErrorApiBuffer IoBuffer::IsOn(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (FALSE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::IsOff(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (TRUE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::On(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeIo;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (TRUE);
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == ioNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = ioNo;
			m_pIoInfo->cnt++;
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoBuffer::Off(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeIo;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (FALSE);
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == ioNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = ioNo;
			m_pIoInfo->cnt++;
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderBuffer::Fwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Cylinder;
			parameter.arg.cylinder.no = m_no;
			parameter.arg.cylinder.state = cylinderStateFwd;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderBuffer::Up()
	{
		return Fwd();
	}

	ErrorApiBuffer CylinderBuffer::Open()
	{
		return Fwd();
	}

	ErrorApiBuffer CylinderBuffer::Bwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Cylinder;
			parameter.arg.cylinder.no = m_no;
			parameter.arg.cylinder.state = cylinderStateBwd;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderBuffer::Dn()
	{
		return Bwd();
	}

	ErrorApiBuffer CylinderBuffer::Close()
	{
		return Bwd();
	}

	ErrorApiBuffer CylinderBuffer::IsFwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;
			
			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Cylinder;
			parameter.arg.cylinder.no = m_no;
			parameter.arg.cylinder.state = cylinderStateIsFwd;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderBuffer::IsUp()
	{
		return IsFwd();
	}

	ErrorApiBuffer CylinderBuffer::IsOpen()
	{
		return IsFwd();
	}

	ErrorApiBuffer CylinderBuffer::IsBwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Cylinder;
			parameter.arg.cylinder.no = m_no;
			parameter.arg.cylinder.state = cylinderStateIsBwd;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderBuffer::IsDn()
	{
		return IsBwd();
	}

	ErrorApiBuffer CylinderBuffer::IsClose()
	{
		return IsBwd();
	}

	ErrorApiBuffer VacuumBuffer::On()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateOn;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::Off()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateOff;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::BlowOn()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateBlowOn;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::BlowOff()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateBlowOff;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::AOn()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateAOn;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::AOff()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateAOff;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::IsOn()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateIsOn;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumBuffer::IsOff()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);
			m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserCondition;

			SeqBufferParameters parameter;
			parameter.type = SeqBufferConditionType::Vacuum;
			parameter.arg.vacuum.no = m_no;
			parameter.arg.vacuum.state = vacuumStateIsOff;

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->logic = SeqBufferConditionLogic::T::AND;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition->list.Add(parameter);
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryBuffer::IsOn(enUSERMEM userMemNo)
	{
		UINT bOn = TRUE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = !bOn;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryBuffer::IsOff(enUSERMEM userMemNo)
	{
		UINT bOn = FALSE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = !bOn;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryBuffer::On(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (TRUE);
		m_pBuffer->count++;

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == userMemNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = userMemNo;
			m_pIoInfo->cnt++;
		}
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryBuffer::Off(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
		m_pBuffer->count++;

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == userMemNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = userMemNo;
			m_pIoInfo->cnt++;
		}
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::AOn(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOInput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress = WMX.m_di[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.invert = (FALSE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::AOff(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOInput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress = WMX.m_di[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioInput.invert = (TRUE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::BOn(enDI ioNo)
	{
		return AOff(ioNo);
	}

	ErrorApiBuffer BufferParameter::BOff(enDI ioNo)
	{
		return AOn(ioNo);
	}

	ErrorApiBuffer BufferParameter::IsOn(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (FALSE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::IsOff(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (TRUE);
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::On(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeIo;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (TRUE);
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == ioNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = ioNo;
			m_pIoInfo->cnt++;
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::Off(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}

		int chIdx = (ioNo / 100);
		int bit16 = ((ioNo % 100) % MAX_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeIo;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::IOOutput;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress = WMX.m_do[chIdx].byteAddress;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.invert = (FALSE);
		if (8 <= bit16)
		{
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.byteAddress++;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.ioOutput.bitAddress = bit16 - 8;
		}

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == ioNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = ioNo;
			m_pIoInfo->cnt++;
		}

		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::IsOn(enUSERMEM userMemNo)
	{
		UINT bOn = TRUE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = !bOn;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::IsOff(enUSERMEM userMemNo)
	{
		UINT bOn = FALSE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeCondition;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = !bOn;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::On(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (TRUE);
		m_pBuffer->count++;

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == userMemNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = userMemNo;
			m_pIoInfo->cnt++;
		}
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::Off(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		int chIdx = (userMemNo / 100);
		int bit08 = ((userMemNo % 100) % MAX_USERMEM_CONTACT);

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeUserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
		m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
		m_pBuffer->count++;

		BOOL isExist = (FALSE);
		for (int iocnt = 0; iocnt < m_pIoInfo->cnt; iocnt++)
		{
			if (m_pIoInfo->num[iocnt] == userMemNo)
			{
				isExist = (TRUE);
				break;
			}
		}
		if (!isExist)
		{
			m_pIoInfo->num[m_pIoInfo->cnt] = userMemNo;
			m_pIoInfo->cnt++;
		}
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::FlowStartIf(BOOL waitFlow)
	{
		if (!waitFlow)
			return (ERR_NONE);

		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (NULL != m_pBufferCondition)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_WRONG_ALLOCATED_BUFFER);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeFlowStartIf;

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);

			m_pBuffer->command[m_pBuffer->count].flowBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].flowBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		m_pBufferCondition = &m_pBuffer->command[m_pBuffer->count].flowBufferCondition;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::FlowIf(FlowIfCondition condition, BOOL waitFlow)
	{
		if (FlowStartIf(waitFlow))
		{
			return m_pErrorBuffer->GetLastError();
		}
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeFlowIf;

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition = condition.Nodes();
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		if (NULL != m_pBufferCondition)
		{
			const SeqBufferCondition& element = m_pBuffer->command[m_pBuffer->count].seqBufferCondition;
			m_pBufferCondition->userCondition->Add(element);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::FlowElseIf(FlowIfCondition condition)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeFlowElseIf;

		bool IsAllocatedBuffer = false;
		for (int i = 0; i < MAX_USERMEM_BUFFER_COUNT; i++)
		{
			if (UserMemoryBufferCount(i))
				continue;

			int chIdx = ((enUSERMEM::userMemLAST / 100) + 1) + (i / MAX_USERMEM_CONTACT);
			int bit08 = (i % MAX_USERMEM_CONTACT);

			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userCondition = condition.Nodes();
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemNo = (chIdx * 100) + bit08;
			m_pBuffer->command[m_pBuffer->count].seqBufferCondition.userMemCount = i;

			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.bufferConditionType = ApiBufferConditionType::UserMemory;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.byteAddress = chIdx;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.bitAddress = bit08;
			m_pBuffer->command[m_pBuffer->count].apiBufferCondition.arg.userMemory.invert = (FALSE);
			UserMemoryBufferCount(i) = true;
			IsAllocatedBuffer = true;
			break;
		}
		if (!IsAllocatedBuffer)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		if (NULL != m_pBufferCondition)
		{
			const SeqBufferCondition& element = m_pBuffer->command[m_pBuffer->count].seqBufferCondition;
			m_pBufferCondition->userCondition->Add(element);
		}
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::FlowElse()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeFlowElse;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::FlowEndIf()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeFlowEndIf;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	ErrorApiBuffer BufferParameter::Sleep(UINT milliseconds)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		//if (!m_pBuffer->useCoding)
		//{
		//	m_pFsm->Set(C_ERROR);
		//	return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		//}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}

		m_pBuffer->command[m_pBuffer->count].type = cmdTypeSleep;
		m_pBuffer->command[m_pBuffer->count].sleepTime = milliseconds;
		m_pBuffer->count++;
		return (ERR_NONE);
	}

	void BufferParameter::CommandClear()
	{
		m_pBufferCondition = NULL;
		ZeroMemory(m_pBuffer->command, sizeof(m_pBuffer->command));
		for (int i = 0; i < MAX_API_BUFFER_COMMAND; i++)
		{
			m_pBuffer->command[i].seqBufferCondition.userCondition = std::make_shared<UserCondition>();
			m_pBuffer->command[i].flowBufferCondition.userCondition = std::make_shared<List<SeqBufferCondition>>();
		}
	}

	ErrorApiBuffer AxisCommand::IsReady()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisIsReady;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisIsReady.axisNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::IsOverrideReady()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisIsOverrideReady;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisIsOverrideReady.axisNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::GetCompletedTime(double time)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisGetCompletedTime;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetCompletedTime.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetCompletedTime.time = time;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::GetCompletedDistance(double distance)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisGetCompletedDistance;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetCompletedDistance.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetCompletedDistance.distance = distance;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::GetRemainingTime(double time)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisGetRemainingTime;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetRemainingTime.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetRemainingTime.time = time;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::GetRemainingDistance(double distance)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisGetRemainingDistance;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetRemainingDistance.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisGetRemainingDistance.distance = distance;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::PosMove(int posIdx)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisPosMove1;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.posIdx = posIdx;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::PosMove(int posIdx, double pos, int velRate, int accRate)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		if (!Between(posIdx, 0, MAX_INDEX_MOTOR - 1))
		{
			if (0 == (int)velRate)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_VEL_VALUE);
			}
			if (0 == (int)accRate)
			{
				m_pFsm->Set(C_ERROR);
				return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACC_VALUE);
			}
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisPosMove2;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.posIdx = posIdx;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.velRate = velRate;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.accRate = accRate;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::PosMove(MtCommand::SingleParamter command)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisPosMove3;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisPosMove.command = command;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::LinearInterpolationMove(InterpolationMotorList line)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (1 > LN[line].m_command.axisCount)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_LINE_AXIS);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeLinearInterpolationMove;
		m_pBuffer->function[m_pBuffer->functionCount].args.linearInterpolationMove.lineNo = line;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::TwoSpeedMove(int idx01, int idx02)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		if (!Between(idx01, 0, MAX_INDEX_MOTOR - 1) || !Between(idx02, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}
		if (MT[m_no].CommandTable(idx01).profile.vel == MT[m_no].CommandTable(idx02).profile.vel)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_VEL_VALUE);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisTwoSpeedMove;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisTwoSpeedMove.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisTwoSpeedMove.idx01 = idx01;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisTwoSpeedMove.idx02 = idx02;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer AxisCommand::SemiTwoSpeedMove(int idx, double slowDist)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->functionCount >= (MAX_API_BUFFER_FUNCTION - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_FUNCTION_RANGE_OUT);
		}
		if (!Between(m_no, 0, motorMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_AXIS);
		}
		if (!Between(idx, 0, MAX_INDEX_MOTOR - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_POS_INDEX_RANGE_OUT);
		}

		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeAxisSemiTwoSpeedMove;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisSemiTwoSpeedMove.axisNo = m_no;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisSemiTwoSpeedMove.idx = idx;
		m_pBuffer->function[m_pBuffer->functionCount].args.axisSemiTwoSpeedMove.slowDist = slowDist;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderCommand::Fwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeCylinderFwd;
		m_pBuffer->function[m_pBuffer->functionCount].args.cylinderFwd.cylinderNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderCommand::Up()
	{
		return Fwd();
	}

	ErrorApiBuffer CylinderCommand::Open()
	{
		return Fwd();
	}

	ErrorApiBuffer CylinderCommand::Bwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}

		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeCylinderBwd;
		m_pBuffer->function[m_pBuffer->functionCount].args.cylinderBwd.cylinderNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderCommand::Dn()
	{
		return Bwd();
	}

	ErrorApiBuffer CylinderCommand::Close()
	{
		return Bwd();
	}

	ErrorApiBuffer CylinderCommand::IsFwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeCylinderIsFwd;
		m_pBuffer->function[m_pBuffer->functionCount].args.cylinderIsFwd.cylinderNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderCommand::IsUp()
	{
		return IsFwd();
	}

	ErrorApiBuffer CylinderCommand::IsOpen()
	{
		return IsFwd();
	}

	ErrorApiBuffer CylinderCommand::IsBwd()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, cylinderMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CYLINDER);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeCylinderIsBwd;
		m_pBuffer->function[m_pBuffer->functionCount].args.cylinderIsBwd.cylinderNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer CylinderCommand::IsDn()
	{
		return IsBwd();
	}

	ErrorApiBuffer CylinderCommand::IsClose()
	{
		return IsBwd();
	}

	ErrorApiBuffer VacuumCommand::On()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumOn.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumCommand::Off()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumOff.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumCommand::BlowOn()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumBlowOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumBlowOn.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumCommand::BlowOff()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumBlowOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumBlowOff.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumCommand::AOn()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumAOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumAOn.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer VacuumCommand::AOff()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (!Between(m_no, 0, vacuumMAX - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_VACUUM);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeVacuumAOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.vacuumAOff.vacuumNo = m_no;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::AOn(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoAOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioAOn.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::AOff(enDI ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoAOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioAOff.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::BOn(enDI ioNo)
	{
		return AOff(ioNo);
	}

	ErrorApiBuffer IoCommand::BOff(enDI ioNo)
	{
		return AOn(ioNo);
	}

	ErrorApiBuffer IoCommand::IsOn(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoIsOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioIsOn.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::IsOff(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoIsOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioIsOff.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::On(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioOn.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer IoCommand::Off(enDO ioNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		if (m_pIoInfo->cnt >= (MAX_API_BUFFER_DIO - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_AXIS_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeIoOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.ioOff.ioNo = ioNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryCommand::IsOn(enUSERMEM userMemNo)
	{
		UINT bOn = TRUE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeUserMemoryIsOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.userMemoryIsOn.usermemoryNo = userMemNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryCommand::IsOff(enUSERMEM userMemNo)
	{
		UINT bOn = FALSE;
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeUserMemoryIsOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.userMemoryIsOff.usermemoryNo = userMemNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryCommand::On(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeUserMemoryOn;
		m_pBuffer->function[m_pBuffer->functionCount].args.userMemoryOn.usermemoryNo = userMemNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer UserMemoryCommand::Off(enUSERMEM userMemNo)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeUserMemoryOff;
		m_pBuffer->function[m_pBuffer->functionCount].args.userMemoryOff.usermemoryNo = userMemNo;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ControlCommand::FlowIf(FlowIfCondition condition, BOOL waitFlow)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeControlFlowIf;
		m_pBuffer->function[m_pBuffer->functionCount].args.controlFlowIf.condition = condition;
		m_pBuffer->function[m_pBuffer->functionCount].args.controlFlowIf.waitFlow = waitFlow;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ControlCommand::FlowElseIf(FlowIfCondition condition)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeControlFlowElseIf;
		m_pBuffer->function[m_pBuffer->functionCount].args.controlFlowElseIf.condition = condition;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ControlCommand::FlowElse()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeControlFlowElse;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ControlCommand::FlowEndIf()
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeControlFlowEndIf;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ControlCommand::Sleep(UINT milliseconds)
	{
		if (m_pFsm->IsBusy())
		{
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_pBuffer->useCoding)
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_pBuffer->channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (m_pBuffer->count >= (MAX_API_BUFFER_COMMAND - 1))
		{
			m_pFsm->Set(C_ERROR);
			return m_pErrorBuffer->Push(__FUNCTION__, ERR_ALLOCATED_COMMAND_RANGE_OUT);
		}
		m_pBuffer->function[m_pBuffer->functionCount].type = fncTypeControlSleep;
		m_pBuffer->function[m_pBuffer->functionCount].args.controlSleep.milliseconds = milliseconds;
		m_pBuffer->functionCount++;
		return (ERR_NONE);
	}

	ErrorApiBuffer ApiBufferCommand::Run(void)
	{
		if (error.GetLastError())
			return error.GetLastError();

		if (m_fsm.IsBusy())
		{
			return error.Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (m_buffer.useCoding)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_ACTIVED_CODING_MODE);
		}
		if (!Between(m_buffer.channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}
		if (1 > m_buffer.functionCount)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_UNALLOCATED_FUNCTION);
		}
		//if (1 > m_buffer.count)
		//{
		//	m_fsm.Set(C_ERROR);
		//	return error.Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		//}
		for (int i = 0; i < m_axisInfo.axisCnt; i++)
		{
			AxisList axis = (AxisList)m_axisInfo.axis[i];
			if (MT[axis].IsRdy())
				continue;

			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_NO_READY_AXIS);
		}

		m_buffer.count = 0;
		bufferParameter.CommandClear();
		for (int i = 0; i < m_buffer.functionCount; i++)
		{
			if (fncTypeAxisIsReady == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisIsReady.axisNo;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].IsReady();
			}
			else if (fncTypeAxisIsOverrideReady == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisIsOverrideReady.axisNo;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].IsOverrideReady();
			}
			else if (fncTypeAxisGetCompletedTime == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisGetCompletedTime.axisNo;
				double time = m_buffer.function[i].args.axisGetCompletedTime.time;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].GetCompletedTime(time);
			}
			else if (fncTypeAxisGetCompletedDistance == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisGetCompletedDistance.axisNo;
				double distance = m_buffer.function[i].args.axisGetCompletedDistance.distance;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].GetCompletedDistance(distance);
			}
			else if (fncTypeAxisGetRemainingTime == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisGetRemainingTime.axisNo;
				double time = m_buffer.function[i].args.axisGetRemainingTime.time;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].GetRemainingTime(time);
			}
			else if (fncTypeAxisGetRemainingDistance == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisGetRemainingDistance.axisNo;
				double distance = m_buffer.function[i].args.axisGetRemainingDistance.distance;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].GetRemainingDistance(distance);
			}
			else if (fncTypeAxisPosMove1 == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisPosMove.axisNo;
				int posIdx = m_buffer.function[i].args.axisPosMove.posIdx;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].Move(posIdx);
			}
			else if (fncTypeAxisPosMove2 == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisPosMove.axisNo;
				int posIdx = m_buffer.function[i].args.axisPosMove.posIdx;
				double pos = m_buffer.function[i].args.axisPosMove.pos;
				int velRate = m_buffer.function[i].args.axisPosMove.velRate;
				int accRate = m_buffer.function[i].args.axisPosMove.accRate;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].Move(posIdx, pos, velRate, accRate);
			}
			else if (fncTypeAxisPosMove3 == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisPosMove.axisNo;
				MtCommand::SingleParamter command = m_buffer.function[i].args.axisPosMove.command;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].Move(command);
			}
			else if (fncTypeLinearInterpolationMove == m_buffer.function[i].type)
			{
				InterpolationMotorList line = m_buffer.function[i].args.linearInterpolationMove.lineNo;
				for (int axisNo = 0; axisNo < LN[line].m_command.axisCount; axisNo++)
				{
					LN[line].m_command.parameter[axisNo].no;
					bufferParameter[(AxisList)axisNo].SetStartPos(MT[AxisList(axisNo)].m_state.cmdCnt);
				}
				bufferParameter[line].LinearInterpolationMove();
			}
			else if (fncTypeAxisTwoSpeedMove == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisTwoSpeedMove.axisNo;
				int idx01 = m_buffer.function[i].args.axisTwoSpeedMove.idx01;
				int idx02 = m_buffer.function[i].args.axisTwoSpeedMove.idx02;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].TwoSpeedMove(idx01, idx02);
			}
			else if (fncTypeAxisSemiTwoSpeedMove == m_buffer.function[i].type)
			{
				AxisList axisNo = m_buffer.function[i].args.axisSemiTwoSpeedMove.axisNo;
				int idx = m_buffer.function[i].args.axisSemiTwoSpeedMove.idx;
				double slowDist = m_buffer.function[i].args.axisSemiTwoSpeedMove.slowDist;
				bufferParameter[axisNo].SetStartPos(MT[axisNo].m_state.cmdCnt);
				bufferParameter[axisNo].SemiTwoSpeedMove(idx, slowDist);
			}
			else if (fncTypeCylinderFwd == m_buffer.function[i].type)
			{
				CylinderList cylinderNo = m_buffer.function[i].args.cylinderFwd.cylinderNo;
				bufferParameter[cylinderNo].Fwd();
			}
			else if (fncTypeCylinderBwd == m_buffer.function[i].type)
			{
				CylinderList cylinderNo = m_buffer.function[i].args.cylinderBwd.cylinderNo;
				bufferParameter[cylinderNo].Bwd();
			}
			else if (fncTypeCylinderIsFwd == m_buffer.function[i].type)
			{
				CylinderList cylinderNo = m_buffer.function[i].args.cylinderIsFwd.cylinderNo;
				bufferParameter[cylinderNo].IsFwd();
			}
			else if (fncTypeCylinderIsBwd == m_buffer.function[i].type)
			{
				CylinderList cylinderNo = m_buffer.function[i].args.cylinderIsBwd.cylinderNo;
				bufferParameter[cylinderNo].IsBwd();
			}
			else if (fncTypeVacuumOn == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumOn.vacuumNo;
				bufferParameter[vacuumNo].On();
			}
			else if (fncTypeVacuumOff == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumOff.vacuumNo;
				bufferParameter[vacuumNo].Off();
			}
			else if (fncTypeVacuumBlowOn == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumBlowOn.vacuumNo;
				bufferParameter[vacuumNo].BlowOn();
			}
			else if (fncTypeVacuumBlowOff == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumBlowOff.vacuumNo;
				bufferParameter[vacuumNo].BlowOff();
			}
			else if (fncTypeVacuumAOn == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumAOn.vacuumNo;
				bufferParameter[vacuumNo].AOn();
			}
			else if (fncTypeVacuumAOff == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumAOff.vacuumNo;
				bufferParameter[vacuumNo].AOff();
			}
			else if (fncTypeVacuumIsOn == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumIsOn.vacuumNo;
				bufferParameter[vacuumNo].IsOn();
			}
			else if (fncTypeVacuumIsOff == m_buffer.function[i].type)
			{
				VacuumList vacuumNo = m_buffer.function[i].args.vacuumIsOff.vacuumNo;
				bufferParameter[vacuumNo].IsOff();
			}
			else if (fncTypeIoAOn == m_buffer.function[i].type)
			{
				enDI ioNo = m_buffer.function[i].args.ioAOn.ioNo;
				bufferParameter.AOn(ioNo);
			}
			else if (fncTypeIoAOff == m_buffer.function[i].type)
			{
				enDI ioNo = m_buffer.function[i].args.ioAOff.ioNo;
				bufferParameter.AOff(ioNo);
			}
			else if (fncTypeIoIsOn == m_buffer.function[i].type)
			{
				enDO ioNo = m_buffer.function[i].args.ioIsOn.ioNo;
				bufferParameter.IsOn(ioNo);
			}
			else if (fncTypeIoIsOff == m_buffer.function[i].type)
			{
				enDO ioNo = m_buffer.function[i].args.ioIsOff.ioNo;
				bufferParameter.IsOff(ioNo);
			}
			else if (fncTypeIoOn == m_buffer.function[i].type)
			{
				enDO ioNo = m_buffer.function[i].args.ioOn.ioNo;
				bufferParameter.On(ioNo);
			}
			else if (fncTypeIoOff == m_buffer.function[i].type)
			{
				enDO ioNo = m_buffer.function[i].args.ioOff.ioNo;
				bufferParameter.Off(ioNo);
			}
			else if (fncTypeUserMemoryIsOn == m_buffer.function[i].type)
			{
				enUSERMEM userMemNo = m_buffer.function[i].args.userMemoryIsOn.usermemoryNo;
				bufferParameter.IsOn(userMemNo);
			}
			else if (fncTypeUserMemoryIsOff == m_buffer.function[i].type)
			{
				enUSERMEM userMemNo = m_buffer.function[i].args.userMemoryIsOff.usermemoryNo;
				bufferParameter.IsOff(userMemNo);
			}
			else if (fncTypeUserMemoryOn == m_buffer.function[i].type)
			{
				enUSERMEM userMemNo = m_buffer.function[i].args.userMemoryOn.usermemoryNo;
				bufferParameter.On(userMemNo);
			}
			else if (fncTypeUserMemoryOff == m_buffer.function[i].type)
			{
				enUSERMEM userMemNo = m_buffer.function[i].args.userMemoryOff.usermemoryNo;
				bufferParameter.Off(userMemNo);
			}
			else if (fncTypeControlFlowIf == m_buffer.function[i].type)
			{
				FlowIfCondition condition = m_buffer.function[i].args.controlFlowIf.condition;
				BOOL waitFlow = m_buffer.function[i].args.controlFlowIf.waitFlow;
				bufferParameter.FlowIf(condition, waitFlow);
			}
			else if (fncTypeControlFlowElseIf == m_buffer.function[i].type)
			{
				FlowIfCondition condition = m_buffer.function[i].args.controlFlowElseIf.condition;
				bufferParameter.FlowElseIf(condition);
			}
			else if (fncTypeControlFlowElse == m_buffer.function[i].type)
			{
				bufferParameter.FlowElse();
			}
			else if (fncTypeControlFlowEndIf == m_buffer.function[i].type)
			{
				bufferParameter.FlowEndIf();
			}
			else if (fncTypeControlSleep == m_buffer.function[i].type)
			{
				UINT milliseconds = m_buffer.function[i].args.controlSleep.milliseconds;
				bufferParameter.Sleep(milliseconds);
			}
		}
		if (1 > m_buffer.count)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_UNALLOCATED_BUFFER);
		}
		if (error.GetLastError())
			return error.GetLastError();

		m_fsm.Set(CMD_START);
		return (ERR_NONE);
	}

	ErrorApiBuffer ApiBufferCommand::Stop(void)
	{
		m_fsm.Set(C_ERROR);
		return (ERR_NONE);
	}

	void ApiBufferCommand::Clear()
	{
		if (Between(m_buffer.channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			//LOG[logSEQ].Message("CLEAR[m_buffer.channel:%d]", m_buffer.channel);
			WMX.apibuffer.EndRecordBufferChannel();
			WMX.apibuffer.Halt(m_buffer.channel);
			BitClear();
			tmBufferChannel[m_buffer.channel].Reset();
			BufferChannel(m_buffer.channel) = false;
		}
		//if (channelClear)
		m_buffer.channel = NOT_DEFINED;

		m_buffer.useCoding = 0;
		m_buffer.count = 0;
		bufferParameter.CommandClear();
		//m_blockCnt = 0;
		ZeroMemory(&m_ioInfo, sizeof(m_ioInfo));
		ZeroMemory(&m_usermemoryInfo, sizeof(m_usermemoryInfo));
		ZeroMemory(&m_axisInfo, sizeof(m_axisInfo));
	}

	void ApiBufferCommand::BitClear(void)
	{
		if (!Between(m_buffer.channel, 0, MAX_API_BUFFER_CHANNEL - 1))
			return;

		for (int i = 0; i < m_buffer.count; i++)
		{
			if (cmdTypeFlowStartIf == m_buffer.command[i].type)
			{
				Off((enUSERMEM)m_buffer.command[i].flowBufferCondition.userMemNo);
				UserMemoryBufferCount(m_buffer.command[i].flowBufferCondition.userMemCount) = false;
				size_t maxCount = m_buffer.command[i].flowBufferCondition.userCondition->Count();
				for (int count = 0; count < maxCount; count++)
				{
					Off((enUSERMEM)m_buffer.command[i].flowBufferCondition.userCondition->data[count].userMemNo);
					UserMemoryBufferCount(m_buffer.command[i].flowBufferCondition.userCondition->data[count].userMemCount) = false;
				}
			}
			else if (cmdTypeUserCondition == m_buffer.command[i].type)
			{
				Off((enUSERMEM)m_buffer.command[i].seqBufferCondition.userMemNo);
				UserMemoryBufferCount(m_buffer.command[i].seqBufferCondition.userMemCount) = false;
			}
		}
	}

	int ApiBufferCommand::Channel(void)
	{
		return m_buffer.channel;
	}

	BOOL ApiBufferCommand::IsBusy(void)
	{
		return m_fsm.IsBusy();
	}

	void ApiBufferCommand::CommandCycle(void)
	{
		if (!m_fsm.IsBusy())
			return;

		if (m_buffer.useCoding || !Between(m_buffer.channel, 0, MAX_API_BUFFER_CHANNEL - 1))
			m_fsm.Set(C_ERROR);

		if (error.GetLastError())
			m_fsm.Set(C_ERROR);

		if (m_fsm.TimeLimit(m_dwTimeLimit))
			m_fsm.Set(C_ERROR);

		switch (m_fsm.Get())
		{
		case CMD_START:
		{
			if (m_fsm.Once())
			{
				if (1 > m_buffer.count)
				{
					m_fsm.Set(C_ERROR);
					return;
				}

				for (int i = 0; i < m_axisInfo.axisCnt; i++)
				{
					AxisList axis = (AxisList)m_axisInfo.axis[i];
					if (MT[axis].IsRdy())
						continue;

					m_fsm.Set(C_ERROR);
					return;
				}

				for (int axisCnt = 0; axisCnt < m_axisInfo.axisCnt; axisCnt++)
				{
					AxisList mtno = AxisList(m_axisInfo.axis[axisCnt]);
					double dMIN = m_axisInfo.info[axisCnt].startPos[0] - 10.;
					double dMAX = m_axisInfo.info[axisCnt].startPos[0] + 10.;
					if (!Between(MT[mtno].m_state.cmdCnt, dMIN, dMAX))
					{
						m_fsm.Set(C_ERROR);
						return;
					}
				}

				if (WMX.apibuffer.Clear(m_buffer.channel))
				{
					m_fsm.Set(C_ERROR);
					return;
				}

				long isError = FALSE;
				isError |= WMX.apibuffer.StartRecordBufferChannel(m_buffer.channel);
				for (int i = 0; i < m_buffer.count; i++)
				{
					if (cmdTypeFlowIf == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.FlowIf(&m_buffer.command[i].apiBufferCondition);
					}
					else if (cmdTypeFlowElseIf == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.FlowElseIf(&m_buffer.command[i].apiBufferCondition);
					}
					else if (cmdTypeFlowElse == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.FlowElse();
					}
					else if (cmdTypeFlowEndIf == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.FlowEndIf();
					}
					else if (cmdTypeCondition == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.Wait(&m_buffer.command[i].apiBufferCondition);
					}
					else if (cmdTypeAxis == m_buffer.command[i].type)
					{
						isError |= WMX.CommandPos(m_buffer.command[i].mtProfile, FALSE);
					}
					else if (cmdTypeLine == m_buffer.command[i].type)
					{
						isError |= WMX.CommandPos(m_buffer.command[i].lnProfile, FALSE);
					}
					else if (cmdTypeIo == m_buffer.command[i].type)
					{
						isError |= WMX.io.SetOutBit(m_buffer.command[i].apiBufferCondition.arg.ioOutput.byteAddress,
							m_buffer.command[i].apiBufferCondition.arg.ioOutput.bitAddress, m_buffer.command[i].apiBufferCondition.arg.ioOutput.invert);
					}
					else if (cmdTypeUserMemory == m_buffer.command[i].type)
					{
						isError |= WMX.usermemory.SetMBit(m_buffer.command[i].apiBufferCondition.arg.userMemory.byteAddress,
							m_buffer.command[i].apiBufferCondition.arg.userMemory.bitAddress, m_buffer.command[i].apiBufferCondition.arg.userMemory.invert);
					}
					else if (cmdTypeSleep == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.Sleep(m_buffer.command[i].sleepTime);
					}
					else if (cmdTypeUserCondition == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.Wait(&m_buffer.command[i].apiBufferCondition);
					}
					else if (cmdTypeFlowStartIf == m_buffer.command[i].type)
					{
						isError |= WMX.apibuffer.Wait(&m_buffer.command[i].apiBufferCondition);
					}

					if (isError)
					{
						WMX.apibuffer.EndRecordBufferChannel();
						m_fsm.Set(C_ERROR);
						return;
					}

					//m_blockCnt++;
				}

				for (int i = 0; i < m_axisInfo.axisCnt; i++)
				{
					AxisList axis = (AxisList)m_axisInfo.axis[i];
					isError |= WMX.apibuffer.Wait(axis);
				}

				//m_blockCnt++;

				isError |= WMX.apibuffer.EndRecordBufferChannel();

				ApiBufferOptions options;
				ZeroMemory(&options, sizeof(options));
				options.stopOnLastBlock = true;
				isError |= WMX.apibuffer.SetOptions(m_buffer.channel, &options);

				if (isError || !m_buffer.count)
				{
					m_fsm.Set(C_ERROR);
					return;
				}

				if (WMX.apibuffer.Execute(m_buffer.channel))
				{
					m_fsm.Set(C_ERROR);
					return;
				}

				for (int i = 0; i < m_axisInfo.axisCnt; i++)
				{
					AxisList axis = (AxisList)m_axisInfo.axis[i];
					MT[axis].m_nxtIdx = P_JOG;

					LOG[logMT].Message("[C_POS_START],[%.2d],[cmdPos],[%f],[cmdCnt],[%f],[nxtIdx],[%.2d],[curIdx],[%.2d],[apibuffer-trigger]",
						axis, m_axisInfo.info[i].startPos[m_axisInfo.info[i].cnt], MT[axis].m_state.cmdCnt, m_axisInfo.idx[i], MT[axis].m_curIdx);

					MT[axis].m_motionMode = C_MOTION_BUSY;
					MT[axis].m_fsmDrv.Set(C_MOTION_BUSY, m_buffer.channel);
				}

				for (int i = 0; i < m_ioInfo.cnt; i++)
				{
					enDO iono = (enDO)m_ioInfo.num[i];
					WMX.SetReadOnly(iono, TRUE);
				}
				for (int i = 0; i < m_usermemoryInfo.cnt; i++)
				{
					enUSERMEM usermemno = (enUSERMEM)m_usermemoryInfo.num[i];
					WMX.SetUserMemoryReadOnly(usermemno, TRUE);
				}

				BitClear();
			}

			if (!m_fsm.Delay(10))
				break;

			//			LOG[logSYS].Message("CMD_START[m_buffer.channel:%d]",m_buffer.channel);
			m_fsm.Set(CMD_BUSY);
		}
		break;
		case CMD_BUSY:
		{
			ApiBufferStatus status;
			ZeroMemory(&status, sizeof(status));
			WMX.apibuffer.GetStatus(m_buffer.channel, &status);
			if (0 < m_buffer.count && 0 < status.blockCount && 0 < status.execBlockCount)
			{
				int currentBlockCount = status.execBlockCount - 1;
				if (cmdTypeFlowStartIf == m_buffer.command[currentBlockCount].type)
				{
					size_t maxCount = m_buffer.command[currentBlockCount].flowBufferCondition.userCondition->Count();
					for (int count = 0; count < maxCount; count++)
					{
						if (IsNodeCondition(m_buffer.command[currentBlockCount].flowBufferCondition.userCondition->data[count].userCondition))
						{
							On((enUSERMEM)m_buffer.command[currentBlockCount].flowBufferCondition.userMemNo);
							On((enUSERMEM)m_buffer.command[currentBlockCount].flowBufferCondition.userCondition->data[count].userMemNo);
							break;
						}
					}
				}
				else if (cmdTypeUserCondition == m_buffer.command[currentBlockCount].type)
				{
					if (IsNodeCondition(m_buffer.command[currentBlockCount].seqBufferCondition.userCondition))
						On((enUSERMEM)m_buffer.command[currentBlockCount].seqBufferCondition.userMemNo);
				}
			}
			if (status.remainingBlockCount || ApiBufferState::T::Idle != status.state && ApiBufferState::T::Stop != status.state)
				break;

			//			LOG[logSYS].Message("CMD_BUSY[m_buffer.channel:%d]",m_buffer.channel);
			for (int i = 0; i < m_ioInfo.cnt; i++)
			{
				enDO iono = (enDO)m_ioInfo.num[i];
				WMX.SetReadOnly(iono, FALSE);
			}
			for (int i = 0; i < m_usermemoryInfo.cnt; i++)
			{
				enUSERMEM usermemno = (enUSERMEM)m_usermemoryInfo.num[i];
				WMX.SetUserMemoryReadOnly(usermemno, FALSE);
			}

			//WMX.apibuffer.EndRecordBufferChannel();
			WMX.apibuffer.Halt(m_buffer.channel);
			m_fsm.Set(CMD_END);
		}
		break;
		case CMD_END:
		{
			if (m_fsm.Once())
			{
				for (int i = 0; i < m_axisInfo.axisCnt; i++)
				{
					AxisList axis = (AxisList)m_axisInfo.axis[i];
					MT[axis].m_nxtIdx = m_axisInfo.idx[i];
					MT[axis].m_fsmDrv.Set(C_POS_END);
					MT[axis].m_fsmDrv.Delay(0);
				}
			}

			BOOL isBreak = FALSE;
			for (int i = 0; i < m_axisInfo.axisCnt; i++)
			{
				AxisList axis = (AxisList)m_axisInfo.axis[i];
				if (MT[axis].m_fsmDrv.IsBusy(C_POS_END))
				{
					isBreak = TRUE;
					break;
				}
			}

			if (isBreak)
				break;

			//			LOG[logSYS].Message("CMD_END[m_buffer.channel:%d]",m_buffer.channel);
			BitClear();
			//tmBufferChannel[m_buffer.channel].Reset();
			//BufferChannel(m_buffer.channel) = false;
			//m_buffer.channel = NOT_DEFINED;
			m_fsm.Set(C_IDLE);
		}
		break;
		case C_ERROR:
		{
			if (m_fsm.Once())
			{
				for (int i = 0; i < m_axisInfo.axisCnt; i++)
				{
					AxisList axis = (AxisList)m_axisInfo.axis[i];
					MT[axis].CancelHomeSearch();
				}
				for (int i = 0; i < m_ioInfo.cnt; i++)
				{
					enDO iono = (enDO)m_ioInfo.num[i];
					WMX.SetReadOnly(iono, FALSE);
				}
				for (int i = 0; i < m_usermemoryInfo.cnt; i++)
				{
					enUSERMEM usermemno = (enUSERMEM)m_usermemoryInfo.num[i];
					WMX.SetUserMemoryReadOnly(usermemno, FALSE);
				}
				Clear();
			}

			m_fsm.Set(C_IDLE);
		}
		break;
		}
	}

	ErrorApiBuffer ApiBufferCommand::Start(DWORD dwTimeLimit)
	{
		if (m_fsm.IsBusy())
		{
			return error.Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (m_buffer.useCoding)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_ACTIVED_CODING_MODE);
		}
		error.Clear();

		Clear();
		for (int i = 0; i < MAX_API_BUFFER_CHANNEL; i++)
		{
			if (BufferChannel(i))
			{
				tmBufferChannel[i].Reset();
				continue;
			}

			if (!tmBufferChannel[i].TmOver(200))
				continue;

			m_buffer.channel = i;
			BufferChannel(i) = true;
			break;
		}

		if (NOT_DEFINED == m_buffer.channel)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}

		m_buffer.functionCount = 0; // Seq Buffer ´ٽø¸µ뮍
		ZeroMemory(m_buffer.function, sizeof(m_buffer.function));
		ZeroMemory(m_buffer.funcSequence, sizeof(m_buffer.funcSequence));

		m_buffer.useCoding = 1;	// Api Buffer Coding Mode·ΠÀüȯ.
		m_dwTimeLimit = dwTimeLimit;
		return (ERR_NONE);
	}

	ErrorApiBuffer ApiBufferCommand::End(void)
	{
		if (m_fsm.IsBusy())
		{
			return error.Push(__FUNCTION__, ERR_ACTIVED_CYCLE);
		}
		if (!m_buffer.useCoding)
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_INACTIVED_CODING_MODE);
		}
		if (!Between(m_buffer.channel, 0, MAX_API_BUFFER_CHANNEL - 1))
		{
			m_fsm.Set(C_ERROR);
			return error.Push(__FUNCTION__, ERR_UNALLOCATED_CHANNEL);
		}

		m_buffer.useCoding = 0;
		return (ERR_NONE);
	}

	template<typename T1, typename T2>
	BOOL CheckNodeListCondition(const std::shared_ptr<TreeNode<T1, T2>>& node)
	{
		int status = (NOT_DEFINED);
		for (int i = 0; i < node->list.Count(); i++)
		{
			if (SeqBufferConditionType::Cylinder == node->list[i].type)
			{
				CylinderList cylno = CylinderList(node->list[i].arg.cylinder.no);
				if (cylinderStateFwd == node->list[i].arg.cylinder.state)
				{
					if (NOT_DEFINED == status)
					{
						CYL[cylno].Fwd();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							CYL[cylno].Fwd();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							CYL[cylno].Fwd();
							status |= (TRUE);
						}
					}
				}
				else if (cylinderStateBwd == node->list[i].arg.cylinder.state)
				{
					if (NOT_DEFINED == status)
					{
						CYL[cylno].Bwd();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							CYL[cylno].Bwd();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							CYL[cylno].Bwd();
							status |= (TRUE);
						}
					}
				}
				else if (cylinderStateIsFwd == node->list[i].arg.cylinder.state)
				{
					if (NOT_DEFINED == status)
						status = CYL[cylno].IsFwd();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= CYL[cylno].IsFwd();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= CYL[cylno].IsFwd();
					}
				}
				else if (cylinderStateIsBwd == node->list[i].arg.cylinder.state)
				{
					if (NOT_DEFINED == status)
						status = CYL[cylno].IsBwd();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= CYL[cylno].IsBwd();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= CYL[cylno].IsBwd();
					}
				}
			}
			else if (SeqBufferConditionType::Vacuum == node->list[i].type)
			{
				VacuumList vcno = VacuumList(node->list[i].arg.vacuum.no);
				if (vacuumStateOn == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
					{
						VC[vcno].On();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							VC[vcno].On();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							VC[vcno].On();
							status |= (TRUE);
						}
					}
				}
				else if (vacuumStateOff == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
					{
						VC[vcno].Off();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							VC[vcno].Off();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							VC[vcno].Off();
							status |= (TRUE);
						}
					}
				}
				else if (vacuumStateBlowOn == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
					{
						VC[vcno].BlowOn();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							VC[vcno].BlowOn();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							VC[vcno].BlowOn();
							status |= (TRUE);
						}
					}
				}
				else if (vacuumStateBlowOff == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
					{
						VC[vcno].BlowOff();
						status = (TRUE);
					}
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							VC[vcno].BlowOff();
							status &= (TRUE);
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
						{
							VC[vcno].BlowOff();
							status |= (TRUE);
						}
					}
				}
				else if (vacuumStateAOn == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
						status = (BOOL)VC[vcno].AOn();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= (BOOL)VC[vcno].AOn();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= (BOOL)VC[vcno].AOn();
					}
				}
				else if (vacuumStateAOff == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
						status = (BOOL)VC[vcno].AOff();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= (BOOL)VC[vcno].AOff();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= (BOOL)VC[vcno].AOff();
					}
				}
				else if (vacuumStateIsOn == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
						status = (BOOL)VC[vcno].IsOn();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= (BOOL)VC[vcno].IsOn();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= (BOOL)VC[vcno].IsOn();
					}
				}
				else if (vacuumStateIsOff == node->list[i].arg.vacuum.state)
				{
					if (NOT_DEFINED == status)
						status = (BOOL)VC[vcno].IsOff();
					else
					{
						if (SeqBufferConditionLogic::AND == node->logic)
						{
							status &= (BOOL)VC[vcno].IsOff();
							if (!status)
								break;
						}
						else if (SeqBufferConditionLogic::OR == node->logic)
							status |= (BOOL)VC[vcno].IsOff();
					}
				}
			}
			else if (SeqBufferConditionType::Function == node->list[i].type)
			{
				if (nullptr == node->list[i].arg.function)
					return (FALSE);

				if (NOT_DEFINED == status)
					status = node->list[i].arg.function();
				else
				{
					if (SeqBufferConditionLogic::AND == node->logic)
						status &= node->list[i].arg.function();
					else if (SeqBufferConditionLogic::OR == node->logic)
						status |= node->list[i].arg.function();
				}
			}
		}

		if (1 == status)
			return (TRUE);

		return (FALSE);
	}

	template<typename T1, typename T2>
	BOOL IsNodeCondition(const std::shared_ptr<TreeNode<T1, T2>>& node)
	{
		if (nullptr == node)
			return (FALSE);

		int nodeCount = 0;
		BOOL nodeCondition = (0 < node->list.Count()) ? CheckNodeListCondition(node) : FALSE;
		for (const auto& child : node->nodes)
		{
			if (0 == nodeCount && 0 == node->list.Count())
				nodeCondition = IsNodeCondition(child);
			else
			{
				if (1 == node->logic)
					nodeCondition &= IsNodeCondition(child);
				else
					nodeCondition |= IsNodeCondition(child);
			}
			nodeCount++;
		}
		return nodeCondition;
	}
};