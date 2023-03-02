#ifndef __REPAIRORDER_H__
#define __REPAIRORDER_H__

#include "WebFetchThread.h"

class RepairOrderTask : public IWebFetchTask
{
public:
	RepairOrderTask();
	virtual ~RepairOrderTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
};

#endif