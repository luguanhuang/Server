#ifndef _ROLE_TASK_UPDATE_TASK_H_
#define _ROLE_TASK_UPDATE_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class RoleTaskUpdateTask : public CMysqlTask
{
public:
	RoleTaskUpdateTask(); 
	virtual ~RoleTaskUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_roleId;
	KKSG::RoleTaskSaveData m_data;
};

#endif
