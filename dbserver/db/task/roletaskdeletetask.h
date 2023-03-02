#ifndef _ROLE_TASK_DEL_TASK_H_
#define _ROLE_TASK_DEL_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class RoleTaskDelTask : public CMysqlTask
{
public:
	RoleTaskDelTask(); 
	virtual ~RoleTaskDelTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_roleId;
};

#endif
