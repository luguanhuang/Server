#include "pch.h"
#include "roledeltask.h"
#include "util.h"
#include "crpc.h"
#include "config.h"

CRoleDelTask::CRoleDelTask()
:CMysqlTask("RoleDelTask")
{
}

CRoleDelTask::~CRoleDelTask()
{ 
}

void CRoleDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	//update account
	std::ostringstream ss;
	ss.str("");
	ss << "update account set lastSelect=0, role" << (m_slot + 1) << "='' where _id='";
	if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength))
	{
		return;
	}
	ss << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRoleDelTask::OnReply()
{
	if (m_nMysqlErr == MYSQL_ERR_SUCESS)
	{
		SSInfo << "del role success, account: " << m_strAccount << ", roleId: " << m_roleId << ", slot: " << m_slot << END;
	}
	else
	{
		SSInfo << "del role failed, account: " << m_strAccount << ", roleId: " << m_roleId << ", slot: " << m_slot 
			   << ", errMsg:" << GetErrStream().str().c_str() << END;
	}
}