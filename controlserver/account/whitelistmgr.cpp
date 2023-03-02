#include "pch.h"
#include "whitelistmgr.h"
#include "config.h"
#include "util.h"
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#include "string.h"
#endif


INSTANCE_SINGLETON(CWhiteListMgr)


CWhiteListMgr::CWhiteListMgr()
{
}

CWhiteListMgr::~CWhiteListMgr()
{
}

bool CWhiteListMgr::Init()
{
	if(!LoadWhiteList()) return false;

	return true;
}

void CWhiteListMgr::Uninit()
{
}

bool CWhiteListMgr::LoadWhiteList()
{
	m_WhiteOpenIDs.clear();
	std::string openIDData = ControlConfig::Instance()->ReadFileContent("serveronly/whitelist.txt");
	std::vector<std::string> Lines = Split(openIDData, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if (!temp.empty())
		{
			m_WhiteOpenIDs.insert(temp);
		}
	}

	return true;
}

bool CWhiteListMgr::IsOpenIDInWhiteList(const std::string& id) const
{
	return m_WhiteOpenIDs.find(id) != m_WhiteOpenIDs.end();
}

void CWhiteListMgr::SaveWhiteList()
{
	FILE* pWhiteListFile = NULL;
#ifdef WIN32
	pWhiteListFile = _wfopen(L"gsconf/serveronly/whitelist.txt", L"r+,ccs=UTF-8");
#else
	pWhiteListFile = fopen("gsconf/serveronly/whitelist.txt", "a+");
#endif

	if (!pWhiteListFile)
	{
		return;
	}

#ifdef WIN32
	if(_chsize(_fileno(pWhiteListFile), 0) != 0)
	{
		return;
	}
#else
	if(ftruncate(fileno(pWhiteListFile), 0) != 0)
	{
		return;
	}
#endif

	fseek(pWhiteListFile, 0L, SEEK_END);
	size_t sz = ftell(pWhiteListFile);
	if (sz != 0)
	{
		LogError("ftruncate file error, size:%u", sz);
	}
	rewind(pWhiteListFile);

#ifdef WIN32
	std::wstringstream ss;
	for (auto iter = m_WhiteOpenIDs.begin(); iter != m_WhiteOpenIDs.end(); ++iter)
	{
		wchar_t* temp = L"";
		ss.str(temp);
		ss << iter->c_str();
		fwprintf(pWhiteListFile, L"%ls\n", ss.str().c_str());
	}
#else
	for (auto iter = m_WhiteOpenIDs.begin(); iter != m_WhiteOpenIDs.end(); ++iter)
	{
		int count = fprintf(pWhiteListFile, "%s\n", iter->c_str());
		if (count < 0)
		{
			LogError("save whitelist error");
		}
	}
#endif
	fflush(pWhiteListFile);
	fclose(pWhiteListFile);
	pWhiteListFile = NULL;
	LogInfo("save whitelist success");
}

void CWhiteListMgr::Add(const std::string& account)
{
	if(!IsOpenIDInWhiteList(account))
	{
		m_WhiteOpenIDs.insert(account);
		SaveWhiteList();
	}
}

void CWhiteListMgr::Del(const std::string& account)
{
	if (IsOpenIDInWhiteList(account))
	{
		m_WhiteOpenIDs.erase(account);
		SaveWhiteList();
	}
}
