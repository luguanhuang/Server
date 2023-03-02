#ifndef _ReadHiddenRoles_Task_H__
#define _ReadHiddenRoles_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class ReadHiddenRolesTask : public CMysqlTask
{
public:
	ReadHiddenRolesTask(); 
	virtual ~ReadHiddenRolesTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32 m_slot;
	UINT32 m_serverid;
	std::string	m_strAccount;

	UINT32 m_dwRpcDelayed;

	KKSG::ReadHiddenRolesRes m_data;

private:
	bool ReadColumns(IMysqlRecordSet* poRes);
};

#endif