#ifndef _ARENASTARTASK_H__
#define _ARENASTARTASK_H__

#include "mysqltask.h"

class CArenaStarTask : public CMysqlTask
{
public:
	CArenaStarTask(const KKSG::ArenaStarRoleData& data);
	virtual ~CArenaStarTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

private:
	bool FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);

	KKSG::ArenaStarRoleData m_data;
};

#endif
