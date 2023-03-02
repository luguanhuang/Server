#ifndef _DBGROUPCHATDELGROUPISSUE_H_
#define _DBGROUPCHATDELGROUPISSUE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatDelGroupIssue : public CMysqlTask
{
public:
	CTaskGroupChatDelGroupIssue(); 
	virtual ~CTaskGroupChatDelGroupIssue();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);  
	virtual void OnReply();

public:
	UINT64  m_Index;
    UINT32  m_TodayBeginTime;
    bool    m_IsClear;
};

#endif
