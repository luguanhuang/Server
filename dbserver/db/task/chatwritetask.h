#ifndef _CHAT_WRITE_Task_H__
#define _CHAT_WRITE_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CChatWriteTask : public CMysqlTask
{
public:
	CChatWriteTask(); 
	virtual ~CChatWriteTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwChannel;
	UINT64				m_dwStoreKey;
	std::string			m_strChatList;
};

#endif