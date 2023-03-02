#include "pch.h"
#include "guild/guildbonus.h"
#include "pb/project.pb.h"
#include "guildgoblin.h"
#include "util/timespecificmgr.h"
#include "foreach.h"
#include "guildlistener.h"
#include "scene/sceneconfig.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "table/guildConfig.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guildbonus.h"

GuildGoblin::GuildGoblin(Guild* pGuild)
{
	m_pGuild = pGuild;
	m_totalKillNum = 0;

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&GuildGoblin::OnTimer, this));
	m_updatetime = 0;
}

GuildGoblin::~GuildGoblin()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

bool GuildGoblin::OnTimer()
{
	Update();
	return true;
}

void GuildGoblin::Update()
{
	m_vecKillInfo.clear();
	m_roleid2rank.clear();
	m_isModified.Set();
	m_updatetime = (int)time(0);
}

bool GuildGoblin::IsChanged()
{
	return m_isModified.TestAndReset();
}

bool GuildGoblin::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GuildGoblinSave goblin;
	if (!guilddata.has_guildgoblin())
	{
		return false;
	}

	if (guilddata.guildgoblin().empty())
	{
		return false;
	}

	if (!goblin.ParseFromString(guilddata.guildgoblin()))
	{
		return false;
	}
	
	m_totalKillNum = goblin.totalkillnum();
	for(auto i = 0; i < goblin.killgoblininfo_size(); ++i)
	{
		KillGoblin info;
		info.load(goblin.killgoblininfo(i));
		m_vecKillInfo.push_back(info);
	}
	m_updatetime = goblin.updatetime();

	if (!XCommon::IsDaySame(m_updatetime, time(0)))
	{
		Update();
	}
	Sort();
	
	return true;
}

void GuildGoblin::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_guildgoblin()->assign(ToBinary());
}


const std::string GuildGoblin::ToBinary()
{
	KKSG::GuildGoblinSave save;
	save.set_totalkillnum(m_totalKillNum);
	for (auto iter = m_vecKillInfo.begin(); iter != m_vecKillInfo.end(); ++iter)
	{
		KKSG::KillGoblinInfo* temp = save.add_killgoblininfo();
		iter->save(*temp);
	}
	if (!m_updatetime)
	{
		m_updatetime = time(0);
	}
	save.set_updatetime(m_updatetime);

	return save.SerializeAsString();

}

int GuildGoblin::getGoblinLevel()
{
	return CGuildConfig::Instance()->getGuildGoblinLevelByCount(m_totalKillNum);
}

int GuildGoblin::getLeftEnterCount(CRole* role)
{
	return 0;
}

void GuildGoblin::getSummaryInfo(CRole* pCRole, KKSG::GuildGoblinInfoRes& res)
{
	res.set_level(getGoblinLevel());
	res.set_leftentercount(getLeftEnterCount(pCRole));
	res.set_guildtotalkillcount(m_totalKillNum);
	foreach(i in m_vecKillInfo)
	{
		KKSG::GuildGoblinRoleKillInfo* info = res.add_memberrankinfo();
		info->set_roleid(i->roleID);
		info->set_rolename(i->roleName);
		info->set_killnum(i->num);
		info->set_ability(i->ability);
		info->set_rolelevel(i->roleLevel);
	}
	res.set_errorcode(KKSG::ERR_SUCCESS);
}

void GuildGoblin::addKillNum(int num)
{
	int level = getGoblinLevel();
	m_totalKillNum += num;
	m_isModified.Set();
	int newLevel = getGoblinLevel();
	if (level != newLevel)
	{
		GuildBonusMgr* bonus = m_pGuild->Get<GuildBonusMgr>();
		if (bonus)
		{
			bonus->addGuildBonus(KKSG::GBONUS_GUILDGOBLIN_LEVELUP, newLevel, 0);
		}
	}
}

void GuildGoblin::addKillNum(UINT64 roleID, int num)
{
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
	if (!summary)
	{
		return;
	}
	auto find = m_roleid2rank.find(roleID);
	if (find != m_roleid2rank.end())
	{
		if (find->second >= m_vecKillInfo.size())
		{
			return;
		}
		m_vecKillInfo[find->second].num += num;
		m_vecKillInfo[find->second].ability = summary->GetPowerPoint();
		m_vecKillInfo[find->second].roleLevel = summary->GetLevel(); 
	}
	else
	{
		KillGoblin kill;
		kill.ability =  summary->GetPowerPoint();
		kill.num = num;
		kill.roleID = summary->GetID();
		kill.roleLevel = summary->GetLevel();
		kill.roleName = summary->GetName();

		m_vecKillInfo.push_back(kill);
	}
	m_isModified.Set();
}

void GuildGoblin::Sort()
{
	std::sort(m_vecKillInfo.begin(), m_vecKillInfo.end());
	m_roleid2rank.clear();
	int rank = 0;
	foreach(i in m_vecKillInfo)
	{
		m_roleid2rank.insert(std::make_pair(i->roleID, rank++));
	}
}

int GuildGoblin::getRank(UINT64 roleID)
{
	auto find = m_roleid2rank.find(roleID);
	if(find == m_roleid2rank.end())
	{
		return 0;
	}
	return (find->second + 1);
}

void GuildGoblin::UpdateGoblinInfo(const KKSG::KillGoblinTotalInfo* info)
{
	if (!info)
	{
		return;
	}
	addKillNum(info->killnum());
	for (size_t i = 0; i < info->members_size(); ++i)
	{
		addKillNum(info->members(i), info->killnum());
	}

	Sort();

	for (auto i = 0; i < info->killelitenum(); ++i)
	{
		GuildBonusMgr* bonus = m_pGuild->Get<GuildBonusMgr>();
		if (bonus)
		{
			CRole* temp = NULL;
			//需要处理
			/*if (!vecCRole.empty())
			{
				temp = vecCRole[0];
			}*/
			bonus->addGuildBonus(KKSG::GBONUS_GUILDGOBLIN, info->goblinlevel(), /*temp*/ 0);
		}
	}

	std::map<UINT32, UINT32> rewardMap;
	foreach (j in info->guildreward())
	{
		rewardMap[j->type()] = j->num();
	}
	m_pGuild->AddGuildReward(rewardMap);
}

void KillGoblin::load(const KKSG::KillGoblinInfo& info)
{
	this->ability = info.ability();
	this->num = info.killnum();
	this->roleID = info.roleid();
	this->roleLevel = info.rolelevel();
	this->roleName = info.rolename();
}

void KillGoblin::save(KKSG::KillGoblinInfo& info)
{
	info.set_ability(this->ability);
	info.set_killnum(this->num);
	info.set_roleid(this->roleID);
	info.set_rolelevel(this->roleLevel);
	info.set_rolename(this->roleName);
}
