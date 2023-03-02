#ifndef _GUILDAUCTTASK_H__
#define _GUILDAUCTTASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"

class CGuildAuctTask : public CMysqlTask
{
public:
	CGuildAuctTask(const KKSG::GuildAuctPara& data);
	virtual ~CGuildAuctTask();

	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

private:
	bool FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength);
	KKSG::GuildAuctPara m_data;
};

#endif