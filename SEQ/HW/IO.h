#pragma once
//#include "IO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"

namespace HW
{
	extern void Set(enDI ioNo, BOOL isOn);
	extern BOOL AOn(enDI ioNo, DWORD delaymS = 0);
	extern BOOL AOff(enDI ioNo, DWORD delaymS = 0);
	extern BOOL BOn(enDI ioNo, DWORD delaymS = 0);
	extern BOOL BOff(enDI ioNo, DWORD delaymS = 0);
	extern void Set(enDO ioNo, BOOL isOn);
	extern void On(enDO ioNo);
	extern void Off(enDO ioNo);
	extern BOOL IsOn(enDO ioNo, DWORD delaymS = 0);
	extern BOOL IsOff(enDO ioNo, DWORD delaymS = 0);
	extern double Get(enAI ioNo);
	extern void Set(enAO ioNo, INT32 value);
	extern void Set(enUSERMEM userMemNo, BOOL isOn);
	extern void On(enUSERMEM userMemNo);
	extern void Off(enUSERMEM userMemNo);
	extern BOOL IsOn(enUSERMEM userMemNo, DWORD delaymS = 0);
	extern BOOL IsOff(enUSERMEM userMemNo, DWORD delaymS = 0);
	extern void Set(seqInput ioNo, BOOL isOn);
	extern BOOL AOn(seqInput ioNo, DWORD delaymS = 0);
	extern BOOL AOff(seqInput ioNo, DWORD delaymS = 0);
	extern BOOL BOn(seqInput ioNo, DWORD delaymS = 0);
	extern BOOL BOff(seqInput ioNo, DWORD delaymS = 0);
	extern void Set(seqOutput ioNo, BOOL isOn);
	extern void On(seqOutput ioNo);
	extern void Off(seqOutput ioNo);
	extern BOOL IsOn(seqOutput ioNo, DWORD delaymS = 0);
	extern BOOL IsOff(seqOutput ioNo, DWORD delaymS = 0);
}