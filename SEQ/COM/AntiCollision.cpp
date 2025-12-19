#include "..\Includes.h"

namespace COM
{
	AntiCollision::AntiCollision()
	{
		m_hwDist = 0;
		m_axis1 = m_axis2 = AxisList(NOT_DEFINED);
		m_skipAntiCollision = (antiCollisionMAX > 0) ? 0 : 1;
	}

	AntiCollision::~AntiCollision()
	{
		if (NULL != this->m_pBase)
			kasiolib::DestroyAntiCollision2D(this->m_pBase);

		this->m_pBase = NULL;
	}

	BOOL AntiCollision::Init(int baseDir, double hwDist, int skipAntiCollision,
		AxisList axis1, double* pAxis1SetPos, AxisList axis2, double* pAxis2SetPos, enDI bumpingSensorNo)
	{
		this->m_pBase = kasiolib::CreateAntiCollision2D();
		if (NULL == this->m_pBase)
			return (FALSE);

		m_hwDist = hwDist;
		m_axis1 = axis1;
		m_axis2 = axis2;
		m_skipAntiCollision = skipAntiCollision;
		this->m_pBase->Init(baseDir, &m_hwDist,
			&MT[axis1].m_state, &MT[axis1].m_command, &MT[axis1].m_limitP, &MT[axis1].m_limitN, pAxis1SetPos,
			&MT[axis2].m_state, &MT[axis2].m_command, &MT[axis2].m_limitP, &MT[axis2].m_limitN, pAxis2SetPos);

		m_bumpingSensorNo = bumpingSensorNo;
		return (TRUE);
	}

	void AntiCollision::Update(void)
	{
		if (m_skipAntiCollision)
			return;

		BOOL isBumpSensorOn = (NOT_DEFINED != m_bumpingSensorNo) && BOn(m_bumpingSensorNo);
		int stopAxis = m_pBase->Update(isBumpSensorOn);
		if (1 == stopAxis)
		{
			if (MT[m_axis1].m_state.isHome)
			{
				MSGNOTICE.Set(MSG_CANMOVE,
					"CLOSE", NULL, NULL, NULL,
					"%s Collision was detected", MT[m_axis1].m_name.c_str());
			}
			MT[m_axis1].Stop();
			MT[m_axis1].m_state.isHome = false;
		}
		if (2 == stopAxis)
		{
			if (MT[m_axis2].m_state.isHome)
			{
				MSGNOTICE.Set(MSG_CANMOVE,
					"CLOSE", NULL, NULL, NULL,
					"%s Collision was detected", MT[m_axis2].m_name.c_str());
			}
			MT[m_axis2].Stop();
			MT[m_axis2].m_state.isHome = false;
		}
	}

	void AntiCollision::Axis1RMove(double cmdDist, double vel, double acc)
	{
		if (m_skipAntiCollision || !cmdDist)
			return;

		if (MT[m_axis1].IsRdy() && m_pBase->Axis1RMove(cmdDist, m_targetPos))
			MT[m_axis1].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollision::Axis2RMove(double cmdDist, double vel, double acc)
	{
		if (m_skipAntiCollision || !cmdDist)
			return;

		if (MT[m_axis2].IsRdy() && m_pBase->Axis2RMove(cmdDist, m_targetPos))
			MT[m_axis2].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollision::Axis1JogFwd(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (MT[m_axis1].IsRdy() && m_pBase->Axis1JogFwd(m_targetPos))
			MT[m_axis1].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollision::Axis1JogBwd(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (MT[m_axis1].IsRdy() && m_pBase->Axis1JogBwd(m_targetPos))
			MT[m_axis1].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollision::Axis2JogFwd(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (MT[m_axis2].IsRdy() && m_pBase->Axis2JogFwd(m_targetPos))
			MT[m_axis2].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollision::Axis2JogBwd(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (MT[m_axis2].IsRdy() && m_pBase->Axis2JogBwd(m_targetPos))
			MT[m_axis2].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	BOOL AntiCollision::Axis1CanMove(double pos)
	{
		if (m_skipAntiCollision)
			return (TRUE);

		return (m_pBase->Axis1CanMove(pos));
	}

	BOOL AntiCollision::Axis2CanMove(double pos)
	{
		if (m_skipAntiCollision)
			return (TRUE);

		return (m_pBase->Axis2CanMove(pos));
	}

	BOOL AntiCollisionCircle::Init(double* pHwDist, int skipAntiCollision,
		AxisList axisX, double* pAxisXSetPos, AxisList axisY, double* pAxisYSetPos)
	{
		this->m_pBase = kasiolib::CreateAntiCollisionC();
		if (NULL == this->m_pBase)
			return (FALSE);

		m_axisX = axisX;
		m_axisY = axisY;
		m_skipAntiCollision = skipAntiCollision;
		this->m_pBase->Init(pHwDist,
			&MT[axisX].m_state, &MT[axisX].m_command, &MT[axisX].m_limitP, &MT[axisX].m_limitN, pAxisXSetPos,
			&MT[axisY].m_state, &MT[axisY].m_command, &MT[axisY].m_limitP, &MT[axisY].m_limitN, pAxisYSetPos);
		return (TRUE);
	}

	void AntiCollisionCircle::Update(void)
	{
		if (m_skipAntiCollision)
			return;

		if (!MT[m_axisX].m_state.isHome || !MT[m_axisY].m_state.isHome)
			return;

		if (NV.use[useMasterKeyMode])
			return;

		if (!m_pBase->Update())
			return;

		MT[m_axisX].Stop();
		MT[m_axisX].m_state.isHome = false;

		MT[m_axisY].Stop();
		MT[m_axisY].m_state.isHome = false;
	}

	AntiCollisionCircle::AntiCollisionCircle()
	{
		m_skipAntiCollision = (antiCollisionCircleMAX > 0) ? 0 : 1;
	}

	AntiCollisionCircle::~AntiCollisionCircle()
	{
		if (NULL != this->m_pBase)
			kasiolib::DestroyAntiCollisionC(this->m_pBase);

		this->m_pBase = NULL;
	}

	BOOL AntiCollisionCircle::CanMoveX(double pos)
	{
		if (m_skipAntiCollision)
			return (TRUE);

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return (FALSE);
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return (FALSE);
		}

		return m_pBase->CanMoveX(pos);
	}

	BOOL AntiCollisionCircle::CanMoveY(double pos)
	{
		if (m_skipAntiCollision)
			return (TRUE);

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return (FALSE);
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return (FALSE);
		}

		return m_pBase->CanMoveY(pos);
	}

	BOOL AntiCollisionCircle::InterpolationCanMoveXY(POINT2D pos)
	{
		if (m_skipAntiCollision)
			return (TRUE);

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return (FALSE);
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return (FALSE);
		}

		return m_pBase->InterpolationCanMoveXY(pos.dX, pos.dY);
	}

	void AntiCollisionCircle::RMoveX(double cmdDist, double vel, double acc)
	{
		if (!cmdDist)
			return;

		if (MT[m_axisX].IsRdy() && m_pBase->RMoveX(cmdDist, m_targetPos))
			MT[m_axisX].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollisionCircle::RMoveY(double cmdDist, double vel, double acc)
	{
		if (!cmdDist)
			return;

		if (MT[m_axisY].IsRdy() && m_pBase->RMoveY(cmdDist, m_targetPos))
			MT[m_axisY].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollisionCircle::JogFwdX(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return;
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return;
		}

		if (MT[m_axisX].IsRdy() && m_pBase->JogFwdX(m_targetPos))
			MT[m_axisX].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollisionCircle::JogBwdX(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return;
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return;
		}

		if (MT[m_axisX].IsRdy() && m_pBase->JogBwdX(m_targetPos))
			MT[m_axisX].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollisionCircle::JogFwdY(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return;
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return;
		}

		if (MT[m_axisY].IsRdy() && m_pBase->JogFwdY(m_targetPos))
			MT[m_axisY].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}

	void AntiCollisionCircle::JogBwdY(double vel, double acc)
	{
		if (m_skipAntiCollision)
			return;

		if (!MT[m_axisX].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisX].m_name.c_str());

			return;
		}

		if (!MT[m_axisY].m_state.isHome)
		{
			MSGNOTICE.Set(MSG_CANMOVE,
				"CLOSE", NULL, NULL, NULL,
				"%sMotor Is Not Home Done", MT[m_axisY].m_name.c_str());

			return;
		}

		if (MT[m_axisY].IsRdy() && m_pBase->JogBwdY(m_targetPos))
			MT[m_axisY].PosMove(P_JOG, m_targetPos, fabs(vel), acc);
	}
}