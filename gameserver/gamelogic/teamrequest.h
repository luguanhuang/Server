#ifndef __TEAMREQUEST_H__
#define __TEAMREQUEST_H__

#include "table/ExpeditionTable.h"
#include "gamelogic/team.h"

class TeamCopy;
class Role;

namespace KKSG
{
	class TeamOPRes;
	class TeamOPM2GRes;
	class TeamHelper;
	class BuyTeamSceneCountRet;
}

//只是判断条件
class TeamRequest
{
public:
	TeamRequest(Role *pRole, TeamCopy* pTeam = NULL);
	~TeamRequest();

	int  CreateExpenditionTeam(UINT32 expID);
	int  JoinTeam();
	int  Invite(UINT64 roleID);
	//匹配
	int  StartMatch(int expid);
	//观战
	int	 SwitchHelper(int expID);
	int SwitchUseTicket();

	int  StartBattleBeginVote(KKSG::TeamOPRes &roRes);
	int  BattleContinue(KKSG::TeamOPM2GRes &m2gRes);
	int  ChangeExpID(int expID, KKSG::TeamOPRes& roRes, KKSG::TeamHelper& nothelpers);

    int GetTeamTypeCount(KKSG::TeamOPRes& roRes);

	void BuyTeamCount(int type, KKSG::BuyTeamSceneCountRet& roRes);

	int GetDayCountLeft(Role *pRole, int Type);

	//开启某个关卡
	int CheckOpenCondtion(Role *pRole, int expid);
	//加入队伍
	int CheckJoinCondtion(Role *pRole);
	//进入战斗
	int CheckEnterCondtion(Role *pRole, int expid, TeamEnterData* pEnterData = &TeamEnterData::GlobalTeamEnterData);

	int CheckStartBattleCondition(UINT64& problemRoleID);

	///> 赏金团
	int HandleTeamCost(UINT32 index);

private:
	int CheckChangeExpCondition(int expID, UINT64& problemRoleID);

private:
	TeamCopy m_teaminfo;
	Role *m_pRole;
};

#endif // __TEAMREQUEST_H__
