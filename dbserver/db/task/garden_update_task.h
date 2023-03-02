#ifndef _GARDEN_UPDATE_TASK_H__
#define _GARDEN_UPDATE_TASK_H__

#include "mysqltask.h"
#include "pb/project.pb.h"


class GardenUpdateTask : public CMysqlTask
{
public:
	GardenUpdateTask(); 
	virtual ~GardenUpdateTask();


	virtual void Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength); 
	virtual void OnReply();

public:
	KKSG::GardenInfo garden_info_;
};


#endif
