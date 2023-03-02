#ifndef __FRIENDDEGREEMGR_H__
#define __FRIENDDEGREEMGR_H__

#include "table/FriendTable.h"
#include "table/FriendDegreeTable.h"
#include "table/FriendSysConfigTable.h"
#include "table/FriendshipAdditionBuff.h"

#define INVALID_DEGREE UINT32(-1)

enum DegreeId
{
	DegreeGM = 0,
	DegreeElite = 1,
	DegreeNest = 2,
	DegreeChat = 3,
	DegreeFlower = 4,
	DegreeFight = 5,
	DegreeTower = 6,
	DegreeSmallMonster = 7,
	DegreePVP = 8,
	DegreeDragon = 9,
	DegreeGoddess = 10,
	DegreeEndless= 11,
};

struct ItemDesc;
struct BuffDesc;
class Role;
class FriendConfig
{
	FriendConfig();
	~FriendConfig();
	DECLARE_SINGLETON(FriendConfig);
public:
	bool Init();
	void Uninit();

	UINT32 OnLevelUp(UINT64 roleid, UINT64 friendid, UINT32 all, UINT32 day);

	float GetDegree(UINT32 id);
	UINT32 GetLimit(UINT32 id);

	UINT32 SceneToType(UINT32 scenetype, UINT32 sceneid);
	void AddDegree(UINT32 type, std::vector<UINT64> roles);

	Sequence<uint, 2>&  GetBuffSequenceByDegree(UINT32 degree);

	
private:
	FriendDegreeTable m_degreetable;

	// 友情加成提供的buff
	FriendshipAdditionBuff	friendship_addition_buff_; 
};
#endif