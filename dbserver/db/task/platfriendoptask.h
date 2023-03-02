#ifndef _PLATFRIEND_OP_TASK_H__
#define _PLATFRIEND_OP_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CPlatFriendOpTask : public CMysqlTask
{
public:
	CPlatFriendOpTask(); 
	virtual ~CPlatFriendOpTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	std::string m_oData;
	std::string m_openid;
};

#endif