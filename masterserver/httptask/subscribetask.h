#ifndef __SUBSCRIBE_TASK_H__
#define __SUBSCRIBE_TASK_H__

#include "WebFetchThread.h"
#include "pb/project.pb.h"

class SubscribeTask : public IWebFetchTask
{
public:
	SubscribeTask();
	virtual ~SubscribeTask();

	virtual const std::string GetUrl();
	void SetRpcId(UINT32 id) { m_rpcId = id; }
	void SetToken(std::string token) { m_Token = token; }
	void SetOpenId(std::string openid) { m_strOpenId = openid; }
	std::string MakeSign(UINT32 ts, std::string key);

protected:
	UINT32 m_rpcId;
	std::string m_Token;
	std::string m_strAppid;
	std::string m_strOpenId;
	std::string m_strUri;
	std::string m_strAppKey;
};
#endif
