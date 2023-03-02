#ifndef _DBGROUPCHATSAVEROLE_H_
#define _DBGROUPCHATSAVEROLE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTaskGroupChatSaveRole : public CMysqlTask
{
public:
    CTaskGroupChatSaveRole(); 
    virtual ~CTaskGroupChatSaveRole();


    virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
    virtual void OnReply();

public:
    UINT64	            m_Index;
    UINT32              m_Time;
    KKSG::DBGroupChatSaveRole m_data;
};

#endif
