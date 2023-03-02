#include "pch.h"
#include "rolereadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "chatdeltask.h"
#include "timeutil.h"

CChatDelTask::CChatDelTask()
:CMysqlTask("ChatDelTask")
,m_dwChannel(0)
,m_dwStoreKey(0)
{
}

CChatDelTask::~CChatDelTask()
{ 
}

void CChatDelTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	
	std::stringstream ss;
	ss << "delete from chatrecord where `storekey`="  << m_dwStoreKey << " and `channel`=" << m_dwChannel;

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}
	
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CChatDelTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("delete chatrecord failed, %u, %llu, reason: %s", m_dwChannel, m_dwStoreKey, GetErrStream().str().c_str());
		return; 
	}
}
