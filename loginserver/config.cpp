#include "pch.h"
#include "config.h"
#include "controllink.h"
#include "clientlink.h"
#include "gmlink.h"
#include "mslink.h"
#include "gmtoollink.h"
#include "util.h"
#include <fstream>
#include "worldlink.h"
#include "cslink.h"

INSTANCE_SINGLETON(LoginConfig)

LoginConfig::LoginConfig()
{
	SetServerTag("LoginServer");

	ControlLink::CreateInstance();
	AddLinkHandler(ControlLink::Instance());

	CClientLink::CreateInstance();
	AddLinkHandler(CClientLink::Instance());

	GMLink::CreateInstance();
	AddLinkHandler(GMLink::Instance());

	MsLink::CreateInstance();
	AddLinkHandler(MsLink::Instance());

	GMToolLink::CreateInstance();
	AddLinkHandler(GMToolLink::Instance());

	WorldLink::CreateInstance();
	AddLinkHandler(WorldLink::Instance());

	CSLink::CreateInstance();
	AddLinkHandler(CSLink::Instance());

	m_UseOpenIDFilter = false;
	m_bDebug = false;
	m_loginMax = 200;

	m_onLineFullNum = 7000;
	m_registerSmoothNum = 20000;
	m_registerFullNum = 30000;

	m_recommendZoneName = std::string("Test");
	m_freeFlow = false;
	m_backFlowCheck = true;
	m_accountBackFlowTime = 7 * 86400;
	m_accountBackFlowExpireTime = 2 * 86400;
}

LoginConfig::~LoginConfig()
{
	CClientLink::DestroyInstance();
	ControlLink::DestroyInstance();
	GMLink::DestroyInstance();
	MsLink::DestroyInstance();
	GMToolLink::DestroyInstance();
	WorldLink::DestroyInstance();
	CSLink::DestroyInstance();
}

bool LoginConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	m_platform = root->Attribute("platform");

	tinyxml2::XMLElement* elem = root->FirstChildElement("DB");
	ReadDBInfo(m_DBInfo, elem);

	const char* isdebug = root->Attribute("debug");
	if(NULL != isdebug)
	{
		m_bDebug = (0 == strcmp("1", isdebug));
	}

	const char* useOpenID = root->Attribute("useOpenID");
	if(NULL != useOpenID)
	{
		m_UseOpenIDFilter = (0 == strcmp("1", useOpenID));
	}
	elem = root->FirstChildElement("MsdkUrl");
	if(elem == NULL)
	{
		LogError("Can't find MsdkUrl element in Server config");
		return false;
	}
	else
	{
		const char* link = elem->Attribute("link");
		if(link == NULL)
		{
			LogError("Can't find link attribute in MsdkUrl element");
			return false; 
		}
		m_msdkUrl = link;
	}

	elem = root->FirstChildElement("LoginControl");
	if (elem != NULL)
	{
		const char* max = elem->Attribute("max");
		if(max != NULL)
		{
			m_loginMax = convert<UINT32>(max);
		}
	}
	LogDebug("max login num[%u] concurent", m_loginMax);

	elem = root->FirstChildElement("ServerState");
	if (elem != NULL)
	{
		m_onLineFullNum = elem->UnsignedAttribute("onlineFull");
		m_registerSmoothNum = elem->UnsignedAttribute("registerSmooth");
		m_registerFullNum = elem->UnsignedAttribute("registerFull");
	}

	elem = root->FirstChildElement("RecommendZone");
	if (elem)
	{
		m_recommendZoneName = elem->Attribute("name");
	}

	LoadIosAuditVersionFile();
	LoadUseFreeFlow();
	LoadBackFlow();

	return true;
}

void LoginConfig::LoadIosAuditVersionFile()
{
	const char* pszFilePath = "ios_audit_version.txt";
	FILE* fp = fopen(GetFilePath(pszFilePath).c_str(), "r");
	if(fp == NULL)
	{
		LogError("Can't open file %s", pszFilePath);
		return;
	}

	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(fileSize == 0)
	{
		m_strIosAuditVersion.clear();
		fclose(fp);
		return;
	}

	m_strIosAuditVersion.resize(fileSize);
	size_t t = fread(&m_strIosAuditVersion[0], fileSize, 1, fp);
	fclose(fp);

	m_strIosAuditVersion = Trim(m_strIosAuditVersion);
}

void LoginConfig::LoadUseFreeFlow()
{
	std::string data = ReadFileContent("freeflow.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (Lines.empty())
	{
		m_freeFlow = false;
		return ;
	}
	UINT32 temp = convert<UINT32>(Lines[0]);
	m_freeFlow = (temp == 0) ? false : true;
	LogInfo("Use FreeFlow: %u", m_freeFlow);
}

void LoginConfig::LoadBackFlow()
{
	std::string data = ReadFileContent("backflow.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (Lines.empty())
	{
		m_backFlowCheck = true;
		return ;
	}
	UINT32 temp = convert<UINT32>(Lines[0]);
	m_backFlowCheck = (temp == 0) ? false : true;

	if (Lines.size() >= 3)
	{
		m_accountBackFlowTime = convert<UINT32>(Lines[1]);
		m_accountBackFlowExpireTime = convert<UINT32>(Lines[2]);
	}
	else
	{
		m_accountBackFlowTime = 7 * 86400;
		m_accountBackFlowExpireTime = 2 * 86400;
	}
	LogInfo("LoadBackFlow, use backflowserver check %u, account backflow time: %u, account backflow expire time: %u", m_backFlowCheck, m_accountBackFlowTime, m_accountBackFlowExpireTime);
}
