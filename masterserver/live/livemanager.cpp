#include "pch.h"
#include "livemanager.h"
#include <time.h>
#include "live/liveconfig.h"
#include "table/LiveTable.h"
#include "foreach.h"
#include "battlewatch/ptcg2c_watchbattleinfontf.h"
#include "battlewatch/ptcg2c_notifywatchdata.h"
#include "role/role.h"
#include "scene/scenemanager.h"
#include "battlewatch/ptcm2g_notifywatchiconnum.h"
#include "network/gslink.h"
#include "friend/friendop.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

INSTANCE_SINGLETON(LiveManager)

#define INVALID_LIVE_ID 0
#define UPDATE_INTERVAL 15*1000

bool CanCreate(int type)
{
	if (type != KKSG::LIVE_RECOMMEND && type != KKSG::LIVE_GUILD && type != KKSG::LIVE_FRIEND && type != KKSG::LIVE_FRIENDANDGUILD)
	{
		return true;
	}

	return false;
}

bool NeedDelete(int type)
{
	if (type != KKSG::LIVE_RECOMMEND && type != KKSG::LIVE_GUILD && type != KKSG::LIVE_FRIEND && type != KKSG::LIVE_FRIENDANDGUILD)
	{
		return true;
	}

	return false;
}

bool compare(KKSG::OneLiveRecordInfo* a, KKSG::OneLiveRecordInfo* b)
{
	if (a->curwatchnum() > b->curwatchnum())
	{
		return true;
	}
	else if (a->curwatchnum() == b->curwatchnum())
	{
		if (a->begintime() > b->begintime())
		{
			return true;
		}
	}

	return false;
}

bool compareNest(KKSG::OneLiveRecordInfo* a, KKSG::OneLiveRecordInfo* b)
{
	CScene* aScene = CSceneManager::Instance()->GetScene(a->sceneid());
	if (!aScene)
	{
		return false;
	}
	CScene* bScene = CSceneManager::Instance()->GetScene(b->sceneid());
	if (!bScene)
	{
		return true;
	}
	UINT32 aSceneType = aScene->GetSceneType();
	UINT32 bSceneType = bScene->GetSceneType();
	if (aSceneType != bSceneType)
	{
		if (aSceneType == KKSG::SCENE_NEST && bSceneType == SCENE_WEEK_NEST)
		{
			return false;
		}
		if (aSceneType == KKSG::SCENE_WEEK_NEST && bSceneType == KKSG::SCENE_NEST)
		{
			return true;
		}
	}

	if (a->curwatchnum() > b->curwatchnum())
	{
		return true;
	}
	else if (a->curwatchnum() == b->curwatchnum())
	{
		if (a->begintime() > b->begintime())
		{
			return true;
		}
	}

	return false;
}

static UINT32 NewLiveID()
{
	static UINT32 sLiveID = 0;
	++sLiveID;
	if (sLiveID == INVALID_LIVE_ID)
	{
		++sLiveID;
	}

	return sLiveID;
}

bool CheckLiveTypeConfigExist(int type)
{
	auto table = LiveConfig::Instance()->GetLiveTableData(type);
	if (!table)
	{
		return false;
	}
	
	if (!table->LiveNum)
	{
		return false;
	}

	return true;
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

void LiveInfoList::AddLiveInfo(KKSG::OneLiveRecordInfo* info)
{
	m_liveInfo.push_back(info);

	//m_liveInfo.sort(compare);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		LiveManager::Instance()->UpdateRecommendList(info, true);
	}
}

void LiveInfoList::RemoveLiveInfo(KKSG::OneLiveRecordInfo* info)
{
	m_liveInfo.remove(info);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
		LiveManager::Instance()->UpdateRecommendList(info, false);

	if (NeedDelete(m_liveType))
	{
		//UpdateRoleLiveRecord(info);
		delete info;
	}
}

std::list<KKSG::OneLiveRecordInfo*>* LiveInfoList::GetLiveList()
{
	return &m_liveInfo;
}

UINT32 LiveInfoList::FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	int num = 0;

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		if (!CheckLiveTypeConfigExist(m_liveType))
		{
			return KKSG::ERR_SUCCESS;
		}
	}

	//KKSG::OneLiveRecordInfo temp;
	foreach(i in m_liveInfo)
	{
		if (num >= data->LiveNum)
		{
			break;
		}
		if (m_liveType == KKSG::LIVE_RECOMMEND && !CheckLiveTypeConfigExist((*i)->livetype()))
		{
			continue;
		}

		KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
		info->CopyFrom(**i);

		bool hasFriend = HasFriend(role, **i);

		info->set_hasfriend(hasFriend);
		info->set_hasguild(HasGuild(role, **i));
		
		++num;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 LiveInfoList::GetSceneID(UINT32 liveID)
{
	KKSG::OneLiveRecordInfo* info = GetLiveInfo(liveID);
	if (!info)
	{
		return 0;
	}

	return info->sceneid();
}

KKSG::OneLiveRecordInfo* LiveInfoList::GetLiveInfo(UINT32 liveID)
{
	foreach(i in m_liveInfo)
	{
		if((*i)->liveid() == liveID)
			return *i;
	}

	return NULL;
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

bool LiveInfoList::HasFriend(CRole* role, KKSG::OneLiveRecordInfo& info)
{
	if (!role)
	{
		return false;
	}
	bool hasFriend = false;
	FriendOp op(role->GetID());
	for (int i = 0; i < info.nameinfos_size(); ++i)
	{
		if (info.nameinfos(i).has_roleinfo())
		{
			if (op.IsFriend(info.nameinfos(i).roleinfo().roleid()))
			{
				hasFriend = true;
				break;
			}
		}
	}

	return hasFriend;
}

bool LiveInfoList::HasGuild(CRole* role, KKSG::OneLiveRecordInfo& info)
{
	if (!role)
	{
		return false;
	}
	bool hasGuild = false;
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild)
	{
		return false;
	}
	for (int i = 0; i < info.nameinfos_size(); ++i)
	{
		if (info.nameinfos(i).has_guildid())
		{
			if(pGuild->GetID() == info.nameinfos(i).guildid())
			{
				hasGuild = true;
				break;
			}
		}
	}

	return hasGuild;
}


NestLiveList::NestLiveList()
{
	m_liveType = KKSG::LIVE_NEST;
}

void NestLiveList::Sort()
{
	m_liveInfo.sort(compareNest);

	if (m_liveType != KKSG::LIVE_RECOMMEND)
	{
		auto list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
		if (list)
		{
			list->Sort();
		}
	}
}

DragonLiveList::DragonLiveList()
{
	m_liveType = KKSG::LIVE_DRAGON;
}


RecommendLiveList::RecommendLiveList()
{
	m_liveType = KKSG::LIVE_RECOMMEND;
}

FriendLiveList::FriendLiveList()
{
	m_liveType = KKSG::LIVE_FRIEND;
}

UINT32 FriendLiveList::FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	LiveInfoList* list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return KKSG::ERR_FAILED;
	}
	std::list<KKSG::OneLiveRecordInfo*>* infoList = list->GetLiveList();
	if (!infoList)
	{
		return KKSG::ERR_FAILED;
	}
	int num = 0;

	m_liveInfo.clear();
	
	foreach(i in (*infoList))
	{
		if (num >= data->LiveNum)
		{
			break;
		}
		if (!CheckLiveTypeConfigExist((*i)->livetype()))
		{
			continue;
		}

		if (!HasFriend(role, **i))
		{
			continue;
		}

		KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
		info->CopyFrom(**i);
		
		++num;
	}

	return KKSG::ERR_SUCCESS;
}

GuildLiveList::GuildLiveList()
{
	m_liveType = KKSG::LIVE_GUILD;
}

UINT32 GuildLiveList::FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	LiveInfoList* list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return KKSG::ERR_FAILED;
	}
	std::list<KKSG::OneLiveRecordInfo*>* infoList = list->GetLiveList();
	if (!infoList)
	{
		return KKSG::ERR_FAILED;
	}
	int num = 0;

	m_liveInfo.clear();
	
	foreach(i in (*infoList))
	{
		if (num >= data->LiveNum)
		{
			break;
		}
		if (!CheckLiveTypeConfigExist((*i)->livetype()))
		{
			continue;
		}

		if (!HasGuild(role, **i))
		{
			continue;
		}

		KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
		info->CopyFrom(**i);
		
		++num;
	}

	return KKSG::ERR_SUCCESS;
}


PVPLiveList::PVPLiveList()
{
	m_liveType = KKSG::LIVE_PVP;
}

PVP2LiveList::PVP2LiveList()
{
	m_liveType = KKSG::LIVE_PVP2;
}


ProtectCaptainLiveList::ProtectCaptainLiveList()
{
	m_liveType = KKSG::LIVE_PROTECTCAPTAIN;
}

GuildBattleLiveList::GuildBattleLiveList()
{
	m_liveType = KKSG::LIVE_GUILDBATTLE;
}

HeroBattleLiveList::HeroBattleLiveList()
{
	m_liveType = KKSG::LIVE_HEROBATTLE;
}

LeagueBattleLiveList::LeagueBattleLiveList()
{
	m_liveType = KKSG::LIVE_LEAGUEBATTLE;
}

CustomPkLiveList::CustomPkLiveList()
{
	m_liveType = KKSG::LIVE_CUSTOMPK;
}

FriendAndGuildLiveList::FriendAndGuildLiveList()
{
	m_liveType = KKSG::LIVE_FRIENDANDGUILD;
}

UINT32 FriendAndGuildLiveList::FillInfo2Client(CRole* role, KKSG::GetWatchInfoByTypeRes& roRes)
{
	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(m_liveType);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	LiveInfoList* list = LiveManager::Instance()->GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return KKSG::ERR_FAILED;
	}
	std::list<KKSG::OneLiveRecordInfo*>* infoList = list->GetLiveList();
	if (!infoList)
	{
		return KKSG::ERR_FAILED;
	}
	int num = 0;

	m_liveInfo.clear();

	
	foreach(i in (*infoList))
	{
		if (num >= data->LiveNum)
		{
			break;
		}
		if (!CheckLiveTypeConfigExist((*i)->livetype()))
		{
			continue;
		}

		bool hasFriend = HasFriend(role, **i);
		bool hasGuild = HasGuild(role, **i);
		if (!hasFriend && !hasGuild)
		{
			continue;
		}

		KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
		info->CopyFrom(**i);
		info->set_hasfriend(hasFriend);
		info->set_hasguild(hasGuild);
		
		++num;
	}

	return KKSG::ERR_SUCCESS;

}



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
		case KKSG::LIVE_PVP2:
			m_liveList[i] = new PVP2LiveList();
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
		case KKSG::LIVE_FRIEND:
			m_liveList[i] = new FriendLiveList();
			break;
		case KKSG::LIVE_GUILD:
			m_liveList[i] = new GuildLiveList();
			break;
		case KKSG::LIVE_FRIENDANDGUILD:
			m_liveList[i] = new FriendAndGuildLiveList();
			break;
		case KKSG::LIVE_HEROBATTLE:
			m_liveList[i] = new HeroBattleLiveList();
			break;
		case KKSG::LIVE_LEAGUEBATTLE:
			m_liveList[i] = new LeagueBattleLiveList();
			break;
		case KKSG::LIVE_CUSTOMPK:
			m_liveList[i] = new CustomPkLiveList();
		default:
			break;
		}
	}

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, UPDATE_INTERVAL, -1, __FILE__, __LINE__);

	return true;
}

void LiveManager::Uninit()
{
	foreach(i in m_liveList)
		delete i->second;
}

bool LiveManager::IsCrossInfo(UINT32 liveID, UINT32* gsLine)
{
	UINT32 line = liveID >> 24;
	if (line >= CROSS_LIVEID_GSLINE_OFFSET)
	{
		if (gsLine)
		{
			*gsLine = line - CROSS_LIVEID_GSLINE_OFFSET;
		}
		return true;
	}

	return false;
}

void LiveManager::OnCrossGSClose(UINT32 gsLine, bool isAll)
{
	bool numChange = false;
	foreach(i in m_liveList)
	{
		std::list<KKSG::OneLiveRecordInfo*>* list = (i->second->GetLiveList());
		for(auto j = list->begin(); j != list->end();)
		{
			UINT32 line = 0;
			if (IsCrossInfo((*j)->liveid(), &line) && (isAll || line == gsLine))
			{
				numChange = true;
				i->second->RemoveLiveInfo(*(j++));
				continue;
			}
			else
				++j;
		}
	}
	if (numChange)
	{
		_SyncNumToGs();
	}
}

void LiveManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	bool numChange = false;
	int now = time(0);
	foreach(i in m_liveList)
	{
		std::list<KKSG::OneLiveRecordInfo*>* list = (i->second->GetLiveList());
		for(auto j = list->begin(); j != list->end();)
		{
			if ((*j)->has_sceneid() && (*j)->sceneid())
			{
				if (!IsCrossInfo((*j)->liveid()) && !CSceneManager::Instance()->GetScene((*j)->sceneid()))
				{
					numChange = true;
					i->second->RemoveLiveInfo(*(j++));
					continue;
				}
				if (IsCrossInfo((*j)->liveid()) && (now - (*j)->begintime()) >= 3600)
				{
					numChange = true;
					i->second->RemoveLiveInfo(*(j++));
					continue;
				}
			}
			++j;
		}
	}
	if (numChange)
	{
		_SyncNumToGs();
	}
}


void LiveManager::UpdateRecommendList(KKSG::OneLiveRecordInfo* info, bool isAdd)
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


KKSG::OneLiveRecordInfo* LiveManager::GetLiveInfo(int liveType, UINT32 liveID)
{
	LiveInfoList* list = GetLiveInfoList(liveType);
	if (!list)
	{
		return NULL;
	}

	return list->GetLiveInfo(liveID);
}

UINT32 LiveManager::FillLiveInfo(CRole* role, int type, KKSG::GetWatchInfoByTypeRes& roRes)
{
	if (!role)
	{
		return KKSG::ERR_FAILED;
	}

	std::vector<LiveTable::RowData*> typeList;
	LiveConfig::Instance()->GetLiveTableDataList(type, typeList);
	if (typeList.empty())
	{
		return KKSG::ERR_FAILED;
	}

	std::set<int> typeSet;
	for (auto iter = typeList.begin(); iter != typeList.end(); ++iter)
	{
		typeSet.insert((*iter)->Type);
	}

	UINT32 error = KKSG::ERR_SUCCESS;
	for (auto iter = typeSet.begin(); iter != typeSet.end(); ++iter)
	{
		LiveInfoList* list = GetLiveInfoList(*iter);
		if (!list)
		{
			//return KKSG::ERR_FAILED;
			continue;
		}
		
		error = list->FillInfo2Client(role, roRes);
		if(error != KKSG::ERR_SUCCESS)
		{
			return error;
		}
	}

	roRes.set_curtime(time(0));
	return error;
}

UINT32 LiveManager::EnterWatchBattle(/*CRole* role,*/ int type, UINT32 liveID, UINT32& sceneID, UINT32& sceneTemplateID)
{
	/*if (!role)
	{
		return KKSG::ERR_FAILED;
	}*/
	//LiveInfoList* list = GetLiveInfoList(type);

	KKSG::OneLiveRecordInfo* info = GetLiveInfo(type, liveID);
	if (!info)
	{
		return KKSG::ERR_WATCH_LIVEISOVER;
	}

	sceneID = info->sceneid();
	if (!sceneID)
	{
		return KKSG::ERR_WATCH_LIVEISOVER;
	}

	const LiveTable::RowData* data = LiveConfig::Instance()->GetLiveTableData(type);
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}

	if (!IsCrossInfo(liveID))
	{
		CScene* scene = CSceneManager::Instance()->GetScene(sceneID);
		if (!scene)
		{
			return KKSG::ERR_WATCH_LIVEISOVER;
		}
		sceneTemplateID = scene->GetMapID();
	}
	else
		sceneTemplateID = info->mapid();

	if (info->curwatchnum() >= data->MaxWatchNum)
	{
		return KKSG::ERR_WATCH_LIVEISFULL;
	}

	return KKSG::ERR_SUCCESS;
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


void LiveManager::SyncInfoFromGS(const KKSG::OneLiveRecordInfo* info, UINT32 deleteID, UINT32 sceneID, bool needSort)
{
	int oldNum = 0;
	LiveInfoList* list = GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return;
	}
	oldNum = list->GetLiveList()->size();
	if (info)
	{
		AddLive(info, sceneID, needSort);
	}
	
	if (deleteID)
	{
		RemoveLive(deleteID);
	}

	int newNum = list->GetLiveList()->size();
	if (oldNum != newNum || newNum == 0)
	{
		_SyncNumToGs(newNum);
	}

}

void LiveManager::AllSyncSort()
{
	for (int i = KKSG::LIVE_RECOMMEND; i < KKSG::LIVE_MAX; ++i)
	{
		if (NeedDelete(i))
			m_liveList[i]->Sort();
	}
}

void LiveManager::AddLive(const KKSG::OneLiveRecordInfo* info, UINT32 sceneID, bool needSort)
{
	if (!info)
	{
		return;
	}

	LiveInfoList* list = GetLiveInfoList(info->livetype());
	if (!list)
	{
		return;
	}

	KKSG::OneLiveRecordInfo* haveInfo = list->GetLiveInfo(info->liveid());
	if (!haveInfo)
	{
		KKSG::OneLiveRecordInfo* newInfo = new KKSG::OneLiveRecordInfo(*info);
		list->AddLiveInfo(newInfo);
		LogInfo("add liveinfo, liveid:%u, type:%u, sceneid:%u", info->liveid(), info->livetype(), info->sceneid());
	}
	else
	{
		haveInfo->CopyFrom(*info);
		if (needSort)
		{
			list->Sort();
		}
	}
}

void LiveManager::RemoveLive(UINT32 liveID)
{
	if (!liveID)
	{
		return;
	}

	LiveInfoList* list = GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (!list)
	{
		return;
	}

	KKSG::OneLiveRecordInfo* info = list->GetLiveInfo(liveID);
	if (!info)
	{
		return;
	}

	LogInfo("remove liveinfo, liveid:%u, type:%u, sceneid:%u", info->liveid(), info->livetype(), info->sceneid());

	LiveInfoList* infoList = GetLiveInfoList(info->livetype());
	infoList->RemoveLiveInfo(info);
}

void LiveManager::_SyncNumToGs(UINT32 num)
{
	PtcM2G_NotifyWatchIconNum ntf;
	ntf.m_Data.set_num(num);

	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, ntf);
}

void LiveManager::_SyncNumToGs()
{
	LiveInfoList* list = GetLiveInfoList(KKSG::LIVE_RECOMMEND);
	if (list)
	{
		_SyncNumToGs(list->GetLiveList()->size());
	}
}

