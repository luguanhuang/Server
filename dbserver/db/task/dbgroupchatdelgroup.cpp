#include "pch.h"
#include "dbgroupchatdelgroup.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CTaskGroupChatDelGroup::CTaskGroupChatDelGroup()
    :CMysqlTask("GroupChatDelGroup") ,m_Index(0), m_TodayBeginTime(0), m_IsClear(false)
{
}

CTaskGroupChatDelGroup::~CTaskGroupChatDelGroup()
{ 
}

void CTaskGroupChatDelGroup::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    if(m_IsClear){
        ss << "delete from groupchatgroup where `ctime`<"  << m_TodayBeginTime;
    }else{
        ss << "delete from groupchatgroup where `_id`="  << m_Index;
    }

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGroupChatDelGroup::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGroupChatDelGroup Failed: GroupID = %llu, reason: %s", m_Index, GetErrStream().str().c_str());
        return; 
    }
}