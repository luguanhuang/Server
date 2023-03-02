#include "pch.h"
#include "livemanager.h"
#include "unit/role.h"
#include <time.h>
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "liveconfig.h"
#include "table/LiveTable.h"
#include "foreach.h"
#include "battlewatch/ptcg2c_watchbattleinfontf.h"
#include "battlewatch/ptcg2c_notifywatchdata.h"
#include "battlewatch/ptcg2m_syncallliveinfotoms.h"
#include "battlewatch/ptcg2m_syncliveinfotoms.h"
#include "unit/rolemanager.h"
#include "liverecord.h"
#include "scene/scenepvp.h"
#include "gamelogic/pkrecord.h"
#include "config.h"
#include "network/mslink.h"
#include "battlewatch/ptcg2c_notifywatchiconnum2client.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/dragonconfig.h"
#include <string>
#include "scene/sceneconfig.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "define/systemiddef.h"
#include "network/gatelink.h"
#include "broadcast/broadcast.h"
#include "servermgr/servermgr.h"
#include "scene/sceneherobattle.h"
#include "scene/sceneleague.h"
#include "scene/scenepk.h"
#include "scene/sceneteam.h"
#include "liveinfo.h"
#include "scene/scenepktwo.h"
#include "define/tlogenum.h"

INSTANCE_SINGLETON(LiveManager)

#define INVALID_LIVE_ID 0
#define UPDATE_INTERVAL 15*1000


LiveManager::LiveManager()
{
}

LiveManager::~LiveManager()
{

}

bool LiveManager::Init()
{
	for (int i = KKSG::LIVE_RECOMMEND; i < KKSG::LIVE_MAX; ++i)
	{
		switch(i)
		{
		case KKSG::LIVE_NEST:
			m_liveList[i] = new NestLiveList();
			break;
		case KKSG::LIVE_RECOMMEND:
			m_liveList[i] = new RecommendLiveList();
			break;
		case KKSG::LIVE_PVP:
			m_liveList[i] = new PVPLiveList();
			break;
		case KKSG::LIVE_PROTECTCAPTAIN:
			m_liveList[i] = new ProtectCaptainLiveList();
			break;
		case KKSG::LIVE_GUILDBATTLE:
			m_liveList[i] = new GuildBattleLiveList();
			break;
		case KKSG::LIVE_DRAGON:
			m_liveList[i] = new DragonLiveList();
			break;
		case KKSG::LIVE_HEROBATTLE:
			m_liveList[i] = new HeroBattleLiveList();
			break;
		case KKSG::LIVE_LEAGUEBATTLE:
			m_liveList[i] = new LeagueBattleLiveList();
			break;
		case KKSG::LIVE_PVP2:
			m_liveList[i] = new PVP2LiveList();
			break;
		case KKSG::LIVE_CUSTOMPK:
			m_liveList[i] = new CustomPkLiveList();
			break;
		default:
			break;
		}
	}

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, UPDATE_INTERVAL, -1, __FILE__, __LINE__);

	m_liveNumInMainHall = 0;

	return true;
}

void LiveManager::Uninit()
{
	foreach(i in m_liveList)
		delete i->second;
}

void LiveManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	foreach(i in m_liveList)
	{
		
		std::list<LiveInfo*>* list = (i->second->GetLiveList());
		for(auto j = list->begin(); j != list->end();)
		{
			if ((*j)->m_sceneID)
			{
				if (!SceneManager::Instance()->FindBySceneID((*j)->m_sceneID))
				{
					i->second->RemoveLiveInfo(*(j++));
					continue;
				}
			}
			++j;
		}
	}
}


void LiveManager::Update(float deltaTime)
{
}

LiveInfo* LiveManager::CreateLiveInfo(int liveType, int sceneID, std::vector<int>* pTeamID, std::vector<UINT64>* pGuildID, std::vector<UINT64>* pRoleIDs)
{
	LiveInfoList* list = GetLiveInfoList(liveType);
	if (!list)
	{
		return NULL;
	}
	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return NULL;
	}

	bool visible = _CheckVisible(liveType, sceneID, pTeamID, pGuildID, pRoleIDs);
	if (!visible)
	{
		return NULL;
	}

	LiveInfo* info = NULL;
	switch(liveType)
	{
	case KKSG::LIVE_NEST:
		if (sceneID && pTeamID && pRoleIDs->size() >= 2)
		{
			info = new NestLiveInfo(sceneID, pTeamID->at(0), *pRoleIDs);
			//info->m_liveID = NewLiveID();
		}
		break;
	case KKSG::LIVE_DRAGON:
		if (sceneID && pTeamID && pRoleIDs->size() >= 2)
		{
			info = new DragonLiveInfo(sceneID, pTeamID->at(0), *pRoleIDs);
		}
		break;
	case KKSG::LIVE_PVP:
		if (sceneID && pRoleIDs && pRoleIDs->size() == 2 && scene->GetPkHandler())
		{
			bool scoreEnough = false;
			foreach(i in (*pRoleIDs))
			{
				if (scene->GetPkHandler()->GetRolePoint(*i) >=  GetGlobalConfig().WatchPVPScore)
				{
					scoreEnough = true;
					break;
				}
			}
			if(scoreEnough)
				info = new PVPLiveInfo(sceneID, pRoleIDs->at(0), pRoleIDs->at(1));
		}
		break;
	case KKSG::LIVE_PROTECTCAPTAIN:
		if (sceneID)
		{
			info = new ProtectCaptainLiveInfo(sceneID);
		}
		break;
	case KKSG::LIVE_GUILDBATTLE:
		if (sceneID && pGuildID && pGuildID->size() == 3)
		{
			info = new GuildBattleLiveInfo(sceneID, pGuildID->at(0), pGuildID->at(1), pGuildID->at(2));
		}
		break;
	case KKSG::LIVE_HEROBATTLE:
		if (sceneID)
		{
			info = new HeroBattleLiveInfo(sceneID);
		}
		break;
	case KKSG::LIVE_LEAGUEBATTLE:
		if (sceneID)
		{
			info = new LeagueBattleLiveInfo(sceneID);
		}
		break;
	case KKSG::LIVE_PVP2:
		if (sceneID  && scene->GetPkTwoHandler())
		{
			if (!scene->GetPkTwoHandler()->GetVsCount())
			{
				break;
			}
			auto matchRole = scene->GetPkTwoHandler()->GetKMatchRole();
			int scoreA = 0;
			int scoreB = 0;
			int totalScore = 0;
			for (int i = 0; i < matchRole.roles_size(); ++i)
			{
				totalScore += matchRole.roles(i).pkrec().point();

				if (i == scene->GetPkTwoHandler()->GetVsCount() - 1)
				{
					scoreA = totalScore / scene->GetPkTwoHandler()->GetVsCount();
					totalScore = 0;
				}
				if (i == matchRole.roles_size() - 1)
				{
					scoreB = totalScore / scene->GetPkTwoHandler()->GetVsCount();
				}
			}
			bool scoreEnough = false;
			if (scoreA >=  GetGlobalConfig().WatchPVPScore || scoreB >= GetGlobalConfig().WatchPVPScore)
			{
				scoreEnough = true;
			}
			if(scoreEnough)
				info = new PVP2LiveInfo(sceneID, scoreA > scoreB ? scoreA : scoreB);
		}
	case KKSG::LIVE_CUSTOMPK:
		if (sceneID && scene->GetCustomPkHandler())
		{
			info = new CustomPkLiveInfo(sceneID);
		}
	default:
		break;
	}
	
	if (!info)
	{
		return NULL;
	}

	scene->SetLiveID(info->m_liveID);

	list->AddLiveInfo(info);

	//UpdateRecommendList(info);
	LogInfo("create liveinfo , liveid:%u, type:%u, sceneid:%u", info->m_liveID, info->m_liveType, info->m_sceneID);

	return info;
}

void LiveManager::UpdateRecommendList(LiveInfo* info, bool isAdd)
{
	LiveInfoList* list = GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (list)
	{
		if (isAdd)
		{
			list->AddLiveInfo(info);
		}
		else
			list->RemoveLiveInfo(info);
	}
}

LiveInfo* LiveManager::GetLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID)
{
	LiveInfoList* list = GetLiveInfoList(liveType);
	if (!list)
	{
		return NULL;
	}

	foreach(i in (*list->GetLiveList()))
	{
		switch(liveType)
		{
		case KKSG::LIVE_NEST:
		case KKSG::LIVE_PVP:
		case KKSG::LIVE_PROTECTCAPTAIN:
		case KKSG::LIVE_GUILDBATTLE:
		case KKSG::LIVE_DRAGON:
		case KKSG::LIVE_HEROBATTLE:
		case KKSG::LIVE_LEAGUEBATTLE:
		case KKSG::LIVE_PVP2:
		case KKSG::LIVE_CUSTOMPK:
			if (sceneID == (*i)->m_sceneID)
			{
				return (*i);
			}
			break;
		default:
			break;
		}
	}

	return NULL;
}


LiveInfo* LiveManager::GetLiveInfo(int liveType, UINT32 liveID)
{
	LiveInfoList* list = GetLiveInfoList(liveType);
	if (!list)
	{
		return NULL;
	}

	return list->GetLiveInfo(liveID);
}

bool LiveManager::RemoveLiveInfo(int liveType, int sceneID, int teamID, int guildID, UINT64 roleID)
{
	LiveInfoList* list = GetLiveInfoList(liveType);
	if (!list)
	{
		return false;
	}
	foreach(i in (*list->GetLiveList()))
	{
		bool find = false;
		switch(liveType)
		{
		case KKSG::LIVE_NEST:
		case KKSG::LIVE_PVP:
		case KKSG::LIVE_PROTECTCAPTAIN:
		case KKSG::LIVE_GUILDBATTLE:
		case KKSG::LIVE_DRAGON:
		case KKSG::LIVE_HEROBATTLE:
		case KKSG::LIVE_LEAGUEBATTLE:
		case KKSG::LIVE_PVP2:
		case KKSG::LIVE_CUSTOMPK:
			if (sceneID == (*i)->m_sceneID)
			{
				LogInfo("remove liveinfo , liveid:%u, type:%u, sceneid:%u", (*i)->m_liveID, (*i)->m_liveType, (*i)->m_sceneID);

				find = true;

				list->RemoveLiveInfo(*i);
				break;
			}
			break;
		default:
			break;
		}
		if (find)
		{
			break;
		}

	}

	return true;

}

UINT32 LiveManager::FillLiveInfo(int type, KKSG::GetWatchInfoByIDRes& roRes)
{
	LiveInfoList* list = GetLiveInfoList(type);
	if (!list)
	{
		return KKSG::ERR_FAILED;
	}

	roRes.set_curtime(time(0));
	
	return list->FillInfo2Client(roRes);
}

void LiveManager::OnEnterWatch(Role* role, UINT32 liveID)
{
	LiveInfo* info = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, liveID);
	if (!info)
	{
		return;
	}

	LiveInfoList* list = GetLiveInfoList(info->m_liveType);
	if (!list)
	{
		return;
	}

	info->AddWatchNum(1);
	NotifyWatchInfoChange(role->GetCurrScene(), info);
	SyncInfoToMs(liveID, false, true);

	list->Sort();

	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(info->m_liveType);
	if (data && role->GetCurrScene())
	{
		Scene* curScene = role->GetCurrScene();
		int watchNum = curScene->GetWatchRoleNum();
		if (watchNum >= data->NoticeWatchNum && !curScene->GetSendLiveNotice())
		{
			curScene->SetSendLiveNotice(true);
			info->SendLiveNotice();
		}
		
	}
}

void LiveManager::LeaveWatchBattle(Role* role, UINT32 liveID)
{
	LiveInfo* info = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, liveID);
	if (!info)
	{
		return;
	}

	LiveInfoList* list = GetLiveInfoList(info->m_liveType);
	if (!list)
	{
		return;
	}

	SyncInfoToMs(liveID, false, true);

	list->Sort();
}

LiveInfoList* LiveManager::GetLiveInfoList(int type)
{
	auto find = m_liveList.find(type);
	if (find == m_liveList.end())
	{
		return NULL;
	}

	return find->second;
}

UINT32 LiveManager::Commend(Role* role)
{
	if (!role->IsWatcher())
	{
		return KKSG::ERR_FAILED;
	}

	int now = time(0);
	if (!role->GetWatchLiveInfo().AddCommendCount(1))
	{
		return KKSG::ERR_COMMENDWATCH_COUNTLIMIT;
	}

	UINT32 liveID = role->GetCurrScene()->GetLiveID();

	LiveInfo* info = GetLiveInfo(KKSG::LIVE_RECOMMEND, liveID);
	if (info)
	{
		info->AddCommendNum(1);

		NotifyWatchInfoChange(role->GetCurrScene(), info);

		SyncInfoToMs(liveID, false, false);
	}
	role->DoChangeSceneLog(TXLikeSceneType);
	return KKSG::ERR_SUCCESS;
}

void LiveManager::NotifyWatchInfoChange(Scene* scene, LiveInfo* info)
{
	if (!info || !scene)
	{
		return;
	}
	PtcG2C_WatchBattleInfoNtf ntf;
	ntf.m_Data.set_commendnum(info->m_commendNum);
	ntf.m_Data.set_watchnum(info->m_watchNum);
	scene->Broadcast(ntf);
}

void LiveManager::EnterSceneNotifyWatchData(Role* role, Scene* scene)
{
	if (!scene || !role)
	{
		return;
	}
	if (!scene->GetLiveID())
	{
		return;
	}

	LiveInfo* info = GetLiveInfo(KKSG::LIVE_RECOMMEND, scene->GetLiveID());
	if (!info)
	{
		return;
	}

	PtcG2C_NotifyWatchData ntf;

	info->Convert2KKSG_OneLiveRecord(&ntf.m_Data);

	role->Send(ntf);
}

void LiveManager::SyncWatchDataToWatcher(Scene* scene, LiveInfo* info)
{
	if(!scene || !info)
	{
		return;
	}

	PtcG2C_NotifyWatchData ntf;

	info->Convert2KKSG_OneLiveRecord(&ntf.m_Data);

	scene->BroadcastWatcher(ntf);
}

void LiveManager::SyncInfoToMs(UINT32 liveID, bool isDelete, bool needSort)
{
	PtcG2M_SyncLiveInfoToMs ntf;
	if (isDelete)
	{
		ntf.m_Data.set_deletelist(liveID);
	}
	else
	{
		LiveInfo* info = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, liveID);
		if (info)
		{
			KKSG::OneLiveRecordInfo temp;
			if (info->Convert2KKSG_OneLiveRecord(&temp))
			{
				ntf.m_Data.mutable_updatelist()->CopyFrom(temp);
			}
		}
	}
	if (!ntf.m_Data.has_deletelist() && !ntf.m_Data.has_updatelist())
	{
		return;
	}
	ntf.m_Data.set_sort(needSort);

	//MSLink::Instance()->SendTo(ntf);
	_SyncToMs(ntf);
}

void LiveManager::SyncAllInfoToMs(UINT32 serverID)
{
	LiveInfoList* list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return;
	}
	auto liveInfoList = list->GetLiveList();
	PtcG2M_SyncAllLiveInfoToMs ntf;
	for (auto iter = liveInfoList->begin(); iter != liveInfoList->end(); ++iter)
	{
		LiveInfo* info = *iter;
		if (info)
		{
			KKSG::OneLiveRecordInfo temp;
			if (info->Convert2KKSG_OneLiveRecord(&temp))
			{
				ntf.m_Data.add_infos()->mutable_updatelist()->CopyFrom(temp);
			}
		}

	}

	if (!ntf.m_Data.infos_size())
	{
		return;
	}
	//MSLink::Instance()->SendTo(ntf);
	_SyncToMs(ntf, serverID);
}

void LiveManager::OnChange(UINT32 liveID)
{
	SyncInfoToMs(liveID, false, false);
}

void LiveManager::OnLeave(Role* role, UINT32 liveID)
{
	if (!role)
	{
		return;
	}

	UINT32 sceneID = 0;
	LiveInfoList* list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	LiveInfo* info = NULL;
	if (list)
	{
		info = list->GetLiveInfo(liveID);
		if (info)
		{
			list->UpdateRoleLiveRecord(info, role);
			sceneID = info->GetSceneID(liveID);
		}
		else
			return;
	}
	else
		return;

	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return;
	}

	const std::list<Role*>& roles = scene->GetAllRoles();
	if (roles.empty())
	{
		//SyncInfoToMs(liveID, true, false);
		RemoveLiveInfo(info->m_liveType, info->GetSceneID(liveID), 0, 0, 0);
	}
	else
		SyncInfoToMs(liveID, false, false);
}

void LiveManager::OnEnter(Role* role, UINT32 liveID)
{
	if (!role)
	{
		return;
	}

	LiveInfoList* list = GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	bool change = false;
	LiveInfo* info = NULL;

	if (list)
	{
		info = list->GetLiveInfo(liveID);
	}
	if (info)
	{
		change = info->OnEnterRole(role);
	}

	if (change)
	{
		SyncInfoToMs(liveID, false, false);

		SyncWatchDataToWatcher(role->GetCurrScene(), info);
	}
}

void LiveManager::OnFirstEnterNotifyIconNum(Role* role)
{
	if (!role)
	{
		return;
	}
	PtcG2C_NotifyWatchIconNum2Client send;
	send.m_Data.set_num(GetMainHallLiveNum());
	role->Send(send);
}

void LiveManager::_SyncToMs(CProtocol& protocl, UINT32 serverID)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		MSLink::Instance()->SendTo(protocl);
	}
	else
	{
		if (serverID)
		{
			ServerMgr::Instance()->SendTo(serverID, protocl);
		}
		else
		{
			ServerMgr::Instance()->SendToAll(protocl);
		}
	}
}

bool LiveManager::_CheckVisible(int liveType, int sceneID, std::vector<int>* teamID, std::vector<UINT64>* guildID, std::vector<UINT64>* roleIDs)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(liveType);
	if (!data)
	{
		return false;
	}
	
	if (data->SettingType == 0)
	{
		return true;
	}
	else if (data->SettingType == 1)
	{
		if (!roleIDs || roleIDs->empty())
		{
			return false;
		}
		bool visible = true;
		for (auto iter = roleIDs->begin(); iter != roleIDs->end(); ++iter)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(*iter);
			if (role && !role->Get<CLiveRecord>()->IsVisible())
			{
				visible = false;
				break;
			}
		}
		return visible;
	}
	else if (data->SettingType == 2)
	{
		if (!roleIDs || roleIDs->empty())
		{
			return false;
		}
		bool visible = true;
		auto iter = roleIDs->begin();
		Role* role = RoleManager::Instance()->FindByRoleID(*iter);
		if (role && !role->Get<CLiveRecord>()->IsVisible())
		{
			visible = false;
		}
		return visible;
	}

	return true;
}
