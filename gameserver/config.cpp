#include "pch.h"
#include "config.h"
#include "gatelink.h"
#include "dblink.h"
#include "mslink.h"
#include "weblink.h"
#include <fstream>
#include <time.h>
#include "util/gametime.h"
#include "netutil.h"
#include "util.h"
#include "tlogwriter.h"
#include "network/worldlink.h"
#include "network/rslink.h"
#include "network/controllink.h"
#include "imysql.h"

INSTANCE_SINGLETON(GSConfig)

GSConfig::GSConfig()
{
	m_frametime = 0.05f;
	m_isdebug = false;
	m_isCross = false;
	m_dwLine = 0;
	m_OpenTimeStamp = 0;
	m_nAppType = KKSG::GAME_APP_QQ;
	m_nPlatType = KKSG::PLAT_ANDROID;
	m_isBackFlowServer = false;
	m_backFlowLevel = 0;

	SetServerTag("GameServer");

	GatetLink::CreateInstance();
	DBLink::CreateInstance();
	WebLink::CreateInstance();
	MSLink::CreateInstance();
	WorldLink::CreateInstance();
	RSLink::CreateInstance();
	ControlLink::CreateInstance();

	AddLinkHandler(GatetLink::Instance());
	AddLinkHandler(DBLink::Instance());
	AddLinkHandler(WebLink::Instance());
	AddLinkHandler(MSLink::Instance());
	AddLinkHandler(WorldLink::Instance());
	AddLinkHandler(RSLink::Instance());
	AddLinkHandler(ControlLink::Instance());
}

GSConfig::~GSConfig()
{
	RSLink::DestroyInstance();
	WorldLink::DestroyInstance();
	WebLink::DestroyInstance();
	GatetLink::DestroyInstance();
	DBLink::DestroyInstance();
	MSLink::DestroyInstance();
	ControlLink::DestroyInstance();
}

int GSConfig::GetGameServerOpenDays()
{
	time_t now = GameTime::GetTime();
	return (int)(now - m_OpenTimeStamp) / (3600 * 24);
}

bool GSConfig::ReadSettings(tinyxml2::XMLElement *root)
{
	const char* isdebug = root->Attribute("debug");
	if (NULL != isdebug)
	{
		m_isdebug = (0 == strcmp("1", isdebug));
	}
	const char* iscross = root->Attribute("cross");
	if (NULL != iscross)
	{
		m_isCross = (0 == strcmp("1", iscross));
	}


	m_strLocalIP = NetUtil::GetLocalHostIP();
	const char* pcLine = root->Attribute("line");
	if(pcLine == NULL)
	{
		LogError("can not find 'line' config");
		//return false;
	}
	m_dwLine = convert<UINT32>(pcLine);

	tinyxml2::XMLElement* elem = NULL;

	// 非跨服GS才读数据库
	if (!m_isCross)
	{
		elem = root->FirstChildElement("DB");
		if(elem == NULL)
		{
			LogError("Can't find DB element in GameServer config");
			return false;
		}

		if(!ReadDBInfo(m_DBInfo, elem))
		{
			return false;
		}
	}

	// TLog 配置
	elem = root->FirstChildElement("TLogConfigFile");
	if(elem == NULL)
	{
		LogError("Can't find TLogConfigFile element in Server config");
		return false;
	}
	const char* pcPath = elem->Attribute("path");
	if(pcPath == NULL)
	{
		LogError("Can't find path attribute in TLogConfigFile element");
		return false; 
	}
	TLogWriter::SetConfigFile(pcPath);

	elem = root->FirstChildElement("SdkUrl");
	if (NULL == elem)
	{
		LogError("Can't find SdkUrl element in Server config");
		return false;
	}
	const char* link = elem->Attribute("link");
	if (link)
	{
		m_strSdkUrl = link;
	}

	elem = root->FirstChildElement("AntiAddictionUrl");
	if (NULL == elem)
	{
		LogError("Can't find AntiAddictionUrl element in Server config");
		return false;
	}
	link = elem->Attribute("link");
	if (link)
	{
		m_strAntiAddictionUrl = link;
	}

	// frametime
	elem = root->FirstChildElement("FrameTime");
	if (elem)
	{
		const char* frametime = elem->Attribute("frametime");
		if (NULL != frametime)
		{
			m_frametime = convert<float>(frametime);
		}
	}
	SSInfo<<"game server frame time:"<<m_frametime<<END;

	if (!m_isCross)
	{
		elem = root->FirstChildElement("DataMoreUrl");
		if (NULL == elem)
		{
			LogError("Can't find DataMoreUrl element in Server config");
			return false;
		}
		link = elem->Attribute("link");
		if (link)
		{
			m_strDataMoreUrl = link;
		}
	}

	if (!m_isCross)
	{
		elem = root->FirstChildElement("GameWeixinUrl");
		if (NULL == elem)
		{
			LogError("Can't find GameWeixinUrl element in Server config");
			return false;
		}
		link = elem->Attribute("link");
		if (link)
		{
			m_strGameWeiXinUrl = link;
		}
	}

	if (!m_isCross && !InitFromDB())
	{
		return false;
	}

	return true;
}

bool GSConfig::InitFromDB()
{
	if (m_isCross)
	{
		return false;
	}
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

	pMysql->Close();
	pMysql->Release();
	return true;

}

bool GSConfig::LoadServerIds(IMysqlConn* pMysql)
{
	m_combineServerIDs.clear();
	m_combineServerIDs.insert(GetServerID());

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
		m_combineServerIDs.insert(serverId);
	}
	poRes->Release();

	std::ostringstream oss;
	for (auto it = m_combineServerIDs.begin(); it != m_combineServerIDs.end(); ++it)
	{
		if (it != m_combineServerIDs.begin())
		{
			oss << ",";
		}
		oss << *it;
	}
	LogInfo("serverIds: %s", oss.str().c_str());
	return true;
}

void GSConfig::SetGameServerOpenTimeStamp(std::string str)
{
	struct tm T;
	sscanf(str.c_str(), "%d-%d-%d", &T.tm_year, &T.tm_mon, &T.tm_mday);
	T.tm_year -= 1900;
	T.tm_mon -= 1;
	T.tm_hour = 0;
	T.tm_min = T.tm_sec = 0;
	m_OpenTimeStamp = mktime(&T);
}

void GSConfig::SetBackFlowLevel(UINT32 level)
{
	m_backFlowLevel = level;
	m_isBackFlowServer = m_backFlowLevel ? true : false;
}

bool GSConfig::CanDoBackFlowInit(UINT32 roleNum)
{
	if (m_isBackFlowServer && roleNum <= 1)
	{
		return true;
	}
	return false;
}

bool GSConfig::IsServerIDIn(UINT32 serverID)
{
	return m_combineServerIDs.find(serverID) != m_combineServerIDs.end();
}
