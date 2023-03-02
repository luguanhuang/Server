#ifndef _JOIN_GROUP_TASK_H_
#define _JOIN_GROUP_TASK_H_

#include "qqgrouptaskbase.h"

class JoinGroupTask : public QQGroupTaskBase
{
public:
	JoinGroupTask();
	virtual ~JoinGroupTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

private:
	INT32 m_platCode;
};

#endif