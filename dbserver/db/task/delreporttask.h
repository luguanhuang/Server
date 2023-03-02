#ifndef _DEL_REPORT_TASK_H_
#define _DEL_REPORT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class DelReportTask : public CMysqlTask
{
public:
	DelReportTask(); 
	virtual ~DelReportTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32 m_rpcId;
	UINT64 m_roleId;
	KKSG::RoleReportData m_data;
};

#endif
