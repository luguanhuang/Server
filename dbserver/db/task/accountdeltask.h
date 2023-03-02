#ifndef _Account_Del_Task_H__
#define _Account_Del_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CAccountDelTask : public CMysqlTask
{
public:
	CAccountDelTask(); 
	virtual ~CAccountDelTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

	bool IsEmpty(IMysqlRecordSet* poRes);

public:
	std::string	m_strAccount;
	UINT32 m_serverId;
};

#endif