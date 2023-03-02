#ifndef __ITEM_H__
#define __ITEM_H__

#include "define/itemdef.h"
#include "itemsuffix.h"
#include "cvsreader.h"

namespace KKSG 
{ 
	class Item; 
	class ItemBrief;
	class EffectData;
}

class Role;
struct XItem
{
	///> 物品基础信息
	UINT64 uid;								///> 物品唯一ID
	UINT32 type;							///> 物品类型
	UINT32 itemID;							///> 物品配置ID
	int    itemCount;						///> 包裹中指定物品个数
	bool   isbind;							///> 是否是绑定的
	UINT32 cooldown;						///> cd时间
	UINT32 expirationTime;					///> 过期时间(仅对背包中的物品有效）
	std::vector<ItemChangeAttr> changeAttr;	///> 物品基础属性
	std::vector<KKSG::EffectData> effects;	///> 物品效果属性	

	///> 物品额外信息
	std::vector<Sequence<UINT32, 3>> circleDrawInfo;	///> 圆盘抽奖信息 针对类型 (CIRCLE_DWAW)
	UINT32 smeltCount;						///> 洗炼总次数
	EnhanceInfo* enhanceInfo;				///> 强化信息
	EmblemInfo* emblemInfo;					///> 纹章信息
	JadeInfo* jadeInfo;						///> 龙玉信息
	RandomAttr* randAttr;					///> 随机属性
	EnchantAttr* enchant;					///> 附魔
	ForgeAttr* forge;						///> 锻造
	FuseAttr* fuse;							///> 熔铸

	XItem();
	XItem(UINT64 id, UINT32 itemID, int Count, UINT32 nType);
	XItem(const XItem &item);
	~XItem();

	const XItem& operator= (const XItem& stItem);

	void  Init();
	void  Init(UINT32 itemID, int Count);
	void  InitBaseData();
	void  Reset();
	void  ClearExtendInfo();
	void  CopyExtendInfo(const XItem& stItem);

	void  ApplyItemEffect(CombatAttribute *combatAttr, Role* pRole);
	void  GetEquipDifferentPartAttr(std::vector<CombatAttribute>& combatAttrs); //得到不同部分的Attribute

	bool  ChangeItemAttr(const ItemChangeAttr& stChangeAttr);
	bool  ChangeEnhanceAttr(std::vector<ItemChangeAttr>& attrs);
	bool  ChangeJade(const JadeSingle& stJadeSingle, bool bIsDetach);
	void  DetachSameJade(std::vector<JadeSingle>& jadesingles);

	bool  IsCanBeOverLapped(bool isbind_) { return isbind_ == isbind; }

	inline bool IsBind() { return isbind; }
	inline void SetBind(bool isbind_) { isbind = isbind_; }

	bool  IsCoolDown();
	inline void  SetCoolDown(UINT32 timeval) { cooldown = timeval; }
	inline void  ClearCoolDown() { cooldown = 0; }

	bool  IsExpire(UINT32 nowTime);

	inline UINT32 GetEnhanceLevel() { return NULL == enhanceInfo ? 0 : enhanceInfo->level; }

	bool DecomposeItem(UINT32 count, std::map<UINT32, UINT32>& items, std::map<UINT32, UINT32>& jades);

	bool IsTreasure();

	KKSG::EmblemSlotType GetEmSlotType();
	void SetEmSlotType(KKSG::EmblemSlotType slotType);

	static void Item2KKSGItem(const XItem *pItem, KKSG::Item *pDBItem);
	static void KKSGItem2Item(XItem *pItem, const KKSG::Item *pDBItem);
	static UINT32 GetType(UINT32 dwItemId);
	static UINT32 GetQuality(UINT32 dwItemId);
	static UINT32 GetLevel(UINT32 dwItemId);
	static UINT32 GetPos(UINT32 dwItemId);
	static float GetDecomposeParam();
	static inline bool IsVirtualItem(UINT32 itemID) { return itemID < VIRTUAL_ITEM_MAX ;}
	static bool CheckProfession(UINT32 itemID, int profession);
	static bool CompareBind(const XItem* itemfirst, const XItem* itemsecond)
	{ 
		if(itemfirst->isbind != itemsecond->isbind) return itemfirst->isbind > itemsecond->isbind;
		if(itemfirst->cooldown != itemsecond->cooldown) return itemfirst->cooldown > itemsecond->cooldown;
		return itemfirst->itemCount < itemsecond->itemCount;
	}
	static bool CompareCount(const XItem* itemfirst, const XItem* itemsecond) { return itemfirst->itemCount < itemsecond->itemCount; }
	static bool IsJewelry(int pos);
	static bool IsWeapon(int pos);

	static void PrintEquipTLog(Role* pRole, XItem* pItem, int oper, std::vector<ItemDesc>& vlist, UINT32 id=0, UINT32 preid=0);
	static void PrintEmblemTLog(Role* pRole, XItem* pItem, int oper, INT32 slot, std::vector<ItemDesc>& vlist, const std::string& strTrans="");
	static void PrintArtifactTLog(Role* pRole, XItem* pItem, int oper, const std::string& strTrans="", bool isFlag=true, UINT32 param=0);
};

struct EqualItemUID
{
	UINT64 m_UID;
	EqualItemUID(UINT64 uid) : m_UID(uid) {}
	bool operator()(const XItem *item)
	{
		return (NULL == item) ? false : item->uid == m_UID;
	}
};

#endif // __ITEM_H__
