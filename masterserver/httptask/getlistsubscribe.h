#ifndef __GETLIST_SUBSCRIBE_H__
#define __GETLIST_SUBSCRIBE_H__

#include "subscribetask.h"

class CGetListSubscribeTask : public SubscribeTask
{
public:
	CGetListSubscribeTask();
	virtual ~CGetListSubscribeTask();

	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData();
};
#endif