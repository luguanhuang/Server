#ifndef __AUCTSALEITEMTASK_H__
#define __AUCTSALEITEMTASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CSaleItemTask : public CMysqlTask
{
public:
	CSaleItemTask(const KKSG::AuctDBItemDale& data);
	virtual ~CSaleItemTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

private:
	bool FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	KKSG::AuctDBItemDale m_data;
};

#endif