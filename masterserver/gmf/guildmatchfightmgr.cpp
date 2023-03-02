#include "pch.h"
#include "guildmatchfightmgr.h"
#include "table/globalconfig.h"
#include "pb/project.pb.h"
#include "guild/guildarena.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "notice/noticemgr.h"
#include "role/rolesummarymgr.h"
#include "network/gslink.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "guild/ptcm2c_gmfjoinbattlem2creq.h"
#include "scene/scenemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "util/XCommon.h"

CGuildMFRoom::CGuildMFRoom(UINT64 guild11, UINT64 guild22, UINT32 warType, UINT32 battleId, GMFRoomType type)
	:m_guildID11(guild11),m_guildID22(guild22),m_oWarType(warType),m_oBattleId(battleId),m_type(type)
{
	static UINT64 UIDIndex = 1;
	m_UID = UIDIndex++;

	m_createTime = time(NULL);
	m_sceneID = 0;
	m_watcherID = 0;
	m_enterStop = false;
}

void CGuildMFRoom::SaveData(KKSG::GmfRoom& data)
{
	data.set_uid(m_UID);
	data.set_guildid11(m_guildID11);
	data.set_guildid22(m_guildID22);
	data.set_perstigerank11(CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, m_guildID11));
	data.set_perstigerank22(CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, m_guildID22));
	data.set_wartype(m_oWarType);
	data.set_battleid(m_oBattleId);
	data.set_createtime((UINT32)m_createTime);
}

void CGuildMFRoom::TestPrint()
{
	SSDebug << " CGuildMFRoom : uid " << m_UID << END;
	SSDebug << " guild11 = " << m_guildID11 << " guild22 = " << m_guildID22 << END;
	SSDebug << " wartype = " << m_oWarType << " battleid = " << m_oBattleId << END;
}

UINT64 CGuildMFRoom::GetUniqueID()
{
	return m_UID;
}

INSTANCE_SINGLETON(GuildMatchFightMgr)

GuildMatchFightMgr::GuildMatchFightMgr()
{

}

GuildMatchFightMgr::~GuildMatchFightMgr()
{

}

bool GuildMatchFightMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);

	m_gmfSceneTemplateID = GetGlobalConfig().GetInt("GMFSceneID", 19);
	m_gprSceneTemplateID = 41;

	return true;
}

void GuildMatchFightMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);

	m_allRooms.clear();
}

UINT64 GuildMatchFightMgr::AddOneGMFRoom(UINT64 guildid11, UINT64 guildid22, UINT32 warType, UINT32 battleId, GMFRoomType type/* = GMF*/)
{
	SSDebug << __FUNCTION__ << " : " << END;
	SSInfo << " g11 = " << guildid11 << " g22 = " << guildid22 << END;
	SSInfo << " warType = " << warType << " battleId = " << battleId << END; 

	CGuildMFRoom room(guildid11, guildid22, warType, battleId, type);

	UINT64 uniqueID = room.GetUniqueID();
	SSInfo << " room unique id = " << uniqueID << END;
	m_allRooms.insert(std::make_pair(uniqueID,room)); 

	bool flag = CreateSceneBegin(uniqueID);
	if(false == flag)
	{
		DelOneGMFRoom(uniqueID);
		return 0;
	}
	return uniqueID;
}

void GuildMatchFightMgr::FightEnd(UINT64 uniqueid, UINT64 winguildid, UINT64 loseguildid, UINT32 winscore, UINT32 losescore)
{
	SSInfo << __FUNCTION__ << " uid = " << uniqueid << END;
	SSInfo << __FUNCTION__ << " wing = " << winguildid << " loseg = " << loseguildid << END;
	SSInfo << __FUNCTION__ << " winc = " << winscore << " losec = " << losescore << END;

	auto it = m_allRooms.find(uniqueid);
	if(it == m_allRooms.end())
	{
		SSError << " find room null uid = " << uniqueid << END;
		return ;
	}

	CGuildMFRoom& room = (it->second);
	if(room.GetType() == GMF_RT_GMF)
	{
		CGuildWarMgr::Instance()->BattleFinish(room.GetOWarType(), room.GetOBattleId(), winguildid);
	}
	else if(room.GetType() == GMF_RT_GPR)
	{
		CGuildArenaIntegralMgr::Instance()->WriteBattleResoult(winguildid, winscore, loseguildid, losescore);
	}

	DelOneGMFRoom(uniqueid);
}

void GuildMatchFightMgr::SetEnterStop(UINT64 uniqueid, bool enterstop)
{
	SSDebug << __FUNCTION__ << " uid = " << uniqueid << " enterstop = " << enterstop << END;

	auto it = m_allRooms.find(uniqueid);
	if(it == m_allRooms.end())
	{
		SSError << " find room null uid = " << uniqueid << END;
		return ;
	}

	CGuildMFRoom& room = (it->second);
	room.SetEnterStop(enterstop);
	if(room.GetType() == GMF_RT_GPR)
	{
		if(enterstop)
		{
			CGuildArenaIntegralMgr::Instance()->SetBattleState(room.GetGuildID11(), room.GetGuildID22(), KKSG::integralwatch);
		}
		else
		{
			CGuildArenaIntegralMgr::Instance()->SetBattleState(room.GetGuildID11(), room.GetGuildID22(), KKSG::integralenterscene);
		}
	}
}

void GuildMatchFightMgr::HandlerGmfUpdate(const KKSG::GmfUpdateData& data)
{
	if(!data.has_roomdata())
	{
		return ;
	}
	UINT64 UID = data.roomdata().uid();
	//观战
	if(data.has_addlive())
	{
		UINT32 liveID = data.addlive().liveid();
		WatchBegin(UID, liveID);	
	}

	//战斗结束
	if(data.has_result())
	{
		UINT64 winguildid = data.result().winguild();
		UINT64 loseguildid = data.result().loseguild();
		UINT32 winscore = data.result().winscore();
		UINT32 losescore = data.result().losescore();
		FightEnd(UID, winguildid, loseguildid, winscore, losescore);
	}

	if(data.has_enterstop())
	{
		SetEnterStop(UID, data.enterstop().stop());
	}
}

//void GuildMatchFightMgr::OnRoleEnterScene(CRole* pRole)
//{
//	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
//	if(0 == gid)
//	{
//		return ;
//	}
//	for(auto it = m_allRooms.begin(); it != m_allRooms.end(); ++it)
//	{
//		auto& room = it->second;
//		if(room.GetGuildID11() == gid || room.GetGuildID22() == gid)
//		{
//			if(room.GetSceneID())
//			{
//				JoinIcon(pRole);
//			}
//
//			break;
//		}
//	}
//}

void GuildMatchFightMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckErrorRoom();
}

void GuildMatchFightMgr::TestMakeFight(CRole* pRole, int type, UINT64 guild11, UINT64 guild22)
{
	if(NULL == pRole)
	{
		return ;
	}

	static UINT32 TestIndex = 1;
	TestIndex++;

	AddOneGMFRoom(guild11, guild22, TestIndex, TestIndex, GMFRoomType(type));	
}

void GuildMatchFightMgr::TestUpdate()
{
}

bool GuildMatchFightMgr::CreateSceneBegin(UINT64 uniqueid)
{
	SSInfo << __FUNCTION__ << " uniqueid = " << uniqueid << END;
	auto it = m_allRooms.find(uniqueid);
	if(it == m_allRooms.end())
	{
		return false;
	}
	CGuildMFRoom& room = it->second;

	KKSG::CreateBattleParam param;
	room.SaveData(*(param.mutable_gmfpara()->mutable_roomdata()));

	param.mutable_gmfpara()->set_guildwartimestate(CGuildWarMgr::Instance()->GetTimeState());

	UINT64 guildID = room.GetGuildID11();
	Guild* guild = CGuildMgr::Instance()->GetGuild(guildID);
	if (guild)
	{
		auto guildinfo = param.add_guildinfo();
		guildinfo->set_guildid(guild->GetID());
		guildinfo->set_guildname(guild->GetName());
		guildinfo->set_guildicon(guild->GetIcon());
	}
	guildID = room.GetGuildID22();
	guild = CGuildMgr::Instance()->GetGuild(guildID);
	if (guild)
	{
		auto guildinfo = param.add_guildinfo();
		guildinfo->set_guildid(guild->GetID());
		guildinfo->set_guildname(guild->GetName());
		guildinfo->set_guildicon(guild->GetIcon());
	}

	UINT32 sceneTempID = m_gmfSceneTemplateID;
	if(GMF_RT_GPR == room.GetType())//积分赛
	{
		sceneTempID = m_gprSceneTemplateID;
	}

	bool flag = CSceneCreator::Instance()->CreateBattleScene(sceneTempID, &ICreateGMFBattleListener::GlobalCreateGMFBattleListener, param, INVALID_LINE_ID, 1);	
	if(false == flag)
	{
		SSError << " create gmfscene begin err gmfuniqueid = " << room.GetUniqueID() << " scenetempid = " << m_gmfSceneTemplateID << END;
		return false;
	}
	
	return true;
}

void GuildMatchFightMgr::CreateSceneEnd(UINT64 uniqueid, UINT32 sceneid)
{
	SSInfo << __FUNCTION__ << " uniqueid = " << uniqueid << " sceneid = " << sceneid << END;
	auto it = m_allRooms.find(uniqueid);
	if(it == m_allRooms.end())
	{
		return ;
	}
	CGuildMFRoom& room = it->second;
	room.SetSceneID(sceneid);
	//JoinBegin(room.GetGuildID11());
	//JoinBegin(room.GetGuildID22());
}

void GuildMatchFightMgr::DelOneGMFRoom(UINT64 uniqueID)
{
	SSDebug << __FUNCTION__ <<" : uniqueID = " << uniqueID << END;
	m_allRooms.erase(uniqueID);
}

//void GuildMatchFightMgr::JoinBegin(UINT64 guildid)
//{
//	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildid);
//	if(NULL == pGuild)
//	{
//		return ;
//	}
//	const auto& mems = pGuild->GetGuildMember();
//	for(size_t i = 0; i < mems.size(); ++i)
//	{
//		CRole* pRole = CRoleManager::Instance()->GetByRoleID(mems[i].qwRoleID);
//		if(pRole)//在线
//		{
//			JoinIcon(pRole);
//		}
//	}
//}
//
//void GuildMatchFightMgr::JoinIcon(CRole* pRole)
//{
//	if(NULL == pRole)
//	{
//		return ;
//	}
//	//to do
//}

void GuildMatchFightMgr::WatchBegin(UINT64 uniqueid, UINT32 liveID)
{
	//开始观战		
	SSDebug << __FUNCTION__ << " uid = " << uniqueid << " liveid = " << liveID << END;

	auto it = m_allRooms.find(uniqueid);
	if(it == m_allRooms.end())
	{
		SSError << " find room null uid = " << uniqueid << END;
		return ;
	}

	CGuildMFRoom& room = (it->second);
	if(room.GetType() == GMF_RT_GMF)
	{
		CGuildWarMgr::Instance()->SetBattleWatchId(room.GetOWarType(), room.GetOBattleId(), liveID);
	}

	(it->second).SetWatcherID(liveID);
	//DelOneGMFRoom(uid);
}

int GuildMatchFightMgr::JoinRequest(CRole* pRole)
{
	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	if(0 == gid)
	{
		return KKSG::ERR_FAILED;
	}
	for(auto it = m_allRooms.begin(); it != m_allRooms.end(); ++it)
	{
		auto& room = it->second;
		if(room.GetGuildID11() == gid || room.GetGuildID22() == gid)
		{
			UINT32 sceneid = room.GetSceneID();
			if(sceneid)
			{
				KKSG::SceneSwitchData oData;
				if(room.EnterStoped() && room.GetWatcherID())//走观战
				{
					oData.add_watchers(pRole->GetID());
				}
				CSceneSwitch::EnterScene(pRole, sceneid, oData);
				return KKSG::ERR_SUCCESS;
			}
			else
			{
				return KKSG::ERR_FAILED;//还没有创建好战场
			}
		}
	}

	//开始观战或者没有开启 无法参战入场
	return KKSG::ERR_FAILED;
}

void GuildMatchFightMgr::TestBegin(UINT32 gamenum)
{
	KKSG::CreateBattleParam param;
	KKSG::SkyCitySceneInfo& data =  *param.mutable_skycity();
	data.set_nfloor(2);
	data.set_ngames(gamenum);
	data.set_ngroupid(4);
	data.set_endtime((UINT32)(time(NULL) + 180));
	
	UINT32 teamid11 = 0;
	UINT32 teamid22 = 0;
	auto itend = CRoleManager::Instance()->End();
	for(auto it = CRoleManager::Instance()->Begin(); it != itend; ++it)
	{
		CRole* pRole = (*it);
		if(pRole->GetTeamID())
		{
			auto pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
			if(NULL == pTeam || SKYCITY_SCENE != pTeam->GetConf()->Type)
			{
				continue;
			}
			if(0 == teamid11)
			{
				teamid11 = pRole->GetTeamID();
			}
			else if(0 == teamid22 && pRole->GetTeamID() != teamid11)
			{
				teamid22 = pRole->GetTeamID();
			}
			else if(teamid11 && teamid22)
			{
				break;
			}
		}
	}

	Team* pTeam = TeamMgr::Instance()->FindTeam(teamid11);
	if(pTeam)
	{
		const auto& mems = pTeam->GetMember();
		for(auto it = mems.begin(); it != mems.end(); ++it)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
			if(pRole)
			{
				auto& tmp = *data.add_self();
				tmp.set_teamid(teamid11);
				tmp.set_uid(pRole->GetID());
				tmp.set_name(pRole->GetName());
				auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
				if(pSummary)
				{
					tmp.set_lv(pSummary->GetLevel());
					tmp.set_ppt(pSummary->GetPowerPoint());
					tmp.set_job(pSummary->GetProfession());
					tmp.set_online(true);
				}
				auto& statistics = *data.add_rolsstatistics();
				statistics.set_roleid(pRole->GetID());
				statistics.set_killcount(100);
				statistics.set_deadcount(100);
				statistics.set_totaldamage(1000.0);
			}
		}
	}
	pTeam = TeamMgr::Instance()->FindTeam(teamid22);
	if(pTeam)
	{
		const auto& mems = pTeam->GetMember();
		for(auto it = mems.begin(); it != mems.end(); ++it)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
			if(pRole)
			{
				auto& tmp = *data.add_target();
				tmp.set_teamid(teamid22);
				tmp.set_uid(pRole->GetID());
				tmp.set_name(pRole->GetName());
				auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
				if(pSummary)
				{
					tmp.set_lv(pSummary->GetLevel());
					tmp.set_ppt(pSummary->GetPowerPoint());
					tmp.set_job(pSummary->GetProfession());
					tmp.set_online(true);
				}
				auto& statistics = *data.add_rolsstatistics();
				statistics.set_roleid(pRole->GetID());
				statistics.set_killcount(100);
				statistics.set_deadcount(100);
				statistics.set_totaldamage(1000.0);
			}
		}
	}

	bool flag = CSceneCreator::Instance()->CreateBattleScene(19, &ICreateTestListener::GlobalCreateTestListener, param);	
	if(!flag)
	{
		SSError << __FUNCTION__ << END;
	}
}

void GuildMatchFightMgr::TestEnd(UINT32 sceneid)
{
	auto itend = CRoleManager::Instance()->End();
	for(auto it = CRoleManager::Instance()->Begin(); it != itend; ++it)
	{
		CRole* pRole = (*it);
		auto pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
		if(pTeam && SKYCITY_SCENE == pTeam->GetConf()->Type)
		{
			KKSG::SceneSwitchData oData;
			CSceneSwitch::EnterScene(pRole, sceneid, oData);
		}
	}
}

void GuildMatchFightMgr::TestPrint()
{
	for(auto it = m_allRooms.begin(); it != m_allRooms.end(); ++it)
	{
		(it->second).TestPrint();		
	}
}

void GuildMatchFightMgr::CheckErrorRoom()
{
	time_t now = time(NULL);
	time_t onedaytime = XCommon::GetOneDayTime(); 
	std::vector<UINT64> waitDel;

	for(auto it = m_allRooms.begin(); it != m_allRooms.end(); ++it)
	{
		auto& room = (it->second);
		UINT64 uid = room.GetUniqueID();
		UINT32 sceneid = room.GetSceneID();		
		CScene* pScene = CSceneManager::Instance()->GetScene(sceneid);
		if(NULL == pScene && sceneid)//gs 宕机
		{
			SSWarn << " scene = null sceneid = " << sceneid << " uid = " << uid << " guild11 = " << room.GetGuildID11() << " guild22 = " << room.GetGuildID22() << END;
			waitDel.push_back(uid);
			continue;
		}
		time_t createtime = room.GetCreateTime();
		if(createtime + onedaytime <= now)
		{
			SSWarn << " room time over sceneid = " << sceneid << " uid = " << uid << " guild11 = " << room.GetGuildID11() << " guild22 = " << room.GetGuildID22() << END;
			waitDel.push_back(uid);
			continue;
		}
	}	

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		DelOneGMFRoom(waitDel[i]);
	}
}
