#ifndef __AUCTTRADERECORD_H__
#define __AUCTTRADERECORD_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CTradeRecordTask : public CMysqlTask
{
public:
	CTradeRecordTask(const KKSG::AuctDBTradeAccount& data);
	virtual ~CTradeRecordTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

private:
	bool FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	KKSG::AuctDBTradeAccount m_data;
};

#endif