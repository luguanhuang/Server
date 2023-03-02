#ifndef __ITEMFINDBACKTOWER_H__
#define __ITEMFINDBACKTOWER_H__

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


struct OneDayTowerInfo : public OneDayTypeInfo
{
	friend struct TowerFindBackInfo;
	virtual bool ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount);
	virtual bool GetOnceFindBackCost(Role* role, std::map<int, int>& dragonItems, std::map<int, int>& goldItems, int& dragonCost, int& goldCost);
protected:
	OneDayTowerInfo(BaseFindBackInfo* baseFindBack, int day);
};

struct TowerFindBackInfo : public BaseFindBackInfo
{
	TowerFindBackInfo(CItemFindBackInfo* info);

	//virtual void AddUseCount(int count, int dayBegin);
	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);
	virtual UINT32 GetOneDayInfoParam();
};

#endif
