#include "pch.h"
#include "config.h"
#include "clientlink.h"
#include "gmtoollink.h"
#include "util.h"
#include <fstream>

INSTANCE_SINGLETON(VersionConfig)

VersionConfig::VersionConfig()
{
	SetServerTag("VersionServer");

	CClientLink::CreateInstance();
	AddLinkHandler(CClientLink::Instance());

	GMToolLink::CreateInstance();
	AddLinkHandler(GMToolLink::Instance());

	m_clientUpdate = true;
}

VersionConfig::~VersionConfig()
{
	CClientLink::DestroyInstance();
	GMToolLink::DestroyInstance();
}

bool VersionConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	if(!LoadVersionFile())
		return false;

	if(!LoadClientUpdateControlFile())
		return false;

	if(!LoadExtendInfoFile())
		return false;

	return true;
}

bool VersionConfig::LoadVersionFile()
{
	std::string strFilePath = GetFilePath("Version.txt");

	std::ifstream infile(strFilePath.c_str());

	std::string strIos;
	std::string strAndriod;

	if (!std::getline(infile, strIos))
	{
		LogError("Read ios version failed, file: %s", strFilePath.c_str());
		return false;
	}
	if (!std::getline(infile, strAndriod))
	{
		LogError("Read android  version failed, file: %s", strFilePath.c_str());
		return false;
	}
	infile.close();

	strIos = Trim(strIos);
	strAndriod = Trim(strAndriod);
	if (strIos.empty() || strAndriod.empty())
	{
		LogError("Read version failed, file: %s", strFilePath.c_str());
		return false;
	}

	m_newestVersion.Clear();
	m_newestVersion.ios.insert(strIos);
	m_newestVersion.android.insert(strAndriod);

	LogInfo("load version file ios:%s, android:%s", strIos.c_str(), strAndriod.c_str());

	return true;
}

bool VersionConfig::LoadClientUpdateControlFile()
{
	std::string strFilePath = GetFilePath("CloseClientUpdate.txt");

	std::ifstream infile(strFilePath.c_str());

	std::string needUpdate;

	if (!std::getline(infile, needUpdate))
	{
		LogWarn("Read need update failed, file: %s", strFilePath.c_str());
		return true;
	}
	infile.close();

	needUpdate = Trim(needUpdate);
	if (needUpdate.empty())
	{
		LogWarn("Read need update empty, file: %s", strFilePath.c_str());
		return true;
	}

	if(strcmp(needUpdate.c_str(), "0") != 0)
		m_clientUpdate = false;

	LogInfo("load need udpate file :%s", needUpdate.c_str());

	return true;
}

bool VersionConfig::LoadExtendInfoFile()
{
	std::string strFilePath = GetFilePath("ExtendInfo.txt");

	std::ifstream infile(strFilePath.c_str());

	std::string extendInfo;

	if (!std::getline(infile, extendInfo))
	{
		LogWarn("Read extendInfo failed, file: %s", strFilePath.c_str());
		return true;
	}
	infile.close();

	extendInfo = Trim(extendInfo);
	if (extendInfo.empty())
	{
		LogWarn("Read extendInfo empty, file: %s", strFilePath.c_str());
		return true;
	}

	m_extendInfo = extendInfo;

	LogInfo("load extendinfo file :%s", extendInfo.c_str());

	return true;
}
