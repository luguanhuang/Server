#ifndef __MILITARY_RANK_MANAGER_H__
#define __MILITARY_RANK_MANAGER_H__

#include "unit/role.h"
#include "table/MilitaryRankByExploit.h"
#include "table/GetMilitaryExploitConfig.h"

class MilitaryRankManager
{
public:
	MilitaryRankManager();
	~MilitaryRankManager();
	DECLARE_SINGLETON(MilitaryRankManager);

	bool Init();
	bool Reload();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void ClearFile();

public:
	// 获取军功
	UINT32 GetMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank);
	// 获取失败减少的军功
	UINT32 GetReduceMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank);
	// 军功获取军阶
	UINT32 GetMilitaryRankByExploit(const UINT32 military_exploit);

	void GetMilitaryRankNameByRank(const UINT32 military_rank,std::string& name);

	void GetMilitaryRankByRank(const UINT32 military_rank, std::vector<ItemDesc>& items);
	// 从MS上拉取最新军衔
	void UpDateMilitaryRankMS(Role *role);

	void GiveRewardForFirstAchieveRank(UINT64 role_id,UINT32 military_rank);
private:
	bool needUpdateRank(UINT32 rank);
private:
	typedef std::pair<UINT32,UINT32> ExploitValue;
	typedef std::unordered_map<UINT32,ExploitValue> MilitaryValue;
	std::unordered_map<UINT32,MilitaryValue> battle_type_military_;

	MilitaryRankByExploit military_rank_;
};

#endif
