#include "pch.h"
#include "reloadmgr.h"
#include "util.h"
#include "config.h"
#include "gatemgr/gatemgr.h"
#include "iplist/whitelistmgr.h"
#include "gmmgr/gmmgr.h"


INSTANCE_SINGLETON(CReloadMgr)


CReloadMgr::CReloadMgr()
{
}

CReloadMgr::~CReloadMgr()
{
}

bool CReloadMgr::Init()
{
	m_oFuncMap["gatelist"] = &CReloadMgr::_ReloadGateList;
	m_oFuncMap["whitelist"] = &CReloadMgr::_ReloadWhiteList;
	m_oFuncMap["iosauditversion"] = &CReloadMgr::_ReloadIosAuditVersion;
	m_oFuncMap["gmaccount"] = &CReloadMgr::_ReloadGMAccountList;
	m_oFuncMap["freeflow"] = &CReloadMgr::_ReloadFreeFlow;
	m_oFuncMap["backflow"] = &CReloadMgr::_ReloadBackFlow;


	return true;
}

void CReloadMgr::Uninit()
{
}

bool CReloadMgr::Reload(const std::string& strParam, std::string& strErrMsg)
{
	CReloadFuncMap::iterator it = m_oFuncMap.find(strParam);
	if(it == m_oFuncMap.end())
	{
		strErrMsg = "[Login] Invalid reload file: " + strParam + "\n";
		LogWarn("%s", strErrMsg.c_str());
		return false;
	}

	if((this->*(it->second))())
	{
		strErrMsg = "[Login] Reload " + strParam + " succeed\n";
		LogInfo("%s", strErrMsg.c_str());
		return true;
	}
	else
	{
		strErrMsg = "[Login] Reload " + strParam + " failed\n";
		LogWarn("%s", strErrMsg.c_str());
		return false;
	}
}

bool CReloadMgr::_ReloadGateList()
{
	GateMgr::Instance()->ReloadFromDB();
	return true;
}

bool CReloadMgr::_ReloadWhiteList()
{
	WhiteListMgr::Instance()->Reload();
	return true;
}

bool CReloadMgr::_ReloadGMAccountList()
{
	return GMMgr::Instance()->Reload();
}

bool CReloadMgr::_ReloadIosAuditVersion()
{
	LoginConfig::Instance()->LoadIosAuditVersionFile();
	LogInfo("New ios audit version: [%s]", LoginConfig::Instance()->GetIosAuditVersion().c_str());
	return true;
}

bool CReloadMgr::_ReloadFreeFlow()
{
	LoginConfig::Instance()->LoadUseFreeFlow();
	return true;
}

bool CReloadMgr::_ReloadBackFlow()
{
	LoginConfig::Instance()->LoadBackFlow();
	return true;
}
