#include "pch.h"
#include "punishtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CPunishTask::CPunishTask()
:CMysqlTask("PunishTask")
{

}

CPunishTask::~CPunishTask()
{

}

void CPunishTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	if (m_oData.op() == KKSG::PUNISH_DATA_DELETE)
	{
		ss << "delete from punishmentlist where `roleid`="<< m_oData.roleid() <<" and `punishtype`="<<m_oData.punishtype();
	}
	else
	{
		ss << "insert into punishmentlist(`account`,`roleid`,`punishtype`,`punishdate`,`bantime`,`reason`, `rankdata`) values('" << m_oData.account() << "',"
			<< m_oData.roleid() << "," << m_oData.punishtype() << "," << m_oData.punishdate() << "," << m_oData.bantime() << ",'";
		if(-1 == poMysqlConn->EscapeString(m_oData.reason().c_str(), (INT32)m_oData.reason().size(), pcBuffer, nLength)) return;
		ss	<< pcBuffer << "','";
		if(-1 == poMysqlConn->EscapeString(m_oData.rankdata().c_str(), (INT32)m_oData.rankdata().size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "') on duplicate key update `account`='" << m_oData.account() << "',`punishdate`=" << m_oData.punishdate() << ",`bantime`=" << m_oData.bantime();
		ss << ",`reason`='";
		if(-1 == poMysqlConn->EscapeString(m_oData.reason().c_str(), (INT32)m_oData.reason().size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "'" << ",`rankdata`='";
		if(-1 == poMysqlConn->EscapeString(m_oData.rankdata().c_str(), (INT32)m_oData.rankdata().size(), pcBuffer, nLength)) return;
		ss << pcBuffer << "'";
	}

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << " sql=" << ss.str().c_str() << " ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}
void CPunishTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("PunishTask failed,roleid: %llu, errMsg: %s", m_oData.roleid(), GetErrStream().str().c_str());
	}
}
