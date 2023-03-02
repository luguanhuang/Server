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
	// ��ȡ����
	UINT32 GetMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank);
	// ��ȡʧ�ܼ��ٵľ���
	UINT32 GetReduceMilitaryExploit(KKSG::SceneType scene_type,UINT32 military_rank);
	// ������ȡ����
	UINT32 GetMilitaryRankByExploit(const UINT32 military_exploit);

	void GetMilitaryRankNameByRank(const UINT32 military_rank,std::string& name);

	void GetMilitaryRankByRank(const UINT32 military_rank, std::vector<ItemDesc>& items);
	// ��MS����ȡ���¾���
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
