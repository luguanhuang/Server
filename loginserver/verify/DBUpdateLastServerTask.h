#ifndef __DBUPDATELASTSERVERTASK_H__
#define __DBUPDATELASTSERVERTASK_H__

#include "SQLTask.h"

class DBUpdateLastServerTask : public ISQLTask
{
public:
	DBUpdateLastServerTask(const std::string &UserID, int ServerID);
	~DBUpdateLastServerTask();

	virtual int Execute(MYSQL *mysql);

	virtual void FinishTask();

private:
	std::string m_UserID;
	unsigned short m_ServerID;
};

#endif // __DBUPDATELASTSERVERTASK_H__