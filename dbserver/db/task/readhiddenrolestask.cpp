#include "pch.h"
#include "readhiddenrolestask.h"
#include "util.h"
#include "crpc.h"
#include "config.h"

ReadHiddenRolesTask::ReadHiddenRolesTask()
:CMysqlTask("AccountReadTask")
,m_slot(0)
,m_serverid(0)
,m_dwRpcDelayed(0)
{
}

ReadHiddenRolesTask::~ReadHiddenRolesTask()
{ 
}

void ReadHiddenRolesTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select role"<<m_slot<<", hiddenroles from account where _id='";
	if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' and serverid=" << m_serverid;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	poRes->FetchRow();
	if(!ReadColumns(poRes))
	{
		poRes->Release();
		return;
	}

	poRes->Release();
	m_nMysqlErr = MYSQL_ERR_SUCESS; 
}

void ReadHiddenRolesTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr == MYSQL_ERR_NOT_EXIST)
	{
		SetMysqlError(MYSQL_ERR_SUCESS);
		nErrCode = KKSG::ERR_REGISTER_NUM_LIMIT;
	}

	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Read account data failed, account: %s, errMsg: %s", m_strAccount.c_str(), GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, (void*)&m_data);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

bool ReadHiddenRolesTask::ReadColumns(IMysqlRecordSet* poRes)
{
	char* pVal   = NULL;
	UINT32 dwLen = 0;

	if(poRes->GetFieldValue(0, &pVal, dwLen))
	{
		m_data.set_slotrole(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role' column failed, " << m_slot;
		return false;
	}

	if(poRes->GetFieldValue(1, &pVal, dwLen))
	{
		m_data.set_hiddenroles(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'hiddenroles' column failed";
		return false;
	}

	return true;
}
