#ifndef __MATCHMGRBASE_H__
#define __MATCHMGRBASE_H__

#include "matchbase.h"
#include "common/roleopdelay.h"

class MatchMgrBase
{
public:
	MatchMgrBase();
	virtual ~MatchMgrBase();

	void Update();

	bool AddMatchUnit(int type, const KKSG::KMatchUnit& kunit);

	void DelMatchUnitTeam(UINT32 serverID, int teamID);
	void DelMatchUnitRole(UINT32 serverID, UINT64 roleID);

	MatchUnit* Get(KMatchID matchID);
	void Del(std::vector<KMatchID>& waitDel, MatchDelReason reason = MDR_NONE);
	void Del(std::list<KMatchID>& waitDel, MatchDelReason reason = MDR_NONE);
	KMatchID GetMatchIDByKey(const KMatchKey& key);

	//fightUnit:这组战斗涉及到的匹配单位
	//data:一些玩法额外的数据
	void MatchOneFight(MatchUnitMgrBase* pUnitMgr, std::list<KMatchID>& fightUnits, KKSG::KMatchFightData& data);

	virtual void CreateSceneBegin(const KKSG::KMatchFightData& typeData, std::vector<MatchUnit*>& matched) = 0;

	std::unordered_map<KMatchID, MatchUnit*>& AllMatchUnits() { return m_matchUnits; }

	void AddUnitMgr(int type, MatchUnitMgrBase* pMgr);
	MatchUnitMgrBase* GetUnitMgr(int type);

	void InfoPrint();
	void Clear();

private:
	//唯一加/删的地方
	KMatchID Add(int type, const KKSG::KMatchUnit& kunit);
	void Del(KMatchID matchID, MatchDelReason reason);

	//条件检查
	bool AddCond(const KKSG::KMatchUnit& kunit);
	//其他数据模块的同步
	void AddSyn(MatchUnit* pUnit);
	void DelSyn(MatchUnit* pUnit);

	virtual void OnDelReason(MatchUnit* pUnit, MatchDelReason reason) {}
	virtual void AddRoleDelay(UINT64 roleID, UINT32 now);

	KMatchID NewMatchID();

private:

	//匹配单元唯一
	std::unordered_map<KMatchID, MatchUnit*> m_matchUnits;

	//roleID->MatchID (所有 包括以队伍的形式匹配的玩家) 防止同一个玩家重复匹配
	std::unordered_map<UINT64, KMatchID> m_role2MatchID;

	//Key(serverid roleid teamid) 用来快速查找
	std::map<KMatchKey, KMatchID> m_key2MatchID;

	//不同匹配类型对应的mgr
	std::map<int, MatchUnitMgrBase*> m_type2Mgr;

	//匹配成功短时间内不让继续匹配 防止匹配成功玩家要切场景的时候又参与了匹配
	RoleOpDelay m_delay;

};

#endif