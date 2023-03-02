#include "pch.h"
#include "config.h"
#include "gslink.h"
#include "mslink.h"
#include "util.h"
#include "controllink.h"
#include "rslink.h"
#include "common/ptcd2n_reportregistercount.h"
#include "network/controllink.h"

INSTANCE_SINGLETON(DBConfig)

DBConfig::DBConfig()
:m_bAutoCreateDB(false)
,m_bLoginEnabled(true)
,m_dwMaxRegisterNum(0)
,m_dwCurRegisterNum(0)
,m_dwTryInterval(3)
,m_dwMaxTryTimes(20)
{
	SetServerTag("DBServer");

	GSLink::CreateInstance();
	AddLinkHandler(GSLink::Instance());

	MSLink::CreateInstance();
	AddLinkHandler(MSLink::Instance());

	ControlLink::CreateInstance();
	AddLinkHandler(ControlLink::Instance());

	RSLink::CreateInstance();
	AddLinkHandler(RSLink::Instance());
}

DBConfig::~DBConfig()
{
	MSLink::DestroyInstance();
	GSLink::DestroyInstance();
	ControlLink::DestroyInstance();
	RSLink::DestroyInstance();
}

void DBConfig::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_dwCurRegisterNum == 0)
	{
		return;
	}
	PtcD2N_ReportRegisterCount ntf;
	ntf.m_Data.set_register_account(m_dwCurRegisterNum);
	ControlLink::Instance()->SendTo(ntf);
}

bool DBConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	tinyxml2::XMLElement *elem = root->FirstChildElement("DB");
	if(elem == NULL)
	{
		LogError("Can't find DB element in DBServer config");
		return false;
	}

	m_DBInfo.m_ip = elem->Attribute("ip");
	m_DBInfo.m_user = elem->Attribute("user");
	m_DBInfo.m_pswd = elem->Attribute("password");
	m_DBInfo.m_database = elem->Attribute("database");
	m_DBInfo.m_threads = elem->Attribute("threads");
	if (elem->Attribute("port") != NULL)
	{
		m_DBInfo.m_port = elem->Attribute("port");
	}
	else
	{
		m_DBInfo.m_port = "3306";
	}

	elem = root->FirstChildElement("OnlineDB");
	if(elem == NULL)
	{
		LogError("Can't find OnlineDB element in DBServer config");
	}
	else
	{
		m_OnlineDBInfo.m_ip = elem->Attribute("ip");
		m_OnlineDBInfo.m_user = elem->Attribute("user");
		m_OnlineDBInfo.m_pswd = elem->Attribute("password");
		m_OnlineDBInfo.m_database = elem->Attribute("database");
		m_OnlineDBInfo.m_threads = elem->Attribute("threads");
		if (elem->Attribute("port") != NULL)
		{
			m_OnlineDBInfo.m_port = elem->Attribute("port");
		}
		else
		{
			m_OnlineDBInfo.m_port = "3306";
		}
	}

	elem = root->FirstChildElement("AutoCreateDB");
	if(elem)
	{
		INT32 nVal = 0;
		elem->QueryIntAttribute("enabled", &nVal);
		m_bAutoCreateDB = (nVal == 0 ? false : true);
	}

	elem = root->FirstChildElement("MaxRegisterNum");
	if (elem == NULL)
	{
		LogError("Can't find MaxRegisterNum element in DBServer config");
		return false;
	}
	else
	{
		INT32 nRegVal = 0;
		elem->QueryIntAttribute("value", &nRegVal);
		if(nRegVal <= 0)
		{
			LogError("MaxRegisterNum [%d] is invalid", nRegVal);
			return false;
		}

		m_dwMaxRegisterNum = (UINT32)nRegVal;
	}

	if(!ReadLoinLimitFile())
	{
		return false;
	}

	LogInfo("MaxRegisterNum [%u], LoginEnabled [%d]", GetMaxRegisterNum(), IsLoginEnabled() ? 1 : 0);

	LoadMysqlReconFile();
	LogInfo("Reconnect Interval [%u], MaxTryTimes [%u]", GetReconTryInterval(), GetReconMaxTryTimes());

	CTimerMgr::Instance()->SetTimer(this, 0, 60 * 1000, -1, __FILE__, __LINE__);
	return true;
}

DBInfo & DBConfig::GetDBInfo()
{
	return m_DBInfo;
}

DBInfo & DBConfig::GetOnlineDBInfo()
{
	return m_OnlineDBInfo;
}

bool DBConfig::ReadLoinLimitFile()
{
	const char* pszFilePath = "LoginLimit.txt";
	FILE* fp = fopen(Config::GetConfig()->GetFilePath(pszFilePath).c_str(), "r");
	if(fp == NULL)
	{
		LogError("Can't open file %s", pszFilePath);
		return false;
	}

	char buffer[64];
	fgets(buffer, 64, fp);
	fclose(fp);

	std::vector<std::string> oStrVec = Split(buffer, '=');
	if(oStrVec.size() != 2)
	{
		LogError("Invalid congif value");
		return false;
	}

	INT32 nValue = convert<INT32>(oStrVec[1]);
	m_bLoginEnabled = (nValue == 0 ? false : true);
	return true;
}

bool DBConfig::ReLoadLoinLimitFile()
{
	bool bOld = IsLoginEnabled();
	if(!ReadLoinLimitFile())
	{
		return false;
	}

	bool bNew = IsLoginEnabled();
	LogInfo("Reload login limit file success: %d -> %d", bOld ? 1 : 0, bNew ? 1: 0);
	return true;
}

bool DBConfig::ReLoadMaxRegisterNum()
{
	tinyxml2::XMLDocument doc;
	const char *pszFilePath = GetConfFile().c_str();
	tinyxml2::XMLError error = doc.LoadFile(pszFilePath);
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("Load file %s failed, XMLError code: %d", pszFilePath, error);
		return false;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement *elem = root->FirstChildElement("MaxRegisterNum");
	if (elem == NULL)
	{
		LogError("Can't find MaxRegisterNum element in DBServer config");
		return false;
	}

	INT32 nRegVal = 0;
	elem->QueryIntAttribute("value", &nRegVal);
	if(nRegVal <= 0)
	{
		LogError("MaxRegisterNum [%d] is invalid", nRegVal);
		return false;
	}

	UINT32 dwOld = GetMaxRegisterNum();
	m_dwMaxRegisterNum = (UINT32)nRegVal;

	LogInfo("Reload MaxRegisterNum success: %u -> %u", dwOld, GetMaxRegisterNum());
	return true;
}

bool DBConfig::LoadMysqlReconFile()
{
	tinyxml2::XMLDocument doc;
	std::string strFilePath = GetFilePath("mysql_reconnect.xml");
	tinyxml2::XMLError error = doc.LoadFile(strFilePath.c_str());
	if (error != tinyxml2::XML_SUCCESS)
	{
		LogError("Load file %s failed, XMLError code: %d", strFilePath.c_str(), error);
		return false;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement *elem = root->FirstChildElement("reconnect");
	if (elem == NULL)
	{
		LogError("Can't find reconnect element in config");
		return false;
	}

	INT32 nVal1 = 0;
	INT32 nVal2 = 0;
	elem->QueryIntAttribute("interval", &nVal1);
	if(nVal1 <= 0)
	{
		LogError("interval [%d] is invalid", nVal1);
		return false;
	}

	elem->QueryIntAttribute("maxtry", &nVal2);
	if(nVal2 <= 0)
	{
		LogError("maxtry [%d] is invalid", nVal2);
		return false;
	}

	m_dwTryInterval = (UINT32)nVal1;
	m_dwMaxTryTimes = (UINT32)nVal2;
	return true;
}