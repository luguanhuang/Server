#ifndef _SINGLE_PUSH_TASK_H_
#define _SINGLE_PUSH_TASK_H_

#include "pushtask.h"

class SinglePushTask : public PushTask
{
public:
	SinglePushTask() {}
	virtual ~SinglePushTask() {}

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	PushMessage& GetMessage()
	{
		return m_message;
	}

protected:
	PushMessage m_message;
};

#endif