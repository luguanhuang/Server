#ifndef __VSCONFIG_H__
#define __VSCONFIG_H__

#include "baseconfig.h"
#include "singleton.h"
#include "pb/enum.pb.h"
#include "version/version.h"

class VersionConfig : public Config
{
	VersionConfig();
	~VersionConfig();
	DECLARE_SINGLETON(VersionConfig)

public:
	virtual bool ReadSettings(tinyxml2::XMLElement *element);
	KKSG::GameAppType GetPlatform() { return m_platform; }

	const Version& GetVersion(){return m_newestVersion;}
	const Version& GetTestVersion(){return m_newestTestVersion;}
	bool ClientUpdate(){return m_clientUpdate;}

	bool LoadVersionFile();
	bool LoadClientUpdateControlFile();
	bool LoadExtendInfoFile();
	std::string& GetExtendInfo(){return m_extendInfo;}
private:
	KKSG::GameAppType m_platform;

	Version m_newestVersion;
	Version m_newestTestVersion;

	bool m_clientUpdate;
	std::string m_extendInfo;
};

#endif