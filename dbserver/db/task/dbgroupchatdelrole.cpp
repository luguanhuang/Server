#include "pch.h"
#include "dbgroupchatdelrole.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CTaskGroupChatDelRole::CTaskGroupChatDelRole()
    :CMysqlTask("GroupChatDelRole") ,m_Index(0), m_TodayBeginTime(0), m_IsClear(false)
{
}

CTaskGroupChatDelRole::~CTaskGroupChatDelRole()
{ 
}

void CTaskGroupChatDelRole::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    ss << "delete from groupchatrole where `_id`="  << m_Index;

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGroupChatDelRole::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGroupChatDelRole Failed: Roleid = %llu, reason: %s", m_Index, GetErrStream().str().c_str());
        return; 
    }
}