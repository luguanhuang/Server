#ifndef _LEAGUE_TEAM_UPDATE_TASK_H_
#define _LEAGUE_TEAM_UPDATE_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class LeagueTeamUpdateTask : public CMysqlTask
{
public:
	LeagueTeamUpdateTask(); 
	virtual ~LeagueTeamUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_id;
	KKSG::LeagueTeamSaveData m_data;
};

#endif
