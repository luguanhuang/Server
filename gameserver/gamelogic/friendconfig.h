//#ifndef __FRIENDDEGREEMGR_H__
//#define __FRIENDDEGREEMGR_H__
//
//#include "table/FriendTable.h"
//#include "table/FriendDegreeTable.h"
//#include "table/FriendSysConfigTable.h"
//
//#define INVALID_DEGREE UINT32(-1)
//
//enum DegreeId
//{
//	DegreeGM = 0,
//	DegreeElite = 1,
//	DegreeNest = 2,
//	DegreeChat = 3,
//	DegreeFlower = 4,
//	DegreeFight = 5,
//	DegreeTower = 6,
//	DegreeSmallMonster = 7,
//	DegreePVP = 8,
//	DegreeDragon = 9,
//	DegreeGoddess = 10,
//};
//
//struct ItemDesc;
//struct BuffDesc;
//class Role;
//class FriendConfig
//{
//	FriendConfig();
//	~FriendConfig();
//	DECLARE_SINGLETON(FriendConfig);
//public:
//	bool Init();
//	void Uninit();
//
//	UINT32 OnLevelUp(UINT64 roleid, UINT64 friendid, UINT32 all, UINT32 day);
//
//	UINT32 GetDegreeLevel(UINT32 degree);
//	UINT32 GetDegreeLeft(UINT32 degree);
//
//	float GetDegree(UINT32 id);
//	UINT32 GetLimit(UINT32 id);
//
//	BuffDesc GetBuff(UINT32 degree);
//
//	ItemDesc GetReward(UINT32 level);
//	UINT32 GetNotice(UINT32 level);
//
//	void RewardDegree(UINT32 level, UINT64 idfirst, UINT64 idsecond);
//	void NoticeDegree(UINT32 level, UINT64 idfirst, UINT64 idsecond);
//
//	UINT32 SceneToType(UINT32 scenetype, UINT32 sceneid);
//	/*
//	UINT32 FriendLimit();
//	UINT32 BlackLimit();
//	UINT32 ApplyLimit();
//	UINT32 DayGiftReceive();
//	UINT32 DayGiftSend();
//	UINT32 DegreeMax();
//	UINT32 DegreeDay();
//	*/
//
//private:
//	FriendTable m_friendtable;
//	FriendDegreeTable m_degreetable;
//	FriendSysConfigTable m_systable;
//};
//#endif