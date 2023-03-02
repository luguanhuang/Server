#include "pch.h"
#include "rolereadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/roledatafields.h"
#include "idutil.h"


CRoleReadTask::CRoleReadTask()
:CMysqlTask("RoleReadTask")
,m_dwRpcDelayed(0)
,m_qwRoleID(0)
,m_nSlot(0)
,m_nReadType(KKSG::ROLE_DATA_READ_IDIP)
{
}

CRoleReadTask::~CRoleReadTask()
{ 
}

void CRoleReadTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	CRoleDataFields oFields(&m_oRoleInfo);

	std::stringstream ss;
	ss << "select * from role where _id=" << m_qwRoleID;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	poRes->FetchRow();
	char* pVal   = NULL;
	UINT32 dwLen = 0;
	for(UINT32 i = 0; i < oFields.GetCount(); ++i)
	{
		google::protobuf::MessageLite* poField = oFields.GetField(i);
		if(poField == NULL)
		{
			GetErrStream() << "Invalid field index when read role";
			poRes->Release();
			return;
		}

		if(poRes->GetFieldValue(6 + i, &pVal, dwLen))
		{
			m_strData.assign(pVal, dwLen);
		}
		else
		{
			GetErrStream() << "Read column '" << oFields.GetFieldName(i) << "' failed";
			poRes->Release();
			return;
		}

		if(dwLen == 0)
		{
			CRoleDataFieldCleanup::Instance().ClearField(&m_oRoleInfo, poField);
			continue;
		}

		if(!poField->ParseFromString(m_strData))
		{
			GetErrStream() << "Parse '" << oFields.GetFieldName(i) << "' failed, role " << m_qwRoleID;
			poRes->Release();
			return;
		}
	}

	if(m_nReadType == KKSG::ROLE_DATA_READ_SELECT_ROLE)
	{
		std::string strAccount;
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strAccount.assign(pVal, dwLen);
		}
		else
		{
			GetErrStream() << "Read 'account' column failed";
			poRes->Release();
			return;
		}

		//update lastSelect
		ss.str("");
		ss << "update account set lastSelect=" << m_nSlot << " where _id='";
		if(-1 == poMysqlConn->EscapeString(strAccount.c_str(), (INT32)strAccount.size(), pcBuffer, nLength))
		{
			poRes->Release();
			return;
		}
		ss << pcBuffer << "' and serverid=" << GetServerIDByRoleID(m_qwRoleID);

		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			poRes->Release();
			return;
		}
	}

	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRoleReadTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Select role data task failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, &m_oRoleInfo);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}