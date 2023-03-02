#ifndef _MEM_KV_H_
#define _MEM_KV_H_
#include "util.h"
#include <list>
#include <unordered_map>
#include "sharememory.h"
#include "table/noncopyable.h"


/**
 * 建立在共享内存上key为UINT64的map
 * Value结构必须为POD类型， 无指针，无stl容器, 无string
 */
template <typename V>
class ShareMemoryMap : public noncopyable
{
public:
	ShareMemoryMap() : m_chunkNum(0),m_chunkSize(0),m_memSize(0),m_memStart(0) {}

	struct StatIno
	{
		INT32 shmId;
		UINT32 totalSize;
		UINT32 chunkSize;
		UINT32 chunkNum;
		UINT32 usedNum;
		UINT32 freeNum;
	};

	struct ChunkHead
	{
		UINT64 key;
		bool used;
	};

	bool Init(UINT64 memKey, UINT32 maxNum)
	{
		m_chunkNum = maxNum;
		m_chunkSize = CalcuChunkSize(sizeof(ChunkHead) + sizeof(V));
		m_memSize = m_chunkNum * m_chunkSize;
		SSDebug << "chunkNum: " << m_chunkNum << ", chunkSize: " << m_chunkSize << ", totalMem: " << m_memSize << END;

#ifdef WIN32
		std::string key = ToString(memKey);
		bool success = m_shm.Init(key, m_memSize);
#else
		bool success = m_shm.Init(memKey, m_memSize);
#endif
		if (!success)
		{
			SSError << "Init share memory Failed" << END;
			return false;
		}
		m_memStart = m_shm.GetMem();
		if (m_shm.IsCreate())
		{
			memset(m_memStart, 0, m_memSize);
		}

		Load();
		return true;
	}

	void Uninit(bool remove = true)
	{
		if (remove)
		{
			m_shm.Remove();
		}
	}

	void Load()
	{
		for (UINT32 i = 0; i < m_chunkNum; ++i)
		{
			ChunkHead* head = GetChunkHead(i);
			if (!head->used)
			{
				m_freeChunk.push_back(i);	
			}
			else
			{
				m_key2ChunkIndex[head->key] = i;
			}
		}
	}

	V* Get(UINT64 key)
	{
		auto it = m_key2ChunkIndex.find(key);
		if (it != m_key2ChunkIndex.end())
		{
			return GetChunkData(it->second);
		}
		if (m_freeChunk.empty())
		{
			SSError << "no chunk" << END;
			return NULL;
		}
		UINT32 index = m_freeChunk.back();
		m_freeChunk.pop_back();
		ChunkHead* head = GetChunkHead(index);
		head->used = true;
		head->key = key;
		m_key2ChunkIndex.insert(std::make_pair(key, index));
		return GetChunkData(index);
	}

	void Remove(UINT64 key)
	{
		auto it = m_key2ChunkIndex.find(key);
		if (it == m_key2ChunkIndex.end())
		{
			return;
		}
		UINT32 index = it->second;
		ChunkHead* head = GetChunkHead(index);
		memset((void*)head, 0, m_chunkSize);
		m_freeChunk.push_back(index);
		m_key2ChunkIndex.erase(it);
	}

	void GetAll(std::vector<std::pair<UINT64, V*>>& data)
	{
		for (auto it = m_key2ChunkIndex.begin(); it != m_key2ChunkIndex.end(); ++it)
		{
			UINT64 key = it->second;
			V* v = GetChunkData(it->second);
			data.push_back(std::make_pair(key, v));
		}
	}

	void GetAll(std::vector<V*>& data)
	{
		for (auto it = m_key2ChunkIndex.begin(); it != m_key2ChunkIndex.end(); ++it)
		{
			V* v = GetChunkData(it->second);
			data.push_back(v);
		}
	}

	void GetStatInfo(StatIno& stat)
	{
		stat.chunkNum = m_chunkNum;
		stat.chunkSize = m_chunkSize;
		stat.totalSize = m_memSize;
		stat.usedNum = m_key2ChunkIndex.size();
		stat.freeNum = m_freeChunk.size();
		stat.shmId = m_shm.GetShmId();
	}

	std::string GetStatStr()
	{
		StatIno info;
		GetStatInfo(info);
		std::ostringstream oss;
		oss << "shmId: " << info.shmId << ", totalSize: " << info.totalSize << ", chunkSize: " << info.chunkSize
			<< ", chunkNum: " << info.chunkNum << ", usedNum: " << info.usedNum << ", freeNum: " << info.freeNum;
		return oss.str();
	}

private:
	inline ChunkHead* GetChunkHead(UINT32 chunkIndex)
	{
		return (ChunkHead*)(m_memStart + m_chunkSize * chunkIndex);
	}

	inline V* GetChunkData(UINT32 chunkIndex)
	{

		return (V*)(m_memStart + m_chunkSize * chunkIndex + sizeof(ChunkHead));
	}

	UINT32 CalcuChunkSize(UINT32 size)
	{
		UINT32 i = 1;
		while (1)
		{
			if (i * 8 >= size)
			{
				return i * 8;
			}
			i++;
		}
		// impossible
		return 0;
	}

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