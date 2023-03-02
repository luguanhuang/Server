#ifndef __DRAGON_GUILD_SHOP_H_
#define __DRAGON_GUILD_SHOP_H_

#include "util/gametime.h"
#include "dragonguildbase.h"

struct DragonGuildShopItem
{
	DragonGuildShopItem(): id(0), buyCount(0), buying(false), buyStartTime(0) {}
	UINT32 id;
	UINT32 buyCount;
	bool buying;			// 保证购买请求同时只有一个
	UINT32 buyStartTime;

	void FromKKSG(const KKSG::DragonGuildShopItemData& data)
	{
		id = data.id();
		buyCount = data.buy_count();
	}

	void ToKKSG(KKSG::DragonGuildShopItemData& data)
	{
		data.set_id(id);
		data.set_buy_count(buyCount);
	}

	bool IsBuying()
	{
		UINT32 now = (UINT32)GameTime::GetTime();
		if (buying && now < buyStartTime + 10)
		{
			buying = false;
		}
		return buying;
	}
	void SetBuying()
	{
		buying = true;
		buyStartTime = (UINT32)GameTime::GetTime();
	}
};

struct DragonGuildShopRecord
{
	DragonGuildShopRecord() : roleId(0), itemId(0), itemCount(0), time(0) {}
	UINT64 roleId;
	UINT32 itemId;
	UINT32 itemCount;
	UINT32 time;

	void FromKKSG(const KKSG::DragonGuildShopRecordData& data)
	{
		roleId = data.roleid();
		itemId = data.itemid();
		itemCount = data.itemcount();
		time = data.time();
	}

	void ToKKSG(KKSG::DragonGuildShopRecordData& data)
	{
		data.set_roleid(roleId);
		data.set_itemid(itemId);
		data.set_itemcount(itemCount);
		data.set_time(time);
	}
};

class DragonGuildShop: public DragonGuildBase
{
public:
	DragonGuildShop(DragonGuild* dragonguild);

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data);
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data);
	virtual void OnDayPass();

	DragonGuildShopItem* GetItem(UINT32 id);
	void RemoveItem(UINT32 id);

	void FillShopItems(UINT64 roleId, KKSG::GetDragonGuildShopRes& res);
	void FillShopRecords(KKSG::GetDragonGuildShopRecordRes& res);

	void AddBuyRecord(UINT64 roleId, UINT32 itemId, UINT32 itemCount);
	void RefreshItem();

	UINT32 GetLastRefreshTime() { return m_lastRefreshTime; }
	void CheckRefresh();

private:
	UINT32 m_lastRefreshTime;
	//	std::vector<PartnerShopItem> m_item;
	std::map<UINT32, DragonGuildShopItem> m_item;
	std::deque<DragonGuildShopRecord> m_record;
};

#endif