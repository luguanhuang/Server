#include "pch.h"
#include "appid.h"
#include "baseconfig.h"
#include "util.h"


INSTANCE_SINGLETON(CAppConfig)


bool CAppConfig::Init()
{
	std::string data = Config::GetConfig()->ReadFileContent("serveronly/apptag.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (!Lines.empty())
	{
		UINT32 temp = convert<UINT32>(Lines[0]);
		m_nAppTag = (temp == APP_TAG_TIYAN ? APP_TAG_TIYAN : APP_TAG_NORMAL);
	}

	LogInfo("Read AppTag: %s", m_nAppTag == APP_TAG_TIYAN ? "AppTag_TiYan" : "AppTag_Normal");
	return true;
}

void CAppConfig::Uninit()
{
}