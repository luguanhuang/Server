#ifndef __DRAGON_GUILD_INSERT_TASK_H_
#define __DRAGON_GUILD_INSERT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class DGuildInsertTask : public CMysqlTask
{
public:
	DGuildInsertTask();
	virtual ~DGuildInsertTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void OnReply();

public:
	KKSG::DragonGuildSaveData m_dguildSaveData;
};

#endif