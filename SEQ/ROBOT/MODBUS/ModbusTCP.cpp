#include "ModbusTCP.h"
#include <stdlib.h>
#pragma comment (lib, "ws2_32.lib")

namespace ModbusTCP
{
	CModbusTCP::CModbusTCP() : m_ctx(NULL), m_connected(FALSE)
	{

	}
	CModbusTCP::~CModbusTCP()
	{

	}

	CModbus::CModbus(const TCHAR* lpszSlaveIpAddress, USHORT port) 
	{
		m_modbus = new CModbusTCP;

		CHAR ipAddress[MAX_PATH];
		ZeroMemory(ipAddress, sizeof(ipAddress));

		WideCharToMultiByte(CP_ACP, 0, lpszSlaveIpAddress, -1, ipAddress, MAX_PATH, NULL, NULL);
		m_modbus->m_ctx = modbus_new_tcp(ipAddress, port);
	}
	CModbus::~CModbus()
	{
		this->CloseModbus();
		modbus_free(m_modbus->m_ctx);

		m_modbus->m_ctx = NULL;
		delete m_modbus;
		m_modbus = NULL;
	}

	BOOL CModbus::OpenModbus(char* result)
	{
		if (m_modbus->m_ctx == NULL)
			return (FALSE);

		modbus_set_response_timeout(m_modbus->m_ctx, 1, 0); // set response timeout 1sec
		modbus_set_byte_timeout(m_modbus->m_ctx, 1, 0); // set response timeout 1sec

		if (modbus_connect(m_modbus->m_ctx) == -1)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		m_modbus->m_connected = TRUE;
		return (TRUE);
	}

	BOOL CModbus::CloseModbus(void)
	{
		if(m_modbus->m_ctx)
			modbus_close(m_modbus->m_ctx);

		m_modbus->m_connected = FALSE;

		return TRUE;
	}

	BOOL CModbus::ReadBit(uint8_t* readBit, int readBitCnt, int readBitAddr, char* result)
	{
		if (!m_modbus->m_connected)
			return (FALSE);

		int nReceive = modbus_read_bits(m_modbus->m_ctx, readBitAddr, readBitCnt, readBit);
		if (nReceive != readBitCnt)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		return (TRUE);
	}
	BOOL CModbus::WriteBit(uint8_t* writeBit, int writeBitCnt, int writeBitAddr, char* result)
	{
		if (!m_modbus->m_connected)
			return (FALSE);

		int nReceive = modbus_write_bits(m_modbus->m_ctx, writeBitAddr, writeBitCnt, writeBit);
		if (nReceive != writeBitCnt)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		return (TRUE);
	}
	BOOL CModbus::ReadReg(uint16_t* readReg, int readRegCnt, int readRegAddr, char* result)
	{
		if (!m_modbus->m_connected)
			return (FALSE);

		int nReceive = modbus_read_registers(m_modbus->m_ctx, readRegAddr, readRegCnt, readReg);
		if (nReceive != readRegCnt)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		return (TRUE);
	}
	BOOL CModbus::WriteReg(uint16_t* writeReg, int writeRegCnt, int writeRegAddr, char* result)
	{
		if (!m_modbus->m_connected)
			return (FALSE);

		int nReceive = modbus_write_registers(m_modbus->m_ctx, writeRegAddr, writeRegCnt, writeReg);
		if (nReceive != writeRegCnt)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		return (TRUE);
	}

	BOOL CModbus::ReadTcp(void* readReg, int readRegCnt, int readRegAddr, char* result)
	{
		if (!m_modbus->m_connected)
			return (FALSE);

		int nReceive = modbus_read_registers(m_modbus->m_ctx, readRegAddr, readRegCnt, (unsigned short*)readReg);
		if (nReceive != readRegCnt)
		{
			strcpy(result, modbus_strerror(errno));
			return (FALSE);
		}

		return (TRUE);
	}
}