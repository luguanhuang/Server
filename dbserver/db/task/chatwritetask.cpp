#include "pch.h"
#include "rolereadtask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "chatwritetask.h"
#include "timeutil.h"

CChatWriteTask::CChatWriteTask()
:CMysqlTask("ChatWriteTask")
,m_dwChannel(0)
,m_dwStoreKey(0)
{
}

CChatWriteTask::~CChatWriteTask()
{ 
}

void CChatWriteTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	
	std::stringstream ss;
	ss << "select `_id` from chatrecord where channel = " << m_dwChannel << " and storekey = " << m_dwStoreKey;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	ss.str("");

	if(-1 == poMysqlConn->EscapeString(m_strChatList.c_str(), (INT32)m_strChatList.size(), pcBuffer, nLength)) 
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		poRes->Release();
		return;
	}

	if(0 == poRes->GetRowNum())
	{
		ss << "insert into chatrecord(`channel`, `storekey`, `chatinfolist`) values(" << m_dwChannel << ", " << m_dwStoreKey << ", '" << pcBuffer << "')";
	}
	else
	{
		ss << "update chatrecord set chatinfolist = '" << pcBuffer << "' where channel = " << m_dwChannel << " and storekey = " << m_dwStoreKey;
	}
	poRes->Release();

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;	
	}
	
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CChatWriteTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("INSERT chat data task failed, channel:%d, storeKeyID: %llu, errMsg: %s", m_dwChannel, m_dwStoreKey, GetErrStream().str().c_str());
		return; 
	}
}