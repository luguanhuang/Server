#ifndef _UPDATE_SERVER_TASK_H_
#define _UPDATE_SERVER_TASK_H_

#include "SQLTask.h"
#include "gateinfo.h"

class UpdateGateInfoTask : public ISQLTask
{
public:
	UpdateGateInfoTask();
	~UpdateGateInfoTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

public:
	INT32 m_serverId;
	INT32 m_registerAccount;
	INT32 m_onlineRole;
	INT32 m_fullRegisterTime;
};

#endif
