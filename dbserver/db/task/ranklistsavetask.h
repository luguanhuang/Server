#ifndef __RANKLISTTASK_H__
#define __RANKLISTTASK_H__

#include "mysqltask.h"

class RankListSaveTask : public CMysqlTask
{
public:
	RankListSaveTask();
	virtual ~RankListSaveTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	UINT32 m_dwType;
	UINT32 m_dwServerID;
	std::string m_strRankData;
};

#endif