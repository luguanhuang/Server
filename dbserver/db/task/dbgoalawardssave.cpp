#include "pch.h"
#include "dbgoalawardssave.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


CTaskGoalAwardsSave::CTaskGoalAwardsSave()
    :CMysqlTask("GoalAwardsSave"), m_Index(0)
{
}

CTaskGoalAwardsSave::~CTaskGoalAwardsSave()
{ 
}

void CTaskGoalAwardsSave::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
    SetMysqlError(MYSQL_ERR_FAILED);

    std::stringstream ss;
    ss << "insert into goalawards(_id, data) values(" << m_Index << ", '"  ;

    std::string data = m_data.SerializeAsString();
    if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
    ss << pcBuffer << "')";

    ss << " on duplicate key update data = '";
    if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
    ss << pcBuffer << "'";

    if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
    {
        GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
        return;
    }

    SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTaskGoalAwardsSave::OnReply()
{
    if(m_nMysqlErr != MYSQL_ERR_SUCESS)
    {
        LogError("CTaskGoalAwardsSave Failed: RoleID: %llu, errMsg: %s", m_Index, GetErrStream().str().c_str());
        return; 
    }
}
