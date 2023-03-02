#ifndef _QUERY_QQ_FRIEND_VIP_TASK_H_
#define _QUERY_QQ_FRIEND_VIP_TASK_H_

#include "WebFetchThread.h"
#include "pb/enum.pb.h"
#include <rapidjson/document.h>

// 只能qq用户查询
class QueryQQFriendsVipTask : public IWebFetchTask
{
public:
	QueryQQFriendsVipTask();
	virtual ~QueryQQFriendsVipTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }
	void SetToken(const std::string &token) { m_token = token; }

	void SetOpenId(const std::string &openid) { m_openid = openid; }
	void AddFriendOpenId(const std::string& openId)
	{
		m_friendOpenIds.push_back(openId);
	}
	void SetPF(const std::string& pf) { m_pf = pf; }

	void BuildPostData();

private:
	std::string GetSign(UINT32 now, const std::string& key);
	int GetIntFromString(const rapidjson::Value& json, const char* key);

private:
	UINT32		m_dwRpcID;
	std::string m_token;
	std::string	m_openid;
	std::string m_pf;
	std::vector<std::string> m_friendOpenIds;
};

#endif
