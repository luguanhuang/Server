#ifndef __DBGETSELFSERVERTASK_H__
#define __DBGETSELFSERVERTASK_H__

#include "SQLTask.h"
#include "tokenverifymgr.h"

class DBGetSelfServerTask : public ISQLTask
{
public:
	DBGetSelfServerTask(UINT32 rpcid, const std::string& account, const std::string& phone, UINT32 ip);
	~DBGetSelfServerTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

private:
	UINT32	m_rpcid;
	UINT64	m_qwBeginTime;
	UINT32	m_ip;
	std::string m_account;
	UserGateInfo m_info;
	UserInfo m_userInfo;
};

#endif