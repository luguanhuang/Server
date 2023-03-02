#ifndef __LOGCONFIG_H__
#define __LOGCONFIG_H__

#include "baseconfig.h"
#include "singleton.h"
#include "DBInfo.h"
#include "pb/enum.pb.h"

class LoginConfig : public Config
{
	LoginConfig();
	~LoginConfig();
	DECLARE_SINGLETON(LoginConfig)

	bool IsUseOpenIDFilter() { return m_UseOpenIDFilter; }

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	const DBInfo &GetDBInfo() { return m_DBInfo; }
	std::string& GetDB() { return m_DBInfo.m_database; }
	const std::string& GetPlatform() { return m_platform; }

	bool IsDebug() const { return m_bDebug; }
	void SetDebug(bool isDebug) { m_bDebug = isDebug; }

	const std::string& GetMSDKUrl() { return m_msdkUrl; }

	UINT32 GetMaxLoginConcurrent() { return m_loginMax; }

	inline UINT32 GetOnlineFullNum() { return m_onLineFullNum; }
	inline UINT32 GetRegisterSmoothNum() { return m_registerSmoothNum; }
	inline UINT32 GetRegisterFullNum() { return m_registerFullNum; }

	const std::string& GetIosAuditVersion() const { return m_strIosAuditVersion; }
	
	void LoadIosAuditVersionFile();
	void LoadUseFreeFlow();
	void LoadBackFlow();

	const std::string& GetRecommendZoneName() { return m_recommendZoneName; }

	bool UseFreeFlow() { return m_freeFlow; }
	bool NeedBackFlowCheck() { return m_backFlowCheck; }
	UINT32 GetAccountBackFlowTime() { return m_accountBackFlowTime; }
	UINT32 GetAccountBackFlowExpireTime() { return m_accountBackFlowExpireTime; }

private:
	std::string m_platform;
	DBInfo	m_DBInfo;

	bool	m_UseOpenIDFilter;
	bool	m_bDebug;
	std::string m_msdkUrl;
	UINT32 m_loginMax;

	UINT32 m_onLineFullNum;
	UINT32 m_registerSmoothNum;
	UINT32 m_registerFullNum;

	//非空则检查IOS审核版本
	std::string m_strIosAuditVersion;
	std::string m_recommendZoneName;
	bool m_freeFlow;
	bool m_backFlowCheck;
	UINT32 m_accountBackFlowTime;
	UINT32 m_accountBackFlowExpireTime;
};

#endif