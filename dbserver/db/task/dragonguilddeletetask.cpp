
#include "pch.h"
#include "dragonguilddeletetask.h"
#include "util.h"
#include "util/dbutil.h"

DGuildDeleteTask::DGuildDeleteTask() : CMysqlTask("DragonGuildDeleteTask")
{

}

DGuildDeleteTask::~DGuildDeleteTask()
{

}

void DGuildDeleteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream sql;
	sql << "delete from dragonguild where _id = " << m_dguildSaveData.id();

	if (poMysqlConn->Query(sql.str().c_str(), sql.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetErrno() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void DGuildDeleteTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("delete dragonguild failed, id: %llu, errMsg: %s", m_dguildSaveData.id(), GetErrStream().str().c_str());
	}
}