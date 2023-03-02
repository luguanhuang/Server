#ifndef _Social_OP_Task_H__
#define _Social_OP_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CSocialOpTask : public CMysqlTask
{
public:
	CSocialOpTask(); 
	virtual ~CSocialOpTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::SocialInfo2Db m_oData;
};

#endif