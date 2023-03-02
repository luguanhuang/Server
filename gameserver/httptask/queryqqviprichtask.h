#ifndef _QUERY_QQ_VIP_RICH_TASK_H_
#define _QUERY_QQ_VIP_RICH_TASK_H_

#include "WebFetchThread.h"
#include "pb/enum.pb.h"
#include <rapidjson/document.h>

// 只能qq用户查询
class QueryQQVipRichTask : public IWebFetchTask
{
public:
	QueryQQVipRichTask();
	virtual ~QueryQQVipRichTask();

	virtual const std::string GetUrl();
	virtual void OnResult(const std::string &response);
	virtual void OnFailed();

	void SetRpcID(UINT32 dwRpcID) { m_dwRpcID = dwRpcID; }
	void SetToken(const std::string &token) { m_token = token; }
	void SetOpenId(const std::string &openid) { m_openid = openid; }
	void SetRoleId(UINT64 roleId) {m_roleId = roleId;}

	void BuildPostData();

private:
	std::string GetSign(UINT32 now, const std::string& key);
	int GetIntFromString(const rapidjson::Value& json, const char* key);

private:
	UINT32		m_dwRpcID;
	std::string m_token;
	std::string	m_openid;
	UINT64		m_roleId;
};

#endif