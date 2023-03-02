#ifndef __PAYSEND_H__
#define __PAYSEND_H__

#include "WebFetchThread.h"

class PaySendTask : public IWebFetchTask
{
public:
	PaySendTask();
	virtual ~PaySendTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void BuildData(INT32 iAmt, std::string billno);
	void SetRoleId(UINT64 roleid) { m_RoleId = roleid; }
	void SetParamData(KKSG::PayParameterInfo oData);
	std::map<std::string, std::string> m_oparams;

private:
	std::string strBillNo;
	UINT64 m_RoleId;
};
#endif
