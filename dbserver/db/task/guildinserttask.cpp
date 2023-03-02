
#include "pch.h"
#include "guildinserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


CGuildInsertTask::CGuildInsertTask() : CMysqlTask("GuildInsertTask")
{
}

CGuildInsertTask::~CGuildInsertTask()
{ 
}

#define SET_FIELD(field) \
	ss << "'"; \
	if(-1 == poMysqlConn->EscapeString(m_guildAllData.field().c_str(), (INT32)m_guildAllData.field().size(), pcBuffer, nLength)) return; \
	ss << pcBuffer << "', ";


void CGuildInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// insert guild
	std::stringstream ss;
	ss << "insert into guild( `_id`, `name`, `brief` , `application` , `history` , `card` , `checkin` , `guildbonus` , `guildgoblin` , `guildactivity` , `guildwage` , `garden`,`guildcamp`, `guilddonate`, `ctime` , `mtime`) values(";
	ss << m_guildAllData.gid() << ", '";
	//ss << m_guildAllData.name() << "', ";
	if(-1 == poMysqlConn->EscapeString(m_guildAllData.name().c_str(), (INT32)m_guildAllData.name().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', ";

	SET_FIELD(brief)
	SET_FIELD(application)
	SET_FIELD(history)
	SET_FIELD(card)
	SET_FIELD(checkin)
	SET_FIELD(guildbonus)
	SET_FIELD(guildgoblin)
	SET_FIELD(guildactivity)
	SET_FIELD(guildwage)
	SET_FIELD(garden)
	SET_FIELD(guildcamp)
	SET_FIELD(guilddonate)

	ss << "NOW(), NOW());";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CGuildInsertTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("insert guild failed, id: %llu, errMsg: %s ", m_guildAllData.gid(), GetErrStream().str().c_str());
	}
}