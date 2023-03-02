#ifndef __CUSTOMBATTLEDELETETASK_H__
#define __CUSTOMBATTLEDELETETASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CustomBattleDeleteTask : public CMysqlTask
{
public:
	CustomBattleDeleteTask();
	virtual ~CustomBattleDeleteTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64 m_uid;
};

#endif