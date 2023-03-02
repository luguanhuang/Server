#ifndef _PARTNER_UPDATE_TASK_H_
#define _PARTNER_UPDATE_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class PartnerUpdateTask : public CMysqlTask
{
public:
	PartnerUpdateTask(); 
	virtual ~PartnerUpdateTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32	m_rpcId;
	UINT64	m_partnerId;
	KKSG::PartnerSaveData  m_data;
};

#endif