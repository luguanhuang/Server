#include "pch.h"
#include "reloadmgr.h"
#include "util.h"
#include "config.h"


INSTANCE_SINGLETON(CReloadMgr)


CReloadMgr::CReloadMgr()
{
}

CReloadMgr::~CReloadMgr()
{
}

bool CReloadMgr::Init()
{
	m_oFuncMap["version"] = &CReloadMgr::_ReloadVersionFile;

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
		strErrMsg = "[VersionServer] Invalid reload file: " + strParam + "\n";
		LogWarn("%s", strErrMsg.c_str());
		return false;
	}

	if((this->*(it->second))())
	{
		strErrMsg = "[VersionServer] Reload " + strParam + " succeed\n";
		LogInfo("%s", strErrMsg.c_str());
		return true;
	}
	else
	{
		strErrMsg = "[VersionServer] Reload " + strParam + " failed\n";
		LogWarn("%s", strErrMsg.c_str());
		return false;
	}
}

bool CReloadMgr::_ReloadVersionFile()
{
	VersionConfig::Instance()->LoadVersionFile();
	VersionConfig::Instance()->LoadClientUpdateControlFile();
	VersionConfig::Instance()->LoadExtendInfoFile();
	return true;
}
