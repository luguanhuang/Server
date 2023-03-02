#ifndef __PAYACCESSINFO_TASK_H__
#define __PAYACCESSINFO_TASK_H__

#include "mysqltask.h"

class CPayAccessInfoTask : public CMysqlTask
{
public:
	CPayAccessInfoTask();
	virtual ~CPayAccessInfoTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	KKSG::SavePayAccessArg m_Data;
};

#endif

