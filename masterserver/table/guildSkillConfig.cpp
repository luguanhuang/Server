#include "pch.h"
#include "guildSkillConfig.h"
#include "foreach.h"

INSTANCE_SINGLETON(CGuildSkillConfig)

CGuildSkillConfig::CGuildSkillConfig()
{

}
CGuildSkillConfig::~CGuildSkillConfig()
{

}

bool CGuildSkillConfig::Init()
{
	if (!m_guildSkillTable.LoadFile("table/GuildSkill.txt"))
	{
		LogWarn("Load GuildSkill.txt failed!");
		return false;
	}
	return true;
}

void CGuildSkillConfig::Uninit()
{

}

int CGuildSkillConfig::needGuildLvl(UINT32 skillId, UINT32 skillLvl)
{
	foreach (i in m_guildSkillTable.Table)
	{
		if ((*i)->skillid == skillId && (*i)->level == skillLvl)
		{
			return (*i)->glevel;
		}
	}
	return -1;
}

bool CGuildSkillConfig::needGuildExp(UINT32 skillId, UINT32 skillLvl, UINT32& needExp)
{
	foreach (i in m_guildSkillTable.Table)
	{
		if ((*i)->skillid == skillId && (*i)->level == skillLvl)
		{
			needExp = (*i)->rexp;
			return true;
		}
	}
	return false;
}

void CGuildSkillConfig::InitGuildSkill(std::map<UINT32, UINT32>& guildSkillMap)
{
	foreach (i in m_guildSkillTable.Table)
	{
		auto iter = guildSkillMap.find((*i)->skillid);
		if (iter == guildSkillMap.end())
		{
			guildSkillMap[(*i)->skillid] = 0;
		}

		if ((*i)->glevel == 0 && (*i)->level > guildSkillMap[(*i)->skillid])
		{
			guildSkillMap[(*i)->skillid] = (*i)->level;
		}
	}
}

std::string CGuildSkillConfig::GetSkillName(UINT32 skillId, UINT32 skillLvl)
{
	foreach (i in m_guildSkillTable.Table)
	{
		if ((*i)->skillid == skillId && skillLvl == (*i)->level)
		{
			return (*i)->name;
		}
	}
	std::string name;
	return name;
}