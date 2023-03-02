#ifndef _SKY_TEAM_INSERT_TASK_H_
#define _SKY_TEAM_INSERT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class SkyTeamInsertTask : public CMysqlTask
{
public:
	SkyTeamInsertTask();	
	virtual ~SkyTeamInsertTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_id;
	KKSG::SkyTeamSaveData m_data;
};

#endif
