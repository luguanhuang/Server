#ifndef _HOMEGARDEN_INSERT_TASK_H__
#define _HOMEGARDEN_INSERT_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class GardenInfoInsertTask : public CMysqlTask
{
public:
	GardenInfoInsertTask(); 
	virtual ~GardenInfoInsertTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::GardenInfo garden_info_;
};

#endif // _HOMEGARDEN_INSERT_TASK_H__
