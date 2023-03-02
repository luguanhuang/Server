#ifndef _DBGROUPCHATSAVEGROUP_H_
#define _DBGROUPCHATSAVEGROUP_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatSaveGroup : public CMysqlTask
{
public:
    CTaskGroupChatSaveGroup(); 
    virtual ~CTaskGroupChatSaveGroup();


    virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
    virtual void OnReply();

public:
    UINT64	            m_Index;
    UINT32              m_Time;
    KKSG::DBGroupChatSaveGroup m_data;
};

#endif
