#include "pch.h"
#include "roleallinforeadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/roledatafields.h"
#include "idutil.h"


CRoleAllInfoReadTask::CRoleAllInfoReadTask()
:CMysqlTask("RoleAllInfoReadTask")
,m_dwRpcDelayed(0)
,m_ServerID(0)
{
}

CRoleAllInfoReadTask::~CRoleAllInfoReadTask()
{ 
}

void CRoleAllInfoReadTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select * from role where account='" << m_szAccount << "'";

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;
		std::string str;

		KKSG::RoleAllInfo sInfo;
		CRoleDataFields oFields(&sInfo);
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
				str.assign(pVal, dwLen);
			}
			else
			{
				GetErrStream() << "Read column '" << oFields.GetFieldName(i) << "' failed";
				poRes->Release();
				return;
			}

			if(!poField->ParseFromString(str))
			{
				GetErrStream() << "Parse '" << oFields.GetFieldName(i) << "' failed, role " << sInfo.brief().roleid() << ", account " << m_szAccount;
				poRes->Release();
				return;
			}
		}
		if (GetServerIDByRoleID(sInfo.brief().roleid()) == m_ServerID)
		{
			KKSG::RoleAllInfo* pInfo =  m_oData.add_roleallinfolist();
			pInfo->CopyFrom(sInfo);
		}
	}
	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRoleAllInfoReadTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
	}
	CUserData oUser((UINT32)nErrCode, (void*)&m_oData);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}