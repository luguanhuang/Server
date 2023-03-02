#include "pch.h"
#include "updatereporttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/sqlbuilder.h"

UpdateReportTask::UpdateReportTask()
:CMysqlTask("UpdateReportTask")
,m_rpcId(0)
,m_roleId(0)
{
}

UpdateReportTask::~UpdateReportTask()
{
}

void UpdateReportTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::ostringstream oss;
	oss << "replace into rolereport (`_id`, `data`) values(" << m_roleId << ", '";

	std::string data;
	m_data.SerializeToString(&data);
	if (!poMysqlConn->EscapeString(data.c_str(), data.size(), pcBuffer, nLength))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	oss << pcBuffer << "')";

	std::string sql = oss.str();
	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void UpdateReportTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update rolereport failed[%llu], errMsg: %s", m_roleId, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}
