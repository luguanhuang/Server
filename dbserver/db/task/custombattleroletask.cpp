#include "pch.h"
#include "custombattleroletask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


CustomBattleRoleTask::CustomBattleRoleTask()
	:CMysqlTask("CustomBattleRoleTask")
{
}

CustomBattleRoleTask::~CustomBattleRoleTask()
{ 
}

void CustomBattleRoleTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into custombattlerole(_id, system, custom, ctime, mtime) values(" << m_roleid;
	ss << ", '";

	std::string system = m_system.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(system.c_str(), (INT32)system.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";

	std::string custom = m_custom.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(custom.c_str(), (INT32)custom.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', NOW(), NOW())";

	ss << " on duplicate key update system = '";
	if(-1 == poMysqlConn->EscapeString(system.c_str(), (INT32)system.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', custom = '";
	if(-1 == poMysqlConn->EscapeString(custom.c_str(), (INT32)custom.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CustomBattleRoleTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert custombattlerole data task failed, uid: %llu, errMsg: %s", m_roleid, GetErrStream().str().c_str());
	}
}