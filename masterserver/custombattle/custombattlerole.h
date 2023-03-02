#ifndef __CUSTOMBATTLEROLE_H__
#define __CUSTOMBATTLEROLE_H__

#include "define/itemdef.h"
#include "custombattle/custombattle.h"

class CRole;
struct SCustomBattleRole
{
	SCustomBattleRole(UINT64 roleid);
	~SCustomBattleRole();

	void ToKKSG(KKSG::CustombattleRoleSystem& system, KKSG::CustomBattleRole& custom);
	void ToThis(const KKSG::CustombattleRoleSystem& system, const KKSG::CustomBattleRole& custom);

	bool Join(UINT64 uid, bool issystem, UINT32 configid);
	bool UnJoin(UINT64 uid, bool issystem);
	bool IsJoin(UINT64 uid);
	bool CanJoin() { return NULL == m_battlejoin; }
	UINT64 GetJoinId();
	UINT32 GetJoinConfigId();
	bool IsJoinPvpTwo();

	// system
	KKSG::CustomBattleRole* GetSystem(UINT64 uid);
	void ClearSystem(UINT64 uid);

	bool CanSystemMatch(UINT64 uid);
	void UpdateWinOrLose(UINT64 uid, bool iswin);
	void EndSystem(KKSG::CustomBattleRole& battle); 
	bool DropSystem(UINT64 uid); 
	void ClearSystemRewardCD(UINT64 uid);

	void FillSystemRole(UINT64 uid, KKSG::CustomBattleRole* datarole);
	bool FillSystemMatch(UINT64 uid, KKSG::CustomBattleMatchInfo& info);

	KKSG::ErrorCode CanSystemReward(UINT64 uid);
	bool IsSystemCD(UINT64 uid);
	UINT32 GetWin(UINT64 uid);

	// custom
	void UpdatePoint(UINT64 uid, int point);
	void ClearBattle();

	///> 上线检测，玩家参加的自定义比赛是否已经被删除
	void OnLogin(CRole* role);

	void SetChanged() { m_ischanged = true; }
	bool IsChanged() { return m_ischanged; }
	bool ResetChanged() { return m_ischanged = false; }

	bool m_ischanged;
	UINT64 m_roleid;
	///> 创建或者参与
	KKSG::CustomBattleRole* m_battlejoin;	
	std::map<UINT64, KKSG::CustomBattleRole> m_uid2battlesystem;	
};

#endif
