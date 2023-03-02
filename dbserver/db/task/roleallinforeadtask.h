#ifndef _UserInfo_Read_Task_H__
#define _UserInfo_Read_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CRoleAllInfoReadTask : public CMysqlTask
{
public:
	CRoleAllInfoReadTask(); 
	virtual ~CRoleAllInfoReadTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	UINT32				m_ServerID;
	std::string			m_szAccount;
private:
	KKSG::GetAllRoleRes m_oData;
};

#endif