 #pragma once
#include "..\Defines.h"
#include "..\..\BASE\BaseAll.h"

#define MAX_LASER_DI_CH				((iLaserMAX / 100) + 1) 
#define MAX_LASER_DO_CH				((oLaserMAX / 100) + 1)

namespace COM
{
	class SharedIO
	{
	private:
		CTimer			tiOn[MAX_CH][MAX_CONTACT];
		CTimer			tiOff[MAX_CH][MAX_CONTACT];

		CTimer			toOn[MAX_CH][MAX_CONTACT];
		CTimer			toOff[MAX_CH][MAX_CONTACT];

		UINT16			iBitMem[MAX_CH];
		UINT16			oBitMem[MAX_CH];

	public:
		void Update(void);

		BOOL AOn(seqInput ioNo, DWORD dwDelayTime = 0);
		BOOL AOff(seqInput ioNo, DWORD dwDelayTime = 0);
		void Set(seqInput ioNo, BOOL isOn);//For Simulation

		BOOL IsOn(seqOutput ioNo, DWORD dwDelayTime = 0);
		BOOL IsOff(seqOutput ioNo, DWORD dwDelayTime = 0);
		void Set(seqOutput ioNo, BOOL isOn);
		void On(seqOutput ioNo);
		void Off(seqOutput ioNo);

	public:
		SharedIO();
		virtual ~SharedIO();
	};
}