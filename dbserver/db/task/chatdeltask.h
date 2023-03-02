#ifndef _CHAT_DEL_Task_H__
#define _CHAT_DEL_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CChatDelTask : public CMysqlTask
{
public:
	CChatDelTask(); 
	virtual ~CChatDelTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwChannel;
	UINT64				m_dwStoreKey;
};

#endif
