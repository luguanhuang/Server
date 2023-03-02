#ifndef __DBUPDATESELFSERVERTASK_H__
#define __DBUPDATESELFSERVERTASK_H__

#include "SQLTask.h"

class DBUpdateSelfServerTask : public ISQLTask
{
public:
	DBUpdateSelfServerTask(const std::string &UserID, int ServerID, int level);
	~DBUpdateSelfServerTask();

	virtual int Execute(MYSQL *mysql);

	virtual void FinishTask();

private:
	std::string m_UserID;
	UINT32 m_ServerID;
	int m_level;
};

#endif // __DBUPDATESELFSERVERTASK_H__