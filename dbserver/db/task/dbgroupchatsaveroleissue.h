#ifndef _DBGROUPCHATSAVEROLEISSUE_H_
#define _DBGROUPCHATSAVEROLEISSUE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatSaveRoleIssue : public CMysqlTask
{
public:
    CTaskGroupChatSaveRoleIssue(); 
    virtual ~CTaskGroupChatSaveRoleIssue();


    virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
    virtual void OnReply();

public:
    UINT64	            m_Index;
    UINT32              m_Time;
    KKSG::DBGroupChatSaveRoleIssue m_data;
};

#endif
