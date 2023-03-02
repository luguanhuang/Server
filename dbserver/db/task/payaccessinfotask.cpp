#include "pch.h"
#include "payaccessinfotask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CPayAccessInfoTask::CPayAccessInfoTask()
	:CMysqlTask("PayAccessInfoTask")
	,m_dwRpcDelayed(0)
{
}

CPayAccessInfoTask::~CPayAccessInfoTask()
{

}

void CPayAccessInfoTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	std::stringstream ss;
	ss << "insert into pay(`roleid`,`saveamt`,`nosaveamt`,`payotherlist`,`consumelist`,`sendlist`) values(" << m_Data.roleid() << 
		"," << m_Data.saveamt() << "," << m_Data.nosaveamt() <<  ",'";
	if(-1 == poMysqlConn->EscapeString(m_Data.other().c_str(), (INT32)m_Data.other().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "','";
	if(-1 == poMysqlConn->EscapeString(m_Data.consume().c_str(), (INT32)m_Data.consume().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "','";
	if(-1 == poMysqlConn->EscapeString(m_Data.send().c_str(), (INT32)m_Data.send().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "') on duplicate key update `saveamt`=" << m_Data.saveamt() << ",`nosaveamt`=" << m_Data.nosaveamt() << ",`payotherlist`='";
	if(-1 == poMysqlConn->EscapeString(m_Data.other().c_str(), (INT32)m_Data.other().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "',`consumelist`='";
	if(-1 == poMysqlConn->EscapeString(m_Data.consume().c_str(), (INT32)m_Data.consume().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "',`sendlist`='";
	if(-1 == poMysqlConn->EscapeString(m_Data.send().c_str(), (INT32)m_Data.send().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CPayAccessInfoTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Write payaccessinfo data task failed, roleid: %llu, errMsg: %s", m_Data.roleid(), GetErrStream().str().c_str());
	}
	if (m_Data.op() == KKSG::PayAccess_Consume)
	{
		CUserData oUser((UINT32)nErrCode, NULL);
		CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
	}
}

