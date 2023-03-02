#include "pch.h"
#include "friendwritetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


FriendInsertTask::FriendInsertTask()
:CMysqlTask("FriendInsertTask")
{
}

FriendInsertTask::~FriendInsertTask()
{ 
}

void FriendInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into friend(_id, name, friend, ctime, mtime) values(" << m_roleid;
	ss << ", '";

	if(-1 == poMysqlConn->EscapeString(m_name.c_str(), (INT32)m_name.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";

	std::string friendinfo = m_info.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(friendinfo.c_str(), (INT32)friendinfo.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', NOW(), NOW())";

	ss << " on duplicate key update friend = '";
	if(-1 == poMysqlConn->EscapeString(friendinfo.c_str(), (INT32)friendinfo.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void FriendInsertTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Insert friend data task failed, name: %s, errMsg: %s", m_name.c_str(), GetErrStream().str().c_str());
	}
}