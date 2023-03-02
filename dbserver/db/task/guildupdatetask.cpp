
#include "pch.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "guildupdatetask.h"

CGuildUpdateTask::CGuildUpdateTask() : CMysqlTask("GuildUpdateTask")
{
}

CGuildUpdateTask::~CGuildUpdateTask()
{ 
}

#define CHECK_FIELD(field) \
	if (m_guildAllData.has_##field())	\
	{	\
		if (!isFirst)	\
		{	\
			ss << ", ";	\
		}	\
		ss << #field" = '"; \
		if(-1 == poMysqlConn->EscapeString(m_guildAllData.field().c_str(), (INT32)m_guildAllData.field().size(), pcBuffer, nLength)) return; \
		ss << pcBuffer ;\
		ss << "'";\
		isFirst = false;\
	}

void CGuildUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// update guild
	std::stringstream ss;
	ss << "update guild set ";

	bool isFirst = true;

	CHECK_FIELD(name)
	CHECK_FIELD(brief)
	CHECK_FIELD(application)
	CHECK_FIELD(history)
	CHECK_FIELD(card)
	CHECK_FIELD(checkin)
	CHECK_FIELD(guildbonus)
	CHECK_FIELD(guildgoblin)
	CHECK_FIELD(guildactivity)
	CHECK_FIELD(guildwage)
	CHECK_FIELD(garden)
	CHECK_FIELD(guildcamp)
	CHECK_FIELD(guilddonate)

	ss <<" where _id = " << m_guildAllData.gid() << ";";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno() << ", Sql: " << ss.str();
		return;
	}
	SetMysqlError(MYSQL_ERR_SUCESS);
}


void CGuildUpdateTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("update guild failed, id: %llu, errMsg: %s ", m_guildAllData.gid(), GetErrStream().str().c_str());
	}
}