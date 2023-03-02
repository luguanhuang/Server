#include "pch.h"
#include "platfriendoptask.h"


CPlatFriendOpTask::CPlatFriendOpTask()
:CMysqlTask("PlatFriendOpTask")
{
}

CPlatFriendOpTask::~CPlatFriendOpTask()
{

}

void CPlatFriendOpTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into platformfriendinfo(_id, data) values('" ;
	if(-1 == poMysqlConn->EscapeString(m_openid.c_str(), (INT32)m_openid.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";
	if(-1 == poMysqlConn->EscapeString(m_oData.c_str(), (INT32)m_oData.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";
	ss << ") ";
	ss << "on duplicate key update data = '" << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CPlatFriendOpTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("PlatFriendOpTask failed, openid:%s, errMsg: %s", m_openid.c_str(), GetErrStream().str().c_str());
	}
}
