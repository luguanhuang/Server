#ifndef _H_Read_Role_Mail_Task_H__
#define _H_Read_Role_Mail_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CReadRoleMailTask : public CMysqlTask
{
public:
	CReadRoleMailTask(); 
	virtual ~CReadRoleMailTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT64						m_qwRoleID;
	UINT32						m_dwRpcDelayed;
	UINT32						m_dwTimeNow;
	std::map<UINT64, KKSG::SMail>	m_oMailMap;
};

#endif