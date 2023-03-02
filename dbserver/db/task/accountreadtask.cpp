#include "pch.h"
#include "accountreadtask.h"
#include "util.h"
#include "crpc.h"
#include "config.h"


CAccountReadTask::CAccountReadTask()
:CMysqlTask("AccountReadTask")
,m_dwRpcDelayed(0)
,m_serverId(0)
{
}

CAccountReadTask::~CAccountReadTask()
{ 
}

void CAccountReadTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select * from account where _id='";
	if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' and serverid=" << m_serverId;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	if(poRes->GetRowNum() == 0)
	{
		poRes->Release();
		if(!DBConfig::Instance()->IsLoginEnabled() || DBConfig::Instance()->IsRegisterNumReachMax())
		{
			SetMysqlError(MYSQL_ERR_NOT_EXIST);
			return;
		}

		std::stringstream ssInsert;
		ssInsert << "insert into account(_id, lastSelect, role1, role2, role3, role4, role5, role6, role7, role8, role9, hiddenroles, ctime, mtime, serverid) values('";

		if(-1 == poMysqlConn->EscapeString(m_strAccount.c_str(), (INT32)m_strAccount.size(), pcBuffer, nLength)) return;
		ssInsert << pcBuffer << "', -1, '', '', '', '', '', '', '', '', '', '', NOW(), NOW()," << m_serverId << ")";
		if(!poMysqlConn->Query(ssInsert.str().c_str(), ssInsert.str().size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}

		if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}

		DBConfig::Instance()->IncCurRegisterNum();
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

void CAccountReadTask::OnReply()
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

	CUserData oUser((UINT32)nErrCode, (void*)&m_oData);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

bool CAccountReadTask::ReadColumns(IMysqlRecordSet* poRes)
{
	char* pVal   = NULL;
	UINT32 dwLen = 0;

	m_oData.set_account(m_strAccount);
	if(poRes->GetFieldValue(1, &pVal, dwLen))
	{
		UINT32 dwSlot = convert<UINT32>(pVal);
		m_oData.set_selectslot(dwSlot);
	}
	else
	{
		GetErrStream() << "Read 'lastSelect' column failed";
		return false;
	}

	if(poRes->GetFieldValue(2, &pVal, dwLen))
	{
		m_oData.set_role1(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role1' column failed";
		return false;
	}

	if(poRes->GetFieldValue(3, &pVal, dwLen))
	{
		m_oData.set_role2(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role2' column failed";
		return false;
	}

	if(poRes->GetFieldValue(4, &pVal, dwLen))
	{
		m_oData.set_role3(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role3' column failed";
		return false;
	}

	if(poRes->GetFieldValue(5, &pVal, dwLen))
	{
		m_oData.set_role4(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role4' column failed";
		return false;
	}

	if(poRes->GetFieldValue(6, &pVal, dwLen))
	{
		m_oData.set_role5(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role5' column failed";
		return false;
	}

	if(poRes->GetFieldValue(7, &pVal, dwLen))
	{
		m_oData.set_role6(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role6' column failed";
		return false;
	}

	if(poRes->GetFieldValue(8, &pVal, dwLen))
	{
		m_oData.set_role7(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role7' column failed";
		return false;
	}

	if(poRes->GetFieldValue(9, &pVal, dwLen))
	{
		m_oData.set_role8(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role8' column failed";
		return false;
	}

	if(poRes->GetFieldValue(10, &pVal, dwLen))
	{
		m_oData.set_role9(pVal, dwLen);
	}
	else
	{
		GetErrStream() << "Read 'role9' column failed";
		return false;
	}

	return true;
}
