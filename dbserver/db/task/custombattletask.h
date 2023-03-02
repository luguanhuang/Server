#ifndef _CUSTOMBATTLETASK_H__
#define _CUSTOMBATTLETASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CustomBattleTask : public CMysqlTask
{
public:
	CustomBattleTask(); 
	virtual ~CustomBattleTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64 m_uid;
	KKSG::CustomBattleData m_data;
};

#endif