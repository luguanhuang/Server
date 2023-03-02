#ifndef __PAYLISTTASK_H__
#define __PAYLISTTASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CPayListTask : public CMysqlTask
{
public:
	CPayListTask();
	virtual ~CPayListTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::PayList m_oData;
};
#endif

