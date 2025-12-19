#pragma once
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\HW\IO.h"
#include "..\HW\Motor.h"

namespace COM
{
	class CapCondition
	{
	public:
		enum
		{
			Linear,
			Skewed,
		};

		double			USL;
		double			LSL;
		int				distributionType;
		int				maxCount;				// 데이터개수
		int				groupCount;				// 부분군개수 최소 1개 이상!
		int				countInGroup;			// 부분군크기
		List<double>	val;

		CapCondition()
		{
			USL = LSL = 0.;
			distributionType = Linear;
			maxCount = groupCount = countInGroup = 0;
		};
		~CapCondition() {};
	};

	class CapResult
	{
	public:
		double	max;							// 최대
		double	min;							// 최소
		double	avg;							// 평균
		double	gap;							// 최대-최소
		double	yield;							// 수율
		double	ST;								// 단기공정 표준편차
		double	CP;
		double	CPU;
		double	CPL;
		double	CPK;
		double	LT;
		double	PP;
		double	PPU;
		double	PPL;
		double	PPK;
		double	K;								// 치우침도
		int		validCount;
		BOOL	comp;

		CapResult()
		{
			ZeroMemory(this, sizeof(*this));
		};
		~CapResult() {};
	};

	CapResult CapAnalyzer(CapCondition capCondition);
}