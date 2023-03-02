#include "pch.h"
#include "dbgroupchatdelgroupissue.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CTaskGroupChatDelGroupIssue::CTaskGroupChatDelGroupIssue()
    :CMysqlTask("GroupChatDelGroupIssue") ,m_Index(0), m_TodayBeginTime(0), m_IsClear(false)
{
}

CTaskGroupChatDelGroupIssue::~CTaskGroupChatDelGroupIssue()
{ 
}

void CTaskGroupChatDelGroupIssue::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    if(m_IsClear){
        ss << "delete from groupchatgroupissue where `ctime`<"  << m_TodayBeginTime;
    }else{
        ss << "delete from groupchatgroupissue where `_id`="  << m_Index;
    }

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGroupChatDelGroupIssue::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGroupChatDelGroupIssue Failed: Index = %llu, reason: %s", m_Index, GetErrStream().str().c_str());
        return; 
    }
}