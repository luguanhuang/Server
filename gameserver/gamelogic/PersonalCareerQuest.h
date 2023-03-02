#ifndef _PERSONAL_CAREER_QUEST_H_
#define _PERSONAL_CAREER_QUEST_H_
#include "unit/role.h"
#include "guild/guildmgr.h"
#include "table/TrophyInfo.h"
#include "table/TrophyReward.h"
#include "gamelogic/stagemgr.h"
namespace KKSG
{
	class PersonalHomePage;
}

class PersonalCareerQuest
{
	PersonalCareerQuest();
	~PersonalCareerQuest();
	DECLARE_SINGLETON(PersonalCareerQuest);

	bool Init();
	bool Reload();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();
public:
	void GetHomePage(
		KKSG::PersonalCareerTotalInfoRes& total_info ,
		Role& role,
		KKSG::PersonalHomePage& home_page);

	void GetPVPInformation(
		KKSG::PersonalCareerTotalInfoRes& total_info ,
		Role& role,
		KKSG::PVPInformation& pvp_info);

	void GetTrophyRank(
		const UINT32 scene_id,
		StageTrophyInfo& trophy,		
		std::vector<ItemDesc>& items);
private:
	bool CheckTrophyType(
		const StageTrophyDataDetail& data,
		const UINT32 trophy_type,
		const UINT32 trophy_type_value);

private:
	UINT32 GetInitRoleTime(UINT32 create_time);
	TrophyInfo trophy_info_;	
	TrophyReward  trophy_reward_;
};

#endif