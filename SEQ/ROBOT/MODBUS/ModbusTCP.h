#pragma once
#include "BASE/modbus.h"
#include "..\RainbowRoboticsDef.h"

#define MAX_MODBUS_DATA 100

namespace ModbusTCP
{
	typedef union {
		uint8_t Value[MAX_MODBUS_DATA];
	}BIT_DATA;

	typedef union {
		uint8_t Value[MAX_MODBUS_DATA];
	}REG_DATA;


	class CModbusTCP
	{
	public:
		modbus_t*			m_ctx;

		BOOL				m_connected;

		CModbusTCP();
		~CModbusTCP();
	};
	class CModbus
	{
	private:
		BIT_DATA			m_bitData;
		REG_DATA			m_regData;
	public:
		CModbusTCP*			m_modbus;

		CModbus(const TCHAR* lpszSlaveIpAddress, USHORT port);
		virtual~CModbus();

	public:
		BOOL OpenModbus(char* result);
		BOOL CloseModbus(void);

		BOOL ReadBit(uint8_t* readBit, int readBitCnt, int readBitAddr, char* result);
		BOOL WriteBit(uint8_t* writeBit, int writeBitCnt, int writeBitAddr, char* result);
		BOOL ReadReg(uint16_t* readReg, int readRegCnt, int readRegAddr, char* result);
		BOOL WriteReg(uint16_t* writeReg, int writeRegCnt, int writeRegAddr, char* result);

		BOOL ReadTcp(void* readReg, int readRegCnt, int readRegAddr, char* result);
	};
}