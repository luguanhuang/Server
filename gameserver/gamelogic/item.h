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
	///> ��Ʒ������Ϣ
	UINT64 uid;								///> ��ƷΨһID
	UINT32 type;							///> ��Ʒ����
	UINT32 itemID;							///> ��Ʒ����ID
	int    itemCount;						///> ������ָ����Ʒ����
	bool   isbind;							///> �Ƿ��ǰ󶨵�
	UINT32 cooldown;						///> cdʱ��
	UINT32 expirationTime;					///> ����ʱ��(���Ա����е���Ʒ��Ч��
	std::vector<ItemChangeAttr> changeAttr;	///> ��Ʒ��������
	std::vector<KKSG::EffectData> effects;	///> ��ƷЧ������	

	///> ��Ʒ������Ϣ
	std::vector<Sequence<UINT32, 3>> circleDrawInfo;	///> Բ�̳齱��Ϣ ������� (CIRCLE_DWAW)
	UINT32 smeltCount;						///> ϴ���ܴ���
	EnhanceInfo* enhanceInfo;				///> ǿ����Ϣ
	EmblemInfo* emblemInfo;					///> ������Ϣ
	JadeInfo* jadeInfo;						///> ������Ϣ
	RandomAttr* randAttr;					///> �������
	EnchantAttr* enchant;					///> ��ħ
	ForgeAttr* forge;						///> ����
	FuseAttr* fuse;							///> ����

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
	void  GetEquipDifferentPartAttr(std::vector<CombatAttribute>& combatAttrs); //�õ���ͬ���ֵ�Attribute

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
