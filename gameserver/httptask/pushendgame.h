#ifndef __REPORTDATA_H__
#define	__REPORTDATA_H__

#include "WebFetchThread.h"
#include "unit/role.h"
class PushEndGameTask : public IWebFetchTask
{
public:
	PushEndGameTask();
	virtual ~PushEndGameTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildPostData(Role* role);
	void SetPlatID(INT32 iPlatID);
	void SetServerID(UINT32 uServerID);

private:
	INT32 m_iPlatID;
	UINT32 m_uServerID;
};

#endif
