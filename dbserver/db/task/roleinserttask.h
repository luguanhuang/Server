#ifndef _Role_Insert_Task_H__
#define _Role_Insert_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CRoleInsertTask : public CMysqlTask
{
public:
	CRoleInsertTask(); 
	virtual ~CRoleInsertTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	KKSG::RoleAllInfo	m_oRoleInfo;
};

#endif