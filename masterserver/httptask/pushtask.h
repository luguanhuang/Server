#ifndef _PUSH_TASK_H_
#define _PUSH_TASK_H_

#include <map>

#include "WebFetchThread.h"
#include "rapidjson/document.h"
#include "pb/enum.pb.h"

struct PushMessage
{
	std::string token;				// token(openId ���� �豸token)
	std::string title;
	std::string content;

	std::string toJson(UINT32 platType);
	std::string toJsonAndroid();
	std::string toJsonIOS();
};

class PushTask : public IWebFetchTask
{
public:
	enum PushType
	{
		PushType_Account = 1,			// �����ʺ�����
		PushType_DeviceToken = 2,		// �����豸����
	};

	const static std::string g_sHost;
	const static std::string g_sMethod;

	const static std::string API_SINGLE_ACCOUNT;
	const static std::string API_SINGLE_DEVICE;
	const static std::string API_CREATE_MULTIPUSH;
	const static std::string API_MULTI_DEVICE;
	const static std::string API_MULTI_ACCOUNT;

	PushTask();
	virtual ~PushTask();

	virtual const std::string GetUrl() = 0;
	virtual void OnResult(const std::string &response) = 0;
	virtual void OnFailed() = 0;

	void SetParam(const std::string& sKey, const std::string& sValue);

	void SetEnvType(UINT32 type) {
		m_envType = type;
	}
	void SetPushType(UINT32 type) {
		m_pushType = type;
	}
	void SetPlatType(UINT32 type) {
		m_platType = type;
	}

protected:
	void AddCommParams();
	std::string GenerateUrl(const std::string& sApi);
	bool IsSuccess(rapidjson::Value& json);

protected:
	UINT32 m_envType;				// ����(1 �������� 2 ������������ios��Ч)
	UINT32 m_pushType;				// ��������(�豸���� �ʺ�����)
	UINT32 m_platType;				// ƽ̨����(IOS Android)
	std::map<std::string, std::string> m_params;
};

#endif