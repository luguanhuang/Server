#ifndef __NOTICE_TASK_H__
#define __NOTICE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CNoticeTask : public CMysqlTask
{
public:
	CNoticeTask();
	virtual ~CNoticeTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::PlatNotice m_oData;
};
#endif