#ifndef _GLOBAL_UPDATE_TASK_H__
#define _GLOBAL_UPDATE_TASK_H__


#include "mysqltask.h"
#include "pb/project.pb.h"

class CGlobalUpdateTask : public CMysqlTask
{
public:
	CGlobalUpdateTask();
	virtual ~CGlobalUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	
	UINT32				m_uID;
	std::string			m_strData;	
};
#endif
