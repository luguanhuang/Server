#ifndef __TEAMSTARTBATTLE_H__
#define __TEAMSTARTBATTLE_H__

#include "cvsreader.h"

namespace KKSG
{
	class SceneSwitchData;
}

class Team;
class CRole;

class TeamStartBattle
{
public:
	TeamStartBattle(Team* pTeam);
	bool StartBattle();	
	static void FillBuffs(std::vector<CRole*>& roles, KKSG::SceneSwitchTeamData& data);

private:
	void StartSkyCity();
	void ToGSStartBattle(KKSG::SceneSwitchData& oData);
	void GetRoles(std::vector<CRole*>& roles);
	static void GetBuffs(CRole* self, const std::vector<CRole*>& roles, std::vector<Sequence<int,2>>& buffs);

	Team* m_pTeam;
};

#endif
