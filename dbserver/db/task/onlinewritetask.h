#ifndef _ONLINE_WRITE_TASK_H__
#define _ONLINE_WRITE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class COnlineWriteTask : public CMysqlTask
{
public:
	COnlineWriteTask();
	virtual ~COnlineWriteTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_uTimeKey;
	UINT32				m_uServerID;
	UINT32				m_uZoneID;
	UINT32				m_uIosNum;
	UINT32				m_uAndroidNum;
	UINT32				m_uAppID;
};
#endif
