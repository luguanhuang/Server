#ifndef _FRIENDWRITETASK_H__
#define _FRIENDWRITETASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class FriendInsertTask : public CMysqlTask
{
public:
	FriendInsertTask(); 
	virtual ~FriendInsertTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64				m_roleid;
	std::string			m_name;
	KKSG::Friend2Db		m_info;
};

#endif