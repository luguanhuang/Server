#include "pch.h"
#include "marriageoptask.h"


CMarriageOpTask::CMarriageOpTask()
:CMysqlTask("MarriageOpTask")
{
}

CMarriageOpTask::~CMarriageOpTask()
{

}

void CMarriageOpTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	if(-1 == poMysqlConn->EscapeString(m_oData.c_str(), (INT32)m_oData.size(), pcBuffer, nLength))
	{
		return;
	}
	ss << "insert into marriage(_id, data) values(" << m_roleID << ", '" << pcBuffer << "') ";
	ss << "on duplicate key update data = '" << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CMarriageOpTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("MarriageOpTask failed, roleID:%llu, errMsg: %s", m_roleID, GetErrStream().str().c_str());
	}
}
