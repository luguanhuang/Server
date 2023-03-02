#ifndef _DBGROUPCHATDELGROUP_H_
#define _DBGROUPCHATDELGROUP_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatDelGroup : public CMysqlTask
{
public:
	CTaskGroupChatDelGroup(); 
	virtual ~CTaskGroupChatDelGroup();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);  
	virtual void OnReply();

public:
	UINT64  m_Index;
    UINT32  m_TodayBeginTime;
    bool    m_IsClear;
};

#endif
