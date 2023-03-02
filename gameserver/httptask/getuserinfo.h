#ifndef __GETUSERINFO_H__
#define __GETUSERINFO_H__


#include "WebFetchThread.h"
#include "unit/role.h"

class GetUserInfoTask : public IWebFetchTask
{
public:
	GetUserInfoTask();
	virtual ~GetUserInfoTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(Role* role);
};
#endif

