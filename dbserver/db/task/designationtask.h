#ifndef _DESIGNATION_Task_H__
#define _DESIGNATION_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class DesignationTask : public CMysqlTask
{
public:
	DesignationTask(); 
	virtual ~DesignationTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	UINT64				m_qwRoleID;
	KKSG::AllDesignationEvent m_Data;
};

#endif