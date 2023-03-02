#ifndef __PUSHSUBSCRIBE_H__
#define __PUSHSUBSCRIBE_H__

#include "subscribetask.h"

class CPushSubscribeTask : public SubscribeTask
{
public:
	CPushSubscribeTask();
	virtual ~CPushSubscribeTask();

	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(UINT32 iMsgId);
	void Print();
};
#endif
