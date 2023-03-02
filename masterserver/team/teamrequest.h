#ifndef __TEAMREQUEST_H__
#define __TEAMREQUEST_H__

#include "table/ExpeditionTable.h"
#include "teamhandler.h"
#include "define/teamdef.h"

class Team;
class CRole;

typedef void (*TeamListCB)(Team *pTeam, void *arg);

namespace KKSG
{
	class TeamOPArg;
	class TeamOPRes;
}

class TeamRequest
{
public:
	TeamRequest(CRole *pRole);
	~TeamRequest();

	int  CreateExpenditionTeam(UINT32 expID, TeamOpType optype = TEAM_OPT_NORMAL, const std::string &password = "");
	int  JoinTeam(int teamID, TeamOpType optype = TEAM_OPT_NORMAL, const std::string &password = "", bool checkpassword = false);
	int  LeaveTeam(int reason);
	int  KickTeam(UINT64 roleID);
	int	 InvitePre(UINT64 roleID, const std::string& account);
	int  InviteAft(UINT64 roleID);

	int  StartMatch(int expid, bool downMatch = false);
	int  StopMatch();

	int TeamMatch(const KKSG::KMatchCommonArg& roArg);

	int	SwitchMemType(KKSG::TeamMemberType type); 
	//int  SwitchHelper(bool helper);
	//int  SwitchUseTicket(int useTicket);

	int  StartBattleBeginVote(const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes &roRes);
	int  StartBattleVote(UINT64 roleID, bool accept);
	int  BattleContinue(const KKSG::TeamHelper& helpers);
	int  ChangeExpIDPre(int expID);
	int  ChangeExpID(int expID, KKSG::TeamOPRes& roRes, const KKSG::TeamHelper& nothelpers);
	void QueryFullData();
	void GetExpenditionTeam(const std::vector<UINT32> &Category, TeamListCB cb, void *arg);
	void GetExpenditionTeam(UINT32 expID, TeamListCB cb, void *arg);

	int TransLeader(UINT64 roleID);
	int ChangePassword(const std::string& password);

	int GetWeekNestCurExp();

	///> 赏金团
	int SetTeamCostPre(UINT32 index, UINT32& diamondCostCount);
	int SetTeamCost(UINT32 index);
	///> 战力限制
	int SetTeamPPTLimit(UINT32 ppt);

	//Team *GetTeam() { return m_pTeam; }

	UINT32 GetExpOpenTime(int expid);

	void TransSendToGs(UINT32 delayid, const KKSG::TeamOPArg& roArg);
    KKSG::ErrorCode DiamondCostCond();

private:

	void FilterTeam(const std::list<int> &Teams, TeamListCB cb, void *arg);

	bool IsLeader(Team* pTeam);
	bool TeamInState(Team* pTeam, int state);

	//bool HasTeam();
	Team* GetTeamInMs();
	//int CheckGuildCondition(CRole* pRole,ExpeditionTable::RowData *pRow, UINT64& gid);

	CRole *m_pRole;
	//Team *m_pTeam;
	TeamHandler m_handler;
};

#endif // __TEAMREQUEST_H__
