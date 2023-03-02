#ifndef _READ_ACCOUNT_TASK_NEW_H_
#define _READ_ACCOUNT_TASK_NEW_H_

#include "mysqltask.h"
#include "pb/project.pb.h"


class CReadAccountTaskNew : public CMysqlTask
{
public:
	CReadAccountTaskNew(); 
	virtual ~CReadAccountTaskNew();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32		m_dwRpcDelayed;
	std::string	m_strAccount;
	UINT32		m_serverId;
	std::vector<KKSG::ZoneRoleInfo> m_roles;
private:
	bool ReadColumns(IMysqlRecordSet* poRes);
};

#endif