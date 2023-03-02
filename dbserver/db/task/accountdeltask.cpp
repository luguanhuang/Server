#include "pch.h"
#include "accountdeltask.h"
#include "util.h"
#include "crpc.h"
#include "config.h"


CAccountDelTask::CAccountDelTask()
:CMysqlTask("AccountDelTask")
,m_serverId(0)
{
}

CAccountDelTask::~CAccountDelTask()
{ 
}

void CAccountDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select role1, role2, role3, role4, role5, role6, role7, role8, role9, hiddenroles from account where _id='";
	if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' and serverid=" << m_serverId;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	// if empty
	poRes->FetchRow();
	if(IsEmpty(poRes))
	{
		poRes->Release();

		std::stringstream ssDel;
		ssDel << "delete from account where _id='";
		if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
		ssDel << pcBuffer << "' and serverid=" << m_serverId;

		UINT32 effectrows = 0;
		if(!poMysqlConn->Query(ssDel.str().c_str(), ssDel.str().size(), &effectrows))
		{
			m_nMysqlErr = MYSQL_ERR_NOT_EXIST;
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}

		m_nMysqlErr = MYSQL_ERR_SUCESS;
		DBConfig::Instance()->DesCurRegisterNum();
		return;
	}

	poRes->Release();
	m_nMysqlErr = MYSQL_ERR_FAILED;
}

void CAccountDelTask::OnReply()
{
	if(m_nMysqlErr == MYSQL_ERR_SUCESS)
	{
		LogInfo("delete account succeed, account: %s, serverid[%u] errMsg: %s",
			m_strAccount.c_str(), m_serverId, GetErrStream().str().c_str());
	}
	else if (m_nMysqlErr == MYSQL_ERR_FAILED)
	{
		LogInfo("account not empty, account: %s, serverid[%u] errMsg: %s", 
			m_strAccount.c_str(), m_serverId, GetErrStream().str().c_str());
	}
	else
	{
		LogError("Del account data failed, account: %s, serverid[%u] errMsg: %s",
			m_strAccount.c_str(), m_serverId, GetErrStream().str().c_str());
	}
}

bool CAccountDelTask::IsEmpty(IMysqlRecordSet* poRes)
{
	std::map<INT32, std::string> oIndex2ColName;
	INT32 nIndex = 0;
	oIndex2ColName[nIndex++] = "role1";
	oIndex2ColName[nIndex++] = "role2";
	oIndex2ColName[nIndex++] = "role3";
	oIndex2ColName[nIndex++] = "role4";
	oIndex2ColName[nIndex++] = "role5";
	oIndex2ColName[nIndex++] = "role6";
	oIndex2ColName[nIndex++] = "role7";
	oIndex2ColName[nIndex++] = "role8";
	oIndex2ColName[nIndex++] = "role9";
	oIndex2ColName[nIndex++] = "hiddenroles";

	bool bHasRole = false;
	for(std::map<INT32, std::string>::iterator it = oIndex2ColName.begin(); it != oIndex2ColName.end(); ++it)
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;
		if(poRes->GetFieldValue(it->first, &pVal, dwLen))
		{
			if(dwLen != 0)
			{
				bHasRole = true;
				break;
			}
		}
		else
		{
			GetErrStream() << "Read '" << it->second << "' column failed";
			return false;
		}
	}

	return !bHasRole;
}

