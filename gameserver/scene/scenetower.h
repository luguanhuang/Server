#ifndef __SCENETOWER_H__
#define __SCENETOWER_H__
#include "scenecb.h"

namespace KKSG
{
	class SceneSwitchData;
}
class Enemy;
class Scene;
class Role;
class SceneTower : public ISceneEventListener
{
public:
	SceneTower(Scene* scene);
	~SceneTower(){}
	static SceneTower* CreateSceneTower(Scene* scene);

	virtual void OnEnemyDie(Scene* pScene, Enemy* enemy);

	void OnEnterScene(Role* role);
	void GetReward(Role* role, std::map<int, int>& rewards);

	bool IsNewRecord(Role* role);
	bool OnChangeRank();

	int GetHardLevel();
	void SendTowerInfo(Role* role);
	bool FinishNoFloor(Role* role);
	int GetTopFloor(Role* role);
	
	//单机版
	void Init(const KKSG::SceneSwitchData& roData);
	void SendTowerInfoV2(Role* role);//单机版
	void AddFetchReward(Role* role, int itemID, int count);
	void AddHisFetchReward(Role* role, int itemID, int count);
	bool CheckSendBattleResult(Role* role = NULL);

	const std::map<int, int>* GetCurFloorFetchReward();
	const std::map<int, int>* GetHisFloorFetchReward();

	void GetFloorReward(Role* role, std::map<int, int>& rewards);
	void GetHisFloorReward(Role* role, std::map<int, int>& rewards);
	void GetRewardV2(Role* role, std::map<int, int>& rewards);

	void OnFinishScene(Role* role);
	bool FinishNoFloorV2(Role* role);

	bool UpdateCurFloorInfo(Role* role);
	bool OnChangeRankV2(Role* role);

	bool IsNewRecordV2(Role* role);

	int GetFinalReachFloor(Role* role);
	int GetFinalReachTime(Role* role);
	int GetCurFloor(){return m_curFloor;}

	void AddLeaveRole(Role* role);
	bool IsRoleLeave(Role* role);

	bool CheckWinDelayEnd();

protected:
	std::map<Role* , int> m_RoleTower;
	Scene* m_pScene;

	//单机版
	std::map<int, int> m_mapTowerCurFloorFetchReward;
	std::map<int, int> m_mapTowerFetchReward;//his
	int m_curFloor;
	int m_bestTime;
	int m_startFloor;

	std::map<Role* , int> m_leaveRole;

};



#endif // 
