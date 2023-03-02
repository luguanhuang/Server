#include "pch.h"
#include "OpenSystemMgr.h"
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "string.h"
#include "errno.h"
#include "other/servercrossdatamgr.h"

INSTANCE_SINGLETON(OpenSystemMgr)

OpenSystemMgr::OpenSystemMgr()
{
}

OpenSystemMgr::~OpenSystemMgr()
{
}

bool OpenSystemMgr::Init()
{
	bool success = LoadIdipOpSystem();
	if (!success)
	{
		return false;
	}

	return LoadFile();
}

void OpenSystemMgr::Uninit()
{
	ClearFile();
}

bool OpenSystemMgr::CheckFile()
{
	OpenSystemTable tmpOpenSystemTable;
	if (!tmpOpenSystemTable.LoadFile("table/SystemList.txt"))
	{
		LogWarn("Load file %s failed!", "table/SystemList.txt");
		return false;
	}
	return true;
}

bool OpenSystemMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oSystemTable.LoadFile("table/SystemList.txt");

	return true;
}

void OpenSystemMgr::ClearFile()
{
	m_oSystemTable.Clear();
}

int OpenSystemMgr::GetPlayerLevelBySystemID(int systemID)
{
	OpenSystemTable::RowData* data = m_oSystemTable.GetBySystemID(systemID);
	if(NULL == data)
	{
		return 0;
	}
	return data->PlayerLevel;	
}

OpenSystemTable::RowData* OpenSystemMgr::GetSystemRowData(int systemID)
{
	OpenSystemTable::RowData* data = m_oSystemTable.GetBySystemID(systemID);

	return data;
}

bool OpenSystemMgr::IsClosedSystem(UINT32 systemID)
{
	return IsIdipCloseSystem(systemID);
}

bool OpenSystemMgr::LoadIdipOpSystem()
{
	std::string closeInfo = MSConfig::Instance()->ReadFileContent("serveronly/idipclosesystem.txt");
	std::vector<std::string> Lines = Split(closeInfo, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if (!temp.empty())
		{
			m_idipSystemClose.insert(convert<UINT32>(temp.c_str()));
			LogDebug("load idip close system:%u", *m_idipSystemClose.rbegin());
		}
	}

	LogDebug("load idip close system success");

	return true;
}

void OpenSystemMgr::SaveIdipOpSystem()
{
	FILE* pIdipCloseSystemFile = NULL;
#ifdef WIN32
	pIdipCloseSystemFile = _wfopen(L"gsconf/serveronly/idipclosesystem.txt", L"r+,ccs=UTF-8");
#else
	pIdipCloseSystemFile = fopen("gsconf/serveronly/idipclosesystem.txt", "a+");
#endif

	if (!pIdipCloseSystemFile)
	{
		LogError("open idip close system file error, %s", strerror(errno));
		return;
	}

#ifdef WIN32
	if(_chsize(_fileno(pIdipCloseSystemFile), 0) != 0)
		LogError("ftruncate file error");
#else
	if(ftruncate(fileno(pIdipCloseSystemFile), 0) != 0)
		LogError("ftruncate file error");
#endif

	fseek(pIdipCloseSystemFile, 0L, SEEK_END);
	size_t sz = ftell(pIdipCloseSystemFile);
	if (sz != 0)
	{
		LogError("ftruncate file error, size:%u", sz);
	}
	rewind(pIdipCloseSystemFile);

#ifdef WIN32
	std::wstringstream ss;
	for (auto iter = m_idipSystemClose.begin(); iter != m_idipSystemClose.end(); ++iter)
	{
		wchar_t* temp = L"";
		ss.str(temp);
		ss<<*iter;
		fwprintf(pIdipCloseSystemFile, L"%ls\n", ss.str().c_str());
	}
#else
	std::stringstream ss;
	for (auto iter = m_idipSystemClose.begin(); iter != m_idipSystemClose.end(); ++iter)
	{
		ss.str("");
		ss<<*iter;
		int count = fprintf(pIdipCloseSystemFile, "%s\n", ss.str().c_str());
		if (count < 0)
		{
			LogError("save idip close system file error, %s", strerror(errno));
		}
	}
#endif
	fflush(pIdipCloseSystemFile);
	fclose(pIdipCloseSystemFile);
	pIdipCloseSystemFile = NULL;
	LogInfo("save idip close system info");
}

void OpenSystemMgr::IdipOpSystem(UINT32 systemID, bool opOpen)
{
	if (opOpen)
	{
		m_idipSystemClose.erase(systemID);
	}
	else
	{
		m_idipSystemClose.insert(systemID);
	}

	LogInfo("idip %s system:%u", opOpen ? "open" : "close", systemID);

	SaveIdipOpSystem();
	ServerCrossDataSyn::SynDataToWorld();
}

bool OpenSystemMgr::IsIdipCloseSystem(UINT32 systemID)
{
	auto find = m_idipSystemClose.find(systemID);
	if (find != m_idipSystemClose.end())
	{
		return true;
	}

	return false;
}

