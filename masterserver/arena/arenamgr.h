#ifndef __ARENAMGR_H__
#define __ARENAMGR_H__

#include "table/ArenaTable.h"
#include "table/ArenaNpcTable.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"

struct ChallengeRole
{
	UINT64 roleid;
	UINT32 rank;

	ChallengeRole()
	{
		roleid = 0;
		rank = 0;
	}
	ChallengeRole(UINT64 _roleid, UINT32 _rank)
	{
		roleid = _roleid;
		rank = _rank;
	}
};

namespace KKSG
{
	class UnitAppearance;
	class ArenaBattleInfo;
	class ArenaEndInfo;
}
class CRole;
class CArenaRealTimeRankList;
class ArenaMgr
{
	ArenaMgr();
	~ArenaMgr();
	DECLARE_SINGLETON(ArenaMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetRank(UINT64 roleid);
	UINT64 GetRoleId(UINT32 rank);

	void PushChallengedRoleId(UINT64 qwRoleId, UINT64 qwChallengedRoleId);
	UINT64 PopChallengedRoleId(UINT64 qwRoleId);
	UINT32 GetChallengedRank(UINT64 qwRoleId);
	bool IsInChallenge(UINT64 roleid);

	// arena npc
	bool IsArenaNpc(UINT64 uid);
	UINT32 GetNpcBattleLevel(UINT32 rank);
	float GetNpcBattleCoefByPPT(UINT32 powerpoint);
	UINT32 GetNpcBattlePoint(UINT32 dwRank);
	UINT32 GetNpcIdByProfession(UINT32 dwProfession);

	UINT32 GetRecommendPower(UINT64 challengid);
	const ArenaNpcInfo* GetArenaNpcInfo(UINT64 uid);
	ArenaNpcTable* GetNpcTable() { return &m_npctable; }

	// ranklist
	bool InvalidRank(UINT32 rank);
	UINT32 GetRankFirstTime(CRole* role);
	void GetCanBeChallenged(UINT32 count, UINT32 rank, std::vector<UINT32>& ranks);
	bool GetBriefInfo(UINT64 roleid, KKSG::UnitAppearance* unitAppearance);
	void GetBattleInfo(UINT64 roleid, UINT64 challengeid, KKSG::ArenaBattleInfo* info);
	bool UpdateRankList(UINT64 qwRoleId1, UINT32 dwRank1, UINT64 qwRoleId2, UINT32 dwRank2);

	// end
	bool OnEnd(UINT64 roleid, bool iswin, KKSG::ArenaEndInfo& info);

	// reward
	void GiveRankReward(UINT64 roleid, UINT32 dwRank);

	void SetRankList(CArenaRealTimeRankList* ranklist) { m_ranklist = ranklist; }

private:
	UINT32 GetDragonCoinCountByRank(UINT32 dwRank);
	UINT32 GetGoldCoinCountByRank(UINT32 dwRank);

	UINT32 GetExceptRole(UINT32 rankbegin, UINT32 rankend0);
	float GetCoef(UINT32 rank, const ArenaTable::RowData* data);
	UINT32 GetCoefResult(float coef, UINT32 begin, UINT32 end);
	const ArenaTable::RowData* GetRowData(UINT32 rank);

private:
	MailConf m_mailconf;

	ArenaTable m_table;
	ArenaNpcTable m_npctable;
	CArenaRealTimeRankList* m_ranklist;
	std::unordered_map<UINT64, ChallengeRole> m_roleid2challenge;
};

#endif