
#include "pch.h"
#include "dragonguildupdatetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

DGuildUpdateTask::DGuildUpdateTask():CMysqlTask("DGuildUpdateTask")
{

}

DGuildUpdateTask::~DGuildUpdateTask()
{

}

void DGuildUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "update dragonguild set ";

#define DG_CHECK_FIELD(field) \
	if (m_dguildSaveData.has_##field())	\
	{	\
	ss << #field" = '"; \
	if(-1 == poMysqlConn->EscapeString(m_dguildSaveData.field().c_str(), (INT32)m_dguildSaveData.field().size(), pcBuffer, nLength)) return; \
	ss << pcBuffer ;\
	ss << "', ";\
	}

	DG_CHECK_FIELD(name)
	DG_CHECK_FIELD(base)
	DG_CHECK_FIELD(liveness)
	DG_CHECK_FIELD(shop)
	DG_CHECK_FIELD(applications)
	DG_CHECK_FIELD(bindinfo)
	DG_CHECK_FIELD(task)

#undef  DG_CHECK_FIELD
	ss << " mtime=NOW() where _id = " << m_dguildSaveData.id() << ";";

	//SSDebug << "sql: " << ss.str() << END;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return ;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void DGuildUpdateTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Update dragonguild failed[%llu], errMsg: %s", m_dguildSaveData.id(), GetErrStream().str().c_str());
	}
}