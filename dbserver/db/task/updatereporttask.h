#ifndef _UPDATE_REPORT_TASK_H_
#define _UPDATE_REPORT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class UpdateReportTask : public CMysqlTask
{
public:
	UpdateReportTask(); 
	virtual ~UpdateReportTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32 m_rpcId;
	UINT64 m_roleId;
	KKSG::RoleReportData m_data;
};

#endif
