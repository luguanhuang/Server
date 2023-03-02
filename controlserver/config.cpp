#include "pch.h"
#include "config.h"
#include "tinyxml2/tinyxml2.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "network/gatelink.h"
#include "network/loginlink.h"
#include "network/gmtoollink.h"
#include "network/mslink.h"
#include "network/rslink.h"
#include "util.h"
#include "imysql.h"


INSTANCE_SINGLETON(ControlConfig)


ControlConfig::ControlConfig()
:m_bUseWhiteList(false)
,m_enableReconnect(false)
,m_nPlatType(KKSG::PLAT_IOS)
,m_nAppType(KKSG::GAME_APP_QQ)
,m_isBackFlowServer(false)
,m_backFlowLevel(0)
{
	SetServerTag("ControlServer");

	DBLink::CreateInstance();
	GSLink::CreateInstance();
	GateLink::CreateInstance();
	LoginLink::CreateInstance();
	GMToolLink::CreateInstance();
	MSLink::CreateInstance();
	RSLink::CreateInstance();

	AddLinkHandler(DBLink::Instance());
	AddLinkHandler(GSLink::Instance());
	AddLinkHandler(GateLink::Instance());
	AddLinkHandler(LoginLink::Instance());
	AddLinkHandler(GMToolLink::Instance());
	AddLinkHandler(MSLink::Instance());
	AddLinkHandler(RSLink::Instance());
}

ControlConfig::~ControlConfig()
{
	MSLink::DestroyInstance();
	DBLink::DestroyInstance();
	GSLink::DestroyInstance();
	RSLink::DestroyInstance();
	GateLink::DestroyInstance();
	LoginLink::DestroyInstance();
	GMToolLink::DestroyInstance();
}

bool ControlConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	tinyxml2::XMLElement *elem = root->FirstChildElement("DB");
	if(elem == NULL)
	{
		LogError("Can't find DB element in Server config");
		return false;
	}

	if(!ReadDBInfo(m_DBInfo, elem))
	{
		return false;
	}

	elem = root->FirstChildElement("AppPlat");
	if(elem == NULL)
	{
		LogError("Can't find AppPlat element in Server config");
		return false;
	}
	else
	{
		const char* pcApp  = elem->Attribute("app");
		const char* pcPlat = elem->Attribute("plat");
		m_nAppType  = (pcApp  != NULL && strcmp(pcApp, "qq") == 0 ? KKSG::GAME_APP_QQ : KKSG::GAME_APP_WECHAT);
		m_nPlatType = (pcPlat != NULL && strcmp(pcPlat, "ios") == 0 ? KKSG::PLAT_IOS : KKSG::PLAT_ANDROID);
	}

	elem = root->FirstChildElement("Shm");
	if (elem)
	{
		tinyxml2::XMLElement* chunk = elem->FirstChildElement("config");
		while (chunk)
		{
			const char* name = chunk->Attribute("name");
			UINT32 num = chunk->UnsignedAttribute("num");

			SSDebug << "name :" << name << ", num: " << num << END;

			std::string strName(name);
			if (!strName.empty())
			{
				m_chunkNum[strName] = num;
			}

			chunk = chunk->NextSiblingElement("config");
		}
	}

	LogInfo("ServerID: [%u], App: [%d], Plat: [%d]", GetServerID(), GetAppType(), GetPlatType());
	if (!LoadUseWhiteList())
	{
		return false;
	}
	LoadReconnect();

	LoadShmKeyOffset();

	if (!InitFromDB())
	{
		LogError("Load From DB failed");
		return false;
	}

	return true;
}

UINT32 ControlConfig::GetChunkNum(const std::string& name, UINT32 defaultValue)
{
	auto it = m_chunkNum.find(name);
	if (it == m_chunkNum.end())
	{
		return defaultValue;
	}
	if (it->second == 0)
	{
		return defaultValue;
	}
	return it->second;
}

bool ControlConfig::LoadUseWhiteList()
{
	std::string data = ReadFileContent("serveronly/usewhitelist.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (Lines.empty())
	{
		return false;
	}
	UINT32 temp = convert<UINT32>(Lines[0]);
	m_bUseWhiteList = (temp == 0) ? false : true;
	LogInfo("UseWhiteList: %d", temp);

	return true;
}

void ControlConfig::LoadReconnect()
{
	std::string data = ReadFileContent("serveronly/reconnect.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (Lines.empty())
	{
		return ;
	}
	UINT32 temp = convert<UINT32>(Lines[0]);
	m_enableReconnect = (temp == 0) ? false : true;
	LogInfo("EnableReconnect: %d", temp);
}

UINT32 ControlConfig::GetShmKeyOffset(const std::string& name)
{
	auto it = m_shmOffset.find(name);
	if (it == m_shmOffset.end())
	{
		return 0;
	}
	return it->second;
}

void ControlConfig::LoadShmKeyOffset()
{
	std::string data = ReadFileContent("serveronly/shmkeyoffset.txt");
	if (data.empty())
	{
		return;
	}
	std::vector<std::string> Lines = Split(data, "\r\n");
	for (size_t i = 0; i < Lines.size(); ++i)
	{
		std::vector<std::string> oneLine = Split(Lines[i], "|");
		if (oneLine.size() < 2)
		{
			continue;
		}
		std::string key = oneLine[0];
		UINT32 offset = convert<UINT32>(oneLine[1]);
		m_shmOffset[key] = offset;
		LogInfo("shmkey %s, offset %u", key.c_str(), offset);
	}
}

bool ControlConfig::InitFromDB()
{
	IMysqlConn* pMysql = CreateMysqlConn();
	if(!pMysql->Connect(m_DBInfo.m_ip.c_str(), convert<UINT32>(m_DBInfo.m_port), m_DBInfo.m_user.c_str(), m_DBInfo.m_pswd.c_str(), m_DBInfo.m_database.c_str()))
	{
		LogError("Can't connect to database %s:%s:%s, ErrDesc: %s", m_DBInfo.m_ip.c_str(), m_DBInfo.m_port.c_str(), m_DBInfo.m_database.c_str(), pMysql->GetError());
		return false;
	}
	LogInfo("Connect to %s:%s:%s succ.", m_DBInfo.m_ip.c_str(), m_DBInfo.m_port.c_str(), m_DBInfo.m_database.c_str());

	if (!LoadServerIds(pMysql))
	{
		LogError("load serverids failed");
		return false;
	}
	if (!LoadBackFlow(pMysql))
	{
		LogError("LoadBackFlow failed");
		return false;
	}

	pMysql->Close();
	pMysql->Release();
	return true;
}

bool ControlConfig::LoadBackFlow(IMysqlConn* pMysql)
{
	m_backFlowLevel = 0;
	m_isBackFlowServer = false;

	std::string sql = "select `level` from backflow";
	IMysqlRecordSet* poRes = NULL;
	if(!pMysql->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read backflow failed, errorMsg: %s", pMysql->GetError());
		return false;
	}

	if (poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT32 level;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			level = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'level' column failed");
			return false;
		}
		m_backFlowLevel = level;
		if (m_backFlowLevel)
		{
			m_isBackFlowServer = true;
		}
	}
	poRes->Release();

	LogInfo("BackFlowLevel: %u", m_backFlowLevel);
	return true;
}

bool ControlConfig::LoadServerIds(IMysqlConn* pMysql)
{
	m_serverIds.clear();
	m_serverIds.insert(GetServerID());

	std::string sql = "select `serverid` from serverids";
	IMysqlRecordSet* poRes = NULL;
	if(!pMysql->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read serverids failed, errorMsg: %s", pMysql->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		UINT32 serverId;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			serverId = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'serverid' column failed");
			return false;
		}
		m_serverIds.insert(serverId);
	}
	poRes->Release();

	std::ostringstream oss;
	for (auto it = m_serverIds.begin(); it != m_serverIds.end(); ++it)
	{
		if (it != m_serverIds.begin())
		{
			oss << ",";
		}
		oss << *it;
	}
	LogInfo("serverIds: %s", oss.str().c_str());
	return true;
}

bool ControlConfig::IsValidServerId(UINT32 serverId)
{
	auto it = m_serverIds.find(serverId);
	return it == m_serverIds.end() ? false : true;
}
