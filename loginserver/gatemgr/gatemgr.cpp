#include "pch.h"
#include "gatemgr.h"
#include "verify/tokenverifymgr.h"
#include "timer.h"
#include "cmdline.h"
#include "controllink.h"
#include "netproc.h"
#include "common/ptci2d_changeserverstatentf.h"
#include <tinyxml2.h>
#include "util.h"
#include "sql/mysqlmgr.h"
#include "LoadGateInfoTask.h"
#include "verify/tokenverifymgr.h"
#include "network/mslink.h"
#include "network/worldlink.h"
#include "server/ptci2m_serverinfontf.h"
#include "server/ptci2k_serverinfontf2world.h"

INSTANCE_SINGLETON(GateMgr)

GateMgr::GateMgr()
{

}

GateMgr::~GateMgr()
{
	
}

bool GateMgr::Init()
{
	LoadFromDB();
	CmdLine::RegistCmd("gate", CmdLineGate);
	return true;
}

void GateMgr::LoadFromDB()
{
	LoadGateInfoTask task;
	task.Execute(MysqlMgr::Instance()->GetMysqlConn());
	task.FinishTask();
}

void GateMgr::ReloadFromDB()
{
	LoadGateInfoTask* task = new LoadGateInfoTask(true);
	TokenVerifyMgr::Instance()->AddDBTask(task);
}

void GateMgr::ParseFrom(std::vector<GateInfo>& infos, bool reload)
{
	if (infos.empty())
	{
		LogWarn("no gate info");
		return;
	}

	Clear();

	for (auto it = infos.begin(); it != infos.end(); ++it)
	{
		GateInfo& info = *it;
		if (m_gateInfos.find(info.serverID) != m_gateInfos.end())
		{
			LogError("duplicate serverid: %d", info.serverID);
			continue;
		}
		ParseRecommendServer(info);

		m_gateInfos.insert(std::make_pair(info.serverID, info));
	}

	CheckRecommendServer();

	if (reload)
	{
		ServerInfoNotifyToMs();
		ServerInfoNotifyToWorld();
	}
}

void GateMgr::Uninit()
{
	Clear();
}

void GateMgr::ParseRecommendServer(GateInfo& gateInfo)
{
	for (size_t i = 0; i < gateInfo.channel.size(); ++i)
	{
		UINT32 loginType = gateInfo.channel[i];
		if (loginType >= KKSG::LoginType_ARRAYSIZE)
		{
			continue;
		}
		if (gateInfo.isOpen == false)
		{
			continue;
		}
		// 直接设置的推荐服
		if (gateInfo.IsRecommend())
		{
			m_recommendServer[loginType].AddServerId(gateInfo.serverID);
		}
	}
}

void GateMgr::CheckRecommendServer()
{
	// 设置推荐服，两种推荐服 1. 直接设置成推荐 2. 自动设置成推荐
	// 自动推荐
	for (int i = 0; i < KKSG::LoginType_ARRAYSIZE; ++i)
	{
		if (m_recommendServer[i].HasRecommend())
		{
			continue;
		}
		GateInfo* temp = NULL;
		for (auto it = m_gateInfos.begin(); it != m_gateInfos.end(); ++it)
		{
			GateInfo& gate = it->second;
			if (std::find(gate.channel.begin(), gate.channel.end(), i) == gate.channel.end())
			{
				continue;
			}
			if (gate.isOpen == false)
			{
				continue;
			}
			// 开服时间最晚的作为推荐服
			if (temp == NULL || temp->openTime <= gate.openTime)
			{
				temp = &gate;
			}
		}

		if (temp)
		{
			m_recommendServer[i].AddServerId(temp->serverID);
			temp->autoRecommend = true;
			temp->UpdateState();
		}
	}
}

int GateMgr::GetRecommendServerId(KKSG::LoginType loginType, bool isWhite)
{
	if (loginType >= KKSG::LoginType_ARRAYSIZE)
	{
		return -1;
	}
	return m_recommendServer[loginType].GetRecommend();
}

GateInfo* GateMgr::GetRecommendServerInfo(KKSG::LoginType loginType, bool isWhite)
{
	int serverId = GetRecommendServerId(loginType, isWhite);
	if (serverId == -1)
	{
		return NULL;
	}
	return GetServerInfo(serverId);
}

GateInfo *GateMgr::GetServerInfo(int serverID)
{
	auto i = m_gateInfos.find(serverID);
	return (i == m_gateInfos.end()) ? NULL : &i->second;
}

void GateMgr::Clear()
{
	m_gateInfos.clear();
	for (UINT32 i = 0; i < KKSG::LoginType_ARRAYSIZE; ++i)
	{
		m_recommendServer[i].Reset();
	}
}

bool GateMgr::CmdLineGate(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() == 1)
	{
		Instance()->DumpGate();
		return true;
	}

	if (args[1] == "reload")
	{
		Instance()->ReloadFromDB();
		LogInfo("reload table");
		return true;
	}

	if (args[1] == "v" || args[1] == "vorbose")
	{
		//Instance()->m_printgatechange = !Instance()->m_printgatechange;
		//LogInfo("print if gateip is not changed [%s]", Instance()->m_printgatechange ? "Yes" : "No");
		return true;
	}

	if (args[1] == "add")
	{
		if (args.size() != 7)
		{
			LogInfo("gate add ServerID ZoneName ServerName ip port");
			return false;
		}

		GateInfo info;
		info.serverID = convert<int>(args[2]);
		info.serverName = args[4];
		info.zoneName = args[3];
		info.state = ServerSetState_Smooth;
		info.clientState = KKSG::ServerState_Smooth;
		info.clientFlag = KKSG::ServerFlag_New;
		info.channel.push_back(KKSG::LOGIN_PASSWORD);

		GateIPAddr addr;
		addr.ip = args[5];
		addr.port = convert<int>(args[6]);
		info.Hosts.push_back(addr);

		Instance()->m_gateInfos.insert(std::make_pair(info.serverID, info));

		LogInfo("add new gateip ok");
		return true;
	}

	LogInfo("\ngate\ngate reload\ngate [v|vorbose]\ngate add ServerID ZoneName ServerName ip port");
	return false;
}

void GateMgr::DumpGate()
{
	SSInfo << "\n";
	for(auto i = m_gateInfos.begin(); i != m_gateInfos.end(); ++i)
	{
		SSInfo << i->first << ". " << i->second.zoneName << " " << i->second.serverName << " ";
		for (auto j = i->second.Hosts.begin(); j != i->second.Hosts.end(); ++j)
		{
			SSInfo << (*j).ip << ":" << (*j).port << "|";
		}

		SSInfo << "\n";
	}

	SSInfo << END;
}

GateInfo* GateMgr::GetRecommendServerInfo(UserGateInfo* pUserInfo, KKSG::LoginType loginType, bool isInWhiteList)
{
	if (pUserInfo == NULL)
	{
		return NULL;
	}
	GateInfo* pInfo = NULL;

	// 判断上次登录服务器是否可见
	if (pUserInfo->serverID != -1)
	{
		pInfo = GateMgr::Instance()->GetServerInfo(pUserInfo->serverID);
		if (pInfo && pInfo->CanSee(loginType, isInWhiteList))
		{
			return pInfo;
		}
	}

	// 从创角的服务器里找一个可见的
	for (auto it = pUserInfo->serverid2level.begin(); it != pUserInfo->serverid2level.end(); ++it)
	{
		pInfo = GateMgr::Instance()->GetServerInfo(it->first);
		if (pInfo == NULL)
		{
			continue;
		}
		if (pInfo->CanSee(loginType, isInWhiteList))
		{
			return pInfo;
		}
	}

	// 没有可见服务器, 返回默认推荐服
	return GateMgr::Instance()->GetRecommendServerInfo(loginType, isInWhiteList);
}

void GateMgr::FillServerGateInfo(KKSG::LoginType loginType, bool inWhiteList, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo)
{
	if (pUserInfo == NULL)
	{
		return;
	}
	for (auto it = m_gateInfos.rbegin(); it != m_gateInfos.rend(); ++it)
	{
		GateInfo& gate = it->second;
		if (!gate.CanSee(loginType, inWhiteList))
		{
			continue;
		}
		KKSG::LoginGateData* data = res.add_allservers();
		gate.FillGateInfo(pUserInfo->ccType, pUserInfo->isFreeFlow, *data);
	}
}

void GateMgr::FillDummyRecommendServerInfo(KKSG::LoginType loginType, bool isWhite, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo)
{
	if (pUserInfo == NULL)
	{
		return;
	}
	GateInfo* gateInfo = GetRecommendServerInfo(loginType, isWhite);
	if (gateInfo == NULL)
	{
		return;
	}
	KKSG::LoginGateData& data = *res.add_allservers();
	gateInfo->FillGateInfo(pUserInfo->ccType, pUserInfo->isFreeFlow, data);
	data.set_serverid(0);
	data.set_flag(6);	// 写死6， 老的测试服(GM可见)
	data.set_zonename(LoginConfig::Instance()->GetRecommendZoneName());
}

void GateMgr::FillRecommendServerInfo(KKSG::LoginType loginType, bool isWhite, KKSG::QueryGateRes& res, UserGateInfo* pUserInfo)
{
	if (pUserInfo == NULL)
	{
		return;
	}
	if (loginType >= KKSG::LoginType_ARRAYSIZE)
	{
		return ;
	}
	_RecommendInfo& info = m_recommendServer[loginType];
	for (size_t i = 0; i < info.serverIds.size(); ++i)
	{
		UINT32 id = info.serverIds[i];
		GateInfo* gate = GetServerInfo(id);
		if (gate == NULL)
		{
			continue;
		}
		KKSG::LoginGateData& data = *res.add_allservers();
		gate->FillGateInfo(pUserInfo->ccType, pUserInfo->isFreeFlow, data);
		data.set_zonename(LoginConfig::Instance()->GetRecommendZoneName());
	}
}

void GateMgr::ServerInfoNotifyToMs(UINT32 dwConnID/* = 0*/)
{
	SSInfo << " dwconnid = " << dwConnID << END;

	PtcI2M_ServerInfoNtf ptc;
	for(auto it = m_gateInfos.begin(); it != m_gateInfos.end(); ++it)
	{
		FillServerInfo(*ptc.m_Data.add_serverinfo(), it->second);
	}
	if (dwConnID)
	{
		MsLink::Instance()->Send(dwConnID, ptc);
	}
	else
	{
		MsLink::Instance()->Brocast(ptc);
	}
}

void GateMgr::ServerInfoNotifyToWorld(UINT32 dwConnID)
{
	PtcI2K_ServerInfoNtf2World ptc;
	for(auto it = m_gateInfos.begin(); it != m_gateInfos.end(); ++it)
	{
		FillServerInfo(*ptc.m_Data.add_serverinfo(), it->second);
	}

	if (dwConnID)
	{
		WorldLink::Instance()->Send(dwConnID, ptc);
	}
	else
	{
		WorldLink::Instance()->SendToAll(ptc);
	}
}

void GateMgr::FillServerInfo(KKSG::ServerInfo& data, GateInfo& info)
{
	data.set_id(info.serverID);
	data.set_name(info.serverName);
	data.set_zonename(info.zoneName);
}

void GateMgr::ServerInfoNotifyToIdip(KKSG::ServerInfoPara* pdata)
{
	if (pdata == NULL)
	{
		return;
	}
	for(auto it = m_gateInfos.begin(); it != m_gateInfos.end(); ++it)
	{
		FillServerInfo(*pdata->add_serverinfo(), it->second);
	}
}