#ifndef __GETCONF_H__
#define __GETCONF_H__

#include "WebFetchThread.h"
#include "unit/role.h"

class GetConfTask : public IWebFetchTask
{
public:
	GetConfTask();
	virtual ~GetConfTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData();
};
#endif
