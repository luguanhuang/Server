#ifndef _GUILD_UPDATE_TASK_H__
#define _GUILD_UPDATE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CGuildUpdateTask : public CMysqlTask
{
public:
	CGuildUpdateTask(); 
	virtual ~CGuildUpdateTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::GuildAllDBData m_guildAllData;
};


#endif
