#include "pch.h"
#include <time.h>
#include "guildmgr.h"
#include "config.h"
#include "role/role.h"
#include "guild.h"
#include "guildboss.h"
#include "guildbonus.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "scene/scenemanager.h"
#include "guild/guildactivity.h"
#include "util/XCommon.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "guild/ptcm2c_loginguildinfo.h"
#include "table/guildConfig.h"
#include "loghelper/tlogr.h"
#include "idip/idip.h"
#include "notice/noticemgr.h"
#include "table/guildSkillConfig.h"
#include "util/timespecificmgr.h"
#include "guild/guildcheckin.h"
#include "guild/guildboss.h"
#include "guild/guildladder.h"
#include "foreach.h"
#include "event/eventmgr.h"
#include "util/gametime.h"
#include "guild/guildcamp.h"
#include "garden/garden_manager.h"
#include "guild/guildarena.h"
#include "table/OpenSystemMgr.h"
#include "guild/guildterritorybattle.h"
#include "loghelper/reportdatamgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "role/rolemanager.h"
#include "role/role.h"

#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "team/teamforceop.h"

#include "guild/guildcard.h"


INSTANCE_SINGLETON(CGuildMgr)

CGuildMgr::CGuildMgr()
{
	m_bIsLoaded = false;
	m_LastSaveGuildTime = 0;
	m_saveMode = 0;
}

CGuildMgr::~CGuildMgr()
{

}

bool CGuildMgr::Init()
{
	std::map<UINT32, std::string> fieldMap;
	fieldMap[0] = "brief,";
	fieldMap[1] = "application,";
	fieldMap[2] = "history,";
	fieldMap[3] = "card,";
	fieldMap[4] = "checkin,";
	fieldMap[5] = "guildbonus,";
	fieldMap[6] = "guildgoblin,";
	fieldMap[7] = "guildactivity,";
	fieldMap[8] = "guildwage,";
	fieldMap[9] = "guildcamp,";
	fieldMap[10] = "garden,";
	fieldMap[11] = "guilddonate";

	std::stringstream sql;
	sql << "select  ";
	foreach (i in fieldMap)
	{
		sql << i->second;
	}
	sql << "  from guild;";

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	IMysqlRecordSet* poRes = NULL;
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	if(!poMySqlConn->QueryWithRecord(sql.str().c_str(), sql.str().size(), &poRes))
	{
		LogError("Read table guild failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while (poRes->FetchRow())
	{
		char* pField   = NULL;
		UINT32 len = 0;
		std::string strData;
		KKSG::GuildAllDBData guildData;

		for (UINT32 cnt = 0; cnt < fieldMap.size(); ++cnt)
		{

			strData.clear();
			if(poRes->GetFieldValue(cnt, &pField, len))
			{
				strData.assign(pField, len);
			}
			else
			{
				LogError("Read guild : %s failed", fieldMap[cnt].c_str());
				return false;
			}

			switch(cnt)
			{
			case (0):
				{
					guildData.set_brief(strData);
				}
				break;
			case (1):
				{
					guildData.set_application(strData);
				}
				break;
			case (2):
				{
					guildData.set_history(strData);
				}
				break;
			case (3):
				{
					guildData.set_card(strData);
				}
				break;
			case (4):
				{
					guildData.set_checkin(strData);
				}
				break;
			case (5):
				{
					guildData.set_guildbonus(strData);
				}
				break;
			case (6):
				{
					guildData.set_guildgoblin(strData);
				}
				break;
			case (7):
				{
					guildData.set_guildactivity(strData);
				}
				break;
			case (8):
				{
					guildData.set_guildwage(strData);
				}
				break;
			case (9):
				{
					guildData.set_guildcamp(strData);
				}
				break;
			case (10):
				{
					guildData.set_garden(strData);
				}
				break;
			case  11:
				guildData.set_guilddonate(strData);
				break;
			default:
				break;
			}
		}

		LoadDBGuild(guildData);
	}
	poRes->Release();
	m_sorter.Sort();

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all guild data from db use time: %llu ms", qwTimeUsed);


	if (!m_SkillTable.LoadFile("table/SkillList.txt"))
	{
		SSWarn << "Load file ProfessionSkill.txt " << " failed!" << END;
		return false;
	}

	UINT32 entityTempID = 0;
	std::string skillName;
	std::vector<SkillList::RowData*> list;
	for (unsigned i = 0; i < m_SkillTable.Table.size(); ++i)
	{
		auto &row = *m_SkillTable.Table[i];

		if (row.LevelupCostType == 22)
		{
			UINT32 hash = XCommon::XHash(row.SkillScript.c_str());
		}

		if (row.Profession != 0)
		{
			unsigned a = row.PhysicalRatio.size();

			if (a != row.PhysicalFixed.size())
			{
				LogError("SkillList.txt PhysicalRatio != PhysicalFixed in row [%s]", row.ScriptName.c_str());
			}

			if (a != row.DecreaseSuperArmor.size())
			{
				LogError("SkillList.txt PhysicalRatio != DecreaseSuperArmor in row [%s]", row.ScriptName.c_str());
			}

			if (a != row.PvPRatio.size())
			{
				LogError("SkillList.txt PhysicalRatio != PvPRatio in row [%s]", row.ScriptName.c_str());
			}

			if (a != row.PvPFixed.size())
			{
				LogError("SkillList.txt PhysicalRatio != PvPFixed in row [%s]", row.ScriptName.c_str());
			}

			if (a != row.PvPDecreaseSuperArmor.size())
			{
				LogError("SkillList.txt PhysicalRatio != PvPDecreaseSuperArmor in row [%s]", row.ScriptName.c_str());
			}
		}

		if (row.SkillType == 3)
		{
			UINT32 hash = XCommon::XHash(row.SkillScript.c_str());
		}

		if (skillName.empty())
		{
			skillName = row.SkillScript;
			entityTempID = row.XEntityStatisticsID;
			list.push_back(&row);
			continue;
		}

		if (skillName == row.SkillScript &&
			entityTempID == row.XEntityStatisticsID)
		{
			list.push_back(&row);
		}
		else
		{
			UINT32 hash = XCommon::XHash(skillName.c_str());
			if (entityTempID != 0)
			{
				/*UINT64 key = hash;
				key = ((key << 32) | entityTempID);
				m_SpecialEnemySkillIndex[key].swap(list);*/
			}
			else
			{
				m_SkillIndex[hash].swap(list);
			}

			skillName = row.SkillScript;
			entityTempID = row.XEntityStatisticsID;
			list.push_back(&row);
		}
	}

	UINT32 hash = XCommon::XHash(skillName.c_str());
	if (entityTempID != 0)
	{
		/*UINT64 key = hash;
		key = ((key << 32) | entityTempID);
		m_SpecialEnemySkillIndex[key].swap(list);*/
	}
	else
	{
		m_SkillIndex[hash].swap(list);
	}

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CGuildMgr::OnTimer, this));
	SetLoaded();
	GetSorter().Sort();

	InitGuildTimeBonus();

	return true;
}

void CGuildMgr::Uninit()
{

}

Guild * CGuildMgr::GetGuild(UINT64 gid)
{
	auto it = m_guilds.find(gid);
	return (it == m_guilds.end()) ? NULL : it->second;
}

Guild * CGuildMgr::GetGuildByRoleId(UINT64 roleId)
{
	UINT64 guildid = GetRoleGuild(roleId);
	if (INVALID_GUILDID == guildid)
	{
		return NULL;
	}
	return GetGuild(guildid);
}

void CGuildMgr::AddGuild(Guild *pGuild)
{
	m_guilds[pGuild->GetID()] = pGuild;
	m_mapGuildNames.insert(std::pair<std::string,Guild*> (pGuild->GetName(),pGuild));
	pGuild->AddListener(this);
}


static int GUILDID = 0;

UINT64 CGuildMgr::MakeGuildID()
{
	// gid 64 位结构
	// [服ID]   [时间戳]  [自增长ID] 
	//  FFFF   FFFFFFFF    FFFF
	time_t ServerTime = time(NULL);
	//UINT64 serverID = GSConfig::Instance()->GetServerID();
	UINT64 serverID = MSConfig::Instance()->GetServerID();
//	UINT64 serverID = 1;
	serverID = ((serverID << 32) | ServerTime);
	serverID = ((serverID << 16) | ((++GUILDID) & 0xFFFF));
	return serverID;
}

int CGuildMgr::CreateGuild(CRole* pRole, const ::std::string& guildName, int icon)
{
	if (GetRoleGuild(pRole->GetID()) != INVALID_GUILDID)
	{
		SSWarn << "CGuildMgr::CreateGuild role already in guild roleid : " << pRole->GetID() << " and guildid : " << GetRoleGuild(pRole->GetID()) << END; 
		return KKSG::ERR_GUILD_ALREADY_IN_GUILD;
	}

	int ret = XCommon::ValidNameCheck(guildName);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	if (!IsNameNotUsed(guildName))
	{
		return KKSG::ERR_GUILD_NAME_EXIST;
	}

	Guild *pGuild = new Guild();
	pGuild->OnCreate(pRole, MakeGuildID(), guildName);
	pGuild->InitSerializer();
	pGuild->SetIcon(icon);
	AddGuild(pGuild);

	pGuild->AddMember(pRole->GetID(), GPOS_LEADER);
	CGuildSkillConfig::Instance()->InitGuildSkill(pGuild->GetStudySkill()); //
	pGuild->InsertTODB();

	CReportDataMgr::Instance()->CreateGuild(pRole, guildName);

	OnGuildCreated(pGuild);
	m_sorter.AddGuild(pGuild);
	m_sorter.Sort();
	CGuildConfig::Instance()->NoticeGuildSystemOpen(pGuild->GetID(), pGuild->GetLevel());

	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = pRole->GetID();
	oLog.m_iActType = TX_GUILD_ACT_TYPE_1;
	pGuild->DoTxLog(oLog);
	oLog.Do();
	return KKSG::ERR_SUCCESS;
}

int CGuildMgr::JoinGuild(UINT64 roleId, UINT32 attr, UINT64 guildId)
{
	SSInfo << "CGuildMgr::JoinGuild try roleId : " << roleId << "guildid" << guildId << END; 

	if (GetRoleGuild(roleId) != INVALID_GUILDID)
	{
		return KKSG::ERR_GUILD_ALREADY_IN_GUILD;
	}

	Guild *pGuild = NULL;
	if (guildId == 0)
	{
		pGuild = FindQuickJoinGuild(attr);
	}
	else
	{
		pGuild = GetGuild(guildId);
	}

	if (pGuild == NULL)
	{
		return KKSG::ERR_GUILD_NOT_EXIST;
	}

	if (pGuild->IsApplyFull())
	{
		return KKSG::ERR_GUILD_APPLYFULL;
	}

	if (pGuild->IsFull())
	{
		return KKSG::ERR_GUILD_FULL;
	}

	if (!CheckJoin(roleId))
	{
		return KKSG::ERR_UNUSE1;
	}

	return pGuild->AddApplication(roleId, attr);
}

UINT64 CGuildMgr::GetRoleGuild(UINT64 qwRoleID)
{
	auto it = m_roleid2guildid.find(qwRoleID);
	return (it == m_roleid2guildid.end()) ? INVALID_GUILDID : it->second;
}

const std::string& CGuildMgr::GetName(UINT64 guildid)
{
	Guild* guild = GetGuild(guildid);
	if (NULL == guild)
	{
		static std::string EmptyString = "";
		return EmptyString;
	}
	else
	{
		return guild->GetName();
	}
}

void CGuildMgr::OnGuildAddPrestige(Guild* pGuild)
{

	if (!m_sorter.IsGuildInSort(pGuild->GetID()))
	{
		m_sorter.AddGuild(pGuild);
		m_sorter.Sort();
	}
	else
	{
		m_sorter.SetChanged(KKSG::GuildSortByPrestige);
	}
}

void CGuildMgr::OnGuildLevelUp(Guild *pGuild)
{
	if (!m_sorter.IsGuildInSort(pGuild->GetID()))
	{
		m_sorter.AddGuild(pGuild);
		m_sorter.Sort();
	}
	else
	{
		m_sorter.SetChanged(KKSG::GuildSortByLevel);
	}
}

void CGuildMgr::OnAddMember(Guild *pGuild, GuildMember *pMember)
{
	m_sorter.SetChanged(KKSG::GuildSortByMemberCount);
	m_roleid2guildid[pMember->qwRoleID] = pGuild->GetID();
}

void CGuildMgr::OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember)
{
	m_roleid2guildid[pMember->qwRoleID] = pGuild->GetID();
}

void CGuildMgr::OnRemoveMember(Guild *pGuild, UINT64 qwRoleID)
{
	CReportDataMgr::Instance()->LeaveGuild(qwRoleID);
	m_sorter.SetChanged(KKSG::GuildSortByMemberCount);
	m_roleid2guildid.erase(qwRoleID);

	//解散公会和离开公会的时候需要记录日志;
	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	pGuild->DoTxLog(oLog);
	oLog.m_iActType = TX_GUILD_ACT_TYPE_5;
	oLog.m_uRoleID = qwRoleID;
	oLog.Do();
}

void CGuildMgr::OnGuildAddExp(Guild* pGuild)
{

	if (!m_sorter.IsGuildInSort(pGuild->GetID()))
	{
		m_sorter.AddGuild(pGuild);
		m_sorter.Sort();
	}
	else
	{
		m_sorter.SetChanged(KKSG::GuildSortByExp);
		m_sorter.SetChanged(KKSG::GuildSortByLevel);
	}
}

void CGuildMgr::OnMemberPosChanged(Guild *pGuild, GuildMember *pMember)
{
	if (NULL == pGuild || NULL == pMember)
	{
		return ;
	}
	CReportDataMgr::Instance()->GuildMemberPosChanged(pMember->qwRoleID, pMember->position);
}

void CGuildMgr::AddDismissGuild(Guild *pGuild)
{
	SSInfo << " CGuildMgr::AddDismissGuild guldid : " << pGuild->GetID() << END;
	m_DismissedGuild.push_back(pGuild);
}

void CGuildMgr::Update()
{
	UINT64 gid;
	foreach (i in m_DismissedGuild)
	{
		Guild *pGuild = *i;
		RemoveGuildName(pGuild->GetName());
		m_sorter.RemoveGuild(pGuild);
		gid = pGuild->GetID();
		GardenManager::Instance()->RemoveGuildGarden(gid);
		pGuild->Dismiss();		
		delete pGuild;
		m_guilds.erase(gid);			
	}

	m_DismissedGuild.clear();

	int now = GameTime::GetTime();
	if (now > m_LastSaveGuildTime + 10)
	{
		UINT32 num = 0;
		UINT32 successNum = 0;
		UINT32 mode = GetSaveMode();
		foreach(i in m_guilds)
		{
			if ((i->second->GetID() % 10) == mode)
			{
				i->second->SaveToDB(successNum);
				++num;
			}
		}

		m_LastSaveGuildTime = now;
		if (num > 0)
		{
			SSInfo << "guild save mode = " << mode << " try saved guild num = " << num << " and success num = " << successNum << END;		
		}
	}

	for (auto iter = m_allGuildTimeBonus.begin(); iter != m_allGuildTimeBonus.end();)
	{
		if (now >= iter->second)
		{
			foreach(i in m_guilds)
			{
				auto record = i->second->Get<GuildBonusMgr>();
				record->addGuildBonus(iter->first, 0);
			}
			m_allGuildTimeBonus.erase(iter++);
		}
		else
			++iter;
	}
}

void CGuildMgr::RemoveGuildName(const std::string &name)
{
	auto iter = m_mapGuildNames.find(name);
	if (iter!=m_mapGuildNames.end())
	{
		m_mapGuildNames.erase(iter);
	}
}

void CGuildMgr::SaveToDB()
{
	UINT64 gid;
	foreach (i in m_DismissedGuild)
	{
		Guild *pGuild = *i;
		RemoveGuildName(pGuild->GetName());
		m_sorter.RemoveGuild(pGuild);
		gid = pGuild->GetID();
		pGuild->Dismiss();
		delete pGuild;
		m_guilds.erase(gid);
	}

	m_DismissedGuild.clear();

	foreach(i in m_guilds)
	{
		UINT32 num = 0;
		i->second->SaveToDB(num);
	}

	m_LastSaveGuildTime = time(NULL);
}

UINT64 CGuildMgr::GetGuildIdByRoleId(UINT64 roleid)
{
	Guild* pGuild = GetGuildByRoleId(roleid);
	if(pGuild)
	{
		return pGuild->GetID();
	}
	return 0;
}

bool CGuildMgr::IsSameGuild(UINT64 roleID11, UINT64 roleID22)
{
	UINT64 gid11 = GetGuildIdByRoleId(roleID11);
	UINT64 gid22 = GetGuildIdByRoleId(roleID22);
	if(0 != gid11 && gid11 == gid22)
	{
		return true;
	}
	return false;
}


void CGuildMgr::LoadDBGuild(const KKSG::GuildAllDBData &guilddata)
{
	Guild *pGuild = new Guild();
	pGuild->InitSerializer();
	pGuild->AddListener(this);
	pGuild->LoadFromDB(guilddata);
	m_mapGuildNames.insert(std::pair<std::string, Guild*>(pGuild->GetName(),pGuild));
	m_guilds[pGuild->GetID()] = pGuild;
	m_sorter.AddGuild(pGuild);
	///>AudioChatMgr::Instance()->LoadChatInfo(KKSG::GuildChannel, pGuild->GetID());
}

Guild * CGuildMgr::FindQuickJoinGuild(double ppt)
{
	std::vector<Guild *> &Guilds = m_sorter.GetSortResult(KKSG::GuildSortByLevel);
	for (UINT32 i = 0; i < Guilds.size(); ++i)
	{
		if (Guilds[i]->GetNeedApproval() == 0 &&
			Guilds[i]->GetRecuritPPT() <= ppt &&
			!Guilds[i]->IsFull())
		{
			return Guilds[i];
		}
	}

	return NULL;
}

void CGuildMgr::OnGuildCreated(Guild* pGuild)
{
	if (pGuild->Get<GuildBonusMgr>())
	{
		pGuild->Get<GuildBonusMgr>()->Update();
	}
	pGuild->Get<CGuildCamp>()->Update();
}

void CGuildMgr::AddRoleContribute(UINT64 qwRoleID, int contribute)
{
	Guild *pGuild = GetGuildByRoleId(qwRoleID);
	if(pGuild)
	{
		pGuild->AddRoleContribute(qwRoleID, contribute);
	}
}

void CGuildMgr::HintNotify(CRole* role)
{
	Guild* guild = GetGuildByRoleId(role->GetID());
	if (NULL == guild)
	{
		return;
	}
}

void CGuildMgr::DoGuildDareReward(const std::set<UINT64>& roleSet, std::string sceneName, std::map<UINT64, UINT32> guildexp)
{
	std::map<UINT64, std::set<UINT64> > guildRoleMap;
	foreach (i in roleSet)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(*i);
		if (pGuild == NULL)
		{
			continue;
		}
		std::set<UINT64>& roleSet = guildRoleMap[pGuild->GetID()];
		roleSet.insert(*i);
	}

	foreach (j in guildRoleMap)
	{
		if (j->second.size() >= 2)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuild(j->first);
			if (pGuild != NULL)
			{
				pGuild->AddDareTimes(j->second.size());
			}
			NoticeMgr::Instance()->NoticeGuildDareFinished(j->second, sceneName, j->first, guildexp[j->first]);
		}
	}
}

void CGuildMgr::BillingActivity()
{
	for (auto i = m_guilds.begin(); i != m_guilds.end(); ++i)
	{
		CGuildActivity* pActicity = i->second->Get<CGuildActivity>();
		if (pActicity != NULL && pActicity->DoCheck())
		{
			TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
			i->second->DoTxLog(oLog);
			oLog.m_iActType = TX_GUILD_ACT_TYPE_2;
			oLog.m_uRoleID = i->second->GetLeaderID();
			oLog.m_DismissMode = Tx_GuildDismiss_System;
			oLog.Do();
			AddDismissGuild(i->second);
		}
	}
	Update();
}

Guild* CGuildMgr::GetGuildByName(const std::string &name)
{
	auto iter = m_mapGuildNames.find(name);
	if (iter!=m_mapGuildNames.end())
	{
		return iter->second;
	}
	return NULL;
}

void CGuildMgr::DoRoleLogin(CRole* pRole)
{
	if (pRole == NULL)
	{
		SSWarn << " CGuildMgr::DoRoleLogin pRole is NULL" << END;
		return ;
	}

	Guild* pGuild = GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		SSWarn << " CGuildMgr::DoRoleLogin pGuild is NULL and roleid : " << pRole->GetID() <<END;
		return ;
	}

	pGuild->DoRoleLogin(pRole);
}

UINT32 CGuildMgr::CanCreateGuild(CRole* pRole, std::string name, UINT32 icon)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (GetGuildByRoleId(pRole->GetID()) != NULL)
	{
		return KKSG::ERR_GUILD_ALREADY_IN_GUILD;
	}

	int ret = XCommon::ValidNameCheck(name);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	if (!IsNameNotUsed(name))
	{
		return KKSG::ERR_GUILD_NAME_EXIST;
	}

	return KKSG::ERR_SUCCESS;
}

SkillList::RowData* CGuildMgr::GetSkillConfig(UINT32 skillHash, int skillLevel)
{
	auto i = m_SkillIndex.find(skillHash);
	if (i == m_SkillIndex.end())
	{
		return NULL;
	}

	auto &skillList = i->second;
	for (UINT32 j = 0; j < skillList.size(); ++j)
	{
		if (skillLevel <= skillList[j]->SkillLevel)
		{
			return skillList[j];
		}
	}

	return NULL;
}

bool CGuildMgr::OnTimer()
{
	ClearJoinCount();
	for (auto iter = m_guilds.begin(); iter != m_guilds.end(); ++iter)
	{
		iter->second->DoDailyWork();
	}

	if (XCommon::GetWeekNum() == 1)
	{
		for (auto iter = m_guilds.begin(); iter != m_guilds.end(); ++iter)
		{
			iter->second->DoWeekWork();
		}
	}

	// 公会排行奖励
	UINT32 nowTime = time(NULL);
	UINT32 guildRankTime = XCommon::GetDayBeginTime(MSConfig::Instance()->GetGameServerOpenTimeStamp()) + (GetGlobalConfig().GuildRankRewardTime) * (24*60*60);
	if (XCommon::GetDayBeginTime(nowTime) == guildRankTime) 
	{

		std::vector<Guild *>& guildList = m_sorter.GetSortResult(KKSG::GuildSortByPrestige);
		auto iter = guildList.begin();
		std::vector<std::string> guildVec;
		for (UINT32 cnt = 1; cnt <= 5; ++cnt)
		{
			if (iter == guildList.end())
			{
				guildVec.push_back("");
			}
			else
			{
				guildVec.push_back((*iter)->GetName());
				++iter;
			}
		}
		NoticeMgr::Instance()->NoticeGuildRankEnd(guildVec);

		UINT32 rank = 1;
		foreach (i in guildList)
		{
			DoRankLog(*i, rank);
			(*i)->SendGuildRankReward(rank);
			++rank;
		}
	}

	// 公会活跃度结算
	BillingActivity(); 


	CGlobalConfigDBBuild::Instance()->SetConfigValue(eGuildMgrTime, ToString(GameTime::GetTime()));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eGuildMgrTime);
	return true;
}

void CGuildMgr::DoOnlineCheck(UINT64 roleId)
{
	CGuildLadder::Instance()->DoOnlineCheck(roleId);
	CGuildBossMgr::Instance()->CheckRoleOnline(roleId);
	CGuildArenaIntegralMgr::Instance()->DoOnlineCheck(roleId);
	CGuildTerritoryBattle::Instance()->DoOnlineCheck(roleId);
}

void CGuildMgr::InitGuildTimeBonus()
{
	int now = time(0);
	auto table = CGuildConfig::Instance()->GetGuildBonusTable();
	for (auto iter = table->Table.begin(); iter != table->Table.end(); ++iter)
	{
		if ((*iter)->GuildBonusType == KKSG::GBONUS_TIMEBONUS)
		{
			int time = XCommon::ParseTimeBackSlash((*iter)->TriggerTime);
			if (time)
			{
				if (now < time)
				{
					m_allGuildTimeBonus.insert(std::make_pair((*iter)->GuildBonusID, time));
					LogInfo("init time bonus, content type:%u, time:%u", (*iter)->GuildBonusID, time);
				}
			}
			else
			{
				LogError("time format error, bonus content type:%u", (*iter)->GuildBonusID);
			}
		}
	}
}

UINT32 CGuildMgr::GetRankNum(UINT32 rankType, UINT64 guildId)
{
	return m_sorter.GetRankNum(rankType, guildId);
}

void CGuildMgr::RemoveGuildFromSort(UINT64 guildId)
{
	Guild* pGuild = GetGuild(guildId);
	if (pGuild != NULL)
	{
		m_sorter.RemoveGuild(pGuild);
	}
}

void CGuildMgr::StartUpGuildmgrEvent()
{
	std::string szEvent = CGlobalConfigDBBuild::Instance()->GetConfigValue(eGuildMgrTime);
	if(!szEvent.empty())
	{
		UINT32 nEventTime = convert<UINT32>(szEvent);
		UINT32 nowTime = GameTime::GetTime();
		if (!XCommon::IsDaySame(nowTime, nEventTime))
		{
			OnTimer();
		}
	}


	for (auto iter = m_guilds.begin(); iter!= m_guilds.end(); iter++)
	{
		Guild* pGuild = iter->second;
		if (pGuild!=NULL)
		{
			CGuildCard* pGuildCard = pGuild->Get<CGuildCard>();
			pGuildCard->Update();
		}
	}

}

void CGuildMgr::StressTest(UINT32 nMaxPlayer)
{
	for (auto iter = m_guilds.begin(); iter!= m_guilds.end(); iter++)
	{
		Guild* pGuild = iter->second;
		if (pGuild)
		{
			std::vector<CRole *> vecRole;
			const std::map<UINT64, GuildMember> mapMember = pGuild->GetGuildMember();
			for (auto siter = mapMember.begin();siter!=mapMember.end();siter++)
			{
				//查找是否在线
				GuildMember member = siter->second;
				CRole *pRole = CRoleManager::Instance()->GetByRoleID(member.qwRoleID);
				if (pRole)
				{
					vecRole.push_back(pRole);
					if (vecRole.size()>=nMaxPlayer)
					{
						TeamForceOp oper;
						Team *pTeam = oper.CreateTeam(vecRole, 42);
						if (pTeam)
						{
							LogInfo("Stress Test CreateTeam===teamid=%d",pTeam->GetID());
						}
						vecRole.clear();
					}
				}
			}
		}
	}

}

bool CGuildMgr::ModifyName(Guild *pGuild,const std::string &oldName,const std::string &newName)
{
	auto iter = m_mapGuildNames.find(oldName);
	if (iter != m_mapGuildNames.end())
	{
		m_mapGuildNames.erase(oldName);
		m_mapGuildNames.insert(std::pair<std::string,Guild*>(newName,pGuild));	
		return true;
	}
	return false;
}

void CGuildMgr::AddJoinInfo(UINT64 nRoleID)
{
	auto iter = m_mapGuildDoor.find(nRoleID);
	if (iter!=m_mapGuildDoor.end())
	{
		GuildDoor &ref = iter->second;
		ref.nCount++;
		if (ref.nCount>=GetGlobalConfig().LeaveGuildCount)
		{
			ref.nNextTime = GameTime::GetTime() + GetGlobalConfig().LeaveGuildCD;
		}
		LogInfo("Role Join Guild nRoleID=%llu nCount=%u nNextTime=%u",nRoleID,ref.nCount,ref.nNextTime);
	}else
	{
		GuildDoor door;
		door.nCount = 1;
		door.nNextTime = 0;
		m_mapGuildDoor.insert(std::pair<UINT64,GuildDoor>(nRoleID,door));
	}
}

void CGuildMgr::ClearJoinCount()
{
	for(auto iter = m_mapGuildDoor.begin(); iter!=m_mapGuildDoor.end(); iter++)
	{
		GuildDoor &ref = iter->second;
		ref.nCount = 0;
	}
}

bool CGuildMgr::CheckJoin(UINT64 nRoleID)
{
	auto iter = m_mapGuildDoor.find(nRoleID);
	if (iter!=m_mapGuildDoor.end())
	{
		if (GameTime::GetTime()>iter->second.nNextTime)
		{
			return true;
		}else
		{
			return false;
		}
	}
	return true;
}

void CGuildMgr::DoRankLog(Guild* pGuild, UINT32 rank)
{
	if (pGuild && rank <= 500)
	{
		TGuildRankFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_GuildID = pGuild->GetID();
		oLog.m_GuildLevel = pGuild->GetLevel();
		oLog.m_Rank = rank;
		oLog.m_Prestige = pGuild->GetPrestige();
		oLog.m_strName = pGuild->GetName();
		oLog.Do();
	}
}

