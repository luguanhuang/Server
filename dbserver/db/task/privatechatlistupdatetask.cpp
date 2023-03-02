#include "pch.h"
#include "privatechatlistupdatetask.h"

CPrivateChatListUpdateTask::CPrivateChatListUpdateTask()
:CMysqlTask("PrivateChatListUpdateTask")
{
	m_dwRoleID = 0;
}

CPrivateChatListUpdateTask::~CPrivateChatListUpdateTask()
{
	
}

void CPrivateChatListUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;	

	if(poMysqlConn->EscapeString(m_privateChatList.c_str(), (INT32)m_privateChatList.size(), pcBuffer, nLength) == -1)
	{
		return;
	}

	ss << "insert into privatechatlist(`_id`, `privatechatlist`) values(" << m_dwRoleID << ",'" << pcBuffer << "') on duplicate key update `privatechatlist` = '";
	ss << pcBuffer << "'";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CPrivateChatListUpdateTask::OnReply()
{

}