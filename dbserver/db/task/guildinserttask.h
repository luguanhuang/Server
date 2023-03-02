#ifndef _GUILD_INSERT_TASK_H__
#define _GUILD_INSERT_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CGuildInsertTask : public CMysqlTask
{
public:
	CGuildInsertTask(); 
	virtual ~CGuildInsertTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::GuildAllDBData m_guildAllData;
};

#endif
