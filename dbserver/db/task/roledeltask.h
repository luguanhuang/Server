#ifndef _ROLE_DEL_TASK_H_
#define _ROLE_DEL_TASK_H_

#include "mysqltask.h"

class CRoleDelTask : public CMysqlTask
{
public:
	CRoleDelTask(); 
	virtual ~CRoleDelTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	std::string	m_strAccount;
	UINT64		m_roleId;
	UINT32		m_slot;

private:
	bool ReadColumns(IMysqlRecordSet* poRes);
};
#endif