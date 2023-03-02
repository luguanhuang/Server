#include "pch.h"
#include "liveinfo.h"
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
#include "scene/scenepktwo.h"
#include "scene/scenecustompk.h"

#define INVALID_LIVE_ID 0
#define UPDATE_INTERVAL 15*1000

void FillBriefInfoFromRole(KKSG::RoleBriefInfo& info, Role* role);

void FillKKSGOneLiveRecordNameInfo(Role* role, bool isLeader, bool isLeft, KKSG::LiveNameInfo* nameInfo)
{
	if (!role || !nameInfo)
	{
		return;
	}
	if (isLeader)
	{
		nameInfo->set_teamleadername(role->GetName());
	}
	nameInfo->set_guildid(role->getGuildId());
	KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();
	brief->set_type((KKSG::RoleType)role->GetProfession());
	brief->set_roleid(role->GetID());
	brief->set_name(role->GetName());
	brief->set_level(role->GetLevel());
	nameInfo->set_isleft(isLeft);
}

bool CanCreate(int type)
{
	if (type != KKSG::LIVE_RECOMMEND && type != KKSG::LIVE_GUILD && type != KKSG::LIVE_FRIEND)
	{
		return true;
	}

	return false;
}

bool NeedDelete(int type)
{
	if (type != KKSG::LIVE_RECOMMEND && type != KKSG::LIVE_GUILD && type != KKSG::LIVE_FRIEND)
	{
		return true;
	}

	return false;
}

bool KKSGRecordShowUseRole(int type)
{
	if (type != KKSG::LIVE_GUILDBATTLE && type != KKSG::LIVE_LEAGUEBATTLE)
	{
		return true;
	}

	return false;
}

static UINT32 NewLiveID()
{
	UINT32 lineID = GSConfig::Instance()->GetLine();
	if (GSConfig::Instance()->IsCrossGS())
	{
		lineID += 100;
	}
	static UINT32 sLiveID = 0;

	++sLiveID;

	if (sLiveID == INVALID_LIVE_ID || sLiveID == 1 << 24)
	{
		sLiveID = 1;
	}
	UINT32 liveID = (lineID << 24) + sLiveID;

	return liveID;
}

void LiveInfo::AddCommendNum(UINT32 num)
{
	m_commendNum += num;

	const std::list<Role*>* roles = GetOnlineRoles();
	if (NULL != roles)
	{
		for (auto i = roles->begin(); i != roles->end(); ++i)
		{
			(*i)->Get<CLiveRecord>()->AddTotalCommend(num);
		}
	}
}

void LiveInfo::AddWatchNum(int num)
{
	m_watchNum += num;
	if (num > 0)
	{
		const std::list<Role*>* roles = GetOnlineRoles();
		if (NULL != roles)
		{
			for (auto i = roles->begin(); i != roles->end(); ++i)
			{
				(*i)->Get<CLiveRecord>()->AddTotalView(num);
			}
		}
	}
}

void LiveInfo::SendLiveNotice()
{
	LiveIconNtf(true);
}

bool LiveInfo::OnEnterRole(Role* role)
{
	if (!role)
	{
		return false;
	}
	if (!KKSGRecordShowUseRole(m_liveType))
	{
		return false;
	}

	auto find = m_roles.find(role->GetID());
	if (find == m_roles.end())
	{
		KKSG::RoleBriefInfo info;
		FillBriefInfoFromRole(info, role);
		m_roles.insert(std::make_pair(role->GetID(), info));
		return true;
	}

	return true;
}

void LiveInfo::LiveIconNtf(bool show)
{
	PtcG2C_HallIconSNtf ntf;
	ntf.m_Data.set_state(show ? KKSG::HICONS_BEGIN : KKSG::HICONS_END);
	ntf.m_Data.set_systemid(SYS_LIVEWATCH);
	KKSG::LiveIconData* liveData = ntf.m_Data.mutable_liveinfo();
	liveData->set_livetype(m_liveType);
	liveData->set_liveid(m_liveID);

	KKSG::OneLiveRecordInfo temp;
	if(Convert2KKSG_OneLiveRecord(&temp))
	{
		liveData->mutable_liveinfo()->CopyFrom(temp);
	}
	
	BroadCastMgr::BroadCast(ntf);
}

const std::list<Role*>* LiveInfo::GetOnlineRoles()
{
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		return &scene->GetAllRoles();
	}
	return NULL;
}


LiveInfo::LiveInfo(int sceneID)
{
	//memset(this, 0, sizeof(*this));
	m_liveType = 0;
	m_startTime = time(0);
	m_watchNum = 0;
	m_commendNum = 0;

	//m_teamID = 0;
	m_sceneID = sceneID;

	m_liveID = NewLiveID();

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
		m_sceneType = scene->GetSceneType();
	else
		m_sceneType = 0;
}

bool LiveInfo::operator<(const LiveInfo& info)
{
	UINT32 sceneID = GetSceneID(m_liveID);
	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return false;
	}
	UINT32 curWatchNum = scene->GetWatchRoleNum();

	sceneID = const_cast<LiveInfo&>(info).GetSceneID(info.m_liveID);
	scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return true;
	}
	UINT32 otherWatchNum = scene->GetWatchRoleNum();

	if (curWatchNum > otherWatchNum)
	{
		return true;
	}
	else if (curWatchNum == otherWatchNum)
	{
		if (this->m_startTime > info.m_startTime)
		{
			return true;
		}
		else if(this->m_startTime == info.m_startTime)
		{
			return this->m_sceneID > info.m_sceneID;
		}
	}

	return false;
}

bool LiveInfo::CheckVisible()
{
	UINT32 sceneID = GetSceneID(m_liveID);
	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return false;
	}
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return false;
	}
	
	bool visible = true;
	if (data->SettingType == 0)
	{
		return true;
	}
	else if (data->SettingType == 1)
	{
		const std::list<Role*>& roles = scene->GetAllRoles();
		if (roles.empty())
		{
			return false;
		}
		for (auto iter = roles.begin(); iter != roles.end(); ++iter)
		{
			if (*iter && !(*iter)->Get<CLiveRecord>()->IsVisible())
			{
				visible = false;
				break;
			}
		}
	}
	else if (data->SettingType == 2)
	{
		SceneTeam* team = scene->GetSceneTeam();
		if (!team)
		{
			visible = false;
		}
		Role* role = RoleManager::Instance()->FindByRoleID(team->GetLeaderID());
		if (!role)
		{
			return false;
		}
		if (!role->Get<CLiveRecord>()->IsVisible())
		{
			visible = false;
		}
	}

	if (!visible)
	{
		scene->SetLiveID(0);
		m_sceneID = 0;
	}

	return visible;
}

bool LiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}
	if (!CheckVisible())
	{
		return false;
	}
	info->set_liveid(m_liveID);
	info->set_watchnum(m_watchNum);
	info->set_commendnum(m_commendNum);
	info->set_begintime(m_startTime);
	info->set_livetype((KKSG::LiveType)m_liveType);
	info->set_hasfriend(false);
	info->set_hasguild(false);

	UINT32 sceneID = GetSceneID(m_liveID);
	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return false;
	}

	info->set_sceneid(GetSceneID(m_liveID));
	info->set_curwatchnum(scene->GetWatchRoleNum());
	info->set_mapid(scene->GetSceneTemplateID());
	info->set_iscross(GSConfig::Instance()->IsCrossGS());

	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (data)
	{
		UINT32 sceneid = GetSceneID(m_liveID);
		Scene* scene = SceneManager::Instance()->FindBySceneID(sceneid);
		if (scene && data->MaxWatchNum > scene->GetWatchRoleNum())
		{
			info->set_canenter(true);
		}
	}

	return true;
}

bool LiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}
	if (!CheckVisible())
	{
		return false;
	}
	info->set_liveid(m_liveID);
	info->set_watchnum(m_watchNum);
	info->set_commendnum(m_commendNum);
	info->set_begintime(m_startTime);
	info->set_livetype((KKSG::LiveType)m_liveType);
	info->set_hasfriend(false);
	info->set_hasguild(false);
	info->set_iscross(GSConfig::Instance()->IsCrossGS());

	return true;
}

UINT32 LiveInfo::GetSceneID(UINT32 liveID)
{
	return m_sceneID;
}

LiveInfoList::~LiveInfoList()
{
	foreach(i in m_liveInfo)
	{
		if (NeedDelete(m_liveType))
		{
			delete *i;
		}
	}
}
bool compare(LiveInfo* a, LiveInfo* b)
{
	return (*a) < (*b);
}

void LiveInfoList::AddLiveInfo(LiveInfo* info)
{
	m_liveInfo.push_back(info);

	//m_liveInfo.sort(compare);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		LiveManager::Instance()->UpdateRecommendList(info, true);

		LiveManager::Instance()->SyncInfoToMs(info->m_liveID, false);
	}
}

void LiveInfoList::BeforeRemove(LiveInfo* info)
{
	if (!info)
	{
		return;
	}

	if (m_liveType == KKSG::LIVE_RECOMMEND)
	{
		return;
	}

	Scene* scene = SceneManager::Instance()->FindBySceneID(info->GetSceneID(info->m_liveID));
	if (!scene)
	{
		return;
	}

	const std::list<Role*>& roles = scene->GetAllRoles();
	foreach(i in roles)
	{
		UpdateRoleLiveRecord(info, *i);
	}
}

void LiveInfoList::RemoveLiveInfo(LiveInfo* info)
{
	BeforeRemove(info);

	m_liveInfo.remove(info);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		LiveManager::Instance()->SyncInfoToMs(info->m_liveID, true);

		LiveManager::Instance()->UpdateRecommendList(info, false);

		info->LiveIconNtf(false);
	}
	if (NeedDelete(m_liveType))
	{
		//UpdateRoleLiveRecord(info);
		delete info;
	}
}

std::list<LiveInfo*>* LiveInfoList::GetLiveList()
{
	return &m_liveInfo;
}

UINT32 LiveInfoList::FillInfo2Client(KKSG::GetWatchInfoByIDRes& roRes)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	int num = 0;

	KKSG::OneLiveRecordInfo temp;
	foreach(i in m_liveInfo)
	{
		if (num >= data->LiveNum)
		{
			break;
		}
		temp.Clear();
		bool success = (*i)->Convert2KKSG_OneLiveRecord(&temp);
		if (!success)
		{
			continue;
		}

		KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
		info->CopyFrom(temp);
		
		++num;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 LiveInfoList::GetSceneID(UINT32 liveID)
{
	LiveInfo* info = GetLiveInfo(liveID);
	if (info)
	{
		return info->GetSceneID(liveID);
	}

	return 0;
}

LiveInfo* LiveInfoList::GetLiveInfo(UINT32 liveID)
{
	foreach(i in m_liveInfo)
	{
		if((*i)->m_liveID == liveID)
			return *i;
	}

	return NULL;
}

void LiveInfoList::AddWatchNum(UINT32 liveID)
{
	LiveInfo* info = GetLiveInfo(liveID);
	if (info)
	{
		info->AddWatchNum(1);
	}
}

void LiveInfoList::AddCommendNum(UINT32 liveID)
{
	LiveInfo* info = GetLiveInfo(liveID);
	if (info)
	{
		info->AddCommendNum(1);
	}
}

UINT32 LiveInfoList::GetWatchNum(UINT32 liveID)
{
	LiveInfo* info = GetLiveInfo(liveID);
	if (info)
	{
		info->GetWatchNum();
	}

	return 0;
}

void LiveInfoList::UpdateRoleLiveRecord(LiveInfo* info, Role* role)
{
	if (!info || !role)
	{
		return;
	}

	role->Get<CLiveRecord>()->UpdateRecord(info);
}

UINT32 LiveInfoList::GetCommendNum(UINT32 liveID)
{
	LiveInfo* info = GetLiveInfo(liveID);
	if (info)
	{
		info->GetCommendNum();
	}

	return 0;
}

void LiveInfoList::Sort()
{
	m_liveInfo.sort(compare);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		auto list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
		if (list)
		{
			list->Sort();
		}
	}

}

TeamLiveInfo::TeamLiveInfo(int liveType, int sceneID, int expID, std::vector<UINT64>& roles):LiveInfo(sceneID)
{
	m_liveType = liveType;

	m_expID = expID;
	m_leaderID = 0;
	if (roles.size() >= 2)
	{
		m_leaderID = roles[0];
	}
	for (size_t i = 1; i < roles.size(); ++i)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(roles[i]);
		if (!role)
		{
			continue;
		}
		KKSG::RoleBriefInfo info;
		FillBriefInfoFromRole(info, role);
		m_roles.insert(std::make_pair(roles[i], info));
	}
}

bool TeamLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}
	const std::list<Role*>& roles = scene->GetAllRoles();
	{
		info->set_dnexpid(m_expID);
		bool hasLeader = false;
		foreach (j in roles)
		{
			Role* role = *j;
			if (role)
			{
				bool isLeader = false;
				KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
				if (role->GetID() == m_leaderID)
				{
					isLeader = true;
					hasLeader = true;
				}
				if (!hasLeader && *j == *roles.rbegin())
				{
					isLeader = true;
				}
				FillKKSGOneLiveRecordNameInfo(role, isLeader, true, nameInfo);
			}
		}
	}

	if (!info->nameinfos_size())
	{
		return false;
	}

	return true;
}

bool TeamLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	info->set_dnexpid(m_expID);
	foreach (j in m_roles)
	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		if (j->first == m_leaderID)
		{
			nameInfo->set_teamleadername(j->second.name());
		}
		KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

		brief->CopyFrom(j->second);
		nameInfo->set_isleft(true);
	}

	if (!info->nameinfos_size())
	{
		return false;
	}

	return true;
}


NestLiveInfo::NestLiveInfo(int sceneID, int expID, std::vector<UINT64>& roles):TeamLiveInfo(KKSG::LIVE_NEST, sceneID, expID, roles)
{
}

void NestLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Role* role = RoleManager::Instance()->FindByRoleID(m_leaderID);
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	auto table = ExpeditionConfigMgr::Instance()->GetExpeditionConf(m_expID);
	if (role && table && data && table->RandomSceneIDs.size())
	{
		UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetRandomScene(table->RandomSceneIDs[0]);
		auto sceneConf = SceneConfig::Instance()->FindSceneInfo(sceneID);
		if (!sceneConf || !sceneConf->m_pConf)
		{
			return;
		}
		std::stringstream ss;
		ss << sceneConf->m_pConf->Comment;

		NoticeMgr::Instance()->NoticeSendNestLiveWatch(role, ss.str(), data->NoticeWatchNum);

	}
}

NestLiveList::NestLiveList()
{
	m_liveType = KKSG::LIVE_NEST;
}


RecommendLiveList::RecommendLiveList()
{
	m_liveType = KKSG::LIVE_RECOMMEND;
}

PVPLiveInfo::PVPLiveInfo(UINT32 sceneID, UINT64 roleID1, UINT64 roleID2):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_PVP;

	m_roleID1 = roleID1;
	m_roleID2 = roleID2;
	m_pkPoint = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (scene && scene->GetPkHandler())
	{
		m_pkPoint = scene->GetPkHandler()->GetRolePoint(m_roleID1);

		KKSG::RoleBriefInfo info1;
		scene->GetPkHandler()->FillRoleBrief(m_roleID1, info1);
		m_roles.insert(std::make_pair(m_roleID1, info1));

		int temp = scene->GetPkHandler()->GetRolePoint(m_roleID2);
		if (temp > m_pkPoint)
		{
			m_pkPoint = temp;
		}

		KKSG::RoleBriefInfo info2;
		scene->GetPkHandler()->FillRoleBrief(m_roleID2, info2);
		m_roles.insert(std::make_pair(m_roleID2, info2));
	}
}

bool PVPLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}
	const std::list<Role*>& roles = scene->GetAllRoles();
	if (roles.size() < 2)
	{
		return false;
	}
	info->set_tiantilevel(m_pkPoint);
	foreach (j in roles)
	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		bool isLeft = (*j)->GetID() == m_roleID1;
		FillKKSGOneLiveRecordNameInfo((*j), false, isLeft, nameInfo);
	}

	return true;
}

bool PVPLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	info->set_tiantilevel(m_pkPoint);

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}

	if (m_roles.size() < 2)
	{
		return false;
	}
	foreach (j in m_roles)
	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

		brief->CopyFrom(j->second);

		if (j->first == m_roleID1)
		{
			nameInfo->set_isleft(true);
		}
		else
			nameInfo->set_isleft(false);
	}

	return true;
}


void PVPLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Role* role = RoleManager::Instance()->FindByRoleID(m_roleID1);
	Role* role2 = RoleManager::Instance()->FindByRoleID(m_roleID2);
	if (role && role2)
	{
		NoticeMgr::Instance()->NoticeSend1V1LiveWatch(role, role2);
	}
}

PVPLiveList::PVPLiveList()
{
	m_liveType = KKSG::LIVE_PVP;
}

ProtectCaptainLiveInfo::ProtectCaptainLiveInfo(UINT32 sceneID):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_PROTECTCAPTAIN;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetPvpHandler();
		if (!handler)
		{
			return;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleid);
			if (!role)
			{
				continue;
			}
			KKSG::RoleBriefInfo info;
			FillBriefInfoFromRole(info, role);
			m_roles.insert(std::make_pair(role->GetID(), info));
		}
	}
}

bool ProtectCaptainLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetPvpHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleid);
			if (!role)
			{
				continue;
			}
			if (role->GetCurrScene())
			{
				if (role->GetCurrScene()->GetSceneID() != m_sceneID)
				{
					continue;
				}
			}
			else
				continue;
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			if (j->second.isleader)
			{
				++leaderNum;
			}
			if (j->second.camp == PVP_CAMP_RED)
			{
				left = true;
			}
			else
			{
				right = true;
			}
			FillKKSGOneLiveRecordNameInfo(role, j->second.isleader, j->second.camp == PVP_CAMP_RED, nameInfo);
		}
	}
	if (!right || !left || leaderNum < 2)
	{
		return false;
	}

	return true;
}

bool ProtectCaptainLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetPvpHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			auto find = m_roles.find(j->second.roleid);
			if (find == m_roles.end())
			{
				continue;
			}
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

			brief->CopyFrom(find->second);

			if (j->second.isleader)
			{
				nameInfo->set_teamleadername(find->second.name());
				++leaderNum;
			}
			if (j->second.camp == PVP_CAMP_RED)
			{
				nameInfo->set_isleft(true);
				left = true;
			}
			else
			{
				nameInfo->set_isleft(false);
				right = true;
			}
		}
	}
	if (!right || !left || leaderNum < 2)
	{
		return false;
	}

	return true;
}

void FillBriefInfoFromRole(KKSG::RoleBriefInfo& info, Role* role)
{
	if (!role)
	{
		return;
	}
	info.set_type((KKSG::RoleType)role->GetProfession());
	info.set_roleid(role->GetID());
	info.set_name(role->GetName());
	info.set_level(role->GetLevel());
}

void ProtectCaptainLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return;
	}
	auto handler = scene->GetPvpHandler();
	if (!handler)
	{
		return;
	}
	Role* role1 = NULL;
	Role* role2 = NULL;
	auto data = handler->GetRoleDatas();
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		if (iter->second.isleader)
		{
			Role* temp = RoleManager::Instance()->FindByRoleID(iter->second.roleid);
			if (!role1)
			{
				role1 = temp;
			}
			else
				role2 = temp;
			if (role1 && role2)
			{
				break;
			}
		}
	}
	if (role1 && role2)
	{
		NoticeMgr::Instance()->NoticeSendProtectCaptainLiveWatch(role1, role2);
	}
}

ProtectCaptainLiveList::ProtectCaptainLiveList()
{
	m_liveType = KKSG::LIVE_PROTECTCAPTAIN;
}

DragonLiveInfo::DragonLiveInfo(int sceneID, int expID, std::vector<UINT64>& roles):TeamLiveInfo(KKSG::LIVE_DRAGON, sceneID, expID, roles)
{
}

void DragonLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Role* role = RoleManager::Instance()->FindByRoleID(m_leaderID);
	auto table = ExpeditionConfigMgr::Instance()->GetExpeditionConf(m_expID);
	const std::string* name = DragonConfig::Instance()->GetDragonTypeName(m_expID);
	if (role && table && name)
	{
		std::stringstream ss;
		ss << *name << "-";
		ss << table->DNExpeditionName;

		NoticeMgr::Instance()->NoticeSendDragonLiveWatch(role, ss.str());
	}
}

DragonLiveList::DragonLiveList()
{
	m_liveType = KKSG::LIVE_DRAGON;
}

GuildBattleLiveInfo::GuildBattleLiveInfo(UINT32 sceneID, UINT64 guildID1, UINT64 guildID2, UINT32 curLevel):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_GUILDBATTLE;
	m_guildID1 = guildID1;
	m_guildID2 = guildID2;
	m_curLevel = curLevel;
}

bool GuildBattleLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	return _Convert2OneLiveRecord(info);
}

bool GuildBattleLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	return _Convert2OneLiveRecord(info);
}

bool GuildBattleLiveInfo::_Convert2OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}

	std::vector<SceneGuildInfo*> guilds;
	SceneGuildInfo* guild = scene->GetSceneGuildInfo(m_guildID1);
	if (!guild)
	{
		return false;
	}
	guilds.push_back(guild);
	
	guild = scene->GetSceneGuildInfo(m_guildID2);
	if (!guild)
	{
		return false;
	}
	guilds.push_back(guild);

	info->set_guildbattlelevel(m_curLevel);
	foreach (j in guilds)
	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		nameInfo->set_guildid((*j)->guildID);
		nameInfo->set_guildname((*j)->guildName);
		nameInfo->set_guildicon((*j)->guildIcon);
		if ((*j)->guildID == m_guildID1)
		{
			nameInfo->set_isleft(true);
		}
		else
			nameInfo->set_isleft(false);
	}

	return true;
}

void GuildBattleLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	NoticeMgr::Instance()->NoticeSendGuildBattleLiveWatch(m_guildID1, m_guildID2);
}

GuildBattleLiveList::GuildBattleLiveList()
{
	m_liveType = KKSG::LIVE_GUILDBATTLE;
}


HeroBattleLiveInfo::HeroBattleLiveInfo(UINT32 sceneID):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_HEROBATTLE;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetHeroBattleHandler();
		if (!handler)
		{
			return;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleid);
			if (!role)
			{
				continue;
			}
			KKSG::RoleBriefInfo info;
			FillBriefInfoFromRole(info, role);
			m_roles.insert(std::make_pair(role->GetID(), info));
		}
	}
}

bool HeroBattleLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetHeroBattleHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleid);
			if (!role)
			{
				continue;
			}
			if (role->GetCurrScene())
			{
				if (role->GetCurrScene()->GetSceneID() != m_sceneID)
				{
					continue;
				}
			}
			else
				continue;
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();

			bool isLeader = false;
			bool isLeft = false;
			if (j->second.info.camp() == handler->GetTeam1())
			{
				if (!left)
				{
					isLeader = true;
					++leaderNum;
				}
				left = true;
				isLeft = true;
			}
			else
			{
				if (!right)
				{
					isLeader = true;
					++leaderNum;
				}
				right = true;
			}
			FillKKSGOneLiveRecordNameInfo(role, isLeader, isLeft, nameInfo);
		}
	}
	if (!right || !left || leaderNum < 2)
	{
		return false;
	}

	return true;
}

bool HeroBattleLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetHeroBattleHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			auto find = m_roles.find(j->second.roleid);
			if (find == m_roles.end())
			{
				continue;
			}
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

			brief->CopyFrom(find->second);

			if (j->second.info.camp() == handler->GetTeam1())
			{
				nameInfo->set_isleft(true);

				if (!left)
				{
					nameInfo->set_teamleadername(find->second.name());
					++leaderNum;
				}
				left = true;
			}
			else
			{
				nameInfo->set_isleft(false);
				if (!right)
				{
					nameInfo->set_teamleadername(find->second.name());
					++leaderNum;
				}
				right = true;
			}
		}
	}
	if (!right || !left || leaderNum < 2)
	{
		return false;
	}

	return true;
}

void HeroBattleLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Role* learder1 = NULL;
	Role* learder2 = NULL;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetHeroBattleHandler();
		if (!handler)
		{
			return;
		}
		auto data = handler->GetRoleDatas();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleid);
			if (!role)
			{
				continue;
			}
			if (role->GetCurrScene())
			{
				if (role->GetCurrScene()->GetSceneID() != m_sceneID)
				{
					continue;
				}
			}
			else
				continue;
			if (j->second.info.camp() == handler->GetTeam1())
			{
				if (!learder1)
				{
					learder1 = role;
				}
				else
				{
					continue;
				}
			}
			else
			{
				if (!learder2)
				{
					learder2 = role;
				}
				else
				{
					continue;
				}
			}
		}
	}
	if (!learder1 || !learder2)
	{
		return;
	}
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (data)
	{
		NoticeMgr::Instance()->NoticeSendHeroBattleLiveWatch(learder1, learder2, data->NoticeWatchNum);
	}
}

HeroBattleLiveList::HeroBattleLiveList()
{
	m_liveType = KKSG::LIVE_HEROBATTLE;
}


LeagueBattleLiveInfo::LeagueBattleLiveInfo(UINT32 sceneID) : LiveInfo(sceneID), m_leagueName1(""), m_leagueName2("") 
{
	m_liveType = KKSG::LIVE_LEAGUEBATTLE;
	Scene* scene = SceneManager::Instance()->FindBySceneID(sceneID);
	if (!scene)
	{
		return;
	}

	SceneLeague* handler = scene->GetLeagueBattleHandler();
	if (!handler)
	{
		return;
	}

	auto& roomInfo = handler->GetLeagueRoomInfo();
	m_leagueName1 = roomInfo.GetRedTeam().m_name;
	m_leagueName2 = roomInfo.GetBlueTeam().m_name;
}


bool LeagueBattleLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	return _Convert2OneLiveRecord(info);
}

bool LeagueBattleLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))return false;

	return _Convert2OneLiveRecord(info);
}

bool LeagueBattleLiveInfo::_Convert2OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return false;
	}

	SceneLeague* handler = scene->GetLeagueBattleHandler();
	if (!handler)
	{
		return false;
	}

	auto& roomInfo = handler->GetLeagueRoomInfo();

	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		nameInfo->set_teamname(m_leagueName1);
		nameInfo->set_leagueid(roomInfo.GetRedTeam().m_leagueTeamId);
		nameInfo->set_isleft(true);
	}

	{
		KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
		nameInfo->set_teamname(m_leagueName2);
		nameInfo->set_leagueid(roomInfo.GetBlueTeam().m_leagueTeamId);
		nameInfo->set_isleft(false);
	}

	return true;
}

void LeagueBattleLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return;
	}
	NoticeMgr::Instance()->NoticeSendLeagueBattleLiveWatch(m_leagueName1, m_leagueName2, data->NoticeWatchNum);
}

LeagueBattleLiveList::LeagueBattleLiveList()
{
	m_liveType = KKSG::LIVE_LEAGUEBATTLE;
}

PVP2LiveInfo::PVP2LiveInfo(UINT32 sceneID, UINT32 score):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_PVP2;
	m_pkPoint = score;
}

bool PVP2LiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	bool left = false;
	bool right = false;
	UINT64 leftLeaderID = 0;
	UINT64 rightLeaderID = 0;

	info->set_tiantilevel(m_pkPoint);
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetPkTwoHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleData();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->second.roleID);
			if (!role)
			{
				continue;
			}
			if (role->GetCurrScene())
			{
				if (role->GetCurrScene()->GetSceneID() != m_sceneID)
				{
					continue;
				}
			}
			else
				continue;
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			if (j->second.group == 1)
			{
				left = true;
				if (!leftLeaderID)
				{
					leftLeaderID = j->second.roleID;
				}
			}
			else
			{
				right = true;
				if (!rightLeaderID)
				{
					rightLeaderID = j->second.roleID;
				}
			}
			FillKKSGOneLiveRecordNameInfo(role, j->second.roleID == leftLeaderID || j->second.roleID == rightLeaderID, j->second.group == 1, nameInfo);
		}
	}
	if (!right || !left)
	{
		return false;
	}

	return true;
}

bool PVP2LiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))
	{
		return false;
	}
	bool left = false;
	bool right = false; 

	info->set_tiantilevel(m_pkPoint);
	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetPkTwoHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleData();
		foreach (j in data)
		{
			auto find = m_roles.find(j->second.roleID);
			if (find == m_roles.end())
			{
				continue;
			}
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			if (j->second.group == 1)
			{
				left = true;
				nameInfo->set_isleft(true);
			}
			else
			{
				right = true;
				nameInfo->set_isleft(false);
			}

			KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

			brief->CopyFrom(find->second);
		}

	}
	if (!right || !left)
	{
		return false;
	}

	return true;
}

void PVP2LiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return;
	}
	auto handler = scene->GetPkTwoHandler();
	if (!handler)
	{
		return;
	}
	Role* role1 = NULL;
	Role* role2 = NULL;
	auto data = handler->GetRoleData();
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		if (iter->second.group == 1 && !role1)
		{
			role1 = RoleManager::Instance()->FindByRoleID(iter->second.roleID);
		}
		if (iter->second.group == 2 && !role2)
		{
			role2 = RoleManager::Instance()->FindByRoleID(iter->second.roleID);
		}
		if (role1 && role2)
		{
			break;
		}
	}
	if (role1 && role2)
	{
		NoticeMgr::Instance()->NoticeSend2V2LiveWatch(role1, role2);
	}
}

PVP2LiveList::PVP2LiveList()
{
	m_liveType = KKSG::LIVE_PVP2;
}

CustomPkLiveInfo::CustomPkLiveInfo(UINT32 sceneID):LiveInfo(sceneID)
{
	m_liveType = KKSG::LIVE_CUSTOMPK;
}

bool CustomPkLiveInfo::Convert2KKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2KKSG_OneLiveRecord(info))
		return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetCustomPkHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleInfo();
		foreach (j in data)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(j->first);
			if (!role)
			{
				return false;
			}
			if (role->GetCurrScene())
			{
				if (role->GetCurrScene()->GetSceneID() != m_sceneID)
				{
					return false;
				}
			}
			else
				return false;
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			if (!left)
			{
				left = true;
			}
			else
			{
				right = true;
			}
			FillKKSGOneLiveRecordNameInfo(role, false, left&&!right, nameInfo);
		}
	}
	if (!right || !left)
	{
		return false;
	}

	return true;

}

bool CustomPkLiveInfo::Convert2DBKKSG_OneLiveRecord(KKSG::OneLiveRecordInfo* info)
{
	if (!info)
	{
		return false;
	}

	if(!LiveInfo::Convert2DBKKSG_OneLiveRecord(info))
		return false;

	bool left = false;
	bool right = false;
	int leaderNum = 0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (scene)
	{
		auto handler = scene->GetCustomPkHandler();
		if (!handler)
		{
			return false;
		}
		auto data = handler->GetRoleInfo();
		foreach (j in data)
		{
			auto find = m_roles.find(j->first);
			if (find == m_roles.end())
			{
				continue;
			}
			KKSG::LiveNameInfo* nameInfo = info->add_nameinfos();
			if (!left)
			{
				left = true;
				nameInfo->set_isleft(true);
			}
			else
			{
				right = true;
				nameInfo->set_isleft(false);
			}

			KKSG::RoleBriefInfo* brief = nameInfo->mutable_roleinfo();

			brief->CopyFrom(find->second);
		}
	}
	if (!right || !left)
	{
		return false;
	}

	return true;
}

void CustomPkLiveInfo::SendLiveNotice()
{
	LiveInfo::SendLiveNotice();

	Scene* scene = SceneManager::Instance()->FindBySceneID(m_sceneID);
	if (!scene)
	{
		return;
	}
	auto handler = scene->GetCustomPkHandler();
	if (!handler)
	{
		return;
	}
	Role* role1 = NULL;
	Role* role2 = NULL;
	auto data = handler->GetRoleInfo();
	for (auto iter = data.begin(); iter != data.end(); ++iter)
	{
		if (!role1)
		{
			role1 = RoleManager::Instance()->FindByRoleID(iter->first);
		}
		if (!role2)
		{
			role2 = RoleManager::Instance()->FindByRoleID(iter->first);
		}
		if (role1 && role2)
		{
			break;
		}
	}
	if (role1 && role2)
	{
		NoticeMgr::Instance()->NoticeSendCustomPkLiveWatch(role1, role2);
	}

}

CustomPkLiveList::CustomPkLiveList()
{
	m_liveType = KKSG::LIVE_CUSTOMPK;
}
