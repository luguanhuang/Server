#include "pch.h"
#include "dbgroupchatsavegroup.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


CTaskGroupChatSaveGroup::CTaskGroupChatSaveGroup()
    :CMysqlTask("GroupChatSaveGroup"), m_Index(0), m_Time(0)
{
}

CTaskGroupChatSaveGroup::~CTaskGroupChatSaveGroup()
{ 
}

void CTaskGroupChatSaveGroup::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    ss << "insert into groupchatgroup(_id, ctime, data) values(" << m_Index << ", " << m_Time << ", '"  ;

    std::string data = m_data.SerializeAsString();
    if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
    ss << pcBuffer << "')";

    ss << " on duplicate key update ctime = "<< m_Time << ", data = '";
    if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
    ss << pcBuffer << "'";

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGroupChatSaveGroup::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGroupChatSaveGroup Failed: GroupID: %llu, Time: %u, errMsg: %s", m_Index, m_Time, GetErrStream().str().c_str());
        return; 
    }
}
