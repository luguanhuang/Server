#ifndef __TEAMMATCHMGR_H__
#define __TEAMMATCHMGR_H__

#include "timer.h"
#include "table/ExpeditionTable.h"

class CRole;
class Team;

struct SingleMatchRole
{
	int   expID;
	CRole *pRole;
};

class TeamEventHandler
{
public:
	virtual ~TeamEventHandler(){}
	virtual void OnTeamStartMatch(Team *pTeam) = 0;
	virtual void OnTeamStopMatch(Team *pTeam) = 0;
	virtual void OnRoleStartMatch(CRole *pRole, ExpeditionTable::RowData *pConf) = 0;
	virtual void OnRoleStopMatch(CRole *pRole) = 0;
};

struct DelayPairRole
{
	UINT64 roleid;
	int expid;
	UINT32 time;
};
struct DelayPairTeam
{
	int teamid;
	UINT32 time;
};
//deley最好去掉 否则需要处理在Delay状态的玩家 *****
class DelayMatchHandler
{
public:
	DelayMatchHandler();
	~DelayMatchHandler();

	void PushRole(UINT64 roleid, int expid);
	void PushTeam(int teamid);
	//void PushFastRole(UINT64 roleid, int expid);
	
	bool PopRole(UINT64 roleid);
	bool PopTeam(int teamid);
	//bool PopFastRole(UINT64 roleid);

	std::list<DelayPairRole> PopRoles();
	std::list<DelayPairTeam> PopTeams();
	//std::list<DelayPairRole> PopFastRoles();

	bool HaveRole(UINT64 roleid);

private:
	std::map<UINT64, DelayPairRole> m_roles;
	std::map<int, DelayPairTeam> m_teams;
	//std::map<UINT64, DelayPairRole> m_fastroles;
};

class FastMatchHandler;
class TeamMatchMgr : public ITimer
{
	TeamMatchMgr();
	~TeamMatchMgr();
	DECLARE_SINGLETON(TeamMatchMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	int StartMatch(CRole *pRole, int expID);
	int StartMatch(Team *pTeam);
	void StopMatch(Team *pTeam);
	void StopMatch(CRole *pRole, int reason = 1/*KKSG::STOPMATCH_NONE*/);

	void DoMatch(CRole* role, int expid);
	void DoMatch(Team* team);
	void DoFastMatch(CRole* role, int expid);
	void DoFastMatch(Team* team);

	void AddHandler(TeamEventHandler *handler);

	FastMatchHandler* GetFastMatchHandler() { return m_fastMatchHandler; }

private:
	CRole *FindMatchRoleByExpID(int expID, UINT64 gID, UINT32 pptLimit = 0);
	Team *FindMatchTeamByExpID(int expID, UINT64 gID, UINT32 roleppt);

	HTIMER m_timer;
	DelayMatchHandler m_delayhandler;
	std::list<TeamEventHandler *> m_Listener;
	FastMatchHandler* m_fastMatchHandler;
	std::list<SingleMatchRole> m_MatchRoles;
	std::list<SingleMatchRole> m_MatchFastRoles;
	std::unordered_map<int, std::list<Team*>> m_MatchTeams;
	std::unordered_map<int, std::list<Team*>> m_MatchFastTeams;
};


struct FastMatchInfo
{
	FastMatchInfo()
	{
		pConf = NULL;
		pRole = NULL;
		time = (time_t)0;
		downMatch = false;
	}
	ExpeditionTable::RowData *pConf;
	CRole *pRole;
	time_t time;	

	bool downMatch;

	void TestPrint();
};


class FastMatchTempRoom
{
public:
	FastMatchTempRoom();

	void AddRole(FastMatchInfo& mr);
	bool HaveRole(CRole* pRole);

	void StartBattleConFirm();
	void StartBattleRefuse(CRole* quitRole, std::list<FastMatchInfo>& leftRoles);

	bool WaitEnough();
	bool RoleCountEnough();
	bool RolecountFull();
	void Dismiss(CRole* quitRole, std::list<FastMatchInfo>& leftRoles);
	void StartBattle(bool addrobot = false);
	void SetAddRobot(){ m_addrobot = true; }

	void FetchRoles(std::vector<CRole*>& roles);

	void Clear();

	void TestPrint();

private:

	bool m_addrobot;
	time_t m_time;
	std::vector<FastMatchInfo> m_roles;
};

class FastMatchHandler: public TeamEventHandler, public ITimer
{
public:

	FastMatchHandler();
	~FastMatchHandler();

	virtual void OnTeamStartMatch(Team *pTeam);
	virtual void OnTeamStopMatch(Team *pTeam);
	virtual void OnRoleStartMatch(CRole *pRole, ExpeditionTable::RowData *pConf);
	virtual void OnRoleStopMatch(CRole *pRole);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void TestPrint();
	void CheckMatchRoom();
	void CheckStartBattle();
	void CheckMatchRobotRoom();
	void CheckMatchRobotTeam();
	void CheckDownMatch();

	void SetRoleDownMatch(UINT64 roleID);

private:

	bool CanDownMatch(FastMatchInfo& info1, FastMatchInfo& info2, time_t now);

	HTIMER m_handler;
	bool m_checkStartBattleLocked;
	std::list<FastMatchInfo> m_matchRoles;//list效率有点低 需优化
	std::list<FastMatchTempRoom> m_fastMatchRooms;//临时的
	std::map<Team *, time_t> m_fastMatchTeams;

	UINT32 m_downMatchTime;
};

#endif // __TEAMMATCHMGR_H__