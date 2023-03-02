#ifndef __DRAGON_GUILD_DELETE_TASK_H__
#define __DRAGON_GUILD_DELETE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class DGuildDeleteTask : public CMysqlTask
{
public:
	DGuildDeleteTask();
	virtual ~DGuildDeleteTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void OnReply();

public:
	KKSG::DragonGuildSaveData m_dguildSaveData;
};

#endif