#include "pch.h"
#include "custombattletask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


CustomBattleTask::CustomBattleTask()
	:CMysqlTask("CustomBattleTask")
{
}

CustomBattleTask::~CustomBattleTask()
{ 
}

void CustomBattleTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into custombattle(_id, custombattle, ctime, mtime) values(" << m_uid;
	ss << ", '";

	std::string data = m_data.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', NOW(), NOW())";

	ss << " on duplicate key update custombattle = '";
	if(-1 == poMysqlConn->EscapeString(data.c_str(), (INT32)data.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CustomBattleTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert custombattle data task failed, uid: %llu, errMsg: %s", m_uid, GetErrStream().str().c_str());
	}
}