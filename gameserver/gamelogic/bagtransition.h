#ifndef __BAGTRANSITION_H__
#define __BAGTRANSITION_H__

#include "bag.h"
#include "rolefashion.h"
#include "item.h"
#include "itemconfig.h"
#include "unit/combatattribute.h"
#include "define/tlogenum.h"
#include "baglistener.h"
#include "spritetransition.h"

enum TransitionAction
{
	INVALID_ACTION,
	COST_ARENA_ENTER,
	COST_BUY_GOLD,
	COST_BUY_FATIGUE,
	COST_SMELT_ITEM,
	COST_REINFORCE_SINGLE,
	COST_REINFORCE_ALL,
	COST_TAKECOMPOSECOST,
	COST_SMELT_TRASFER,
	COST_BUY_DRAGONCOIN,
	COST_BUY_BLUEBIRD, 
};

class Role;
class BagTransition
{
public:
	BagTransition(Role *pRole);
	virtual ~BagTransition();

	virtual void BeforeNotifyClient(){}
	virtual void AfterNotifyClient(){}
	
	void SetRecyle() { m_Record.SetRecyle(); }
	
	void NotifyClient();

	void SetAction(TransitionAction type){m_actionType = type;}
	TransitionAction GetAction(){return m_actionType;}

	void SetTransTag(const std::string& strbill) { m_strTrans = strbill; }

protected:
	Bag *m_pBag;
	RoleFashion* m_pFashion;
	Role *m_pRole;
	BagItemOperationRecord m_Record;
	TransitionAction m_actionType;
	bool m_isUsedNotify;
	std::string m_strTrans;
};

///> 删除物品
class BagTakeItemTransition : public BagTransition
{
public:
	BagTakeItemTransition(Role *pRole);
	~BagTakeItemTransition();

	bool TakeItem(UINT32 itemID, int Count, ItemBind filter = Bind_ALL);
	bool TakeUniqueItem(UINT64 uid, int Count);
	void RollBack();
	
	virtual void BeforeNotifyClient();

	void SetReason(INT32 nReason, INT32 nSubReason = -1) { m_nReason = nReason; m_nSubReason = nSubReason; LogDebug("taketransition reason:%d, subreason:%d", nReason, nSubReason); }
	void SetParam(INT32 nParam) { m_nParam = nParam; }
private:
	bool TakeVirtualItem(UINT32 itemID, int Count);
	bool TakeBagItem(XItem* pItem, int Count);

	void Record(UINT32 dwItemID, INT32 nCount);
	INT32 GetRecordNum(UINT32 dwItemID) const;

	void DoTXLog(UINT32 dwItemID, INT32 nType, INT64 llCount);

	std::map<XItem*, int> m_ItemOldCount;
	std::map<UINT32, UINT64>   m_VirtualItemOldCount;
	std::map<UINT32, int>	 m_ItemChangeCount;

private:
	INT32		m_nReason;			//一级原因(货币，道具)
	INT32		m_nSubReason;		//二级原因(货币，道具)
	INT32		m_nParam;			//自选参数

};

///> 给予物品
class BagGiveItemTransition : public BagTransition
{
public:
	BagGiveItemTransition(Role *pRole);
	~BagGiveItemTransition();

	void SetDoLog(bool flag) { m_dolog = flag; }
	void SetReason(INT32 nReason, INT32 nSubReason = -1, bool bIsCheckZeroProfit = true);
	void SetBuyCost(INT32 nMoneyCost, TMoneyType nMoneyType) { m_nMoneyCost = nMoneyCost; m_nMoneyType = nMoneyType; }

	void GiveItem(const ItemDesc& desc);
	void GiveItem(UINT32 itemID, UINT32 itemCount, bool isbind = true);
	// copy item to role bag
	void GiveItem(XItem* pItem);
	void SetIsRealExp(bool isRealExp);

	virtual void BeforeNotifyClient();
	virtual void AfterNotifyClient();

private:
	void DoTXLog(UINT32 dwItemID, INT32 nType, INT64 llCount);
	void Record(UINT32 dwItemID, INT32 nType, INT64 llCount);
private:
	struct ItemAddRecord
	{
		INT32	m_nType;
		INT64	m_llCount;
		ItemAddRecord():m_nType(0), m_llCount(0) {}
		ItemAddRecord(INT32 nType, INT64 llCount):m_nType(nType), m_llCount(llCount) {}
	};
private:
	bool		m_dolog;
	INT32		m_nReason;			//一级原因(货币，道具，经验)
	INT32		m_nSubReason;		//二级原因(货币，道具，经验)
	INT32		m_nMoneyCost;		//花费代币或金币购买道具情况下输出消耗的钱数量，否则填0
	TMoneyType	m_nMoneyType;		//钱的类型MONEYTYPE

	INT32		m_level;
	INT32		m_nOldLevel;
	std::map<UINT32, ItemAddRecord>	m_oAddMap;
	bool		m_bIsCheckZeroProfit;//true：需要零收益检查  false：不需要检查(默认是true需要检查，目前只有商城龙币钻石购买和充值不需要检查)
	bool		m_isRealExp;


	SpriteTransition m_spriteTransition;	
};

///> 穿戴装备
class BagWearItemTransition : public BagTransition
{
public:
	BagWearItemTransition(Role *pRole);
	~BagWearItemTransition();

	///> 脱的时候如果满了要发邮件，必须设置subreason
	void SetSubReason(int subReason);

	KKSG::ErrorCode WearItem(XItem* pItem, bool isSwap);
	KKSG::ErrorCode WearEmblem(XItem* pItem);
	KKSG::ErrorCode WearArtifact(XItem* pItem);
	KKSG::ErrorCode TakeoffItem(XItem* pItem, bool isFullSendMail = false);
	virtual void BeforeNotifyClient();
};

namespace KKSG
{
	class UseItemRes;
}
///> 使用物品
class BagUseItemTransition : public BagTransition
{
public:
	BagUseItemTransition(Role *pRole);
	~BagUseItemTransition();

	KKSG::ErrorCode UseItem(UINT64 uid, std::vector<UINT64>& uids, int Count, UINT32 dwOpType, KKSG::UseItemRes &roRes, UINT64 petId = 0);
	KKSG::ErrorCode UseItem(std::vector<UINT64>& uids, UINT32 dwOpType);
		
	KKSG::ErrorCode DisplayFashion(const UINT32 item_id,const UINT32 operator_type);
	KKSG::ErrorCode DisplaySuitFashion(const UINT32 suit_id,const UINT32 operator_type);
	KKSG::ErrorCode ActivationFashion(UINT64 uid); 		
	
private:
	KKSG::ErrorCode UseEquip(XItem *pItem, bool isSwap, UINT32 dwOpType);	
	KKSG::ErrorCode UseEmblem(XItem* pItem, UINT32 dwOpType);
	KKSG::ErrorCode UseArtifact(XItem* pItem, UINT32 dwOpType);
	KKSG::ErrorCode UsePeck(XItem *pItem, int Count, KKSG::UseItemRes& roRes);
	KKSG::ErrorCode UseCircleDraw(XItem *pItem);
	KKSG::ErrorCode UsePetBorn(XItem *pItem);
	KKSG::ErrorCode UseSceneCountTicket(XItem *pItem);
	KKSG::ErrorCode UseItemBuff(XItem *pItem);
	KKSG::ErrorCode UsePetSkillBook(XItem *pItem, UINT64 petId);
	bool TakePeckItem(UINT32 itemID, int Count);
	KKSG::ErrorCode UseTarja(XItem *pItem);
	KKSG::ErrorCode UseGuildBonus(XItem *pItem, UINT32 count);
	KKSG::ErrorCode UseDesignation(XItem *pItem);
	KKSG::ErrorCode UseHeroTicket(UINT32 itemID);
	KKSG::ErrorCode UseBagExpand(UINT32 itemID, KKSG::UseItemRes& roRes);
};

///> 修改装备属性
class BagChangeItemAttrTransition : public BagTransition
{
public:
	BagChangeItemAttrTransition(Role* pRole);
	~BagChangeItemAttrTransition();

	void ChangeItemAttr(XItem *pItem, const ItemChangeAttr& stChangeAttr);
	void ChangeItemEnhanceAttr(XItem *pItem, std::vector<ItemChangeAttr>& attrs);
	void ChangeItemJade(XItem* pItem, const JadeSingle& stJadeSingle, bool bIsDetach = false);
};

class BagUpdateItemTransition : public BagTransition
{
public:
	BagUpdateItemTransition(Role* pRole);
	~BagUpdateItemTransition();

	void AddChangedItem(XItem *pItem);
};

class BagCompositeTransition : public BagTransition
{
public:
	BagCompositeTransition(Role* pRole);
	~BagCompositeTransition();

	bool Take(BagTakeItemTransition& trans, UINT32 itemid, int count);

	KKSG::ErrorCode Composite(UINT32 compID, std::vector<UINT64>& uids, KKSG::UseItemRes& roRes);
};

#endif // __BAGTRANSITION_H__