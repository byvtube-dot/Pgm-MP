#pragma once
#include <windows.h>
#include <tchar.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <shlwapi.h>
#include <time.h>
#include <tchar.h>
#include <mmsystem.h>
#include <cassert>

class CMMF
{
private:
	int		m_nSize;
	HANDLE	m_hFile;
	HANDLE	m_hMap;
	PBYTE	m_pMapAddr;
	BOOL	m_bCreateFile;
	WCHAR	m_lpName[MAX_PATH];
	WCHAR	m_lpFilename[MAX_PATH];

public:
	void Init(LPCTSTR lpName, LPCTSTR lpFileName, int nSize, BOOL bCreateFile = TRUE)
	{
		wcscpy(m_lpName, lpName);
		wcscpy(m_lpFilename, lpFileName);
		m_nSize = nSize;

		m_pMapAddr = NULL;
		m_hFile = NULL;
		m_hMap = NULL;
		m_bCreateFile = bCreateFile;
	}

	void Flush(void)
	{
		if (m_bCreateFile)
		{
			if (NULL != m_pMapAddr)
				FlushViewOfFile((LPVOID)m_pMapAddr, NULL);
		}
	}

	BOOL Open(void)
	{
		if (m_bCreateFile)
		{
			m_hFile = ::CreateFile(m_lpFilename,
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_ALWAYS,
				NULL,
				NULL);

			if (NULL == m_hFile || INVALID_HANDLE_VALUE == m_hFile)
				return (FALSE);
		}

		m_hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, m_lpName);

		if (NULL == m_hMap)
		{
			if (m_bCreateFile)
			{
				m_hMap = ::CreateFileMapping(m_hFile,
					NULL,
					PAGE_READWRITE | SEC_COMMIT,
					0,
					m_nSize,
					m_lpName);
			}
			else
			{
				m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
					NULL,
					PAGE_READWRITE | SEC_COMMIT,
					0,
					m_nSize,
					m_lpName);
			}

			if (NULL == m_hMap || INVALID_HANDLE_VALUE == m_hMap)
				return (FALSE);
		}

		m_pMapAddr = (PBYTE)::MapViewOfFile(m_hMap,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			m_nSize);

		if (NULL == m_pMapAddr)
			return (FALSE);

		return (TRUE);
	}

	PBYTE GetAddr(void)
	{
		return (m_pMapAddr);
	}

	CMMF()
	{
		ZeroMemory(this, sizeof(CMMF));
	}

	~CMMF()
	{
		if (m_bCreateFile)
		{
			if (NULL != m_pMapAddr)
				FlushViewOfFile((LPVOID)m_pMapAddr, NULL);
			if (NULL != m_pMapAddr)
				UnmapViewOfFile((LPVOID)m_pMapAddr);
		}
		if (NULL != m_hMap)
			CloseHandle(m_hMap);
		if (NULL != m_hFile)
			CloseHandle(m_hFile);

		m_pMapAddr = NULL;
		m_hFile = NULL;
		m_hMap = NULL;
		m_bCreateFile = FALSE;
	}
};

