#include "pch.h"
#include "dbgroupchatdelroleissue.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CTaskGroupChatDelRoleIssue::CTaskGroupChatDelRoleIssue()
    :CMysqlTask("GroupChatDelRoleIssue") ,m_Index(0), m_TodayBeginTime(0), m_IsClear(false)
{
}

CTaskGroupChatDelRoleIssue::~CTaskGroupChatDelRoleIssue()
{ 
}

void CTaskGroupChatDelRoleIssue::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    if(m_IsClear){
        ss << "delete from groupchatroleissue where `ctime`<"  << m_TodayBeginTime;
    }else{
        ss << "delete from groupchatroleissue where `_id`="  << m_Index;
    }

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGroupChatDelRoleIssue::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGroupChatDelRoleIssue Failed: Index = %llu, reason: %s", m_Index, GetErrStream().str().c_str());
        return; 
    }
}