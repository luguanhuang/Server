#ifndef _QUERY_GARDENINFO_TASK_
#define _QUERY_GARDENINFO_TASK_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CQueryGardenInfoTask : public CMysqlTask
{
public:
	CQueryGardenInfoTask(); 
	virtual ~CQueryGardenInfoTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_dwRpcDelayed;
	UINT64	m_qwRoleID;
	std::string m_strData;
	KKSG::GardenInfo garden_info_;
};

#endif