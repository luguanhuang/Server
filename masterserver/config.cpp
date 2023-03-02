#include "pch.h"
#include "config.h"
#include "tinyxml2/tinyxml2.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "network/gatelink.h"
#include "network/gmlink.h"
#include "network/loginlink.h"
#include "network/gmtoollink.h"
#include "network/cslink.h"
#include "network/rslink.h"
#include "network/worldlink.h"
#include "network/controllink.h"
#include "network/fmlink.h"
#include "network/teamlink.h"
#include <time.h>
#include <fstream>
#include "common/ptcm2t_versionnotify.h"
#include "common/ptcm2n_m2nversoinnotify.h"
#include "tlogwriter.h"
#include "util/gametime.h"
#include "common/ptcm2c_mserrornotify.h"
#include "role/rolemanager.h"
#include "util.h"
#include "imysql.h"

INSTANCE_SINGLETON(MSConfig)

MSConfig::MSConfig()
:m_OpenTimeStamp(0)
,m_bDebug(false)
,m_nPlatType(KKSG::PLAT_IOS)
,m_nAppType(KKSG::GAME_APP_QQ)
,m_nServerTag(KKSG::SERVER_TAG_NORMAL)
,m_bPayDelayTest(false)
,m_uPollID(0)
,m_bFM(true)
,m_bPayDebug(true)
,m_PayNum(10)
,m_isBackFlowServer(false)
,m_backFlowLevel(0)
{
	SetServerTag("MasterServer");
	DBLink::CreateInstance();
	GSLink::CreateInstance();
	GateLink::CreateInstance();
	GMLink::CreateInstance();
	LoginLink::CreateInstance();
	GMToolLink::CreateInstance();
	CSLink::CreateInstance();
	RSLink::CreateInstance();
	WorldLink::CreateInstance();
	ControlLink::CreateInstance();
	FMLink::CreateInstance();
	TeamLink::CreateInstance();

	AddLinkHandler(DBLink::Instance());
	AddLinkHandler(GSLink::Instance());
	AddLinkHandler(GateLink::Instance());
	AddLinkHandler(GMLink::Instance());
	AddLinkHandler(LoginLink::Instance());
	AddLinkHandler(GMToolLink::Instance());
	AddLinkHandler(CSLink::Instance());
	AddLinkHandler(RSLink::Instance());
	AddLinkHandler(WorldLink::Instance());
	AddLinkHandler(ControlLink::Instance());
	AddLinkHandler(FMLink::Instance());
	AddLinkHandler(TeamLink::Instance());
}

MSConfig::~MSConfig()
{
	FMLink::DestroyInstance();
	WorldLink::DestroyInstance();
	RSLink::DestroyInstance();
	CSLink::DestroyInstance();
	DBLink::DestroyInstance();
	GSLink::DestroyInstance();
	GateLink::DestroyInstance();
	GMLink::DestroyInstance();
	LoginLink::DestroyInstance();
	GMToolLink::DestroyInstance();
	ControlLink::DestroyInstance();
	TeamLink::DestroyInstance();
}

bool MSConfig::ReadSettings(tinyxml2::XMLElement *root)
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

	/*
	elem = root->FirstChildElement("OpenServerTime");
	if (elem == NULL)
	{
		LogWarn("gsconfig.xml not contain <OpenServerTime> tag!");
		m_OpenTimeStamp = time(NULL);
	}
	else
	{
		struct tm T;
		const char *Time = elem->Attribute("time");
		sscanf(Time, "%d-%d-%d", &T.tm_year, &T.tm_mon, &T.tm_mday);
		T.tm_year -= 1900;
		T.tm_mon -= 1;
		T.tm_hour = HOUR_ONE_DAY_BEGIN;
		T.tm_min = T.tm_sec = 0;
		m_OpenTimeStamp = mktime(&T);
	}
	*/
	const char* isdebug = root->Attribute("debug");
	if(NULL != isdebug)
	{
		m_bDebug = (0 == strcmp("1", isdebug));
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
		m_nAppType = _ReadAppType(pcApp);
		m_nPlatType = _ReadPlatType(pcPlat);
	}

	elem = root->FirstChildElement("FM");
	if(elem == NULL)
	{
		LogError("Can't find FM element in Server config");
		return false;
	}else
	{
		const char* pcFM  = elem->Attribute("enabled");
		if(strcmp(pcFM,"1")==0)
		{
			m_bFM = true;
		}else
		{
			m_bFM = false;
		}
	}


	elem = root->FirstChildElement("TLogConfigFile");
	if(elem == NULL)
	{
		LogError("Can't find TLogConfigFile element in Server config");
		return false;
	}
	else
	{
		const char* pcPath = elem->Attribute("path");
		if(pcPath == NULL)
		{
			LogError("Can't find path attribute in TLogConfigFile element");
			return false; 
		}
		TLogWriter::SetConfigFile(pcPath);
	}
	elem = root->FirstChildElement("PayUrl");
	if (NULL == elem)
	{
		LogError("Can't find payurl element in Server config");
		return false;
	}
	const char* link = elem->Attribute("link");
	if (link)
	{
		LogDebug("pay link=%s", link);
		m_strPayUrl = link;
	}

	const char* slavelink = elem->Attribute("slavelink");
	if (slavelink)
	{
		LogDebug("pay slavelink=%s", slavelink);
		m_strPaySlaveUrl = slavelink;
	}

	const char* buygoodslink = elem->Attribute("buygoodslink");
	if (buygoodslink)
	{
		LogDebug("buygoods link=%s", buygoodslink);
		m_strBuyGoodsUrl = buygoodslink;
	}

	const char* buygoodsslavelink = elem->Attribute("buygoodsslavelink");
	if (buygoodsslavelink)
	{
		LogDebug("buygoods slavelink=%s", buygoodsslavelink);
		m_strBuyGoodsSlaveUrl = buygoodsslavelink;
	}
	INT32 count = elem->IntAttribute("count");
	m_PayNum = count?count:m_PayNum;

	const char* ispaydebug = elem->Attribute("paydebug");
	if (ispaydebug)
	{
		LogDebug("midas paydebug=%s", ispaydebug);
		m_bPayDebug = (0 == strcmp("1", ispaydebug));
	}

	const char* isdelay = elem->Attribute("paydelaytest");
	if(isdelay)
	{
		m_bPayDelayTest = (0 == strcmp("1", isdelay));
	}

	elem = root->FirstChildElement("MsdkUrl");
	if (NULL == elem)
	{
		LogError("Can't find MsdkUrl element in Server config");
		return false;
	}

	const char* msdklink = elem->Attribute("link");
	if (msdklink)
	{
		LogDebug("msdk link=%s", msdklink);
		m_strMsdkUrl = msdklink;
	}

	elem = root->FirstChildElement("XingeUrl");
	if (elem == NULL)
	{
		LogError("Can't find XingeUrl");
		return false;
	}
	m_xingeUrl = elem->Attribute("link");
	LogInfo("xinge url: %s", m_xingeUrl.c_str());

	if(!LoadVersionFile()) return false;

	LoadShmKeyOffset();

	if (!InitFromDB())
	{
		LogError("Load From DB failed");
		return false;
	}

	LoadDataClear();
	return true;
}

bool MSConfig::LoadVersionFile()
{
	std::string strFilePath = GetFilePath("serveronly/Version.txt");

	std::ifstream infile(strFilePath.c_str());

	std::string strIos;
	std::string strAndriod;

	if (!std::getline(infile, strIos))
	{
		LogError("Read ios version failed, file: %s", strFilePath.c_str());
		return false;
	}
	if (!std::getline(infile, strAndriod))
	{
		LogError("Read android  version failed, file: %s", strFilePath.c_str());
		return false;
	}
	infile.close();

	std::vector<std::string> iosVec = Split(strIos, '|');
	std::vector<std::string> androidVec = Split(strAndriod, '|');
	if (iosVec.empty() || androidVec.empty())
	{
		LogError("Read version failed, file: %s", strFilePath.c_str());
		return false;
	}

	Version tempVersion;

	for (UINT32 i = 0; i < iosVec.size(); ++i)
	{
		std::string temp = Trim(iosVec[i]);
		if (temp.empty())
		{
			continue;
		}
		tempVersion.ios.insert(temp);
	}
	for (UINT32 i = 0; i < androidVec.size(); ++i)
	{
		std::string temp = Trim(androidVec[i]);
		if (temp.empty())
		{
			continue;
		}
		tempVersion.android.insert(temp);
	}
	if (tempVersion.ios.empty() || tempVersion.android.empty())
	{
		LogError("Read version failed, file: %s", strFilePath.c_str());
		return false;
	}

	_UpdateVersionType(tempVersion);

	m_oVersion.Clear();
	m_oVersion = tempVersion;

	time_t now = time(NULL);
	m_ServerInfo = "Version: ";
	if (m_bDebug)
	{
		m_ServerInfo += "\ndebug";
	}
	for (auto iter = m_oVersion.ios.begin(); iter != m_oVersion.ios.end(); ++iter)
	{
		m_ServerInfo += "\niOS:" + *iter;
	}
	for (auto iter = m_oVersion.android.begin(); iter != m_oVersion.android.end(); ++iter)
	{
		m_ServerInfo += "\nAndroid:" + *iter;
	}
	m_ServerInfo += "\nCompile: " __DATE__ " " __TIME__;
	m_ServerInfo += "\nStart: ";
	m_ServerInfo += ctime(&now);
	m_ServerInfo += "GIT:";
	m_ServerInfo += "GIT";

	LogInfo("load version file %s success, ios:%s, android:%s", strFilePath.c_str(), strIos.c_str(), strAndriod.c_str());

	return true;
}

bool MSConfig::HotLoadVersionFile()
{
	if(!LoadVersionFile())
	{
		std::string strFilePath = GetFilePath("serveronly/Version.txt");
		LogError("hot load failed, %s", strFilePath.c_str());
		return false;
	}

	SyncVersion2Control();

	//PtcM2C_MSErrorNotify ptc;
	//ptc.m_Data.set_errorno(KKSG::ERR_VERSION_FAILED);
	//CRoleManager::Instance()->BroadCast(ptc);

	return true;
}

bool MSConfig::Reload()
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
	tinyxml2::XMLElement* elem = NULL;
	const char* isdebug = root->Attribute("debug");
	if(NULL != isdebug)
	{
		m_bDebug = (0 == strcmp("1", isdebug));
	}

	_AfterReloadMSConfig();

	return true;
}

void MSConfig::_UpdateVersionType(Version& tempVersion)
{
	if (m_bDebug)
	{
		tempVersion.type = Version_None;
	}
	else
	{
		tempVersion.type = Version_All;
	}
}

KKSG::GameAppType MSConfig::_ReadAppType(const char* pcApp)
{
	return (pcApp  != NULL && strcmp(pcApp, "qq") == 0 ? KKSG::GAME_APP_QQ : KKSG::GAME_APP_WECHAT);
}

KKSG::PlatType MSConfig::_ReadPlatType(const char* pcPlat)
{
	return (pcPlat != NULL && strcmp(pcPlat, "ios") == 0 ? KKSG::PLAT_IOS : KKSG::PLAT_ANDROID);
}

void MSConfig::_AfterReloadMSConfig()
{
	_UpdateVersionType(m_oVersion);
	SyncVersion2Control();
}

void MSConfig::SyncVersion2Control()
{
	PtcM2N_M2NVersoinNotify oPtc;
	oPtc.m_Data.set_type(MSConfig::Instance()->GetVersion()->type);
	auto& ios = MSConfig::Instance()->GetVersion()->ios;
	for (auto iter = ios.begin(); iter != ios.end(); ++iter)
	{
		oPtc.m_Data.add_ios(*iter);
	}
	auto& android = MSConfig::Instance()->GetVersion()->android;
	for (auto iter = android.begin(); iter != android.end(); ++iter)
	{
		oPtc.m_Data.add_android(*iter);
	}

	ControlLink::Instance()->SendTo(oPtc);
}

std::string MSConfig::GetMidasPayUrl(bool isMaster)
{
	return isMaster?m_strPayUrl:m_strPaySlaveUrl;
}

UINT32 MSConfig::GetShmKeyOffset(const std::string& name)
{
	auto it = m_shmOffset.find(name);
	if (it == m_shmOffset.end())
	{
		return 0;
	}
	return it->second;
}

void MSConfig::LoadShmKeyOffset()
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

bool MSConfig::InitFromDB()
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

bool MSConfig::LoadServerIds(IMysqlConn* pMysql)
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

bool MSConfig::LoadBackFlow(IMysqlConn* pMysql)
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
		SetBackFlowLevel(level);
	}
	poRes->Release();

	LogInfo("BackFlowLevel: %u", m_backFlowLevel);
	return true;
}

void MSConfig::LoadDataClear()
{
	memset(m_iscleardata, 0, sizeof(m_iscleardata));

	std::string data = ReadFileContent("serveronly/clearexpiredata.txt");
	if (data.empty())
	{
		return;
	}
	std::vector<std::string> Lines = Split(data, "\r\n");
	int index = 0;
	for (size_t i = 0; i < Lines.size(); ++i)
	{
		std::vector<std::string> oneLine = Split(Lines[i], ":");
		if (oneLine.size() < 2)
		{
			continue;
		}
		std::string type = oneLine[0];
		bool isopen = convert<bool>(oneLine[1]);
		if (type == "rolesummary")
		{
			m_iscleardata[DataClear_Summary] = isopen;
		}
		else if (type == "friend")
		{
			m_iscleardata[DataClear_Friend] = isopen;
		}
		else if (type == "garden")
		{
			m_iscleardata[DataClear_Garden] = isopen;
		}
		SSInfo<<"type:"<<type<<" isopen:"<<isopen<<END;
	}
}

bool MSConfig::IsDataClear(DataClearType type)
{
	if (type < 0 || type >= DataClear_Dummy)
	{
		return false;
	}
	return m_iscleardata[type];
}

void MSConfig::SetBackFlowLevel(UINT32 level)
{
	m_backFlowLevel = level;
	m_isBackFlowServer = m_backFlowLevel ? true : false;
}
