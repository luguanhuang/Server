#ifndef _QQ_GROUP_TASK_BASE_H_
#define _QQ_GROUP_TASK_BASE_H_


#include <map>

#include "WebFetchThread.h"
#include "rapidjson/document.h"
#include "pb/enum.pb.h"
#include "util/jsonutil.h"

class QQGroupTaskBase : public IWebFetchTask
{
public:
	QQGroupTaskBase();
	virtual ~QQGroupTaskBase();

	virtual const std::string GetUrl() = 0;
	virtual void OnResult(const std::string &response) = 0;
	virtual void OnFailed() = 0;
	INT32 GetRetCode(rapidjson::Value& json);

	void SetRpcId(UINT32 id) { m_rpcId = id; }
	void SetOpenId(const std::string& openId) { m_openId = openId; }
	void SetToken(const std::string& token) { m_token = token; }

	void SetGuild(UINT64 id, const std::string& name, UINT32 serverId);
	void SetRoleId(UINT64 id);
	void SetGroupCode(const std::string& code) { m_groupCode = code; }
	void SetPlatType(UINT32 platType) { m_platType = platType; }

	void AddParam(const std::string& key, const std::string& value);
	std::string GetParamJsonString();
	std::string GetSign(UINT32 now, const std::string& key);
	std::string GetUri();
	void BuildPostData();

public:
	UINT32 m_rpcId;
	std::string m_openId;
	std::string m_token;
	UINT32 m_platType;

	UINT64 m_guildId;
	UINT32 m_serverId;
	UINT64 m_roleId;
	std::string m_guildName;
	std::string m_groupCode;

	std::map<std::string, std::string> m_params;
	INT32 m_retCode;
	std::string m_strAppID;
	std::string m_strAppKey;
};

#endif