#ifndef _FREE_FLOW_TASK_H_
#define _FREE_FLOW_TASK_H_

#include "WebFetchThread.h"
#include "pb/enum.pb.h"


class FreeFlowTask : public IWebFetchTask
{
public:
	FreeFlowTask();
	virtual ~FreeFlowTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetIP(UINT32 ip) { m_ip = ip; }
	void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }
	void SetOpenId(const std::string &openid) { m_openid = openid; }
	void SetLoginType(KKSG::LoginType type) { m_loginType = type; }

	void BuildPostData();

private:
	std::string GetSign(UINT32 now, const std::string& key);

private:
	UINT32		m_ip;
	UINT32		m_dwRpcID;
	UINT64		m_qwBeginTime;
	KKSG::LoginType m_loginType;
	std::string	m_openid;
	bool		m_isFree;
	int			m_ccType;
	std::string m_debugUrl;
};



#endif