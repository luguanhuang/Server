#ifndef _DBGROUPCHATSAVEGROUPISSUE_H_
#define _DBGROUPCHATSAVEGROUPISSUE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatSaveGroupIssue : public CMysqlTask
{
public:
    CTaskGroupChatSaveGroupIssue(); 
    virtual ~CTaskGroupChatSaveGroupIssue();


    virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
    virtual void OnReply();

public:
    UINT64	            m_Index;
    UINT32              m_Time;
    KKSG::DBGroupChatSaveGroupIssue m_data;
};

#endif
