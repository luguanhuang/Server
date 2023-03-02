#include "pch.h"
#include "roleinserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "utildb/roledatafields.h"
#include "idutil.h"


CRoleInsertTask::CRoleInsertTask()
:CMysqlTask("RoleInsertTask")
,m_dwRpcDelayed(0)
{
}

CRoleInsertTask::~CRoleInsertTask()
{ 
}

void CRoleInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	CRoleDataFields oFields(&m_oRoleInfo);

	//insert role
	std::stringstream ss;
	ss << "insert into role(_id, name, account, ctime, mtime";
	for(UINT32 i = 0; i < oFields.GetCount(); ++i)
	{
		ss << ", " << oFields.GetFieldName(i);
	}
	ss << ") values(" << m_oRoleInfo.brief().roleid();
	ss << ", '";

	if(-1 == poMysqlConn->EscapeString(m_oRoleInfo.brief().name().c_str(), (INT32)m_oRoleInfo.brief().name().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";

	if(-1 == poMysqlConn->EscapeString(m_oRoleInfo.brief().accountid().c_str(), (INT32)m_oRoleInfo.brief().accountid().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', NOW(), NOW()";

	for(UINT32 i = 0; i < oFields.GetCount(); ++i)
	{
		google::protobuf::MessageLite* poField = oFields.GetField(i);
		if(poField == NULL)
		{
			GetErrStream() << "Invalid field index when insert role";
			return;
		}

		std::string strField = poField->SerializeAsString();
		if(-1 == poMysqlConn->EscapeString(strField.c_str(), (INT32)strField.size(), pcBuffer, nLength)) return;
		ss << ", '" << pcBuffer << "' ";
	}
	ss << ")";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}


	//update nickid
	UINT64 qwInsertID = 0;
	if(!poMysqlConn->GetLastInsertID(qwInsertID)) return;

	m_oRoleInfo.mutable_brief()->set_nickid((UINT32)qwInsertID);
	std::string strData = m_oRoleInfo.brief().SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(strData.c_str(), (INT32)strData.size(), pcBuffer, nLength)) return;

	ss.str("");
	ss << "update role set RoleBrief='";
	ss << pcBuffer << "' where _id=" << m_oRoleInfo.brief().roleid();
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	//update account
	KKSG::RoleBriefInfo brief;
	brief.set_name(m_oRoleInfo.brief().name());
	brief.set_type(m_oRoleInfo.brief().type());
	brief.set_roleid(m_oRoleInfo.brief().roleid());
	brief.set_level(1);
	std::string strBriefData = brief.SerializeAsString();

	UINT32 serverId = GetServerIDByRoleID(m_oRoleInfo.brief().roleid());
	ss.str("");
	INT32 nSlot = MapProfessionToSlot(m_oRoleInfo.brief().type());
	ss << "update account set role" << (nSlot + 1) << "='";
	if(-1 == poMysqlConn->EscapeString(strBriefData.c_str(), (INT32)strBriefData.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' where _id='";

	if(-1 == poMysqlConn->EscapeString(m_oRoleInfo.brief().accountid().c_str(), (INT32)m_oRoleInfo.brief().accountid().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "' and serverid = " << serverId;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CRoleInsertTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert role data task failed, account: %s, errMsg: %s", m_oRoleInfo.brief().accountid().c_str(), GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, &m_oRoleInfo);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}