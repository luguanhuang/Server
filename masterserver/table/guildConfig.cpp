#include "pch.h"
#include "guildConfig.h"
#include "foreach.h"
#include "table/GuildPermissionTable.h"
#include "notice/notice.h"


INSTANCE_SINGLETON(CGuildConfig)

CGuildConfig::CGuildConfig()
{

}

CGuildConfig::~CGuildConfig()
{

}


static GuildPermission ToGuildPermission(const std::string &guild)
{
#define ENUM2STR(X) if (guild == #X) return X
	ENUM2STR(GPEM_DISMISS);
	ENUM2STR(GPEM_CHANGELEADER);
	ENUM2STR(GPEM_SET_VICELEADER);
	ENUM2STR(GPEM_SET_OFFICER);
	ENUM2STR(GPEM_SET_ELITEMEMBER);
	ENUM2STR(GPEM_SETTINGS);
	ENUM2STR(GPEM_ANNOUNCEMENT);
	ENUM2STR(GPEM_APPROVAL);
	ENUM2STR(GPEM_FIREMEMBER);
	ENUM2STR(GPEM_SENDCHECKINBONUS);
	ENUM2STR(GPEM_STUDY_SKILL);
	ENUM2STR(GPEM_ADMINISTRATOR_REWAD);
	ENUM2STR(GPEM_ADMINISTRATOR_DISTRIBUTION);
#undef  ENUM2STR
	return GPEM_MAX;
}

bool CGuildConfig::Init()
{
	if (!m_oTable.LoadFile("table/GuildConfig.txt"))
	{
		LogWarn("Load GuildConfig.txt failed!");
		return false;
	}
	//return true;


	GuildPermissionTable oTable;
	if (!oTable.LoadFile("table/GuildPermission.txt"))
	{
		LogWarn("Load GuildPermission.txt failed!");
		return false;
	}
	if (!m_oTable.LoadFile("table/GuildConfig.txt"))
	{
		LogWarn("Load GuildConfig.txt failed!");
		return false;
	}
	if (!m_desctable.LoadFile("table/GuildSystem.txt"))
	{
		LogWarn("Load GuildSystem.txt failed!");
		return false;
	}
	if (!m_oGuildBonusTable.LoadFile("table/GuildBonus.txt"))
	{
		LogWarn("Load GuildBonus.txt failed!");
		return false;
	}
	if (!m_oGuildGoblinTable.LoadFile("table/GuildGoblin.txt"))
	{
		LogWarn("Load GuildGoblin.txt failed!");
		return false;
	}
	if (!m_oGuildChallegeTable.LoadFile("table/GuildChallege.txt"))
	{
		LogWarn("Load GuildChallege.txt failed!");
		return false;
	}
	if (!m_oRankRewardTable.LoadFile("table/GuildRankReward.txt"))
	{
		LogWarn("Load GuildRankReward.txt failed!");
		return false;
	}
	if (!m_oGuildSalaryTable.LoadFile("table/Guildsalary.txt"))
	{
		LogWarn("Load Guildsalary.txt failed!");
		return false;
	}
	if (!m_oGuildCampTable.LoadFile("table/GuildCamp.txt"))
	{
		LogWarn("Load GuildCamp.txt failed!");
		return false;
	}
	if (!m_oGuildCampRankTable.LoadFile("table/GuildCampRank.txt"))
	{
		LogWarn("Load GuildCampRank.txt failed!");
		return false;
	}


	for (UINT32 i = 0; i < oTable.Table.size(); ++i)
	{
		auto *pRow = oTable.Table[i];
		GuildPermission pem = ToGuildPermission(pRow->GuildID);
		if (pem == GPEM_MAX)
		{
			LogWarn("Load GuildPermission.txt with error in line %d, %s", i, pRow->GuildID.c_str());
			return false;
		}

		m_PERMMATRIX[pem][GPOS_LEADER]      = !!pRow->GPOS_LEADER;
		m_PERMMATRIX[pem][GPOS_VICELEADER]  = !!pRow->GPOS_VICELEADER;
		m_PERMMATRIX[pem][GPOS_OFFICER]     = !!pRow->GPOS_OFFICER;
		m_PERMMATRIX[pem][GPOS_ELITEMEMBER] = !!pRow->GPOS_ELITEMEMBER;
		m_PERMMATRIX[pem][GPOS_MEMBER]      = !!pRow->GPOS_MEMBER;
	}

	std::set<UINT32> tempids;
	for (UINT32 i = 0; i < oTable.Table.size(); ++i)
	{
		for (UINT32 j = 0; j < GuildDummy; ++j)
		{
			auto k = tempids.find(j);
			if (k == tempids.end())
			{
				if (GetConfig(i + 1, j) > 0)
				{
					m_level2systemid[i + 1].push_back(j);
					tempids.insert(j);
				}
			}
		}
	}

	for (UINT32 cnt = 0; cnt < m_oGuildChallegeTable.Table.size(); ++cnt)
	{
		std::vector<UINT32>& vec = m_guildChallegeMap[m_oGuildChallegeTable.Table[cnt]->GuildRank];
		vec.push_back(m_oGuildChallegeTable.Table[cnt]->Mantime);
	}

	return true;
}

void CGuildConfig::Uninit()
{

}

void CGuildConfig::ClearFile()
{
	m_oTable.Clear();
}

bool CGuildConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_oTable.LoadFile("table/GuildConfig.txt"))
	{
		LogWarn("Load GuildConfig.txt failed!");
		return false;
	}
	return true;
}

bool CGuildConfig::CheckFile()
{
	GuildConfigTable tempTable;
	if (!tempTable.LoadFile("table/GuildConfig.txt"))
	{
		LogWarn("Load GuildConfig.txt failed!");
		return false;
	}
	return true;
}

UINT32 CGuildConfig::GetConfig(UINT32 level, UINT32 type)
{
	GuildConf* config = GetGuildConf(level);
	if (NULL == config)
	{
		return 0;
	}
	switch(type)
	{
	case GuildExp:
		{
			return config->GuildExpNeed;
		}
		break;
	case GuildNumber:
		{
			return config->GuildNumber;
		}
		break;
	case PokerTimes:
		{
			return config->PokerTimes;
		}
		break;
	case PokerReplaceTimes:
		{
			return config->PokerReplaceTimes;
		}
		break;
	case GuildSign:
		{
			return config->GuildSign;
		}
		break;
	case GuildWelfare:
		{
			return config->GuildWelfare;
		}
		break;
	case GuildStore:
		{
			return config->GuildStore;
		}
		break;
	case GuildSkill:
		{
			return config->GuildSkill;
		}
		break;
	case GuildStage:
		{
			return config->GuildStage;
		}
		break;
	case GuildPokerTimes:
		{
			return config->GuildPokerTimes;
		}
		break;
	case AllGuildSign:
		{
			return config->AllGuildSign;
		}
		break;
	case AllGuildStage:
		{
			return config->AllGuildStage;
		}
		break;
	case GuildDragon:
		{
			return config->GuildDragon;
		}
		break;
	case GuildWar:
		{
			return config->GuildWar;
		}
		break;
	case GuildStudySkillTime:
		{
			return config->StudySkillTimes;
		}
		break;

	default:
		{
			return 0;
		}	
	}
	return 0;
}


GuildConf * CGuildConfig::GetGuildConf(int GuildLevel)
{
	if (GuildLevel <= 0 || GuildLevel > (int)m_oTable.Table.size())
	{
		return NULL;
	}

	return m_oTable.Table[GuildLevel-1];
}

UINT32 CGuildConfig::GetCanDareRewardTimes(UINT32 guildLvl, UINT32 rewardType)
{
	UINT32 times = 0;
	auto iterMap = m_guildChallegeMap.find(guildLvl);
	if (iterMap != m_guildChallegeMap.end())
	{
		foreach (i in iterMap->second)
		{
			if (*i <= rewardType)
			{
				++times;
			}
		}
	}

	return times;
}


GuildChallegeConf* CGuildConfig::GetChallegeConf(int GuildLvl, int RewardPoint)
{
	if (GuildLvl <= 0)
	{
		return NULL;
	}

	for (auto iter = m_oGuildChallegeTable.Table.begin(); iter != m_oGuildChallegeTable.Table.end(); ++iter)
	{
		if ((*iter)->GuildRank == GuildLvl && (*iter)->Mantime == RewardPoint)
		{
			return (*iter);
		}
	}

	return NULL;
}

void CGuildConfig::NoticeGuildSystemOpen(UINT64 guildid, UINT32 level)
{
	auto i = m_level2systemid.find(level);
	if (i != m_level2systemid.end())
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			GuildSystemTable::RowData* data = m_desctable.GetBysystem(*j);
			if (NULL != data && !data->desc.empty())
			{
				Notice notice(GuildSystemNotice);
				//notice.AddSrc(summary);
				notice.Add(level);
				notice.Replace("$C", data->desc);
				notice.SendGuild(guildid);
			}
		}
	}
}

const GuildBonusTable::RowData* CGuildConfig::GetBonusTemplateInfo(UINT32 bonusType, UINT32 bonusVar)
{
	for (auto iter = m_oGuildBonusTable.Table.begin(); iter != m_oGuildBonusTable.Table.end(); ++iter)
	{
		if (bonusType == (*iter)->GuildBonusType && (*iter)->GuildBonusVar == bonusVar)
		{
			return (*iter);
		}
	}
	return NULL;
}

const GuildBonusTable::RowData* CGuildConfig::GetBonusTemplateInfo(UINT32 bonusTemplateID)
{
	return m_oGuildBonusTable.GetByGuildBonusID(bonusTemplateID);
}

const GuildBonusTable* CGuildConfig::GetGuildBonusTable()
{
	return &m_oGuildBonusTable;
}

int CGuildConfig::getGuildGoblinLevelByCount(int num)
{
	int maxLevel = 0;
	foreach(i in m_oGuildGoblinTable.Table)
	{
		if( num < (*i)->UpgradeNum)
		{
			return (*i)->Level;
		}

		maxLevel = (*i)->Level;
	}

	return maxLevel;
}

const GuildGoblinTable::RowData* CGuildConfig::getGoblinInfo(int level)
{
	foreach(i in m_oGuildGoblinTable.Table)
	{
		if ((*i)->Level == level)
		{
			return *i;
		}
	}
	return NULL;
}

bool CGuildConfig::IsOpen(UINT32 level, UINT32 type)
{
	GuildConf* config = GetGuildConf(level);
	if (NULL == config)
	{
		return false;
	}
	switch(type)
	{
	case PokerTimes:
		{
			return config->PokerTimes > 0;
		}
	case GuildSign:
		{
			return config->GuildSign > 0;
		}
	case GuildWelfare:
		{
			return config->GuildWelfare > 0;
		}
	case GuildStore:
		{
			return config->GuildStore > 0;
		}
	case GuildSkill:
		{
			return config->GuildSkill > 0;
		}
	case GuildDragon:
		{
			return config->GuildDragon > 0;
		}
	case GuildWar:
		{
			return config->GuildWar > 0;
		}
	default:
		{
			return false;
		}	
	}
	return false;
}

void CGuildConfig::GetGuildRankReward(UINT32 rank, UINT32 rewardType, std::vector<ItemDesc>& rewardVec, std::map<UINT32, UINT32>& guildReward)
{
	rewardVec.clear();

	foreach (i in m_oRankRewardTable.Table)
	{
		if (rank >= (*i)->Rank.seq[0] && rank <= (*i)->Rank.seq[1])
		{
			if (rewardType == 1)
			{
				foreach (j in (*i)->LeaderReward)
				{
					ItemDesc item(j->seq[0], j->seq[1]);
					rewardVec.push_back(item);
				}
			}
			else if (rewardType == 2)
			{
				foreach (j in (*i)->OfficerRreward)
				{
					ItemDesc item(j->seq[0], j->seq[1]);
					rewardVec.push_back(item);
				}
			}
			else if (rewardType == 3)
			{
				foreach (j in (*i)->MemberReward)
				{
					ItemDesc item(j->seq[0], j->seq[1]);
					rewardVec.push_back(item);
				}
			}
			foreach (k in (*i)->guildreward)
			{
				guildReward[k->seq[0]] = k->seq[1];
			}
		}
	}
}

const GuildSalaryTable::RowData* CGuildConfig::GetGuildSalaryInfo(UINT32 guildLvl)
{
	if (m_oGuildSalaryTable.Table.empty())
	{
		return NULL;
	}

	foreach (i in m_oGuildSalaryTable.Table)
	{
		if ((*i)->GuildLevel == guildLvl)
		{
			return *i;
		}
	}

	return *(m_oGuildSalaryTable.Table.rbegin());
}

const GuildCamp::RowData* CGuildConfig::GetGuildCampTableData(UINT32 guildCampID)
{
	for (auto iter = m_oGuildCampTable.Table.begin(); iter != m_oGuildCampTable.Table.end(); ++iter)
	{
		if (guildCampID == (*iter)->ID)
		{
			return *iter;
		}
	}

	return NULL;
}

bool CGuildConfig::GetGuildCampRankReward(UINT32 rank ,std::map<UINT32, UINT32>& rewards)
{
	for (auto iter = m_oGuildCampRankTable.Table.begin(); iter != m_oGuildCampRankTable.Table.end(); ++iter)
	{
		if (rank <= (*iter)->Rank)
		{
			for (auto iterReward = (*iter)->Reward.begin(); iterReward != (*iter)->Reward.end(); ++iterReward)
			{
				rewards[iterReward->seq[0]] += iterReward->seq[1];
			}

			return true;
		}
	}

	return false;
}
