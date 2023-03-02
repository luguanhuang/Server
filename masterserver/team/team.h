#ifndef __TEAM_H__
#define __TEAM_H__

#include "table/ExpeditionTable.h"
#include "define/teamdef.h"
#include "team/teambase.h"

namespace KKSG 
{ 
	class TeamBrief; 
	class TeamOPRes;
	class TeamSynAll;
	class TeamSynExtraData;
}

class CRole;
class CProtocol;
class CTeamMember;
typedef void (*TeamCB)(CTeamMember *pMember, void *arg);
typedef void (*TeamOnLineCB)(CTeamMember *pMember, CRole *pRole, void *arg);


class Team : public TeamBase
{
public:
	Team();
	virtual ~Team();
	
	///>
	inline int    GetID() { return m_id; }
	inline void   SetID(int id) { m_id = id; }
	inline int    GetMemberCount() { return m_members.size(); }
	inline const std::string &GetLeaderName() { return m_leaderName; }
	inline int    GetTeamSize() { return m_pConf->PlayerNumber; }//最大人数
	inline bool   IsFull() { return GetMemberCount() >= GetTeamSize(); }
	inline void   SetPassword(const std::string &password) { m_password = password; }
	inline const std::string &GetPassword() { return m_password; }
	inline UINT32 GetExpeditionID() { return (m_pConf != NULL) ? m_pConf->DNExpeditionID : 0; }
	inline UINT32 GetExpeditionType() { return (m_pConf != NULL) ? m_pConf->Type : 0; }
	inline void SetTeamState(int st) { m_teamState = st; }
	inline UINT32 GetTeamState() { return m_teamState; }
	inline ExpeditionTable::RowData *GetConf() { return m_pConf; }
	inline void   SetConf(ExpeditionTable::RowData *pRow) { m_pConf = pRow; }
	inline void   SetGuildID(UINT64 gid) { m_qwGuildID = gid; }
	inline UINT64 GetGuildID() { return m_qwGuildID; }
	
	///>
	UINT64 GetRobotID();
	void   AddRobotAndNotify();
	void   RemoveRobotAndNotify(UINT64 qwRoleID);
	void   RemoveAllRobotAndNotify();
	void   Add(UINT64 qwRoleID);
	void   Remove(UINT64 qwRoleID);
	void   SetLeader(UINT64 qwRoleID, const std::string &leaderName);
	UINT64 GetLeader();
	int    GetAVGPPT();
	int    GetLeaderLevel();

	CTeamMember *FindMember(UINT64 qwRoleID);
	void   UpdateMemberInfo(CRole *pRole);
	void   UpdateMemberExtraInfo(CRole* pRole, const KKSG::TeamSynExtraData& data);
	void   BroadCast(CProtocol &proto, UINT64 qwExludeRole = 0);
	void   Foreach(TeamCB cb, void *arg);
	void   Foreach(TeamOnLineCB cb, void *arg);
	const std::list<CTeamMember *>& GetMember();
	void GetRoleIDs(std::vector<UINT64>& roleIDs);

	void Update();
	void OnAllSceneEnd();

	///>
	void SaveBrief(KKSG::TeamBrief *pTeamBrief, bool needpw = true);
	const char *GetTeamStateStr();

	int  GetTeamStateErrorCode();
	void StartBattle();	
	int  GetMemberCountWithoutRobot();
	bool HasRobotInTeam();

	void SetBeginVote(); 
	void SetMemberVoteState(CRole *pRole, int state);
	void VoteUpdate(time_t Now);
	void CheckVoteResult();

	void SetMatching(bool match);
	void ChangeLeaderTo(UINT64 roleID);
	void ResetMemberState(bool notify = true);

	void NotifyTeamStateChanged();
	void NotifyMemberStateChanged(CTeamMember *pMember, CRole *pHitRole);
	void NotifyAllMemberStateChanged();

	void OnRoleLeaveBattle(CRole* pRole);
	void CheckFinishBattle();
	bool AllMemLeaveBattle();

	void SetInBattleState();
	void OnStartBattleFailed();

	bool NeedStartMatch();
	bool NeedStopMatch();
	bool NeedMatchRole();
	bool NeedMatchRobot();
	bool NeedMatchCommon();

	void ResetHelperState(UINT64 roleid = 0);
	void SetUseTicket(int useTicket, UINT64 roleID = 0);
	//void UseTicketNtf(UINT64 roleID);

	void ChangeTeamExpIDCheck();

	void GetSessions(std::vector<SessionType>& sessions);

	void SaveSynData(KKSG::TeamSynAll& data);

	void OnChangeLeader(UINT64 roleID);
	void OnChangeExpId();

	///> 赏金团
	void SetCostIndex(UINT32 index) { m_costindex = index; NotifyTeamStateChanged(); }
	UINT32 GetCostIndex() { return m_costindex; }
	void ResetCostNtf2G();
	///> 战力限制
	void SetPPTLimit(UINT32 ppt) { m_pptlimit = ppt; NotifyTeamStateChanged(); }
	UINT32 GetPPTLimit() { return m_pptlimit; }
	void SetLeagueTeamName(const std::string& name) { m_leagueTeamName = name; }
	const std::string& GetLeagueTeamName() { return m_leagueTeamName; }

	void ResetTeamSetting();

	void ResetLeaderLastBattleTime();
	time_t GetLeaderLastBattleTime();
	bool NeedKickLeader(time_t now);
	bool NeedKickLeaderBeforeNotice(time_t now, UINT32 beforeTime);
	UINT32 GetLeaderLeftCount();

	int GetMatchType() { return m_matchType; }
	UINT64 GetMatchUniIndex() { return m_matchUniIndex; }
	void SetMatchType(int type);

	virtual bool IsKingBack();

	void InfoPrint();
	
protected:
	
	std::list<CTeamMember *>::iterator FindMemberIterator(UINT64 qwRoleID);

	int    m_id;
	int    m_teamState;
	UINT64 m_qwGuildID;
	UINT64 m_leaderID;
	time_t m_voteTime;
	std::string m_leaderName;
	std::string m_password;
	///> 战力限制
	UINT32 m_pptlimit;
	///> 赏金团
	UINT32 m_costindex;
	std::string m_leagueTeamName;	// 联赛战队起的名字

	time_t m_lastLeaderBattleTime;

	ExpeditionTable::RowData *m_pConf;
	
	std::list<CTeamMember *> m_members;

	int m_matchType;
	UINT64 m_matchUniIndex;
};

#endif