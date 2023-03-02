#ifndef _Mail_OP_Task_H__
#define _Mail_OP_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CMailOpTask : public CMysqlTask
{
public:
	CMailOpTask(); 
	virtual ~CMailOpTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64				m_qwRoleID;
	bool				m_bByTemplate;
	UINT64				m_qwTemplateID;
	KKSG::MailOpType	m_nOP;
	KKSG::SMail			m_oData;
};

#endif