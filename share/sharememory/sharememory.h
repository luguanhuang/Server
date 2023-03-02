#ifndef _SHARE_MEMORY_H_
#define _SHARE_MEMORY_H_

#ifndef WIN32
#include <sys/shm.h>
#endif

class ShareMemory
{
public:
#ifdef WIN32
	ShareMemory():m_mem(NULL), m_size(0), m_handle(NULL), m_create(false) {}
	bool Init(const std::string& key, UINT32 size);
#else
	ShareMemory():m_mem(NULL),m_size(0), m_shmId(0), m_create(false) {}
	bool Init(key_t key, UINT32 size);
	bool Detach();
#endif

	void Remove();
	char* GetMem() { return (char*)m_mem; }
	UINT32 GetSize() { return m_size; }
	bool IsCreate() { return m_create; }
	INT32 GetShmId()
	{
#ifdef WIN32
		return (INT32)(m_handle);
#else
		return m_shmId;
#endif
	}

private:
	void* m_mem;
	UINT32 m_size;
#ifdef WIN32
	HANDLE m_handle;
#else
	INT32 m_shmId;
#endif
	bool m_create;
};

#endif