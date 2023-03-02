#ifndef _PARTNER_REPLACE_ID_TASK_H_
#define _PARTNER_REPLACE_ID_TASK_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class PartnerReplaceIdTask : public CMysqlTask
{
public:
	PartnerReplaceIdTask(); 
	virtual ~PartnerReplaceIdTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32 m_rpcId;
	UINT64 m_partnerId;
	UINT64 m_oldId;
};

#endif
