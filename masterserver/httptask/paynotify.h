#ifndef __PAYNOTIFY_H__
#define __PAYNOTIFY_H__

#include "WebFetchThread.h"

class PayNotifyTask : public IWebFetchTask
{
public:
	PayNotifyTask();
	virtual ~PayNotifyTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();
	void SetRpcId(UINT32 id) { m_rpcId = id; }
	void SetParamType(KKSG::PayParamType paytype) { m_paytype = paytype; }
	void SetParamId(std::string paramid) { m_paramId = paramid; }
	void SetRoleId(UINT64 roleid) { m_RoleId = roleid; }
	void SetParamData(KKSG::PayParameterInfo oData);
	void OnReply();
	std::map<std::string, std::string> m_oparams;

private:
	UINT32 m_rpcId;
	KKSG::PayParamType m_paytype;
	UINT64 m_RoleId;
	UINT32 m_CostSum;
	std::string m_paramId;
};

#endif
