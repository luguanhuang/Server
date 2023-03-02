#ifndef __ROLESUMMARRYTASK_H__
#define __ROLESUMMARRYTASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class RolesummaryTask : public CMysqlTask
{
public:
	RolesummaryTask(); 
	virtual ~RolesummaryTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64				m_roleid;
	KKSG::RoleSummaryStored m_info;
};

#endif