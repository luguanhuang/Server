#ifndef __MOBAREADTASK_H__
#define __MOBAREADTASK_H__


#include "mysqltask.h"
#include "pb/project.pb.h"


class MobaReadTask : public CMysqlTask
{
public:
	MobaReadTask(); 
	virtual ~MobaReadTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	virtual void OnReply();

public:
	UINT64 m_qwRoleID;
	UINT32 m_dwRpcDelayed;
	KKSG::QueryMobaRoleRecordRes m_oRes;
};




#endif