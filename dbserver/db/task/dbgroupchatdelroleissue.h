#ifndef _DBGROUPCHATDELROLEISSUE_H_
#define _DBGROUPCHATDELROLEISSUE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatDelRoleIssue : public CMysqlTask
{
public:
	CTaskGroupChatDelRoleIssue(); 
	virtual ~CTaskGroupChatDelRoleIssue();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);  
	virtual void OnReply();

public:
	UINT64  m_Index;
    UINT32  m_TodayBeginTime;
    bool    m_IsClear;
};

#endif
