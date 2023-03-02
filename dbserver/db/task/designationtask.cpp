#include "pch.h"
#include "rolereadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "designationtask.h"
#include "timeutil.h"

DesignationTask::DesignationTask():CMysqlTask("DesignationTask"),m_dwRpcDelayed(0),m_qwRoleID(0){}
DesignationTask::~DesignationTask(){}

void DesignationTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	std::string strData = m_Data.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(strData.c_str(), (INT32)strData.size(), pcBuffer, nLength)) return;
	ss << "replace into designationevent(`roleid`, `data`) values(" << m_qwRoleID << ", '" << pcBuffer  << "'" <<  ")" ;
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void DesignationTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("INSERT designation failed, roleID: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
		return; 
	}
}