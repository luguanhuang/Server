#include "pch.h"
#include "crossrolesummarytask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CrossRoleSummaryTask::CrossRoleSummaryTask()
:CMysqlTask("CrossRoleSummaryTask")
{
}

CrossRoleSummaryTask::~CrossRoleSummaryTask()
{ 
}

void CrossRoleSummaryTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into cross_rolesummary(_id, data) values(" << m_roleid;
	ss << ", '";

	std::string data = m_info.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "')";

	ss << " on duplicate key update data= '";
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CrossRoleSummaryTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Insert cross rolesummary data task failed, name: %s, errMsg: %s", m_info.name().c_str(), GetErrStream().str().c_str());
	}
}
