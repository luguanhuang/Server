#ifndef __CROSS_ROLESUMMARRYTASK_H__
#define __CROSS_ROLESUMMARRYTASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CrossRoleSummaryTask : public CMysqlTask
{
public:
	CrossRoleSummaryTask(); 
	virtual ~CrossRoleSummaryTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64				m_roleid;
	KKSG::RoleSummaryStored m_info;
};

#endif
