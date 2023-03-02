#ifndef _DBGROUPCHATDELROLE_H_
#define _DBGROUPCHATDELROLE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatDelRole : public CMysqlTask
{
public:
	CTaskGroupChatDelRole(); 
	virtual ~CTaskGroupChatDelRole();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);  
	virtual void OnReply();

public:
	UINT64  m_Index;
    UINT32  m_TodayBeginTime;
    bool    m_IsClear;
};

#endif
