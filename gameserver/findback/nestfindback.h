#ifndef __ITEMFINDBACKNEST_H__
#define __ITEMFINDBACKNEST_H__

#include "table/ItemBackTable.h"
#include "itemfindbackbase.h"

class Role;
struct CItemFindBackInfo;
struct BaseFindBackInfo;

namespace KKSG
{
	class ItemFindBackInfo;
	class RoleFindBackRecord;
	class ItemFindBackInfoRes;
	class ItemFindBackInfo2Client;
}


struct OneDayNestInfo : public OneDayTypeInfo
{
	friend struct NestFindBackInfo;
	virtual bool ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount);
protected:
	OneDayNestInfo(BaseFindBackInfo* baseFindBack, int day);
};

struct NestFindBackInfo : public BaseFindBackInfo
{
	NestFindBackInfo(CItemFindBackInfo* info);

	//virtual void AddUseCount(int count, int dayBegin);
	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);
	virtual UINT32 GetOneDayInfoParam();
};

#endif
