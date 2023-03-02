#ifndef _UNLINK_GROUP_TASK_H_
#define _UNLINK_GROUP_TASK_H_

#include "qqgrouptaskbase.h"

class UnlinkGroupTask : public QQGroupTaskBase
{
public:
	UnlinkGroupTask();
	virtual ~UnlinkGroupTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
};

#endif