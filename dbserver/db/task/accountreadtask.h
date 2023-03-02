#ifndef _Account_Read_Task_H__
#define _Account_Read_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CAccountReadTask : public CMysqlTask
{
public:
	CAccountReadTask(); 
	virtual ~CAccountReadTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32		m_dwRpcDelayed;
	std::string	m_strAccount;
	UINT32		m_serverId;
private:
	bool ReadColumns(IMysqlRecordSet* poRes);

private:
	KKSG::LoadAccountData	m_oData;
};

#endif