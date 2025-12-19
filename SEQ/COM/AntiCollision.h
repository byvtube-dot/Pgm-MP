#pragma once
#include "..\BASE\KASIO.h"
#include "..\COM\FSM.h"
#include "..\COM\WmxLib.h"
#include "..\HW\IO.h"
#include "..\HW\Motor.h"

namespace COM
{
	class AntiCollision
	{
	private:
		kasiolib::AntiCollision2D*	m_pBase;
		double						m_targetPos;
		double						m_hwDist;

	public:
		enum BaseDir
		{
			DIR_RR,		//	DIR: AXIS1(- +)"->", AXIS2(- +)"->"
			DIR_RL,		//	DIR: AXIS1(- +)"->", AXIS2(+ -)"<-"
			DIR_LR,		//	DIR: AXIS1(+ -)"<-", AXIS2(- +)"->"
			DIR_LL,		//	DIR: AXIS1(+ -)"<-", AXIS2(+ -)"<-"
		};
		int					m_no;
		AxisList			m_axis1;
		AxisList			m_axis2;

		static const int	maxDevice = (antiCollisionMAX > 0) ? antiCollisionMAX : 1;
		int					m_skipAntiCollision;
		enDI				m_bumpingSensorNo;
	public:
		AntiCollision();
		virtual ~AntiCollision();

	public:
		BOOL Init(int baseDir, double hwDist, int skipAntiCollision,
			AxisList axis1, double* pAxis1SetPos, AxisList axis2, double* pAxis2SetPos, enDI bumpingSensorNo = (enDI)NOT_DEFINED);
		void Update(void);
		void Axis1RMove(double cmdDist, double vel, double acc);
		void Axis2RMove(double cmdDist, double vel, double acc);
		void Axis1JogFwd(double vel, double acc);
		void Axis1JogBwd(double vel, double acc);
		void Axis2JogFwd(double vel, double acc);
		void Axis2JogBwd(double vel, double acc);
		BOOL Axis1CanMove(double pos);
		BOOL Axis2CanMove(double pos);
	};

	class AntiCollisionCircle
	{
	private:
		kasiolib::AntiCollisionC*		m_pBase;
		double							m_targetPos;

	public:
		int					m_no;
		AxisList			m_axisX;
		AxisList			m_axisY;
		POINT2D				m_centerPos;
		double*				m_radiusCnt; // 반지름.

		static const int	maxDevice = (antiCollisionCircleMAX > 0) ? antiCollisionCircleMAX : 1;
		int					m_skipAntiCollision;

	public:
		AntiCollisionCircle();
		virtual ~AntiCollisionCircle();

		BOOL Init(double* pHwDist, int skipAntiCollision,
			AxisList axisX, double* pAxisXSetPos, AxisList axisY, double* pAxisYSetPos);
		void Update(void);
		BOOL CanMoveX(double pos);
		BOOL CanMoveY(double pos);
		BOOL InterpolationCanMoveXY(POINT2D pos);
		void RMoveX(double cmdDist, double vel, double acc);
		void RMoveY(double cmdDist, double vel, double acc);
		void JogFwdX(double vel, double acc);
		void JogBwdX(double vel, double acc);
		void JogFwdY(double vel, double acc);
		void JogBwdY(double vel, double acc);
	};
}