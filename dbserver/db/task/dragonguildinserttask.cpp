
#include "pch.h"
#include "dragonguildinserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

DGuildInsertTask::DGuildInsertTask(): CMysqlTask("DGuildInsertTask")
{

}

DGuildInsertTask::~DGuildInsertTask()
{

}

void DGuildInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream sql;
	sql << "insert into dragonguild(`_id`, `name`, `base`, `liveness`, `shop`, `applications`, `bindinfo`, `task`, `ctime`, `mtime`) values(";
	sql << m_dguildSaveData.id() << ", ";

#define DG_SET_FIELD(field) \
	sql << "'"; \
	if(-1 == poMysqlConn->EscapeString(m_dguildSaveData.field().c_str(), (INT32)m_dguildSaveData.field().size(), pcBuffer, nLength)) return; \
	sql << pcBuffer << "', ";

	DG_SET_FIELD(name)
	DG_SET_FIELD(base)
	DG_SET_FIELD(liveness)
	DG_SET_FIELD(shop)
	DG_SET_FIELD(applications)
	DG_SET_FIELD(bindinfo)
	DG_SET_FIELD(task)

#undef DG_SET_FIELD

	sql << "NOW(), NOW());";

	SSInfo << "sql: " << sql.str() << END;

	if (!poMysqlConn->Query(sql.str().c_str(), sql.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetErrno() << ", ErrNo: " << poMysqlConn->GetErrno();
		return ;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void DGuildInsertTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("insert into dragonguild failed, id: %llu, ErrMsg: %s", m_dguildSaveData.id(), GetErrStream().str().c_str());
	}
}