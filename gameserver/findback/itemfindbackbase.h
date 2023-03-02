#ifndef __ITEMFINDBACKBASE_H__
#define __ITEMFINDBACKBASE_H__

#include "table/ItemBackTable.h"

class Role;
class CFindBackRecord;
struct CItemFindBackInfo;

namespace KKSG
{
	class ItemFindBackInfo;
	class RoleFindBackRecord;
	class ItemFindBackInfoRes;
	class ItemFindBackInfo2Client;
}

struct OneDaySubTypeInfo
{
	OneDaySubTypeInfo(int type)
	{
		m_type = type;
		m_todayUseCount= 0;
		m_towerLevel = 0;
	}
	void Reset();
	bool IsDayValid(Role* role, int day);

	UINT32 m_type;
	UINT32 m_todayUseCount;
	UINT32 m_towerLevel;
};

struct OneDayTypeInfo
{
	friend struct BaseFindBackInfo;

	virtual ~OneDayTypeInfo();

	virtual OneDayTypeInfo& operator = (const OneDayTypeInfo& info){assert(false);return *this;}
	virtual OneDayTypeInfo& assign (const OneDayTypeInfo& info);

	virtual bool IsDayValid();

	//2 client  GenerateFindBackInfo,GetOnceFindBackCost call
	virtual bool Convert2KKSGBackInfo(Role* role, KKSG::ItemFindBackInfo2Client& temp);
	virtual bool ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount);

	//2 reward
	virtual bool GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost);

	virtual int GetOneDayMaxCount(Role* role, int type);

	virtual void UpdateExtraSaveInf();

	void Clear();

	UINT32 GetLeftFindBackCount(Role* role);
	UINT32 GetAlreadyFindBackCount(){return m_alreadyFindBackCount;}

	Role* GetRole();

	bool IsFirstUnlockDay();
	void CheckUpdateExpInfo(std::map<int, int>& dragonItems, std::map<int, int>& goldItems);


	UINT32 m_type;
	UINT32 m_day;
	int m_level;
	UINT32 m_alreadyFindBackCount;
	BaseFindBackInfo* m_pBaseFindBack;
	std::map<UINT32, UINT32> m_onceBackExp;//money type map value

	OneDaySubTypeInfo* m_subTypeInfos;

protected:
	OneDayTypeInfo(BaseFindBackInfo* baseFindBack, int type, int day);
};

struct BaseFindBackInfo
{
	BaseFindBackInfo(int type, CItemFindBackInfo* info);
	virtual ~BaseFindBackInfo();

	virtual void Save(KKSG::RoleFindBackRecord* record);
	virtual bool Load(const KKSG::RoleFindBackRecord* record);

	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);

	virtual UINT32 CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards);

	virtual UINT32 GiveFindBackReward(int type, int count);

	virtual UINT32 GiveRewards(std::map<int, int>& rewards);

	virtual UINT32 GetCanFindBackCount();

	virtual UINT32 GetOneDayInfoParam();

	virtual void AddUseCount(int count, int dayBegin);

	virtual void AddAlreadyFindBackCount(int count);

	virtual void GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes);

	void Update(int nowBegin, int infoBegin);

	void UpdateExtraSaveInfo(int day = 0);

	UINT32 CheckFindBackRewardAndCost(int type, std::map<int, int>& costs, std::map<int, int>& rewards);

	void OnOpenInit();

	void DebugReset();

	void ClearHis(int dayBegin=0);

	void ClearCur();

	Role* GetRole();

protected:
	void InitData(int dayBegin);
	void _UpdatePerDay(int nowBegin, int infoBegin);
	void _UpdatePerWeek(int nowBegin, int infoBegin);

public:
	int m_type;
	CItemFindBackInfo* m_pItemBack;

	OneDayTypeInfo* m_usedCountMap;//不使用当天信息里面的m_day,m_level
	std::map<int, OneDayTypeInfo*> m_usedCountMap_h;
};

#endif

