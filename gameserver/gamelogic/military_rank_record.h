#ifndef __MILITARY_RANK_RECORD_H__
#define __MILITARY_RANK_RECORD_H__
#include "unit/roleserilizeinterface.h"

class Role;
class MilitaryRankRecord : public RoleSerializeInterface
{
public:
	MilitaryRankRecord(Role* role);
	~MilitaryRankRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update();
public:
	UINT32 GetMilitaryRank() {return military_rank_;}
	UINT32 GetMilitaryMaxRankHis(){return military_max_rank_his_;}
	UINT32 GetMilitaryExploit() { return military_exploit_;}
	UINT32 GetMilitaryMaxExploitHis() { return military_max_exploit_his_;}

	// 获取当前战斗能增加的军功
	UINT32 GetMilitaryExploitByType(const KKSG::SceneType scene_type);
	// 获取当前战斗减少的军功
	UINT32 GetReduceMilitaryExploitByType(const KKSG::SceneType scene_type);
	// 增加当前一场胜利的军功
	UINT32 AddBattleRecord(const KKSG::SceneType scene_type);
	// 扣当前一场失败的军功
	UINT32 ReduceBattleRecord(const KKSG::SceneType scene_type);
	void UpdateMSRankList();
	void SetMilitaryRrank(UINT32 military_rank) 
	{ 
		military_rank_ = military_rank;
		NotifyClientMilitaryChange();
		m_isModify.Set();
		LogDebug("military_rank_ :%d\n",military_rank_);
	}

	void SetMilitaryExploit(UINT32 military_exploit);

	void SendMilitaryRankUpReward(UINT32 military_rank);

	void NotifyClientMilitaryChange();

	void GiveItemTransacionForMilitary(UINT32 count);
	void ConsumeItemForMilitary(UINT32 military_exploit);
	// debug 接口
	void DebugClear();

	void ReduceMilitaryExploit(UINT32 military_exploit);
private:
	Role* role_;
	Switch m_isModify;
	UINT32 m_lastUpdateTime;

	UINT32 military_rank_;
	UINT32 military_max_rank_his_;
	UINT32 military_exploit_;
	UINT32 military_max_exploit_his_;

	std::unordered_set<UINT32> achieve_rank_;
};


#endif
