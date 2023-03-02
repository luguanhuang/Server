#ifndef _CARD_MATCH_MGR_
#define _CARD_MATCH_MGR_

#include "guild/guildcardmatch.h"
#include <unordered_map>
#include "timer.h"
#include "guild/guildcard.h"
#include "table/PokerTournamentReward.h"

class CardMatchMgr : ITimer
{
	CardMatchMgr();
	~CardMatchMgr();
	DECLARE_SINGLETON(CardMatchMgr)

public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
public:
	GuildCardMatch* GetCardMatchByRoleID(const UINT64 role_id);
	void AddRoleTOCardMatch(const UINT64 role_id);
	void DeleteFromCardMatch(const UINT64 role_id);
	bool CheckRoleExist(const UINT64 role_id);
	UINT32 GetSignUpNum(const UINT64 role_id);
	
public:
	void DispatchPokerTournamentReward(std::set<CardPoint>& match_points);

private:
	void StartTimer();
	void Cancel();

private:
	HTIMER timer_handle_;
	std::unordered_set<UINT64> match_roles_;
	std::unordered_map<UINT64,GuildCardMatch*> role_match_;
	std::unordered_set<GuildCardMatch*> all_matchs_;
	std::unordered_set<GuildCardMatch*> remove_matchs_;
	std::unordered_map<UINT64,UINT32> role_signup_num_;
	PokerTournamentReward porker_reward_;
};

#endif // _CARD_MATCH_MGR_