#ifndef _H_TX_LOGINTASK_H__
#define _H_TX_LOGINTASK_H__

#include "WebFetchThread.h"
#include "pb/enum.pb.h"


class TXLoginTask : public IWebFetchTask
{
public:
	TXLoginTask();
	virtual ~TXLoginTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetIP(UINT32 ip) { m_ip = ip; }
	void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }

	void SetToken(const std::string &token) { m_token = token; }
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
	std::string m_token;
	std::string	m_openid;
};

#endif // __SNDALOGINTASK_H__