#pragma once
#include <windows.h>
#include <tchar.h>		
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>
#include <stdint.h>

#ifdef ETHERCATCOMMLIB_EXPORTS
#define DLLINTERFACE __declspec(dllexport)
#else
#define DLLINTERFACE __declspec(dllimport)
#endif

namespace EtherCATPacketCommLib
{
	static const int PACKET_WAIT_TIMEOUT = 20;
	static const int IO_WAIT_TIMEOUT = 10;
	static const int HOSTSTATE_TIMEOUT = 5000;
	static const int maxQueue = 10;
	static const int maxChannel = 16;
	static const int maxContact = 8;
	static const UINT8 BITMASK8[8] =
	{
		0x01, 0x02, 0x04, 0x08,
		0x10, 0x20, 0x40, 0x80,
	};

	enum
	{
		PROTOCOL_VERSION	= 1,		// 프로토콜 버전

		MAX_PACKET_SIZE		= 2000,
		IO_SIZE				= 16,
		VERSION_SIZE		= 1,
		PADDING_SIZE		= 3,
		TX_SIZE				= 20,
		RX_SIZE				= 16,
		PACKET_SIZE			= 200,
	};

	enum
	{
		C_SEND_START		= 100,
		C_SEND_REVIEW		= 101,
		C_SEND_PROCESS		= 102,
		C_SEND_END			= 199,
	};

#pragma pack(push, 1)
	typedef struct tagIO		//	16 BYTE
	{
		BYTE		val[IO_SIZE];
	}IO, *PIO;

	typedef struct tagTX		//	20 BYTE
	{
		int			id;
		int			port;
		BYTE		padding[1];
		INT16		packetSize;
		INT16		packetCount;
		INT16		maxSize;
		INT16		maxCount;
		INT16		offset;

		BYTE		start : 1;
		BYTE		dummy : 7;
	}TX, *PTX;

	typedef struct tagRX		//	16 BYTE
	{
		int			id;
		int			port;
		BYTE		padding[5];
		INT16		packetCount;

		BYTE		busy : 1;
		BYTE		dummy : 7;
	}RX, *PRX;

	typedef struct tagPACKET
	{
		BYTE		data[PACKET_SIZE];
	}PACKET, *PPACKET;

	typedef struct tagPACKET_INFO
	{
		IO			io;
		BYTE		version;
		BYTE		padding[3];
		TX			tx;
		RX			rx;
		PACKET		packet;
	}PACKET_INFO, *PPACKET_INFO;

	typedef struct tagDATA_INFO
	{
		int			size;
		int			port;
		int			count;
		BYTE		data[MAX_PACKET_SIZE];
	}DATA_INFO, *PDATA_INFO;

	class DLLINTERFACE PacketTimer
	{
	private:
		LARGE_INTEGER   m_startTime;
		LARGE_INTEGER   m_endTime;
		LARGE_INTEGER   m_elapsedTime;

		static LARGE_INTEGER& GetTm(void);

	public:
		static LARGE_INTEGER& Frequency(void);
		static void Update(void);
		BOOL Reset();
		BOOL Reset(BOOL shouldRst);
		long Elapsed_nSec();
		double Elapsed_mSec();
		double ElapsedSec();
		double Elapsed();
		BOOL TmOver(DWORD dwDelay);
		BOOL TmOverSec(double secDelay);

		PacketTimer();
		~PacketTimer();
	};

	class DLLINTERFACE PacketQueue
	{
	private:
		static const int	m_maxCnt = 100;
		unsigned int		m_st;
		unsigned int		m_ed;
		DATA_INFO			data[m_maxCnt];

	public:
		void __stdcall push(DATA_INFO result);
		DATA_INFO __stdcall pop(void);
		DATA_INFO __stdcall peek(void);
		bool __stdcall empty(void);
		size_t __stdcall size(void);
		void __stdcall clear(void);

		PacketQueue();
		~PacketQueue();
	};

	class DLLINTERFACE EtherCATMaster
	{
	private:
		int					m_state;
		int					m_once;
		int					m_count;
		PACKET_INFO			m_sendPacketInfo;
		PACKET_INFO			m_recvPacketInfo;
		DATA_INFO			m_sendDataInfo;
		DATA_INFO			m_recvDataInfo;
		PacketQueue			m_recvPacketQueue;

		HANDLE				m_hCreateEvent;
		BYTE				m_readSendData[PACKET_SIZE];

		void SendCycle(void);
		void RecvCycle(void);

	private:
		bool				m_isClose;
		HANDLE				m_hMutex;
		HANDLE				m_hSendEvent;
		HANDLE				m_hRecvEvent;
		HANDLE				m_hThread;
		DWORD				m_dwThreadId;
		bool				m_isBlocking;
		int					m_errCode;

		HANDLE				m_hDriver;
		HANDLE				m_hChannel;
		uint32_t			m_ulChannel;
		uint32_t			m_ulState;

		bool SendCommand(PBYTE strMessage, int nSize, int nPort, DWORD dwMilliseconds);
		static void UpdateProcess(LPVOID lpParam);
		PACKET_INFO& SendPacketInfo(void);
		PACKET_INFO& RecvPacketInfo(void);

	public:
		int __stdcall Open(LPCTSTR lpName, bool isBlocking);
		int __stdcall Close(void);
		PBYTE __stdcall GetSendPacketInfoAddr(void);
		PBYTE __stdcall GetRecvPacketInfoAddr(void);
		PBYTE __stdcall GetReadSendPacketInfoAddr(void);
		int __stdcall Update(void);
		bool __stdcall Send(PBYTE strMessage, int nSize, int nPort, DWORD dwMilliseconds);
		bool __stdcall Recv(PBYTE strMessage, int& nSize, int& nPort, DWORD dwMilliseconds);

		bool __stdcall AOn(int ioNo);
		bool __stdcall AOff(int ioNo);
		void __stdcall Set(int ioNo, bool isOn);
		bool __stdcall IsOn(int ioNo);
		bool __stdcall IsOff(int ioNo);

		EtherCATMaster();
		~EtherCATMaster();
	};

	class DLLINTERFACE EtherCATSlave
	{
	private:
		int					m_state;
		int					m_once;
		int					m_count;
		PACKET_INFO			m_sendPacketInfo;
		PACKET_INFO			m_recvPacketInfo;
		DATA_INFO			m_sendDataInfo;
		DATA_INFO			m_recvDataInfo;
		PacketQueue			m_recvPacketQueue;

		HANDLE				m_hCreateEvent;
		BYTE				m_readSendData[PACKET_SIZE];

		void SendCycle(void);
		void RecvCycle(void);

	private:
		bool				m_isClose;
		HANDLE				m_hMutex;
		HANDLE				m_hSendEvent;
		HANDLE				m_hRecvEvent;
		HANDLE				m_hThread;
		DWORD				m_dwThreadId;
		bool				m_isBlocking;
		int					m_errCode;

		HANDLE				m_hDriver;
		HANDLE				m_hChannel;
		uint32_t			m_ulChannel;
		uint32_t			m_ulState;

		bool SendCommand(PBYTE strMessage, int nSize, int nPort, DWORD dwMilliseconds);
		static void UpdateProcess(LPVOID lpParam);
		PACKET_INFO& SendPacketInfo(void);
		PACKET_INFO& RecvPacketInfo(void);

	public:
		int __stdcall Open(LPCTSTR lpName, bool isBlocking);
		int __stdcall Close(void);
		PBYTE __stdcall GetSendPacketInfoAddr(void);
		PBYTE __stdcall GetRecvPacketInfoAddr(void);
		PBYTE __stdcall GetReadSendPacketInfoAddr(void);
		int __stdcall Update(void);
		bool __stdcall Send(PBYTE strMessage, int nSize, int nPort, DWORD dwMilliseconds);
		bool __stdcall Recv(PBYTE strMessage, int& nSize, int& nPort, DWORD dwMilliseconds);

		bool __stdcall AOn(int ioNo);
		bool __stdcall AOff(int ioNo);
		void __stdcall Set(int ioNo, bool isOn);
		bool __stdcall IsOn(int ioNo);
		bool __stdcall IsOff(int ioNo);

		EtherCATSlave();
		~EtherCATSlave();
	};
#pragma pack(pop)
}//namespace