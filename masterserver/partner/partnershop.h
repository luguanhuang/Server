#ifndef _PARTNER_SHOP_H_
#define _PARTNER_SHOP_H_

#include "partnerbase.h"
#include "util/gametime.h"

struct PartnerShopItem
{
	PartnerShopItem(): id(0), buyCount(0), buying(false), buyStartTime(0) {}
	UINT32 id;
	UINT32 buyCount;
	bool buying;			// 保证购买请求同时只有一个
	UINT32 buyStartTime;

	void FromKKSG(const KKSG::PartnerShopItemData& data)
	{
		id = data.id();
		buyCount = data.buy_count();
	}

	void ToKKSG(KKSG::PartnerShopItemData& data)
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

struct PartnerShopRecord
{
	PartnerShopRecord() : roleId(0), itemId(0), itemCount(0), time(0) {}
	UINT64 roleId;
	UINT32 itemId;
	UINT32 itemCount;
	UINT32 time;

	void FromKKSG(const KKSG::PartnerShopRecordData& data)
	{
		roleId = data.roleid();
		itemId = data.itemid();
		itemCount = data.itemcount();
		time = data.time();
	}

	void ToKKSG(KKSG::PartnerShopRecordData& data)
	{
		data.set_roleid(roleId);
		data.set_itemid(itemId);
		data.set_itemcount(itemCount);
		data.set_time(time);
	}
};

class PartnerShop : public PartnerBase
{
public:
	PartnerShop(Partner* partner);

	virtual bool LoadFromDB(const KKSG::PartnerSaveData& data);
	virtual void SaveToDB(KKSG::PartnerSaveData& data);
	virtual void OnDayPass();

	PartnerShopItem* GetItem(UINT32 id);
	void RemoveItem(UINT32 id);

	void FillShopItems(UINT64 roleId, KKSG::GetPartnerShopRes& res);
	void FillShopRecords(KKSG::GetPartnerShopRecordRes& res);

	void AddBuyRecord(UINT64 roleId, UINT32 itemId, UINT32 itemCount);
	void RefreshItem();

	UINT32 GetLastRefreshTime() { return m_lastRefreshTime; }

	bool HasRedPoint(UINT64 roleId);
	void CheckRefresh();

private:
	UINT32 m_lastRefreshTime;
//	std::vector<PartnerShopItem> m_item;
	std::map<UINT32, PartnerShopItem> m_item;
	std::deque<PartnerShopRecord> m_record;
};

#endif