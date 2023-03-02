#ifndef __DB_GETBESPEAK_TASK_H_
#define __DB_GETBESPEAK_TASK_H_

#include "SQLTask.h"
#include "tokenverifymgr.h"

struct AccountBeSpeakInfo
{
	std::vector<UINT32> bespeakServerIds;	
	std::set<UINT32> hasRoleServerIds;
	std::string account;

	bool HasBeSpeak(UINT32 serverId)
	{
		return std::find(bespeakServerIds.begin(), bespeakServerIds.end(), serverId) != bespeakServerIds.end();
	}

	bool HasRole(UINT32 serverId)
	{
		return hasRoleServerIds.find(serverId) != hasRoleServerIds.end();
	}
};

class DBGetBespeakTask : public ISQLTask
{
public:
	DBGetBespeakTask(UINT32 rpcid, const std::string& account);
	~DBGetBespeakTask();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

private:
	UINT32	m_rpcid;
	UINT64	m_qwBeginTime;
	std::string m_account;
	AccountBeSpeakInfo m_info;
};

#endif
