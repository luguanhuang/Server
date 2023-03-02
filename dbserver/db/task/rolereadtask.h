#ifndef _Role_Read_Task_H__
#define _Role_Read_Task_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class CRoleReadTask : public CMysqlTask
{
public:
	CRoleReadTask(); 
	virtual ~CRoleReadTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32				m_dwRpcDelayed;
	UINT64				m_qwRoleID;
	INT32				m_nSlot;
	KKSG::RoleDataReadType	m_nReadType;
	KKSG::RoleAllInfo	m_oRoleInfo;
private:
	std::string			m_strData;
};

#endif