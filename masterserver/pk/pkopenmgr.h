#ifndef __PKOPENMGR_H__
#define __PKOPENMGR_H__

#include "table/PkOpenTable.h"

struct PkOpenTime
{
	UINT32 hour;
	UINT32 min;
	UINT32 sec;
	PkOpenTime()
	{
		hour = 0;
		min = 0;
		sec = 0;
	}
};

struct PkOpenNode
{
	UINT32 id;
	PkOpenTime begin;
	PkOpenTime end;
};

class PkOpenMgr
{
	DECLARE_SINGLETON(PkOpenMgr);
public:
	bool Init();
	void Uninit();

	UINT32 TimeLimit();

private:
	PkOpenTable m_table;
	PkOpenNode m_workday;
	PkOpenNode m_weekend;
	std::map<time_t, PkOpenNode> m_opens; 
};

#endif