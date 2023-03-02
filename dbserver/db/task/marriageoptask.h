#ifndef _MARRIAGE_OP_TASK_H__
#define _MARRIAGE_OP_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CMarriageOpTask : public CMysqlTask
{
public:
	CMarriageOpTask(); 
	virtual ~CMarriageOpTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	std::string m_oData;
	UINT64 m_roleID;
};

#endif