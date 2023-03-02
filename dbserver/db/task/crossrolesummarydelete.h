#ifndef _CROSS_ROLESUMMARY_DELETE_H_
#define _CROSS_ROLESUMMARY_DELETE_H_

#include "mysqltask.h"
#include "pb/project.pb.h"

class CrossRoleSummaryDelete : public CMysqlTask
{
public:
	CrossRoleSummaryDelete(); 
	virtual ~CrossRoleSummaryDelete();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();
};

#endif