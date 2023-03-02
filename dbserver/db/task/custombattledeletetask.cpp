#include "pch.h"
#include "custombattledeletetask.h"
#include "util.h"
#include "utildb/sqlbuilder.h"
#include "crpc.h"

CustomBattleDeleteTask::CustomBattleDeleteTask()
	:CMysqlTask("CustomBattleDeleteTask")
{
	m_uid = 0;
}

CustomBattleDeleteTask::~CustomBattleDeleteTask()
{

}

void CustomBattleDeleteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "delete from custombattle where _id=" << m_uid;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CustomBattleDeleteTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Task Failed, errMsg: %s", GetErrStream().str().c_str());
	}
	else
	{
		SSInfo<<"delete uid:"<<m_uid<<" succeed"<<END;
	}
}
