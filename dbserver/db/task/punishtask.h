#ifndef __PUNISHTASK_
#define __PUNISHTASK_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CPunishTask : public CMysqlTask
{
public:
	CPunishTask();
	virtual ~CPunishTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::PunishData m_oData;
};
#endif
