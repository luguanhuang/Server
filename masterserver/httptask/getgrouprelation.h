#ifndef _GET_GROUP_INFO_TASK_H_
#define _GET_GROUP_INFO_TASK_H_

#include "qqgrouptaskbase.h"

class GetGroupRelationTask : public QQGroupTaskBase
{
public:
	GetGroupRelationTask();
	virtual ~GetGroupRelationTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
};

#endif