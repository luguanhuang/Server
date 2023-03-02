#ifndef __GUILDINHERIT_H__
#define __GUILDINHERIT_H__

#include "table/GuildInheritTable.h"

struct InheritData
{
	UINT64 m_roleOne;
	UINT64 m_roleTwo;
	UINT32 m_startTime;
};

class CGuildInherit
{
	CGuildInherit();
	~CGuildInherit();
	DECLARE_SINGLETON(CGuildInherit);

public:
	bool Init(); 
	void Uninit();

public:
	void AddInherit(UINT64 roleId, UINT64 reqRoleId);
	void ClearInherit();
	UINT32 AcceptInherit(UINT64 roleId, UINT64 reqRoleId);
	UINT32 CanInherit(UINT64 roleId, UINT64 reqRoleId);
	bool IsInherit(UINT64 roleId,  UINT64 reqRoleId);
	void DelInherit(UINT64 roleId, UINT64 reqRoleId);
	void GetReqRole(UINT64 roleId, std::map<UINT64, UINT32>& dataMap);
	void DelAll(UINT64 roleId);
	bool IsMapRight(UINT64 roleOne, UINT64 roleTwo);
	void NoticeReqNum(UINT64 roleId);
	void DoRoleOnline(UINT64 roleId);
	KKSG::ErrorCode StartTeacherInherit(UINT64 roleOne, UINT64 roleTwo);

private:
	GuildInheritTable m_table;
	std::map<UINT64, std::map<UINT64, UINT32> > m_inheritDataMap;
};

#endif
