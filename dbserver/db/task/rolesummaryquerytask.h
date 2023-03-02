#ifndef _ROLESUMMARYQUERY_H_
#define _ROLESUMMARYQUERY_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CRolesummaryQueryTask : public CMysqlTask
{
public:
	CRolesummaryQueryTask(); 
	virtual ~CRolesummaryQueryTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_dwRpcDelayed;
	UINT64	m_qwRoleID;
	std::string m_strData;
	KKSG::RoleSummaryStored m_oData;
};

#endif