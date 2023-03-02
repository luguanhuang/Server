#include "pch.h"
#include "whitelistmgr.h"
#include "config.h"
#include "util.h"
#include "cmdline.h"


INSTANCE_SINGLETON(WhiteListMgr)

WhiteListMgr::WhiteListMgr()
{

}

WhiteListMgr::~WhiteListMgr()
{

}

bool CmdReload(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		WhiteListMgr::Instance()->DumpWhiteList();
		return true;
	}

	if (args[1] == "reload")
	{
		WhiteListMgr::Instance()->Reload();
		LogInfo("reload whitelist");
		return true;
	}

	outputMessage.append("whitelist | whitelist reload\n");
	return false;
}

bool WhiteListMgr::Init()
{
	CmdLine::RegistCmd("whitelist", CmdReload);
	LoadWhiteList();
	return true;
}

void WhiteListMgr::Uninit()
{

}

void WhiteListMgr::Reload()
{
	LoadWhiteList();
}

void WhiteListMgr::LoadWhiteList()
{
	m_WhiteOpenIDs.clear();
	std::string openIDData = LoginConfig::Instance()->ReadFileContent("whitelist.txt");
	std::vector<std::string> Lines = Split(openIDData, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if (!temp.empty())
		{
			m_WhiteOpenIDs.insert(temp);
		}
	}

	for (auto j = m_WhiteOpenIDs.begin(); j != m_WhiteOpenIDs.end(); ++j)
	{
		LogInfo("Load openid: %s", j->c_str());
	}
}

bool WhiteListMgr::IsOpenIDInWhiteList(const std::string &id)
{
	return m_WhiteOpenIDs.find(id) != m_WhiteOpenIDs.end();
}

void WhiteListMgr::DumpWhiteList()
{
	SSInfo << "whitelist: \n";
	for (auto j = m_WhiteOpenIDs.begin(); j != m_WhiteOpenIDs.end(); ++j)
	{
		SSInfo << *j << "\n";
	}
	SSInfo << END;
}
