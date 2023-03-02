#include "pch.h"
#include "guildconfig.h"
#include "foreach.h"
#include "gamelogic/noticemgr.h"
#include "table/GuildPermissionTable.h"
#include "define/itemdef.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/itemconfig.h"
#include "config/itemdropconfig.h"

INSTANCE_SINGLETON(GuildConfig)

GuildConfig::GuildConfig()
{
	memset(m_PERMMATRIX, 0, sizeof(m_PERMMATRIX));
}

GuildConfig::~GuildConfig()
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

bool GuildConfig::Init()
{

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
		LogWarn("Load GuildBonus.txt failed!");
		return false;
	}
	if (!m_oGuildChallegeTable.LoadFile("table/GuildChallege.txt"))
	{
		LogWarn("Load GuildChallege.txt failed!");
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

	for (auto iter = m_oGuildBonusTable.Table.begin(); iter != m_oGuildBonusTable.Table.end(); ++iter)
	{
		m_guildBonusMap[(*iter)->GuildBonusType].insert(std::make_pair((*iter)->GuildBonusVar, (*iter)->GuildBonusID));
	}

	return true;
}

void GuildConfig::Uninit()
{

}

void GuildConfig::ClearFile()
{
	m_oTable.Clear();
}

bool GuildConfig::LoadFile()
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

bool GuildConfig::CheckFile()
{
	GuildConfigTable tempTable;
	if (!tempTable.LoadFile("table/GuildConfig.txt"))
	{
		LogWarn("Load GuildConfig.txt failed!");
		return false;
	}
	return true;
}

bool GuildConfig::HasPermission(GuildPosition pos, GuildPermission pem)
{
	return m_PERMMATRIX[pem][pos];
}

GuildConf * GuildConfig::GetGuildConf(int GuildLevel)
{
	if (GuildLevel <= 0 || GuildLevel > (int)m_oTable.Table.size())
	{
		return NULL;
	}

	return m_oTable.Table[GuildLevel-1];
}

GuildChallegeConf* GuildConfig::GetChallegeConf(int GuildLvl, int RewardPoint)
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

UINT32 GuildConfig::GetConfig(UINT32 level, UINT32 type)
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

UINT32 GuildConfig::GetMaxLevel()
{
	return m_oTable.Table.size();
}

UINT32 GuildConfig::CalculateLevel(UINT32 exp)
{
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		GuildConfigTable::RowData* rowdata = *i;
		if (exp < rowdata->GuildExpNeed)
		{
			return rowdata->GuildLevel;
		}
		else
		{
			exp -= rowdata->GuildExpNeed;
		}
	}
	return GetMaxLevel();
}

bool GuildConfig::IsOpen(UINT32 level, UINT32 type)
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

UINT32 GuildConfig::GetBonusTemplateID(UINT32 bonusType, UINT32 var)
{
	auto find = m_guildBonusMap.find(bonusType);
	if (find == m_guildBonusMap.end())
	{
		return 0;
	}

	auto findVar = find->second.find(var);
	if (findVar == find->second.end())
	{
		return 0;
	}

	return findVar->second;
}

std::map<UINT32, UINT32>* GuildConfig::GetBonusContent(UINT32 bonusType)
{
	auto find = m_guildBonusMap.find(bonusType);
	if (find == m_guildBonusMap.end())
	{
		return NULL;
	}

	return &find->second;

}

const GuildBonusTable::RowData* GuildConfig::GetBonusTemplateInfo(UINT32 bonusTemplateID)
{
	return m_oGuildBonusTable.GetByGuildBonusID(bonusTemplateID);
}

const GuildBonusTable::RowData* GuildConfig::GetBonusTemplateInfo(UINT32 bonusType, UINT32 bonusVar)
{
	UINT32 templateID = GetBonusTemplateID(bonusType, bonusVar);
	if (templateID)
	{
		return GetBonusTemplateInfo(templateID);
	}

	return NULL;
}

int GuildConfig::getGuildGoblinLevelByCount(int num)
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

const GuildGoblinTable::RowData* GuildConfig::getGoblinInfo(int level)
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

void GuildConfig::NoticeGuildSystemOpen(RoleSummary* summary, UINT64 guildid, UINT32 level)
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

bool GuildConfig::ExistDareReward(UINT32 guildLvl, UINT32 rewardType)
{
	auto iterMap = m_guildChallegeMap.find(guildLvl);
	if (iterMap != m_guildChallegeMap.end())
	{
		foreach (i in iterMap->second)
		{
			if (*i == rewardType)
			{
				return true;
			}
		}
	}

	return false;
}

UINT32 GuildConfig::GetCanDareRewardTimes(UINT32 guildLvl, UINT32 rewardType)
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

UINT32 GuildConfig::GetDareGuildExp(UINT32 guildLvl)
{
	foreach (i in m_oTable.Table)
	{
		if ((*i)->GuildLevel == guildLvl)
		{
			foreach (j in (*i)->GuildChallengeReward)
			{
				if ((*j)[0] == GUILD_CONTRIBUTE)
				{
					return ((*j)[1]);
				}
			}
		}
	}

	return 0;
}

void GuildConfig::GetStageGuildExp(UINT32 guildlevel, UINT32 scenetype, std::map<UINT32, UINT32>& guildreward)
{
	GuildConfigTable::RowData* rowdata = m_oTable.GetByGuildLevel(guildlevel);
	if (NULL == rowdata)
	{
		return ;
	}
	for (auto i = rowdata->GuildChallengeGExp.begin(); i != rowdata->GuildChallengeGExp.end(); ++i)
	{
		if (scenetype == i->seq[0])
		{
			guildreward[i->seq[1]] = i->seq[2];
		}
	}
	return ;
}

ItemDesc GuildConfig::GetStageRoleContribute(UINT32 guildlevel, UINT32 scenetype)
{
	ItemDesc item;
	GuildConfigTable::RowData* rowdata = m_oTable.GetByGuildLevel(guildlevel);
	if (NULL == rowdata)
	{
		return item;
	}
	for (auto i = rowdata->GuildChallengeSolo.begin(); i != rowdata->GuildChallengeSolo.end(); ++i)
	{
		if (scenetype == i->seq[0])
		{
			const ItemDesc* conf = ItemDropConfig::Instance()->GetRandomDrop(i->seq[1]);
			if (NULL != conf)
			{
				return *conf;
			}
		}
	}
	return item;
}

UINT32 GuildConfig::GetContributeNum(UINT32 guildlevel, UINT32 scenetype)
{
	GuildConfigTable::RowData* rowdata = m_oTable.GetByGuildLevel(guildlevel);
	if (NULL == rowdata)
	{
		return 0;
	}

	for (auto i = rowdata->GuildChallengeSolo.begin(); i != rowdata->GuildChallengeSolo.end(); ++i)
	{
		if (scenetype == i->seq[0])
		{
			std::vector<ItemDesc> vecItems;
			ItemDropConfig::Instance()->GetAllDropItem(i->seq[1], vecItems);

			UINT32 cnt = 0;
			std::map<UINT32, ItemDesc> itemMap;
			foreach (iter in vecItems)
			{
				if (iter->itemID == GUILD_CONTRIBUTE)
				{
					itemMap[cnt++] = *iter;
				}
			}

			if (itemMap.size() == 0)
			{
				return 0;
			}
			UINT32 randNum = XRandom::randInt(0, itemMap.size());

			return itemMap[randNum].itemCount;
		}
	}
	return 0;
}
	
const GuildCamp::RowData* GuildConfig::GetGuildCampData(UINT32 guildCampID)
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

