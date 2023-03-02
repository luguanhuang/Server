#ifndef __GUILDSKILLCONFIG_H_
#define __GUILDSKILLCONFIG_H_
#include "table/GuildSkillTable.h"

class CGuildSkillConfig
{
	CGuildSkillConfig();
	~CGuildSkillConfig();
	DECLARE_SINGLETON(CGuildSkillConfig);

public:
	bool Init();
	void Uninit();
	
public:
	int needGuildLvl(UINT32 skillId, UINT32 skillLvl);
	bool needGuildExp(UINT32 skillId, UINT32 skillLvl, UINT32& needExp);
	void InitGuildSkill(std::map<UINT32, UINT32>& guildSkillMap);
	std::string GetSkillName(UINT32 skillId, UINT32 skillLvl);

private:
	GuildSkillTable m_guildSkillTable;
};

#endif
