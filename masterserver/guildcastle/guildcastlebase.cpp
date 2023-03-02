#include "pch.h"
#include "guildcastlebase.h"
#include <ctime>
#include "scene/sceneconfig.h"
#include "ranklist/guildcastleranklist.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "scene/scenecreate.h"
#include "role/role.h"
#include "guildcastle/ptcm2g_gcfsynm2g.h"
#include "scene/scenemanager.h"
#include "network/gslink.h"
#include "guildcastlemgr.h"
#include "table/globalconfig.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "notice/notice.h"
#include "guild/guildterritorybattle.h"
#include "scene/sceneconfig.h"
#include "scene/scene.h"
#include "util/XCommon.h"

GCJvDian::GCJvDian(KKSG::GCFJvDianType type)
{
	m_type = type;
	ResetOccupy(0, 0);
}

void GCJvDian::ResetOccupy(UINT64 occupier, UINT32 now)
{
	m_occupier = occupier;
	m_ctime = now;
}

GCJvDian::~GCJvDian()
{

}

void GCJvDian::TestPrint()
{
	SSDebug << " type = " << m_type << " occupy = " << m_occupier << " time = " << m_ctime << END; 	
}

GCBattleField::GCBattleField(UINT32 mapID, bool isWait/* = false*/)
{
	m_mapID = mapID;
	m_sceneID = 0;

	if(false == isWait)
	{
		m_jvDians.push_back(GCJvDian(KKSG::GCF_JUDIAN_UP));
		m_jvDians.push_back(GCJvDian(KKSG::GCF_JUDIAN_MID));
		m_jvDians.push_back(GCJvDian(KKSG::GCF_JUDIAN_DOWN));
	}

	SetPointMulti(1, 0); 
}

GCBattleField::~GCBattleField()
{

}

void GCBattleField::ResetJvDian(KKSG::GCFJvDianType type, UINT64 occupier)
{
	for(size_t i = 0; i < m_jvDians.size(); ++i)
	{
		if(m_jvDians[i].CType() == type)
		{
			m_jvDians[i].ResetOccupy(occupier, UINT32(time(NULL)));
		}
	}
}

void GCBattleField::TestPrint()
{
	SSDebug << __FUNCTION__ << END;
	SSDebug << " map = " << m_mapID << " scene = " << m_sceneID << " mulpoint " << GetPointMulti(UINT32(time(NULL))) << END;
	SSDebug << " rolecountone = " << m_groupCount[GCF_GROUP_ONE] << " rolecounttwo = " << m_groupCount[GCF_GROUP_TWO] << END;
	for(size_t i = 0; i < m_jvDians.size(); ++i)
	{
		m_jvDians[i].TestPrint();	
	}
}

UINT32 GCBattleField::GetPointMulti(UINT32 now)
{
	if(m_pMultiTime > now)
	{
		return m_pointMulti;
	}
	return 1;
}

void GCBattleField::SetPointMulti(UINT32 multi, UINT32 t)
{
	m_pointMulti = multi;
	m_pMultiTime = t;
}

UINT32 GCBattleField::CurRoleCount(GCFGroup group)
{
	return m_groupCount[group];
}

void GCBattleField::IncreaseRole(UINT64 roleID, GCFGroup group)
{
	if(m_roleGroup.find(roleID) != m_roleGroup.end())
	{
		return ;
	}
	m_roleGroup[roleID] = group;
	m_groupCount[group] += 1;
}

bool GCBattleField::HaveRole(UINT64 roleID)
{
	return (m_roleGroup.find(roleID) != m_roleGroup.end());
}

void GCBattleField::ReduceRole(UINT64 roleID)
{
	if(m_roleGroup.find(roleID) == m_roleGroup.end())
	{
		return ;
	}
	GCFGroup group = m_roleGroup[roleID];
	m_roleGroup.erase(roleID);
	if(m_groupCount[group] > 0)
	{
		m_groupCount[group] -= 1;
	}
}

GCGuildInfo::GCGuildInfo(UINT64 guildID, const std::string& guildName, UINT32 guildIcon, GCFGroup group)
{
	m_guildID = guildID;
	m_guildName = guildName;
	m_guildIcon = guildIcon;
	m_group = group;
	m_point = 0;
}

GCGuildInfo::~GCGuildInfo()
{

}

void GCGuildInfo::TestPrint()
{
	SSDebug << " guild = " << m_guildID << " gname = " << m_guildName << " group = " << m_group << " point = " << m_point << END;
}

void GCGuildInfo::FillInfo(KKSG::GCFGuildBrief& data)
{
	data.set_guildid(m_guildID);
	data.set_guildname(m_guildName);
	data.set_guildicon(m_guildIcon);
	data.set_point(m_point);
	data.set_group(m_group);

}

GCOneFight::GCOneFight(UINT64 uid, UINT32 territoryID)
{
	m_UID = uid;
	m_territoryID  = territoryID;
	m_stage = CGFS_READY;
	m_cTime = UINT32(time(NULL));
	m_pRankList = new GuildCastleRankList();
	m_winGuildID = 0;
	m_lastMulPointTime = 0;
	m_lastGuildWinNoticeTime = 0;
}

GCOneFight::~GCOneFight()
{
	delete m_pRankList;
}

bool GCOneFight::Init(const std::vector<UINT64>& guilds)
{
	for(size_t i = 0; i < guilds.size(); ++i)
	{
		auto pGuild = CGuildMgr::Instance()->GetGuild(guilds[i]);			
		if(NULL == pGuild)
		{
			return false;
		}
		GCGuildInfo ginfo(guilds[i], pGuild->GetName(), pGuild->GetIcon(), (0==i)?GCF_GROUP_ONE:GCF_GROUP_TWO);
		m_guildInfos.push_back(ginfo);
	}
	const auto& sceneConf = GetGlobalConfig().GuildCastleScenes;
	for(size_t i = 0; i < sceneConf.size(); ++i)
	{
		UINT32 mapID = sceneConf[i];
		auto pConf = CSceneConfig::Instance()->GetSceneConf(mapID);
		if(pConf)
		{
			GCBattleField bfield(mapID, (GetGCWaitMapID() == mapID));
			m_battleFields.push_back(bfield);
		}
		else
		{
			SSError << " find not scene config mapid = " << mapID << END;
			return false;
		}
	}
	return true;
}

void GCOneFight::MakeStop()
{
	SSInfo << __FUNCTION__ << END;

	GCFGroup winGroup = GCF_GROUP_NONE;
	auto pGuild = GetWinGuild();
	if(pGuild)
	{
		m_winGuildID = pGuild->GuildID();
		winGroup = pGuild->Group();

		BreakDrawPoint(pGuild);//分数平衡则破坏
	}

	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		GuildCastleMgr::Instance()->SetGuildResult(m_guildInfos[i].GuildID(), m_territoryID, (m_guildInfos[i].Group() == winGroup), m_winGuildID);
		SSInfo << " guildid = " << m_guildInfos[i].GuildID() << " point = " << m_guildInfos[i].GetPoint() << END;
	}

	GuildCastleMgr::Instance()->FightResultReport(m_territoryID, m_winGuildID);

	PtcM2G_GCFSynM2G ptc;
	ptc.m_Data.set_type(KKSG::GCF_SYN_FIGHT_END);

	BroadcastSyn(ptc);
}

void GCOneFight::Update()
{
	CheckAddPoint();
	CheckRandMulPoint();
	CheckGuildWinNotice();
}

void GCOneFight::ChangeStage(CGFightStage newStage)
{
	CGFightStage oldStage = m_stage;
	if(oldStage == newStage)
	{
		return ;
	}
	m_stage = newStage; 
	m_cTime = UINT32(time(NULL));

	if(CGFS_END == m_stage)
	{
		MakeStop();
	}
}

void GCOneFight::TestPrint()
{
	SSDebug << __FUNCTION__ << END;
	SSDebug << " uid = " << m_UID << " terrID = " << m_territoryID << " stage = " << m_stage << " time = " << m_cTime << END;
	SSDebug << " winguild = " << m_winGuildID << " lmt = " << m_lastMulPointTime << END;
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		m_guildInfos[i].TestPrint();
	}
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		m_battleFields[i].TestPrint();
	}
	if(m_pRankList)
	{
		m_pRankList->TestPrint();
	}
}

void GCOneFight::CreateSceneBegin()
{
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		KKSG::CreateBattleParam para;
		KKSG::GCFCreateBattleInfo& data = *para.mutable_guildcastle();
		FillCreateSceneCommon(data);
		data.set_iswait(m_battleFields[i].IsWaitField());
		
		CSceneCreator::Instance()->CreateBattleScene(m_battleFields[i].MapID(), &ICreateGuildCastleListner::GlobalCreateGuildCastleListener, para);
		SSInfo << " uid = " << m_UID << " mapid = " << m_battleFields[i].MapID() << END;
	}
}

void GCOneFight::CreateSceneEnd(UINT32 mapID, UINT32 sceneID)
{
	SSInfo << " uid = " << m_UID << " mapid = " << mapID << " sceneid = " << sceneID << END;
	auto pField = GetBattleFieldByMapID(mapID);
	if(pField)
	{
		pField->SetSceneID(sceneID);
	}
	else
	{
		SSError << " find mapid failed map = " << mapID << " sceneid = " << sceneID << END;
	}
}

GCBattleField* GCOneFight::GetBattleFieldByMapID(UINT32 mapID)
{
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		if(m_battleFields[i].MapID() == mapID)
		{
			return &m_battleFields[i];
		}
	}
	return NULL;
}

GCBattleField* GCOneFight::GetBattleFieldBySceneID(UINT32 sceneID)
{
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		if(m_battleFields[i].SceneID() == sceneID)
		{
			return &m_battleFields[i];
		}
	}
	return NULL;
}

bool GCOneFight::HaveGuild(UINT64 guildID)
{
	auto pInfo = GetGuildInfo(guildID);
	if(pInfo)
	{
		return true;
	}
	return false;
}

void GCOneFight::FillCreateSceneCommon(KKSG::GCFCreateBattleInfo& data)
{
	data.set_uid(m_UID);
	data.set_territoryid(m_territoryID);
	data.set_existtime(GetGlobalConfig().GuildCastleReadyTime + GetGlobalConfig().GuildCastleFightTime);
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		KKSG::GCFGuildGroup& gg = *data.add_guilds();
		gg.set_guildid(m_guildInfos[i].GuildID());
		gg.set_group(m_guildInfos[i].Group());
	}
}

void GCOneFight::FillZhanChInfo(KKSG::GCFZhanChBriefInfo& data, GCBattleField& field, GCFGroup myGroup, UINT32 now)
{
	data.set_mapid(field.MapID());
	data.set_curusercount(field.CurRoleCount(myGroup));
	data.set_maxusercount(GetGlobalConfig().GuildCastleBattleRoleMax);
	data.set_isopen((CGFS_FIGHT == GetStage()));
	data.set_multipoint(field.GetPointMulti(now));
}

void GCOneFight::FillReadyInfo(KKSG::GCFReadyInfoRes& data, UINT64 roleID)
{
	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(roleID);
	GCFGroup myGroup = GetGuildGroup(gid);
	if(GCF_GROUP_NONE == myGroup)
	{
		return ;
	}
	UINT32 now = UINT32(time(NULL));
	data.set_lefttime(GetLeftTime());

	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		if(m_battleFields[i].IsWaitField())
		{
			continue;
		}
		FillZhanChInfo(*data.add_allinfo(), m_battleFields[i], myGroup, now);
	}
}

void GCOneFight::FillBattleReport(KKSG::GCFCommonRes& data, UINT64 roleID)
{
	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(roleID);
	GCFGroup myGroup = GetGuildGroup(gid);
	if(GCF_GROUP_NONE == myGroup)
	{
		return ;
	}
	UINT32 now = UINT32(time(NULL));
	data.set_territoryid(m_territoryID);
	data.set_lefttime(GetLeftTime());//客户端未使用
	
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		if(m_battleFields[i].IsWaitField())
		{
			continue;
		}
		KKSG::GCFBattleField& filed = *data.add_fields();
		filed.set_mapid(m_battleFields[i].MapID());
		FillZhanChInfo(*filed.mutable_zhanchinfo(), m_battleFields[i], myGroup, now);
		auto& jvdians = m_battleFields[i].JvDians();		
		for(size_t j = 0; j < jvdians.size(); ++j)
		{
			KKSG::GCFJvDianInfo& info = *filed.add_jvdians();				
			info.set_type(jvdians[j].CType());
			info.set_guildname(GetGuildName(jvdians[j].Occupier()));
		}
	}

	auto pGuild = GetWinGuild();
	if(pGuild)
	{
		pGuild->FillInfo(*data.mutable_winguild());
	}

}

std::string GCOneFight::GetGuildName(UINT64 guildID)
{
	auto pInfo = GetGuildInfo(guildID);
	if(pInfo)
	{
		return pInfo->GuildName();
	}
	return "";
}

void GCOneFight::FillFightInfo(KKSG::GCFFightInfoRes& data, UINT64 roleID, UINT32 mapID)
{
	UINT32 lefttime = GetLeftTime();
	if(GetStage() == CGFS_READY)
	{
		lefttime += GetGlobalConfig().GuildCastleFightTime;
	}
	data.set_lefttime(lefttime);
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		m_guildInfos[i].FillInfo(*data.add_guilds());
	}
	GCFGroup myGroup = GetGuildGroup(CGuildMgr::Instance()->GetGuildIdByRoleId(roleID));
	data.set_mygroup(myGroup);
	auto pField = GetBattleFieldByMapID(mapID);
	if(pField)
	{
		auto& jvdians = pField->JvDians();		
		for(size_t i = 0; i < jvdians.size(); ++i)
		{
			KKSG::GCFJvDianInfo& info = *data.add_jvdians();
			info.set_type(jvdians[i].CType());
			info.set_guildname(GetGuildName(jvdians[i].Occupier()));
		}
	}
	if(m_pRankList)
	{
		m_pRankList->FillMyRank(*data.mutable_myinfo(), roleID);
	}
}

UINT32 GCOneFight::GetLeftTime()
{
	UINT32 now = UINT32(time(NULL));
	UINT32 ConfigT = 0;
	switch(GetStage())
	{
	case CGFS_READY:
		{
			ConfigT = GetGlobalConfig().GuildCastleReadyTime;
		}
		break;
	case CGFS_FIGHT:
		{
			ConfigT = GetGlobalConfig().GuildCastleFightTime;
		}
		break;
	case CGFS_END:
		{
			;
		}
		break;
	default:
		break;
	}
	if(m_cTime + ConfigT > now)
	{
		return (m_cTime + ConfigT - now);
	}
	return 0;
}

void GCOneFight::AddGuildPoint(UINT64 guildID, UINT32 point)
{
	auto pInfo = GetGuildInfo(guildID);
	if(pInfo)
	{
		pInfo->AddPoint(point);
	}
}

GCGuildInfo* GCOneFight::GetGuildInfo(UINT64 guildID)
{
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		if(m_guildInfos[i].GuildID() == guildID)
		{
			return &m_guildInfos[i];
		}
	}
	return NULL;
}

void GCOneFight::MakeMultiPoint(UINT32 mapID, UINT32 multi, UINT32 endTime)
{
	auto pField = GetBattleFieldByMapID(mapID);
	if(NULL == pField)
	{
		return ;
	}

	pField->SetPointMulti(multi, endTime);
	MulPointNotice(mapID, multi);

	PtcM2G_GCFSynM2G ptc;
	ptc.m_Data.set_type(KKSG::GCF_SYN_MUL_POINT);
	ptc.m_Data.set_mapid(mapID);
	ptc.m_Data.set_pmulti(multi);

	BroadcastSyn(ptc);
}

GCFGroup GCOneFight::GetGuildGroup(UINT64 guildID)
{
	auto pInfo = GetGuildInfo(guildID);
	if(pInfo)
	{
		return pInfo->Group();
	}
	return GCF_GROUP_NONE;
}

GCGuildInfo* GCOneFight::GetWinGuild()
{
	if(m_guildInfos.size() == 2)
	{
		return GetBettleGuild(&m_guildInfos[0], &m_guildInfos[1]);
	}
	else if(m_guildInfos.size() == 3)
	{
		if(m_guildInfos[0].GetPoint() >= m_guildInfos[1].GetPoint() + m_guildInfos[2].GetPoint())
		{
			return &m_guildInfos[0];
		}
		else
		{
			return GetBettleGuild(&m_guildInfos[1], &m_guildInfos[2]);
		}
	}
	return NULL;
}

GCGuildInfo* GCOneFight::GetBettleGuild(GCGuildInfo* guild11, GCGuildInfo* guild22)
{
	Guild* pGuild11 = CGuildMgr::Instance()->GetGuild(guild11->GuildID());
	Guild* pGuild22 = CGuildMgr::Instance()->GetGuild(guild22->GuildID());
	if(NULL == pGuild11)
	{
		return guild22;
	}
	if(NULL == pGuild22)
	{
		return guild11;
	}

	if(guild11->GetPoint() > guild22->GetPoint())
	{
		return guild11;
	}
	if(guild11->GetPoint() < guild22->GetPoint())
	{
		return guild22;
	}

	if(pGuild11->GetPrestige() >= pGuild22->GetPrestige())
	{
		return guild11;
	}
	return guild22;
}

void GCOneFight::FillResult(KKSG::GCFCommonRes& data, UINT64 roleID)
{
	data.set_territoryid(m_territoryID);
	if(m_pRankList)
	{
		m_pRankList->FillRankData(data, roleID);
	}
	auto pGuild = GetGuildInfo(m_winGuildID);
	if(pGuild)
	{
		pGuild->FillInfo(*data.mutable_winguild());
	}

	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		m_guildInfos[i].FillInfo(*data.add_guilds());
	}

	//const auto& rewards = GetGlobalConfig().GuildCastleRewards;
	//for(size_t i = 0; i < rewards.size(); ++i)
	//{
	//	KKSG::ItemBrief& item = *data.add_rewards();	
	//	item.set_itemid(rewards[i][0]);
	//	item.set_itemcount(rewards[i][1]);
	//}
}

void GCOneFight::KillCountSyn(UINT64 roleID, UINT32 killCount)
{
	PtcM2G_GCFSynM2G ptc;
	ptc.m_Data.set_type(GCF_SYN_KILL);
	ptc.m_Data.set_roleid(roleID);
	ptc.m_Data.set_killcount(killCount);

	BroadcastSyn(ptc);
}

void GCOneFight::BroadcastSyn(PtcM2G_GCFSynM2G& ptc)
{
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		CScene* pScene = CSceneManager::Instance()->GetScene(m_battleFields[i].SceneID());
		if(pScene)
		{
			ptc.m_Data.set_sceneid(pScene->GetID());
			GSLink::Instance()->SendToLine(pScene->GetLine(), ptc);
		}
	}
}

void GCOneFight::BreakDrawPoint(GCGuildInfo* pWinGuild)
{
	if(NULL == pWinGuild)
	{
		return;
	}
	bool needAddPointFlag = false;
	if(m_guildInfos.size() == 2)
	{
		if(m_guildInfos[0].GetPoint() == m_guildInfos[1].GetPoint())
		{
			if(pWinGuild->GuildID() == m_guildInfos[0].GuildID() || pWinGuild->GuildID() == m_guildInfos[1].GuildID())
			{
				needAddPointFlag = true;
			}
		}
	}
	else if(m_guildInfos.size() == 3)
	{
		if(m_guildInfos[0].GetPoint() == m_guildInfos[1].GetPoint() + m_guildInfos[2].GetPoint())
		{
			if(pWinGuild->GuildID() == m_guildInfos[0].GuildID())
			{
				needAddPointFlag = true;
			}
		}
		else if(m_guildInfos[0].GetPoint() < m_guildInfos[1].GetPoint() + m_guildInfos[2].GetPoint() 
			&& m_guildInfos[1].GetPoint() == m_guildInfos[2].GetPoint())
		{
			if(pWinGuild->GuildID() == m_guildInfos[1].GuildID() || pWinGuild->GuildID() == m_guildInfos[2].GuildID())
			{
				needAddPointFlag = true;
			}
		}
	}

	if(needAddPointFlag)
	{
		pWinGuild->AddPoint(GetGlobalConfig().GuildCastleAddPointCount);
		SSInfo << " add extra point guildid = " << pWinGuild->GuildID() << END;
	}
}

void GCOneFight::CheckAddPoint()
{
	UINT32 now = UINT32(time(NULL));
	for(size_t i = 0; i < m_battleFields.size(); ++i)
	{
		if(m_battleFields[i].IsWaitField())
		{
			continue;
		}
		auto& allJvDians = m_battleFields[i].JvDians();
		for(size_t j = 0; j < allJvDians.size(); ++j)
		{
			auto& jvdian = allJvDians[j];
			if(0 == jvdian.Occupier())
			{
				continue;
			}
			if(jvdian.CTime() + GetGlobalConfig().GuildCastleAddPointInterval <= now)
			{
				jvdian.SetTime(now);
				AddGuildPoint(jvdian.Occupier(), GetGlobalConfig().GuildCastleAddPointCount * m_battleFields[i].GetPointMulti(now));	
			}
		}
	}
}

void GCOneFight::CheckRandMulPoint()
{
	if(CGFS_FIGHT != GetStage())
	{
		return ;
	}
	UINT32 now = UINT32(time(NULL));
	if(0 == m_lastMulPointTime)
	{
		m_lastMulPointTime = now;
		return ;
	}

	const auto& conf = GetGlobalConfig().GuildCastleMulPoint;
	if(conf.size() != 4)
	{
		return ;
	}
	if(m_lastMulPointTime + conf[0] > now)
	{
		return ;
	}

	m_lastMulPointTime = now;

	UINT32 index = XRandom::randInt(2, conf.size());
	UINT32 mulpoint = conf[index];

	const auto& mps = GetGlobalConfig().GuildCastleScenes;
	if(mps.size() <= 1)
	{
		return ;
	}

	index = XRandom::randInt(1, mps.size());

	MakeMultiPoint(mps[index], mulpoint, (now + conf[1]));
}

UINT32 GCOneFight::GetMaxGroupPoint()
{
	UINT32 point11 = 0;
	UINT32 point22 = 0;
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		if(m_guildInfos[i].Group() == GCF_GROUP_ONE)
		{
			point11 += m_guildInfos[i].GetPoint();
		}
		else if(m_guildInfos[i].Group() == GCF_GROUP_TWO)
		{
			point22 += m_guildInfos[i].GetPoint();
		}
	}
	if(point11 > point22)
	{
		return point11;
	}
	return point22;
}

void GCOneFight::TestAddPoint(UINT32 add)
{
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		m_guildInfos[i].AddPoint(add);
	}
}

void GCOneFight::MulPointNotice(UINT32 mapID, UINT32 multi)
{
	const SceneConf* pSceneConf = CSceneConfig::Instance()->GetSceneConf(mapID);
	if(NULL == pSceneConf)
	{
		return ;
	}
	auto pConf = CGuildTerritoryBattle::Instance()->GetTerrConf(m_territoryID); 
	if(NULL == pConf)
	{
		return ;
	}

	int noticetype = GuildTerrMulPointOne;
	if(2 == pConf->territorylevel)
	{
		noticetype = GuildTerrMulPointTwo;
	}
	else if(3 == pConf->territorylevel)
	{
		noticetype = GuildTerrMulPointThree;
	}

	Notice notice(noticetype);
	notice.Replace("$S", pConf->territoryname);
	notice.Replace("$S", pSceneConf->Comment);
	notice.Replace("$N", ToString(multi));
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		notice.SendGuild(m_guildInfos[i].GuildID());
	}
}

void GCOneFight::CheckGuildWinNotice()
{
	if(GetStage() != CGFS_FIGHT)
	{
		return ;
	}
	UINT32 now = UINT32(time(NULL));
	if(0 == m_lastGuildWinNoticeTime)
	{
		m_lastGuildWinNoticeTime = now;
		return ;
	}

	UINT32 interval = 300;//wait

	if(m_lastGuildWinNoticeTime + interval > now)
	{
		return;
	}
	m_lastGuildWinNoticeTime = now;

	int noticetype = GuildTerrWinGuildOne;
	auto pConf = CGuildTerritoryBattle::Instance()->GetTerrConf(m_territoryID);
	if(NULL == pConf)
	{
		return ;
	}
	if(2 == pConf->territorylevel)
	{
		noticetype = GuildTerrWinGuildTwo;
	}
	else if(3 == pConf->territorylevel)
	{
		noticetype = GuildTerrWinGuildThree;
	}
	auto pGuild = GetWinGuild();
	if(NULL == pGuild)
	{
		return ;
	}
	Notice notice(noticetype);	
	notice.Replace("$S", pConf->territoryname);
	notice.Replace("$G", pGuild->GuildName());
	notice.Replace("$N", ToString(GetGlobalConfig().GuildCastleEndPoint));
	for(size_t i = 0; i < m_guildInfos.size(); ++i)
	{
		notice.SendGuild(m_guildInfos[i].GuildID());
	}
}

void GCOneFight::TestMakeMultiPoint(UINT32 mapID, UINT32 multi, UINT32 endTime)
{
	MakeMultiPoint(mapID, multi, endTime);
}

UINT32 GetGCWaitMapID()
{
	if(!GetGlobalConfig().GuildCastleScenes.empty())
	{
		return GetGlobalConfig().GuildCastleScenes[0];
	}
	return 0;
}

GCActStateMgr::GCActStateMgr()
{
	m_state = GCF_ACT_S_NONE;
	m_ctime = 0;
}

GCActStateMgr::~GCActStateMgr()
{

}

void GCActStateMgr::ChangeState(GCActState state, UINT32 now)
{
	if(m_state == state)
	{
		return ;
	}
	
	//SSDebug << __FUNCTION__ << " state = " << state << " now = " << now << END;

	m_state = state;
	m_ctime = now;
}

GCGuildJoin::GCGuildJoin()
{

}

GCGuildJoin::~GCGuildJoin()
{

}

void GCGuildJoin::AddRole(UINT64 roleID)
{
	m_roles.insert(roleID);
}

void GCGuildJoin::SetAttack(UINT32 territoryID, bool haveFight)
{
	m_attackResult.m_territoryID = territoryID;
	if(!haveFight)
	{
		m_attackResult.m_isWin = true;
	}
}

void GCGuildJoin::SetDefent(UINT32 territoryID, bool haveFight)
{
	m_defentResult.m_territoryID = territoryID;
	if(!haveFight)
	{
		m_defentResult.m_isWin = true;
	}
}

GCGResult GCGuildJoin::GetDefent()
{
	return m_defentResult;
}

GCGResult GCGuildJoin::GetAttack()
{
	return m_attackResult;
}

void GCGuildJoin::SetResult(UINT32 territoryID, bool win)
{
	if(m_attackResult.m_territoryID == territoryID)
	{
		m_attackResult.m_isWin = win;
	}
	else if(m_defentResult.m_territoryID  == territoryID)
	{
		m_defentResult.m_isWin = win;
	}
}



void GCGuildJoin::GiveReward(UINT64 guildID)
{
	SSDebug << __FUNCTION__ << " guildid = " << guildID << " rolesize = " << m_roles.size() << END;
	SSDebug << " attack " << m_attackResult.m_territoryID << " " << m_attackResult.m_isWin << END;
	SSDebug << " defent " << m_defentResult.m_territoryID << " " << m_defentResult.m_isWin << END;

	if(m_attackResult.m_territoryID && !m_defentResult.m_territoryID)
	{
		if(m_attackResult.m_isWin)
		{
			SSDebug << " give attack win reward " << END;
			GiveJoinReward(m_attackResult.m_territoryID, GC_JRT_ATTACK_WIN, guildID);
		}
		else
		{
			SSDebug << " give attack lose reward " << END;
			GiveJoinReward(m_attackResult.m_territoryID, GC_JRT_ATTACK_LOSE, guildID);
		}
	}
	if(!m_attackResult.m_territoryID && m_defentResult.m_territoryID)
	{
		if(m_defentResult.m_isWin)
		{
			SSDebug << " give defent win reward " << END;
			GiveJoinReward(m_defentResult.m_territoryID, GC_JRT_DEFENT_WIN, guildID);
		}
		else
		{
			SSDebug << " give defent lose reward " << END;
			GiveJoinReward(m_defentResult.m_territoryID, GC_JRT_DEFENT_LOSE, guildID);
		}
	}
	if(m_attackResult.m_territoryID && m_defentResult.m_territoryID)
	{
		if(m_attackResult.m_isWin)
		{
			SSDebug << " give attack win reward " << END;
			GiveJoinReward(m_attackResult.m_territoryID, GC_JRT_ATTACK_WIN, guildID);
		}
		else if(m_defentResult.m_isWin)
		{
			SSDebug << " give defent win reward " << END;
			GiveJoinReward(m_defentResult.m_territoryID, GC_JRT_DEFENT_WIN, guildID);
		}
		else
		{
			SSDebug << " give attack lose reward " << END;
			GiveJoinReward(m_attackResult.m_territoryID, GC_JRT_ATTACK_LOSE, guildID);
		}
	}
}

void GCGuildJoin::GetRoles(std::vector<UINT64> &refRoles)
{
	for (auto iter = m_roles.begin(); iter!=m_roles.end(); iter++)
	{
		refRoles.push_back(*iter);
	}
}

void GCGuildJoin::GiveJoinReward(UINT32 territoryID, GCJoinRewardType rewardType, UINT64 guildID)
{
	TerritoryBattle::RowData* pConf = CGuildTerritoryBattle::Instance()->GetTerrConf(territoryID);
	if(NULL == pConf)
	{
		SSError << " find terr conf failed id = " << territoryID << END;
		return ;
	}
	TerritoryBattleReward::RowData* pRewardConf = GuildCastleMgr::Instance()->GetJoinRewardConf(pConf->territorylevel, rewardType);
	if(NULL == pRewardConf)
	{
		SSError << " find terr reward failed level = " << pConf->territorylevel << " rewawdtype = " << rewardType << END;
		return ;
	}
	std::vector<ItemDesc> mailReward;
	for(size_t i = 0; i < pRewardConf->DropReward.size(); ++i)
	{
		mailReward.push_back(ItemDesc(pRewardConf->DropReward[i][0], pRewardConf->DropReward[i][1]));
	}

	//for(auto it = m_roles.begin(); it != m_roles.end(); )
	//{
	//	UINT64 roleID = (*it);
	//	UINT64 tmpgid = CGuildMgr::Instance()->GetGuildIdByRoleId(roleID);	
	//	if(tmpgid == guildID)//判断是否还是同一个公会
	//	{
	//		SendMail(mailReward, roleID, pRewardConf->MailContent, pConf->territoryname);
	//		it++;
	//	}
	//	else
	//	{
	//		m_roles.erase(it++);
	//	}
	//}


	auto pGuild = CGuildMgr::Instance()->GetGuild(guildID);
	if(pGuild)
	{
		pGuild->AddExp(pRewardConf->guildexp);
		pGuild->AddPrestige(pRewardConf->guildprestige);

		const auto& allMem = pGuild->GetGuildMember();
		for(auto it = allMem.begin(); it != allMem.end(); ++it)
		{
			UINT64 roleID = (it->first);
			SendMail(mailReward, roleID, pRewardConf->MailContent, pConf->territoryname);
		}
	}
	else
	{
		SSWarn << " find not guild = " << guildID << END;
	}
}

void GCGuildJoin::SendMail(const std::vector<ItemDesc>& mailReward, UINT64 roleID, int mailID, const std::string& terrName)
{
	std::string mailtitle = "null";
	std::string mailcontent = "null";
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(mailID);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "$S", terrName);
	}
	
	MailData maildata(mailtitle, mailcontent, mailReward, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_GuildTerrJoin);
	CMailMgr::Instance()->SendMail(roleID, maildata);
}
