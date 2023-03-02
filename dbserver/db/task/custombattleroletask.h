#ifndef _CUSTOMBATTLETASK_H__
#define _CUSTOMBATTLETASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CustomBattleRoleTask : public CMysqlTask
{
public:
	CustomBattleRoleTask(); 
	virtual ~CustomBattleRoleTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64 m_roleid;
	KKSG::CustomBattleRole m_custom;
	KKSG::CustombattleRoleSystem m_system;
};

#endif