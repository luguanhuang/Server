#ifndef _GUILD_DELETE_TASK_H__
#define _GUILD_DELETE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CGuildDeleteTask : public CMysqlTask
{
public:
	CGuildDeleteTask(); 
	virtual ~CGuildDeleteTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::GuildAllDBData m_guildAllData;
};
 

#endif
