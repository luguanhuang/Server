#ifndef _SAVE_CHARGE_BACK_ROLE_H_
#define _SAVE_CHARGE_BACK_ROLE_H_

#include "SQLTask.h"
#include "tokenverifymgr.h"

class SaveChargeBackRole : public ISQLTask
{
public:
	SaveChargeBackRole();
	~SaveChargeBackRole();

public:
	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

public:
	UINT32 m_rpcid;
	UINT64	m_qwBeginTime;
	std::string m_account;
	int m_serverID;
	UINT64 m_roleID;
	INT32 m_payCnt;
};

#endif