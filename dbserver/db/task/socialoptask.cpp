#include "pch.h"
#include "socialoptask.h"


CSocialOpTask::CSocialOpTask()
:CMysqlTask("SocialOpTask")
{
}

CSocialOpTask::~CSocialOpTask()
{

}

void CSocialOpTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	std::string strFlowerData = "";
	if (m_oData.has_flower())
	{
		strFlowerData = m_oData.flower();
	}
	if(-1 == poMysqlConn->EscapeString(strFlowerData.c_str(), (INT32)strFlowerData.size(), pcBuffer, nLength))
	{
		return;
	}
	ss << "insert into social(_id, flower) values(" << m_oData.roleid() << ", '" << pcBuffer << "') ";
	ss << "on duplicate key update flower = '" << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CSocialOpTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("SocialOptask failed, id: %llu, roleid: %llu, errMsg: %s", m_oData.roleid(), m_oData.roleid(), GetErrStream().str().c_str());
	}
}
