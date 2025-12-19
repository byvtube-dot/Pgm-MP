#include "..\Includes.h"

namespace IPC_LASER
{
	BOOL CIPC::Init(void)
	{
		int nSize = sizeof(IpcBuffer);
		if (FALSE == m_kamelas.Run(L"LASER2SEQ", nSize, TRUE))
			return (FALSE);

		return (TRUE);
	}

	void CIPC::Comm(void)
	{
		UINT32 nSize = 0;
		static IpcBuffer laserToSeq;
		if (FALSE == m_kamelas.Recv((PBYTE)&laserToSeq, 0, nSize))
			return;

		switch (laserToSeq.command)
		{
			case CMD_RD_SCAN1_DATA:
			{
				LaserList scanNo = LaserList(laserToSeq.command - CMD_RD_SCAN1_DATA);

				ScannerData scanData = { 0 };
				memcpy(&scanData, &LAS[scanNo].m_scanData, sizeof(scanData));

				scanData.scanMode.iMode = LAS[scanNo].IsMode();

				int scanRotAngle = (int)NV.sys[setScanRotAngle];
				int drawRotAngle = 0;
				memcpy(&laserToSeq.buffer.scanData[scanNo], &scanData, sizeof(laserToSeq.buffer.scanData[scanNo]));

				if (scanData.scanMode.iMode != (int)MODE_DEVICE)
					LAS[scanNo].m_isWaitCoordData2 = false;

				LOG[logLAS].Message("[CMD_RD_SCAN%d_COORD]", scanNo + 1);
				break;
			}
			case CMD_WR_SCAN1_DATA:
			{
				LaserList scanNo = LaserList(laserToSeq.command - CMD_WR_SCAN1_DATA);

				memcpy(&LAS[scanNo].m_scanData, &laserToSeq.buffer.scanData[scanNo], sizeof(IPC_LASER::ScannerData));

				LAS[scanNo].m_isReadData = true;
				LOG[logLAS].Message("[CMD_WR_SCAN%d_COORD]", scanNo + 1);
				break;
			}
		}

		if (m_kamelas.Send((PBYTE)&laserToSeq, 0, nSize))
			tmWatchDog.Reset();
		else
			LOG[logLAS].Message("Failed to send data");
	}
};