#ifndef __PRIVATECHALISTUPDATETASK_H__
#define __PRIVATECHALISTUPDATETASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CPrivateChatListUpdateTask : public CMysqlTask
{
public:
	CPrivateChatListUpdateTask();
	~CPrivateChatListUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64 m_dwRoleID;
	std::string m_privateChatList;
};

#endif