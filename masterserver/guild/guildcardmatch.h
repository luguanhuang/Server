#ifndef __GUILDCARDMATCH_H__
#define __GUILDCARDMATCH_H__

#include "pb/enum.pb.h"
#include "guild/guildcardmgr.h"
#include "table/globalconfig.h"
#include "guild/guildcard.h"
#define GUILDCARD_MATCH_SYSID 822

struct GuildCardBestInfo
{
	UINT32 result;
	UINT64 roleid;
	std::vector<UINT32> cards;

	void Reset()
	{
		result = CardDummy;
		roleid = 0;
		cards.clear();
	}
	GuildCardBestInfo()
	{
		Reset();
	}
};

struct GuildCardMatchRoleInfo
{
	bool isnew;
	bool isend;
	UINT32 result;
	UINT32 changecount;
	UINT32 store;
	std::vector<UINT32> cards;
	std::vector<UINT32> cardstore;

	GuildCardMatchRoleInfo()
	{
		isnew = true;
		isend = true;
		changecount = 0;
		store = 0;
		result = CardDummy;
	}

	void Reset()
	{
		isend = true;
		result = CardDummy;
		cards.clear();
		cardstore.clear();
	}
};

class Guild;
class GuildCardMatch : ITimer
{
public:
	GuildCardMatch(Guild* guild);
	GuildCardMatch(const std::unordered_set<UINT64>& roles);
	~GuildCardMatch();

public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool IsBegin() { return KKSG::CardMatch_StateDummy != m_state; }
	KKSG::CardMatchState GetState() { return m_state; }
	UINT32 GetRound() { return m_round; }

	void Run();
	void Cancel();

	void DelRoleFromMatch();

	bool AddRole(UINT64 roleid, UINT32& timeleft, UINT32& round);
	void DelRole(UINT64 roleid);

	std::vector<UINT32>* ChangeRoundRole(UINT64 roleid, UINT32 card, bool& canchange);
	UINT32 EndRoundRole(UINT64 roleid);

	void WaitRound();
	void BeginRound();
	void EndRound();

	void BeginMatch();
	void EndMatch();

	UINT32 GetRolesNum();
	const std::set<CardPoint>* GetMatchPoints() { return &match_points_; }
	const std::unordered_map<UINT64,std::unordered_map<UINT32,UINT32>>& GetTotalAwards(){ return role_totalawards_;}
	void UpdateRank();

	std::vector<UINT64>& GetRoles() {return roles_;}
	void SaveAddRoleId(UINT64 role_id) {add_roles_.insert(role_id);}
	std::unordered_set<UINT64>& GetAddRoleId() {return add_roles_;}
private:
	bool UpdateBestCards(UINT64 roleid, UINT32 result, std::vector<UINT32>& cards);
	void NotifyBestCards();
	void NotifyClient(UINT64 roleid, UINT32 type, GuildCardMatchRoleInfo* info);

	void EmptyWait();
	UINT32 EndRoundRole(UINT64 roleid, GuildCardMatchRoleInfo* info);
	GuildCardMatchRoleInfo* GetInfo(UINT64 roleid);	

	void UpdateMatchPoints(UINT64 roleid, UINT32 point);
private:
	Guild* m_guild;
	HTIMER m_timer;
	KKSG::CardMatchState m_state;

	time_t m_time;
	time_t m_timeround;

	UINT32 m_round;
	GuildCardBestInfo m_bestinfo;

	std::deque<UINT64> m_waitingroles;
	std::map<UINT64, UINT32> m_roleid2changecount;
	std::map<UINT64, GuildCardMatchRoleInfo> m_roleid2cards;

	std::vector<UINT64> roles_;
	std::unordered_map<UINT64,std::unordered_map<UINT32,UINT32>> role_totalawards_;
	///> 比赛积分
	std::set<CardPoint> match_points_;

	std::unordered_set<UINT64> add_roles_;

	UINT32 GUILDCARD_MATCH_WAITING;
	UINT32 GUILDCARD_MATCH_ROUNDWAITING;
	UINT32 GUILDCARD_MATCH_ROUNDING;
	UINT32 GUILDCARD_MATCH_ROUND;
	UINT32 GUILDCARD_MATCH_CHANGE;
	UINT32 GUILDCARD_MATCH_CHANGEADD;
};

#endif