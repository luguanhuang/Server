#ifndef __ITEMFINDBACKMISC_H__
#define __ITEMFINDBACKMISC_H__

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

struct OneDayGuildInfo : public OneDayTypeInfo
{
	friend struct GuildFindBackInfo;
	virtual bool Convert2KKSGBackInfo(Role* role, KKSG::ItemFindBackInfo2Client& temp);
protected:
	OneDayGuildInfo(BaseFindBackInfo* baseFindBack, int day);
};

struct GuildFindBackInfo : public BaseFindBackInfo
{
	GuildFindBackInfo(int type, CItemFindBackInfo* info);

	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);
};

struct OneDayFatigueBuyInfo : public OneDayTypeInfo
{
	friend struct FatigueBuyFindBackInfo;
	virtual bool ConvertSingleSubtype(Role* role, KKSG::ItemFindBackInfo2Client& temp, int leftCount);
protected:
	OneDayFatigueBuyInfo(BaseFindBackInfo* baseFindBack, int day);
};

struct FatigueBuyFindBackInfo : public BaseFindBackInfo
{
	FatigueBuyFindBackInfo(CItemFindBackInfo* info);

	virtual OneDayTypeInfo* CreateOneDayBackInfo(int day);
};


#endif
