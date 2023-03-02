#ifndef _TemplateMail_Insert_Task_H__
#define _TemplateMail_Insert_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CTemplateMailInsertTask : public CMysqlTask
{
public:
	CTemplateMailInsertTask(); 
	virtual ~CTemplateMailInsertTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::SMail		m_oData;
};

#endif