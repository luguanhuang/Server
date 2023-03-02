#include "pch.h"
#include "mobasavetask.h"


MobaSaveTask::MobaSaveTask()
:CMysqlTask("MobaSaveTask")
{
	m_qwRoleID = 0;
}

MobaSaveTask::~MobaSaveTask()
{

}

void MobaSaveTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into mobarecord(_id, record) values(" << m_qwRoleID;
	ss << ", '";

	std::string data = m_oData.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "')";

	ss << " on duplicate key update record= '";
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void MobaSaveTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("MobaSaveTask failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}
}