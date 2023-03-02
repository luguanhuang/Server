#include "pch.h"
#include "readaccounttasknew.h"
#include "util.h"
#include "crpc.h"
#include "config.h"


CReadAccountTaskNew::CReadAccountTaskNew()
:CMysqlTask("AccountReadTaskNew")
,m_dwRpcDelayed(0)
,m_serverId(0)
{
}

CReadAccountTaskNew::~CReadAccountTaskNew()
{ 
}

void CReadAccountTaskNew::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
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

	if (poRes->FetchRow())
	{
		if(!ReadColumns(poRes))
		{
			poRes->Release();
			return;
		}
	}
	poRes->Release();
	m_nMysqlErr = MYSQL_ERR_SUCESS; 
}

void CReadAccountTaskNew::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr == MYSQL_ERR_NOT_EXIST)
	{
		SetMysqlError(MYSQL_ERR_SUCESS);
	}
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Read account data failed, account: %s, errMsg: %s", m_strAccount.c_str(), GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, (void*)&m_roles);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

bool CReadAccountTaskNew::ReadColumns(IMysqlRecordSet* poRes)
{
	char* pVal   = NULL;
	UINT32 dwLen = 0;

	if(poRes->GetFieldValue(1, &pVal, dwLen))
	{
		UINT32 dwSlot = convert<UINT32>(pVal);
	}
	else
	{
		GetErrStream() << "Read 'lastSelect' column failed";
		return false;
	}

	std::vector<std::string> roleDatas;
	if(poRes->GetFieldValue(2, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role1' column failed";
		return false;
	}

	if(poRes->GetFieldValue(3, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role2' column failed";
		return false;
	}

	if(poRes->GetFieldValue(4, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role3' column failed";
		return false;
	}

	if(poRes->GetFieldValue(5, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role4' column failed";
		return false;
	}

	if(poRes->GetFieldValue(6, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role5' column failed";
		return false;
	}

	if(poRes->GetFieldValue(7, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role6' column failed";
		return false;
	}

	if(poRes->GetFieldValue(8, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role7' column failed";
		return false;
	}

	if(poRes->GetFieldValue(9, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role8' column failed";
		return false;
	}

	if(poRes->GetFieldValue(10, &pVal, dwLen))
	{
		std::string data;
		if (dwLen != 0)
		{
			data.assign(pVal, dwLen);
			roleDatas.push_back(data);
		}
	}
	else
	{
		GetErrStream() << "Read 'role9' column failed";
		return false;
	}

	for (auto it = roleDatas.begin(); it != roleDatas.end(); ++it)
	{
		KKSG::RoleBriefInfo info;
		if (info.ParseFromString(*it))
		{
			KKSG::ZoneRoleInfo role;
			role.set_roleid(info.roleid());
			role.set_rolename(info.name());
			role.set_roleprofession(info.type());
			role.set_rolelevel(info.level());
			m_roles.push_back(role);
		}
	}

	return true;
}