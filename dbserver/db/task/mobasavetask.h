#ifndef __MOBASAVETASK_H__
#define __MOBASAVETASK_H__


#include "mysqltask.h"
#include "pb/project.pb.h"


class MobaSaveTask : public CMysqlTask
{
public:
	MobaSaveTask(); 
	virtual ~MobaSaveTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void OnReply();

public:	
	UINT64 m_qwRoleID;	
	KKSG::MobaBattleRoleRecord m_oData;
};


#endif