#pragma once
#define _USE_MATH_DEFINES
#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>
#include <mmsystem.h>
#include <cassert>
#include <queue>
#include <math.h>

using namespace std;

namespace IPC_LASER
{
	enum
	{
		MAX_SEND_SCAN_COORD	= 500,
		MAX_UNIT			= 10,
		MAX_FIDUCIAL		= 4,
		MAX_STEP			= 50,
	};

#pragma pack(push, 1)
	typedef struct
	{
		WORD inCh[128];
		WORD outCh[128];
	}SharedMem;

	typedef struct
	{
		double x[MAX_SEND_SCAN_COORD];
		double y[MAX_SEND_SCAN_COORD];
		double t[MAX_SEND_SCAN_COORD];
		int idxX[MAX_SEND_SCAN_COORD];
		int idxY[MAX_SEND_SCAN_COORD];
		int Step[MAX_SEND_SCAN_COORD];
		int unitCnt;
	}ScannerCoord;

	typedef struct _ScanCal
	{
		int iCalPoint;
		POINT2D dFOV;
		POINT2D dPitch;
		POINT2D val[51][51];
	}ScanCal;

	typedef struct
	{
		int iMode;
	}ScanMode;

	typedef struct
	{
		int markingMode; 	// 0.메뉴얼마킹 1.Gem마킹
		int unitCntX;
		int unitCntY;
		char lotId[MAX_PATH];
		char stripId[MAX_PATH];
	}MarkingData;

	typedef struct
	{
		char boxId[128];			// BOX ID (SBOX ID or LBOX ID)				ex) HTEL380MAH01280213101			// 작업BOX에따른 SBOX or LBOX의 ID
		char readLabelId[256];		// SEQ->LAS) Vision 기준이미지파일 명칭		ex) HTEL380MAH01280213101			// BoxId + Partial (.bmp 파일명칭)
		char writeLabelId[256];		// LAS->SEQ) Vision 기준이미지파일 명칭		ex) HTEL380MAH01280213101Partial	// BoxId + Partial (.bmp 파일명칭)
		XYT prsData;				// Prs Data									ex) 0, 0, 0.5						// Only Theta Laser출사 위치보정값
		char readZpl[20000];		// SEQ->LAS) ZPL Data						ex)									// 고객사로부터 전달받은 규격없음
		char writeZpl[20000];		// LAS->SEQ) ZPL Data						ex)									// 고객사로부터 전달받은 규격없음
	}LabelData;

	typedef struct
	{
		int iStageNo;
		ScanMode scanMode;
		ScanCal scanCal;
		ScannerCoord scanCoord;
		LabelData	laserLabelData;
	}ScannerData;

	typedef struct
	{
		POINT2D bcdUnit;
		int nFiducialCnt;
		int nStepCnt;
		int nPreQcCnt;
		int nQcCnt;
		POINT2D Fiducial[MAX_FIDUCIAL];
		POINT2D StepPos[MAX_STEP];
		POINT2D PreQcPos[MAX_STEP];
		POINT2D QcPos[MAX_STEP];
	}UnitDwg;

	typedef struct
	{
		int nUnitCnt;
		//POINT2D bcdFilm;
		//UnitDwg Unit[MAX_UNIT];
	}LaserDwg;
#pragma pack(pop)
};

#define LASER_BUFFER_SIZE			1024 * 320
#define MAX_LASER					1
#define MAX_STAGE					2

namespace IPC_LASER
{
	enum CMD
	{
		CMD_NOTHING					= 0,

		// LabelData Read/Write
		CMD_RD_SCAN1_DATA			= 1,
		
		CMD_WR_SCAN1_DATA			= 5,
	};

#pragma pack(push, 1)
	typedef union _IpcShared
	{
		CHAR			bufferSize[LASER_BUFFER_SIZE]; 
		ScannerData		scanData[MAX_STAGE];			
		LaserDwg		laserDwg[MAX_STAGE];			
		char			strRecipeName[MAX_PATH];
	}IpcShared;

	typedef struct _IpcBuffer
	{
		IpcShared		buffer;
		INT32			command;
	}IpcBuffer;
#pragma pack(pop)
}