#ifndef __SAVEGIFTIBITEM_TASK_H__
#define __SAVEGIFTIBITEM_TASK_H__

#include "mysqltask.h"

class CGiftIBItemTask : public CMysqlTask
{
public:
	CGiftIBItemTask();
	virtual ~CGiftIBItemTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	KKSG::GiftIbItemArg m_Data;
};

#endif

