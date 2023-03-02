#ifndef _CREATE_BIND_TASK_H_
#define _CREATE_BIND_TASK_H_

#include "qqgrouptaskbase.h"

class CreateBindTask : public QQGroupTaskBase
{
public:
	CreateBindTask();
	virtual ~CreateBindTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
};

#endif