#ifndef _SHM_KEY_VALUE_H_
#define _SHM_KEY_VALUE_H_

#include <unordered_map>
#include "sharememory.h"
#include "table/noncopyable.h"

class ShmKV
{
public:

	bool Init(UINT64 memKey, UINT32 chunkSize, UINT32 chunkNum);
	bool Init(UINT64 memKey, UINT32 keyChunkSize, UINT32 keyChunkNum);

private:
	struct MemHead
	{
		UINT32 chunkSize;
		UINT32 chunkNum;
	}
#ifdef WIN32
	;
#else
	__attribute__ ((packed));
#endif	

	struct ChunkHead
	{
		UINT32 preChunkId;
		UINT32 nextChunkId;
	}
#ifdef WIN32
	;
#else
	__attribute__ ((packed));
#endif	



private:
	UINT32 m_chunkNum;
	UINT32 m_chunkSize;
	UINT32 m_memSize;
	char* m_memStart;
	std::vector<UINT32> m_freeChunk;
	std::unordered_map<UINT64, UINT32> m_key2ChunkIndex;
	ShareMemory m_shm;
};

#endif