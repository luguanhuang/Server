#ifndef _PARTNER_DEL_TASK_H_
#define _PARTNER_DEL_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class PartnerDelTask : public CMysqlTask
{
public:
	PartnerDelTask(); 
	virtual ~PartnerDelTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_partnerId;
};

#endif
