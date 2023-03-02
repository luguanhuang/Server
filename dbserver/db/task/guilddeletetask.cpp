
#include "pch.h"
#include "guilddeletetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"



CGuildDeleteTask::CGuildDeleteTask() : CMysqlTask("GuildDeleteTask")
{
}

CGuildDeleteTask::~CGuildDeleteTask()
{ 
}


void CGuildDeleteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// insert guild
	std::stringstream sql;
	sql << "delete from guild where _id = " << m_guildAllData.gid();

	if(!poMysqlConn->Query(sql.str().c_str(), sql.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CGuildDeleteTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("delete guild failed, id: %llu, errMsg: %s  ", m_guildAllData.gid(), GetErrStream().str().c_str());
	}
}