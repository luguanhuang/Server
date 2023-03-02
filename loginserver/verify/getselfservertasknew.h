#ifndef _GET_SELFSERVER_TASK_NEW_H_
#define _GET_SELFSERVER_TASK_NEW_H_

#include "SQLTask.h"

class GetSelfServerTaskNew : public ISQLTask
{
public:
	GetSelfServerTaskNew(UINT32 rpcid, const std::string& account);
	virtual ~GetSelfServerTaskNew() {}

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

private:
	UINT32 m_rpcid;
	UINT64 m_qwBeginTime;
	std::string m_account;
	std::set<UINT32> m_serverIds;
	UINT64 m_bindRoleId;
	INT32 m_bindPay;
};

#endif