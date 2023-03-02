#ifndef __DBCHECKPASSWORDTASK_H__
#define __DBCHECKPASSWORDTASK_H__

#include "SQLTask.h"

class DBCheckPasswordTask : public ISQLTask
{
public:
	DBCheckPasswordTask();
	~DBCheckPasswordTask();

	virtual int  Execute(MYSQL *mysql);
	virtual void FinishTask();

	inline void SetAccount(const std::string& userid) { m_userid = userid; }
	inline void SetPassword(const std::string& password) { m_password = password; }
	inline void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }
	inline void SetIP(UINT32 ip) { m_ip = ip; }

private:
	UINT32 m_ip;
	UINT32 m_dwRpcID;
	std::string m_userid;
	std::string m_password;
	std::string m_dbpassword;
};

#endif // __DBCHECKPASSWORDTASK_H__
