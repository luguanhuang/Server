#include "pch.h"
#include "rolewritetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "idutil.h"


CRoleWriteTask::CRoleWriteTask()
:CMysqlTask("RoleWriteTask")
,m_dwRpcDelayed(0)
,m_qwRoleID(0)
,m_dwTotalSize(0)
,m_nSlot(-1)
,m_lastslot(-1)
{
}

CRoleWriteTask::~CRoleWriteTask()
{ 
}

void CRoleWriteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	//update role
	std::stringstream ss;
	ss << "update role set mtime=NOW()";
	for(CFieldMap::const_iterator it = m_oFieldMap.begin(); it != m_oFieldMap.end(); ++it)
	{
		ss << ", " << it->first << "='";
		if(-1 == poMysqlConn->EscapeString(it->second.c_str(), (INT32)it->second.size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "'";
	}
	ss << " where _id=" << m_qwRoleID;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	if(m_strBriefData.empty())
	{
		SetMysqlError(MYSQL_ERR_SUCESS);
		return;
	}

	UINT32 serverId = GetServerIDByRoleID(m_qwRoleID);
	//update account
	ss.str("");
	ss << "update account set role" << (m_nSlot + 1) << "='";
	if(-1 == poMysqlConn->EscapeString(m_strBriefData.c_str(), (INT32)m_strBriefData.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' where _id='";

	if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' and serverid = " << serverId;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	//update hidden roles
	if (-1 != m_lastslot)
	{
		ss.str("");
		ss << "update account set role" << (m_lastslot) << "='";
		if(-1 == poMysqlConn->EscapeString(m_lastslotrole.c_str(), (INT32)m_lastslotrole.size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "', hiddenroles ='";
		if(-1 == poMysqlConn->EscapeString(m_hiddenroles.c_str(), (INT32)m_hiddenroles.size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "' ";

		ss << "where _id='";
		if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "' and serverid = " << serverId;

		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRoleWriteTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Write role data task failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}

	std::stringstream ss1;
	std::stringstream ss2;
	bool bLogDebug = LogMgr::Instance()->IsEnabled(LOG_DEBUG);
	for(CFieldMap::const_iterator it = m_oFieldMap.begin(); it != m_oFieldMap.end(); ++it)
	{
		if(bLogDebug)
		{
			ss1 << it->first << "=" << it->second.size() << " ";
		}
		if(it->second.size() >= 30 * 1024)
		{
			ss2 << it->first << "=" << it->second.size() << " ";
		}
	}

	LogDebug("Role %llu update data, totalSize %u, fieldNum %u, details: %s", m_qwRoleID, m_dwTotalSize, GetFieldNum(), ss1.str().c_str());
	if(!ss2.str().empty())
	{
		LogWarn("Role %llu update data, field length too big, totalSize %u, fieldNum %u, details: %s", m_qwRoleID, m_dwTotalSize, GetFieldNum(), ss2.str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

void CRoleWriteTask::AddField(const std::string& strField, const std::string& strData)
{
	m_oFieldMap[strField] = strData;
	m_dwTotalSize += strData.size();
}