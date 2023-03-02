#ifndef __DRAGON_GUILD_UPDATE_TASK_H__
#define __DRAGON_GUILD_UPDATE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class DGuildUpdateTask : public CMysqlTask
{
public:
	DGuildUpdateTask();
	virtual ~DGuildUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void OnReply();

public:
	KKSG::DragonGuildSaveData m_dguildSaveData;
};

#endif