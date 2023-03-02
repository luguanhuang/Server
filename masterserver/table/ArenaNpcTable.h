#ifndef __ARENANPCTABLE_H__
#define __ARENANPCTABLE_H__

#include "table/globalconfig.h"

#define ARENANPC_NAME_LENGTH 32
#define ARENANPC_COUNT (UINT32)( GetGlobalConfig().ArenaNpcCount)

struct ArenaNpcInfo
{
	UINT64 qwNpcId;
	UINT32 dwNpcLevel;
	UINT32 dwNpcProfession;
	char szNpcName[ARENANPC_NAME_LENGTH];
	ArenaNpcInfo()
	:qwNpcId(0)
	,dwNpcLevel(0)
	,dwNpcProfession(0)
	{
		memset(szNpcName, 0, ARENANPC_NAME_LENGTH);
	}
};

class ArenaNpcTable
{
public:
	ArenaNpcTable();
	~ArenaNpcTable();

	void Clear();
	bool LoadFile(const char* pszFile);

	UINT32 Count() { return m_vecNpcInfo.size(); }

	bool InTable(UINT64 uid);
	const ArenaNpcInfo* GetInfo(UINT64 uid);

public:
	std::vector<ArenaNpcInfo*> m_vecNpcInfo;
	std::unordered_map<UINT64, ArenaNpcInfo*> m_uid2info;
};

#endif