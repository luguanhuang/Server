#include "pch.h"
#include "arenastartask.h"

CArenaStarTask::CArenaStarTask(const KKSG::ArenaStarRoleData& data)
	:CMysqlTask("arenastarroledata")
	,m_data(data)
{

}

CArenaStarTask::~CArenaStarTask()
{

}

void CArenaStarTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	bool ret = FillSql(ss, poMysqlConn, pcBuffer, nLength);
	if(!ret)
	{
		return;
	}
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CArenaStarTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		SSError << " arenastarroledata task failed op = " << " role = " << m_data.roleid() << " errMsg = " << GetErrStream().str() << END;
	}
}

bool CArenaStarTask::FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	if(0 == m_data.roleid())
	{
		return false;
	}

	std::string stmp;
	m_data.SerializePartialToString(&stmp);
	if(-1 == poMysqlConn->EscapeString(stmp.c_str(), (int)stmp.size(), pcBuffer, nLength))
	{
		return false;
	}

	ss << "insert into arenastarroledata values("
		<< m_data.roleid() << "," 
		<< "'" << pcBuffer << "') on duplicate key update data = '" 
		<< pcBuffer << "'";

	return true;
}


