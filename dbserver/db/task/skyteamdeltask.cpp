#include "pch.h"
#include "skyteamdeltask.h"

#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

SkyTeamDelTask::SkyTeamDelTask()
:CMysqlTask("SkyTeamDelTask")
,m_isDeleteAll(false)
,m_rpcId(0)
,m_id(0)
{
}

SkyTeamDelTask::~SkyTeamDelTask()
{
}

void SkyTeamDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// delete partner
	std::stringstream ss;
	if (m_isDeleteAll)
	{
		ss << "delete from skyteam";
	}
	else
	{
		ss << "delete from skyteam where _id=" << m_id;
	}

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void SkyTeamDelTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if (m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Update skyteam failed[%llu], errMsg: %s", m_id, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_rpcId, oUser);
}