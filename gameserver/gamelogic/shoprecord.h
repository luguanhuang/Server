#ifndef __SHOPRECORD_H__
#define __SHOPRECORD_H__

#include "item.h"
#include "unit/roleserilizeinterface.h"

namespace KKSG
{
	class ShopRecordOne;
};

struct CShopRecordOne
{
public:
	void Clear(bool iscleardailycount);
	void ToKKSG(KKSG::ShopRecordOne* one);
	void ToThis(const KKSG::ShopRecordOne* one);
	void ToThisBrief(const KKSG::ShopRecordOne* one);
	void ToItems(const KKSG::ShopRecordOne* one);

	bool IsEmpty() { return m_items.empty(); }
	void FillItems(const std::map<UINT32, XItem>& items) { m_items = items; }
	std::map<UINT32, XItem>* GetItems() { return &m_items; }

	CShopRecordOne()
	{
		isbrief = false;
		ishint = false;
		updatetime = 0;
		refreshcount = 0;
		refreshtime = 0;
		refreshday = 0;
	}

public:
	bool isbrief;
	bool ishint;
	UINT32 updatetime;
	UINT32 refreshcount;
	UINT32 refreshtime;
	UINT32 refreshday;

	std::map<UINT32, UINT32> m_itemid2buycount;
	std::map<UINT32, UINT32> m_itemid2dailybuycount;
	std::map<UINT32, UINT32> m_itemid2weekbuycount;
private:
	std::map<UINT32, XItem> m_items;
};

class Role;
class CShopRecord : public RoleSerializeInterface
{
public:
	CShopRecord(Role* role);
	
	virtual bool Load(const KKSG::RoleAllInfo* roleallinfo);
	virtual void Save(KKSG::RoleAllInfo* roleallinfo, CDataFieldSet& roChanged);

	void LoadItems(UINT32 type);

	void Update();
	void ResetDay();
	void ResetWeek();
	std::set<UINT32> TryRefresh();
	bool RefreshOne(UINT32 type);

	void OnFirstEnterScene();

	bool IsEmpty(UINT32 type);
	bool CheckRefresh(UINT32 type, bool& isnew);
	bool Refresh(UINT32 type, UINT32 rolelevel, UINT32 roleprofession, bool isadd, bool isnew, bool iscleardailycount);
	UINT32 GetRefreshCount(UINT32 type);

	CShopRecordOne* GetShop(UINT32 type);
	XItem* GetItem(UINT32 type, UINT32 uid);

	UINT32 GetDailyBuyCount(UINT32 type, UINT32 uid);
	void AddDailyBuyCount(UINT32 type,  UINT32 uid, UINT32 count);

	UINT32 GetWeekBuyCount(UINT32 type, UINT32 uid);
	void AddWeekBuyCount(UINT32 type,  UINT32 uid, UINT32 count);

	UINT32 GetBuyCount(UINT32 type, UINT32 uid);
	void AddBuyCount(UINT32 type, UINT32 uid, UINT32 count);

	void OnLevelUp();
	void SetModify() { m_ismodify.Set(); }

private:
	Role* m_role;
	Switch m_ismodify;
	UINT32 m_updateday;
	UINT32 m_weektime;
	std::map<UINT32, CShopRecordOne> m_id2shop;
};

#endif