#ifndef _LEAGUE_TEAM_INSERT_TASK_H_
#define _LEAGUE_TEAM_INSERT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class LeagueTeamInsertTask : public CMysqlTask
{
public:
	LeagueTeamInsertTask(); 
	virtual ~LeagueTeamInsertTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_id;
	KKSG::LeagueTeamSaveData m_data;
};

#endif
