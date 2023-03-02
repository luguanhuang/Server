#ifndef __BAG_H__
#define __BAG_H__

#include "define/itemdef.h"
#include "item.h"
#include "itembrief.h"
#include "unit/roleserilizeinterface.h"
#include "baglistener.h"

namespace KKSG
{
	class OutLookEquip;
}

struct EffectDataParams
{
	struct TypeData
	{
		UINT32 effectID;
		UINT32 templatebuffID;
		std::vector<int> params;
		std::vector<std::string> constantParams;
	};

	std::unordered_map<UINT32, std::vector<TypeData>> type2DataMap;
};


typedef std::function<void(XItem*)> ItemCB;

class Role;
class CombatAttribute;
class Bag : public RoleSerializeInterface
{
public:
	Bag(Role* pRole);
	~Bag();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	inline void SetModify() { m_isModify.Set(); }

	void UpdatePerMinute();

	///> 可以加 [虚拟道具] 和 [真实道具], 返回值是真正进入背包的数量
	UINT32 AddAnyItem(const ItemDesc& desc, BagListener* listener);
	UINT32 AddAnyItem(const XItem *pItem, BagListener* listener);

	///> 通过uid找物品
	XItem* FindUniqueItem(UINT64 uid);								///> 包里和身上的
	XItem* FindUniqueItemInBag(UINT64 uid);							///> 包里的
	XItem* FindUniqueItemOnBody(UINT64 uid, int* index = NULL);		///> 身上的(装备+纹章+龙器)
	XItem* FindUniqueEquipOnBody(UINT64 uid, int* index = NULL);	///> 装备, index是位置
	XItem* FindUniqueEmblemOnBody(UINT64 uid, int* index = NULL);	///> 纹章, index是位置
	XItem* FindUniqueArtifactOnBody(UINT64 uid, int* index = NULL); ///> 龙器, index是位置

	///> 通过类型+位置获取身上穿的物品
	XItem* GetItemOnBody(ItemType eType, int slot);

	///> 筛选出满足itemid和itembind的物品
	void FindItemsAll(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter = Bind_ALL);	///> 包里和身上的
	void FindItemsInBag(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter = Bind_ALL);	///> 包里的
	void FindItemsOnBody(UINT32 itemID, std::vector<XItem*>& items);							///> 身上的
	
	inline const std::list<XItem*>& GetEquipItemList() { return m_Equips; }
	inline const std::list<XItem*>& GetEmblemItemList() { return m_Emblems; }
	inline const std::list<XItem*>& GetArtifactItemList() { return m_Artifacts; }
	inline const std::list<XItem*>& GetInscriptionItemList() { return m_Inscription; }
	inline const std::list<XItem*>& GetBagItemList() { return m_Bag; }

	///> 计数
	UINT32 CountItemTotal(UINT32 itemID);	///> 包里的和身上的 or 虚拟物品
	UINT32 CountItemInBag(UINT32 itemID);	///> 包里的
	UINT32 CountItemOnBody(UINT32 itemID);	///> 身上的

	UINT32 CountEmblemOnBody();				///> 身上纹章个数	

	static void SortBind(std::vector<XItem*>& items);
	static void SortCount(std::vector<XItem*>& items);

	///> 能否放入背包
	bool CanAddThisItem(UINT32 itemID);
	bool CanAddThisTypeItem(UINT32 type);
	bool CanAddItem(const std::vector<ItemDesc>* itemDesc, const std::vector<XItem>* item = NULL);	

	///> 操作物品
	void ChangeItemCount(XItem* pItem, int change, UINT32 cooldown, BagListener* listener);
	void RemoveItemUnique(UINT64 uid, BagListener* listener);

	KKSG::ErrorCode WearEquip(XItem* pItem, int isSwap, BagListener* listener);
	KKSG::ErrorCode WearEmblem(XItem* pItem, BagListener* listener);
	KKSG::ErrorCode WearArtifact(XItem* pItem, BagListener* listener);
	KKSG::ErrorCode TakeoffItem(XItem* pItem, bool isFullSendMail, BagListener* listener);	///> 脱下装备or纹章or龙器	
	
	///> 获得slot位置的技能纹章（频率高，所以预处理）
	const SkEmblem* GetWearedSkillEmblemBySlot(UINT32 slot);
	void ReCalSkillEmblem();

	///> 物品属性
	void ChangeItemAttr(XItem* pItem, const ItemChangeAttr& stChangeAttr, BagListener* listener);
	void ChangeItemEnhanceAttr(XItem* pItem, std::vector<ItemChangeAttr>& attrs, BagListener* listener);
	void ChangeItemJade(XItem* pItem, const JadeSingle& stJadeSingle, bool bIsDetach, BagListener* listener);
	void MarkItemChanged(XItem *pItem, BagListener* listener);

	///> 应用属性
	void ApplyEquipEffect(CombatAttribute *combatAttr);

	///> 得到不同模块属性
	void GetDifferentModuleAttr(std::vector<CombatAttribute>& combatAttrs); 

	///> 处理虚拟道具的接口
	UINT64 CountVirtualItem(UINT32 itemID);
	void DecVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener);
	void IncVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener);
	void IncVirtualItemRaw(UINT32 itemID, UINT32 Count);
	void SetVirtualItem(UINT32 itemID, UINT64 Count, bool isRollBack, BagListener* listener);
	void ClearVirtualItem(UINT32 itemID, BagListener* listener);

	void ApplyVirtualItems(Role *pRole, bool isRealExp = false);
	UINT64 GetVirtualItemsExp() { return m_VirtualItem[EXP]; }

	///> 清空某种类型的物品
	void ClearBag(Role* pRole, UINT32 dwType);

	void ForeachItem(ItemCB cb);
	void ForeachEquip(ItemCB cb);
	void ForeachEquipOnBody(ItemCB cb);

	void SetSummaryEquip(EquipBrief* brief, void* arg);
	void SetSummaryEmblem(EmblemBrief* brief, void* arg);
	void SetSummaryArtifact(ArtifactBrief* brief, void* arg);

	///> 获取物品
	void GetBodyEquip(std::vector<XItem*>& items);
	void GetBodyEmblem(std::vector<XItem*>& items);
	void GetBodyArtifact(std::vector<XItem*>& items);
	void GetBagItem(std::vector<XItem*>& items, ItemType type = ItemTypeAll);

	void GetOutLook(KKSG::OutLookEquip& outlookequip);

	///> 整理背包（合并物品至最大堆叠）
	void RearrangeBag();

	///> 强化大师等级
	inline UINT32 GetEnhanceMaster() { return m_enhanceMaster; }
	void SetEnhanceMaster(UINT32 enhanceMaster);
	void NotifyEnhanceMaster();

	///> 变职业
	void ChangeProfession(UINT32 from, UINT32 to);
	void TransEquip(XItem* equip, UINT32 from, UINT32 to);
	void DoDetachJade(XItem* equip, const std::vector<JadeSingle>& jadesingles);

	///> 升级装备
	KKSG::ErrorCode UpgradeEquip(UINT64 uid);

	///> 创建机器人模板时用的函数，危险，别乱用
	void AddItemToEquipPosition(UINT32 itemID, int pos);

	///> 效果加成数据（龙器）
	void UpdateArtifactEffectData();
	void ArrangeEffectData();
	const EffectDataParams* GetBuffEffectData(UINT32 buffID);
	const EffectDataParams* GetSkillEffectData(UINT32 skillID);

	UINT32 GetExtraSkillEmblemSlot() { return m_extraSkillEmblemSlot; }
	void AddExtraSkillEmblemSlot();

	///> 背包扩容
	KKSG::ErrorCode AddBagExpand(KKSG::BagType type, UINT32 num);
	std::pair<UINT32, UINT32> GetBagExpandData(KKSG::BagType type);

	void GMResetFuseCompensation() { m_GMFuseCompensation = false; m_isModify.Set(); }
	void GMResetForgeCompensation() { m_GMForgeCompensation = false; m_isModify.Set(); }

private:
	///> 创建一个新的 [真实道具]
	UINT32 _AddItem(const ItemDesc& desc, BagListener* listener);
	UINT32 _AddNewItem(const ItemDesc& desc, BagListener* listener);
	UINT32 _AddNewItem(const XItem* pItem, BagListener* listener);
	bool _InsertToBag(XItem* pItem, bool isDelete);
	bool _InsertToBag(std::list<XItem*>* itemList, XItem* pItem, bool isDelete);
	///> 获得该物品所属列表
	std::list<XItem*>* _GetItemList(UINT32 itemID);
	std::list<XItem*>& _GetItemListByType(UINT32 type);
	///> 获得所属列表最大容量
	UINT32 _GetItemListMaxCapacity(UINT32 itemID);
	UINT32 _GetItemListMaxCapacityByType(UINT32 type);
	///> 删除物品 + delete item
	void _DeleteItem(std::list<XItem*>* itemList, std::list<XItem*>::iterator& iter);
	///> BackFlow初始化
	void _BackFlowFirstInitEquip();
	void _BackFlowFirstInitEmblem();
	///> 熔铸补偿
	void _GiveFuseCompensation();
	///> 锻造补偿
	void _GiveForgeCompensation();

private:
	Role* m_pRole;
	Switch m_isModify;
	///> 穿在身上的装备
	XItem* m_Equip[EquipCount];
	///> 穿在身上的纹章
	XItem* m_Emblem[EmblemCount];
	///> 穿在身上的技能纹章
	SkEmblem* m_WearedSkillEmblem[SkillEmblemCount];
	///> 穿在身上的龙器
	XItem* m_Artifact[ArtifactCount];
	///> 虚拟物品：钻石，龙币，etc
	UINT64 m_VirtualItem[VIRTUAL_ITEM_MAX];
	
	std::list<XItem*> m_Equips;						///> 装备
	std::list<XItem*> m_Emblems;					///> 纹章	
	std::list<XItem*> m_Atlas;						///> 图鉴(目前设置容量最大1000）
	std::list<XItem*> m_Artifacts;					///> 龙器
	std::list<XItem*> m_Inscription;				///> 铭文
	std::list<XItem*> m_Bag;						///> 背包
	std::unordered_map<UINT64, XItem*> m_mapItems;	///> 以上list里所有物品

	///> 强化大师等级
	UINT32 m_enhanceMaster;
	///> 额外的技能纹章开孔数目
	UINT32 m_extraSkillEmblemSlot;
	///> 背包扩容
	std::map<KKSG::BagType, std::pair<UINT32, UINT32>> m_bagExpand; ///> BagType -> pair<num, count>
	///> 效果加成
	std::unordered_map<UINT32, EffectDataParams> m_buff2EffectData;
	std::unordered_map<UINT32, EffectDataParams> m_skill2EffectData;
	///> GM
	bool m_GMFuseCompensation;
	bool m_GMForgeCompensation;
};

#endif // __BAG_H__
