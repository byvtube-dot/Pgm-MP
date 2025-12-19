#pragma once
#include <windows.h>
#include <tchar.h>		
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>

#ifdef KASIOLIB_EXPORTS
#define DLLINTERFACE __declspec(dllexport)
#else
#define DLLINTERFACE __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	namespace kasiolib
	{
		DLLINTERFACE BOOL __stdcall OffsetFrom2Point(LPVOID lpData, LPVOID lpCoord, LPVOID lpOffset);
		DLLINTERFACE BOOL __stdcall OffsetFrom3Point(LPVOID lpData, LPVOID lpCoord, LPVOID lpOffset);
		DLLINTERFACE BOOL __stdcall OffsetFrom4Point(LPVOID lpData, LPVOID lpCoord, LPVOID lpOffset);
		DLLINTERFACE void __stdcall GetCalibrationPosXY(LPVOID lpCalData, int idxX, int idxY, LPVOID lpPos);
		DLLINTERFACE BOOL __stdcall GetCalibrationErr(LPVOID lpCalData, LPVOID lpPos, LPVOID lpCalErr);

		class DLLINTERFACE AntiCollision1D
		{
		public:
			virtual void __stdcall Init(BOOL(*isStopFwdCondition)(void), BOOL(*isStopBwdCondition)(void), int baseDir, double* pHwDist,
				LPVOID pState, LPVOID pCommand, double* pLimitP, double* pLimitN, double* pSetP, double* pSetN) = 0;
			virtual BOOL __stdcall CheckError(void) = 0;
			virtual BOOL __stdcall AxisRMove(double cmdDist, double& targetPos) = 0;
			virtual BOOL __stdcall AxisJogFwd(double& targetPos) = 0;
			virtual BOOL __stdcall AxisJogBwd(double& targetPos) = 0;
			virtual BOOL __stdcall AxisCanMove(double pos) = 0;
			virtual ~AntiCollision1D();
		};

		DLLINTERFACE AntiCollision1D* __stdcall CreateAntiCollision1D(void);
		DLLINTERFACE void __stdcall	DestroyAntiCollision1D(AntiCollision1D* pBase);

		class DLLINTERFACE AntiCollision2D
		{
		public:
			virtual void __stdcall Init(int baseDir, double* pHwDist,
				LPVOID pAxis1State, LPVOID pAxis1Command, double* pAxis1LimitP, double* pAxis1LimitN, double* pAxis1SetPos,
				LPVOID pAxis2State, LPVOID pAxis2Command, double* pAxis2LimitP, double* pAxis2LimitN, double* pAxis2SetPos) = 0;
			virtual int __stdcall Update(BOOL isBumpSensorOn = FALSE) = 0;
			virtual BOOL __stdcall Axis1RMove(double cmdDist, double& targetPos) = 0;
			virtual BOOL __stdcall Axis2RMove(double cmdDist, double& targetPos) = 0;
			virtual BOOL __stdcall Axis1JogFwd(double& targetPos) = 0;
			virtual BOOL __stdcall Axis1JogBwd(double& targetPos) = 0;
			virtual BOOL __stdcall Axis2JogFwd(double& targetPos) = 0;
			virtual BOOL __stdcall Axis2JogBwd(double& targetPos) = 0;
			virtual BOOL __stdcall Axis1CanMove(double pos) = 0;
			virtual BOOL __stdcall Axis2CanMove(double pos) = 0;
			virtual ~AntiCollision2D();
		};

		DLLINTERFACE AntiCollision2D* __stdcall CreateAntiCollision2D(void);
		DLLINTERFACE void __stdcall	DestroyAntiCollision2D(AntiCollision2D* pBase);

		class DLLINTERFACE AntiCollisionC
		{
		public:
			virtual void __stdcall Init(double* pHwDist,
				LPVOID pAxis1State, LPVOID pAxis1Command, double* pAxis1LimitP, double* pAxis1LimitN, double* pAxis1SetPos,
				LPVOID pAxis2State, LPVOID pAxis2Command, double* pAxis2LimitP, double* pAxis2LimitN, double* pAxis2SetPos) = 0;
			virtual int __stdcall Update(void) = 0;
			virtual BOOL __stdcall CanMoveX(double pos) = 0;
			virtual BOOL __stdcall CanMoveY(double pos) = 0;
			virtual BOOL __stdcall InterpolationCanMoveXY(double xPos, double yPos) = 0;
			virtual BOOL __stdcall RMoveX(double cmdDist, double& targetPos) = 0;
			virtual BOOL __stdcall RMoveY(double cmdDist, double& targetPos) = 0;
			virtual BOOL __stdcall JogFwdX(double& targetPos) = 0;
			virtual BOOL __stdcall JogBwdX(double& targetPos) = 0;
			virtual BOOL __stdcall JogFwdY(double& targetPos) = 0;
			virtual BOOL __stdcall JogBwdY(double& targetPos) = 0;
			virtual ~AntiCollisionC();
		};

		DLLINTERFACE AntiCollisionC* __stdcall CreateAntiCollisionC(void);
		DLLINTERFACE void __stdcall	DestroyAntiCollisionC(AntiCollisionC* pBase);
	}//namespace
#ifdef __cplusplus
}
#endif  //__cplusplus