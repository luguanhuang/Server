#ifndef __SCENEGPR_H__
#define __SCENEGPR_H__

#include "scenegwb.h"

namespace KKSG
{
	class GmfGuildCombat;
}

class SceneGPR : public SceneGWB
{
public:
	SceneGPR(Scene* scene);
	~SceneGPR();

	virtual void OnJoinStart();
	virtual VsStateType OnJoinStop();

	virtual VsStateType CheckFightEnd();
	bool CheckGuildFail(GMFHalfRoomType halftype);
	GMFHalfRoomType GetFailedGuildType();

	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	virtual void OnStateEnd();

	virtual void OnRoleQuit(UINT64 roleid);
	virtual void OnRoleDie(UINT64 roleid);
	virtual void OnRoleRevive(UINT64 roleid);

	virtual void SubUpdate();
	void CheckRevive(bool force);

	virtual UINT32 GetJoinMaxCount() { return 6; }
	inline UINT32 MaxFightCount() { return 3; }

	virtual int HandlerJoinRequest(Role* pRole, const KKSG::GmfReadyArg& oArg, KKSG::GmfReadyRes& oRes);

	inline UINT32 GetScore(GMFHalfRoomType halftype) { return m_score[halftype]; }
	inline UINT32 GetAllScore() { return GetScore(GMF_HRT_ONE) + GetScore(GMF_HRT_TWO); }
	inline void AddScore(GMFHalfRoomType halftype) { m_score[halftype] += 1; }

	virtual void BattleCombatToClient(Role* pRole);
	void OneBattleEndToClient(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf);
	void AllBattleEndToClient(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf);
	void FillGuildCombat(KKSG::GmfGuildCombat& guildCombat, UINT64 guildid);
	void FillRoleCombat(KKSG::GmfRoleCombat& roleCombat, UINT64 roleid);

	void TLogOneBattleEnd(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf);
	void BattleEndNotice(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf, bool allend);
	void MakeNotice(UINT32 noticeid, UINT64 mygid, UINT64 opgid, UINT32 para);

	inline UINT32 OneBattleScore() { return 10; }//√ª”–≈‰÷√

	void ChangePPT();

private:

	std::unordered_map<int,UINT32> m_score;
	VsRoleDataMgr m_datamgr;
	std::unordered_set<UINT64> m_fightHistory;
	std::unordered_map<UINT64,time_t> m_dieRecord;
	time_t m_lastCombatUpdateTime;
};

#endif
