#include "pch.h"
#include "savegiftibitem.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CGiftIBItemTask::CGiftIBItemTask()
	:CMysqlTask("PayAccessInfoTask")
	,m_dwRpcDelayed(0)
{
}

CGiftIBItemTask::~CGiftIBItemTask()
{

}

void CGiftIBItemTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	std::string strData = m_Data.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(strData.c_str(), (INT32)strData.size(), pcBuffer, nLength)) return;
	ss << "replace into giftibitem(`roleid`, `giftself`,`gifttarget`) values(" << m_Data.roleid() << ", '" << pcBuffer  << "'" <<  ")" ;
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CGiftIBItemTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("update gitfitem, roleid: %llu, errMsg: %s", m_Data.roleid(), GetErrStream().str().c_str());
	}
	CUserData oUser((UINT32)nErrCode, &m_Data);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}

