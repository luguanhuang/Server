#ifndef _SKY_TEAM_DEL_TASK_H_
#define _SKY_TEAM_DEL_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class SkyTeamDelTask : public CMysqlTask
{
public:
	SkyTeamDelTask(); 
	virtual ~SkyTeamDelTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	bool m_isDeleteAll;
	UINT32	m_rpcId;
	UINT64	m_id;
};

#endif
