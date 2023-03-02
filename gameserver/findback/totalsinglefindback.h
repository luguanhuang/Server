#ifndef __ITEMFINDBACKTOTALSINGLE_H__
#define __ITEMFINDBACKTOTALSINGLE_H__

#include "table/ItemBackTable.h"
#include "itemfindbackbase.h"

class Role;
class CFindBackRecord;
struct CItemFindBackInfo;
struct BaseFindBackInfo;

namespace KKSG
{
	class ItemFindBackInfo;
	class RoleFindBackRecord;
	class ItemFindBackInfoRes;
	class ItemFindBackInfo2Client;
}

struct OneDayTotalSingleSubtypeInfo : public OneDayTypeInfo
{
	friend struct TotalSigleSubtypeFindBackInfo;
	virtual int GetOneDayMaxCount(Role* role, int type);
protected:
	OneDayTotalSingleSubtypeInfo(BaseFindBackInfo* baseFindBack, int type, int day);
};

struct TotalSigleSubtypeFindBackInfo : public BaseFindBackInfo
{
	TotalSigleSubtypeFindBackInfo(int type, CItemFindBackInfo* info);

	virtual UINT32 CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards);

	virtual UINT32 GetCanFindBackCount();

	virtual void AddAlreadyFindBackCount(int count);

	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);

	//CalcFindBackRewardAndCost, GenerateFindBackInfo call this
	bool GetOneFindBackCost(std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost);

	virtual void GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes);
};

struct OneDayFatigueRecoverInfo : public OneDayTotalSingleSubtypeInfo 
{
	friend struct FatigueRecoverFindBackInfo;

	//FatigueRecoverFindBackInfo::CalcFindBackRewardAndCost,GenerateFindBackInfo call this
	virtual bool GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost);
protected:
	OneDayFatigueRecoverInfo(BaseFindBackInfo* baseFindBack, int type, int day);
};

struct FatigueRecoverFindBackInfo : public TotalSigleSubtypeFindBackInfo
{
	FatigueRecoverFindBackInfo(int type, CItemFindBackInfo* info);

	virtual UINT32 CalcFindBackRewardAndCost(int type, int count, std::map<int, int>& costs, std::map<int, int>& rewards);

	virtual UINT32 GetCanFindBackCount();

	virtual void AddAlreadyFindBackCount(int count);

	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);

	virtual void GenerateFindBackInfo(KKSG::ItemFindBackInfoRes& roRes);
};

struct DiceFindBackInfo : public TotalSigleSubtypeFindBackInfo
{
	DiceFindBackInfo(int type, CItemFindBackInfo* info);

	//virtual UINT32 GiveRewards(std::map<int, int>& rewards);
};

#endif
