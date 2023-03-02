#ifndef _PARTNER_INSERT_TASK_H_
#define _PARTNER_INSERT_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class PartnerInsertTask : public CMysqlTask
{
public:
	PartnerInsertTask(); 
	virtual ~PartnerInsertTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_partnerId;
	KKSG::PartnerSaveData  m_data;
};

#endif