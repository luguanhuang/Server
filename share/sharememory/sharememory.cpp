#include "pch.h"
#include "sharememory.h"
#include <iostream>
#include <cstring>
#include <string>
#include <errno.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
LPCWSTR stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length()-1));// ×ÔÐÐÊÍ·Å
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}

bool ShareMemory::Init(const std::string& key, UINT32 size)
{
	m_handle = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, TEXT(key.c_str()));
	if (NULL == m_handle)
	{
		m_create = true;
		m_handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, TEXT(key.c_str()));
		m_mem = (void*)::MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		SSWarn << "Create new share memory: " << key << ", size: " << size << END;
		return true;
	}
	else
	{
		m_create = false;
		m_mem = (void*)::MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		SSWarn << "Attach share memory: " << key << ", size: " << size << END;
	}
	return false;
}

void ShareMemory::Remove()
{
	if (m_mem)
	{
		::UnmapViewOfFile(m_mem);
		m_mem = NULL;
	}
	if (m_handle)
	{
		::CloseHandle(m_handle);
		m_handle = NULL;
	}
}

#else

bool ShareMemory::Init(key_t key, UINT32 size)
{
	m_shmId = shmget(key, size, IPC_CREAT | IPC_EXCL | 0666);
	if (m_shmId < 0)
	{
		m_create = false;
		m_shmId = shmget(key, size, 0666);
		if (m_shmId < 0)
		{
			LogError("shmget error: %s", strerror(errno));
			return false;
		}
	}
	else
	{
		m_create = true;
	}

	m_mem = shmat(m_shmId, NULL, 0);
	if (m_mem == (void*)-1)
	{
		LogError("shmat error: %s", strerror(errno));
		return false;
	}
	m_size = size;
	if (m_create)
	{
		SSWarn << "Create new share memory: " << key << ", size: " << size << END;
	}
	else
	{
		SSWarn << "Attach share memory: " << key << ", size: " << size << END;
	}
	return true;
}

bool ShareMemory::Detach()
{
	if (!m_shmId)
	{
		return false;
	}
	return shmdt(m_mem) == 0;
}

void ShareMemory::Remove()
{
	if (!m_shmId)
	{
		return;
	}
	shmctl(m_shmId, IPC_RMID, 0);
}

#endif