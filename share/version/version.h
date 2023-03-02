#ifndef __H_VersionDef_H__
#define __H_VersionDef_H__


enum
{
	Version_None,
	Version_iOS,
	Version_Android,
	Version_PC,
	Version_All,
};

struct Version
{
	UINT32 type;
	std::unordered_set<std::string> ios;
	std::unordered_set<std::string> android;
	std::unordered_set<std::string> pc;

	const Version& operator = (const Version& ver);

	std::string CheckVersion(const std::string& _ios, const std::string& _android, const std::string& _pc);

	std::string GetClientVersion(const std::string& _ios, const std::string& _android, const std::string& _pc);

	std::string Check(const std::string& user, const std::unordered_set<std::string>& version);

	void SetEmpty();

	void Clear();

	Version()
	:type(Version_None)
	{
	}
};


#endif