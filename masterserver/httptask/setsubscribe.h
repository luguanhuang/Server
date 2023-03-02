#ifndef __SETSUBSCRIBE_H__
#define __SETSUBSCRIBE_H__

#include "subscribetask.h"

class CSetSubscribeTask : public SubscribeTask
{
public:
	CSetSubscribeTask();
	virtual ~CSetSubscribeTask();

	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(const KKSG::SetSubscirbeArg& oData);

};
#endif
