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

	///> ���Լ� [�������] �� [��ʵ����], ����ֵ���������뱳��������
	UINT32 AddAnyItem(const ItemDesc& desc, BagListener* listener);
	UINT32 AddAnyItem(const XItem *pItem, BagListener* listener);

	///> ͨ��uid����Ʒ
	XItem* FindUniqueItem(UINT64 uid);								///> ��������ϵ�
	XItem* FindUniqueItemInBag(UINT64 uid);							///> �����
	XItem* FindUniqueItemOnBody(UINT64 uid, int* index = NULL);		///> ���ϵ�(װ��+����+����)
	XItem* FindUniqueEquipOnBody(UINT64 uid, int* index = NULL);	///> װ��, index��λ��
	XItem* FindUniqueEmblemOnBody(UINT64 uid, int* index = NULL);	///> ����, index��λ��
	XItem* FindUniqueArtifactOnBody(UINT64 uid, int* index = NULL); ///> ����, index��λ��

	///> ͨ������+λ�û�ȡ���ϴ�����Ʒ
	XItem* GetItemOnBody(ItemType eType, int slot);

	///> ɸѡ������itemid��itembind����Ʒ
	void FindItemsAll(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter = Bind_ALL);	///> ��������ϵ�
	void FindItemsInBag(UINT32 itemID, std::vector<XItem*>& items, ItemBind filter = Bind_ALL);	///> �����
	void FindItemsOnBody(UINT32 itemID, std::vector<XItem*>& items);							///> ���ϵ�
	
	inline const std::list<XItem*>& GetEquipItemList() { return m_Equips; }
	inline const std::list<XItem*>& GetEmblemItemList() { return m_Emblems; }
	inline const std::list<XItem*>& GetArtifactItemList() { return m_Artifacts; }
	inline const std::list<XItem*>& GetInscriptionItemList() { return m_Inscription; }
	inline const std::list<XItem*>& GetBagItemList() { return m_Bag; }

	///> ����
	UINT32 CountItemTotal(UINT32 itemID);	///> ����ĺ����ϵ� or ������Ʒ
	UINT32 CountItemInBag(UINT32 itemID);	///> �����
	UINT32 CountItemOnBody(UINT32 itemID);	///> ���ϵ�

	UINT32 CountEmblemOnBody();				///> �������¸���	

	static void SortBind(std::vector<XItem*>& items);
	static void SortCount(std::vector<XItem*>& items);

	///> �ܷ���뱳��
	bool CanAddThisItem(UINT32 itemID);
	bool CanAddThisTypeItem(UINT32 type);
	bool CanAddItem(const std::vector<ItemDesc>* itemDesc, const std::vector<XItem>* item = NULL);	

	///> ������Ʒ
	void ChangeItemCount(XItem* pItem, int change, UINT32 cooldown, BagListener* listener);
	void RemoveItemUnique(UINT64 uid, BagListener* listener);

	KKSG::ErrorCode WearEquip(XItem* pItem, int isSwap, BagListener* listener);
	KKSG::ErrorCode WearEmblem(XItem* pItem, BagListener* listener);
	KKSG::ErrorCode WearArtifact(XItem* pItem, BagListener* listener);
	KKSG::ErrorCode TakeoffItem(XItem* pItem, bool isFullSendMail, BagListener* listener);	///> ����װ��or����or����	
	
	///> ���slotλ�õļ������£�Ƶ�ʸߣ�����Ԥ����
	const SkEmblem* GetWearedSkillEmblemBySlot(UINT32 slot);
	void ReCalSkillEmblem();

	///> ��Ʒ����
	void ChangeItemAttr(XItem* pItem, const ItemChangeAttr& stChangeAttr, BagListener* listener);
	void ChangeItemEnhanceAttr(XItem* pItem, std::vector<ItemChangeAttr>& attrs, BagListener* listener);
	void ChangeItemJade(XItem* pItem, const JadeSingle& stJadeSingle, bool bIsDetach, BagListener* listener);
	void MarkItemChanged(XItem *pItem, BagListener* listener);

	///> Ӧ������
	void ApplyEquipEffect(CombatAttribute *combatAttr);

	///> �õ���ͬģ������
	void GetDifferentModuleAttr(std::vector<CombatAttribute>& combatAttrs); 

	///> ����������ߵĽӿ�
	UINT64 CountVirtualItem(UINT32 itemID);
	void DecVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener);
	void IncVirtualItem(UINT32 itemID, UINT32 Count, BagListener* listener);
	void IncVirtualItemRaw(UINT32 itemID, UINT32 Count);
	void SetVirtualItem(UINT32 itemID, UINT64 Count, bool isRollBack, BagListener* listener);
	void ClearVirtualItem(UINT32 itemID, BagListener* listener);

	void ApplyVirtualItems(Role *pRole, bool isRealExp = false);
	UINT64 GetVirtualItemsExp() { return m_VirtualItem[EXP]; }

	///> ���ĳ�����͵���Ʒ
	void ClearBag(Role* pRole, UINT32 dwType);

	void ForeachItem(ItemCB cb);
	void ForeachEquip(ItemCB cb);
	void ForeachEquipOnBody(ItemCB cb);

	void SetSummaryEquip(EquipBrief* brief, void* arg);
	void SetSummaryEmblem(EmblemBrief* brief, void* arg);
	void SetSummaryArtifact(ArtifactBrief* brief, void* arg);

	///> ��ȡ��Ʒ
	void GetBodyEquip(std::vector<XItem*>& items);
	void GetBodyEmblem(std::vector<XItem*>& items);
	void GetBodyArtifact(std::vector<XItem*>& items);
	void GetBagItem(std::vector<XItem*>& items, ItemType type = ItemTypeAll);

	void GetOutLook(KKSG::OutLookEquip& outlookequip);

	///> ���������ϲ���Ʒ�����ѵ���
	void RearrangeBag();

	///> ǿ����ʦ�ȼ�
	inline UINT32 GetEnhanceMaster() { return m_enhanceMaster; }
	void SetEnhanceMaster(UINT32 enhanceMaster);
	void NotifyEnhanceMaster();

	///> ��ְҵ
	void ChangeProfession(UINT32 from, UINT32 to);
	void TransEquip(XItem* equip, UINT32 from, UINT32 to);
	void DoDetachJade(XItem* equip, const std::vector<JadeSingle>& jadesingles);

	///> ����װ��
	KKSG::ErrorCode UpgradeEquip(UINT64 uid);

	///> ����������ģ��ʱ�õĺ�����Σ�գ�������
	void AddItemToEquipPosition(UINT32 itemID, int pos);

	///> Ч���ӳ����ݣ�������
	void UpdateArtifactEffectData();
	void ArrangeEffectData();
	const EffectDataParams* GetBuffEffectData(UINT32 buffID);
	const EffectDataParams* GetSkillEffectData(UINT32 skillID);

	UINT32 GetExtraSkillEmblemSlot() { return m_extraSkillEmblemSlot; }
	void AddExtraSkillEmblemSlot();

	///> ��������
	KKSG::ErrorCode AddBagExpand(KKSG::BagType type, UINT32 num);
	std::pair<UINT32, UINT32> GetBagExpandData(KKSG::BagType type);

	void GMResetFuseCompensation() { m_GMFuseCompensation = false; m_isModify.Set(); }
	void GMResetForgeCompensation() { m_GMForgeCompensation = false; m_isModify.Set(); }

private:
	///> ����һ���µ� [��ʵ����]
	UINT32 _AddItem(const ItemDesc& desc, BagListener* listener);
	UINT32 _AddNewItem(const ItemDesc& desc, BagListener* listener);
	UINT32 _AddNewItem(const XItem* pItem, BagListener* listener);
	bool _InsertToBag(XItem* pItem, bool isDelete);
	bool _InsertToBag(std::list<XItem*>* itemList, XItem* pItem, bool isDelete);
	///> ��ø���Ʒ�����б�
	std::list<XItem*>* _GetItemList(UINT32 itemID);
	std::list<XItem*>& _GetItemListByType(UINT32 type);
	///> ��������б��������
	UINT32 _GetItemListMaxCapacity(UINT32 itemID);
	UINT32 _GetItemListMaxCapacityByType(UINT32 type);
	///> ɾ����Ʒ + delete item
	void _DeleteItem(std::list<XItem*>* itemList, std::list<XItem*>::iterator& iter);
	///> BackFlow��ʼ��
	void _BackFlowFirstInitEquip();
	void _BackFlowFirstInitEmblem();
	///> ��������
	void _GiveFuseCompensation();
	///> ���첹��
	void _GiveForgeCompensation();

private:
	Role* m_pRole;
	Switch m_isModify;
	///> �������ϵ�װ��
	XItem* m_Equip[EquipCount];
	///> �������ϵ�����
	XItem* m_Emblem[EmblemCount];
	///> �������ϵļ�������
	SkEmblem* m_WearedSkillEmblem[SkillEmblemCount];
	///> �������ϵ�����
	XItem* m_Artifact[ArtifactCount];
	///> ������Ʒ����ʯ�����ң�etc
	UINT64 m_VirtualItem[VIRTUAL_ITEM_MAX];
	
	std::list<XItem*> m_Equips;						///> װ��
	std::list<XItem*> m_Emblems;					///> ����	
	std::list<XItem*> m_Atlas;						///> ͼ��(Ŀǰ�����������1000��
	std::list<XItem*> m_Artifacts;					///> ����
	std::list<XItem*> m_Inscription;				///> ����
	std::list<XItem*> m_Bag;						///> ����
	std::unordered_map<UINT64, XItem*> m_mapItems;	///> ����list��������Ʒ

	///> ǿ����ʦ�ȼ�
	UINT32 m_enhanceMaster;
	///> ����ļ������¿�����Ŀ
	UINT32 m_extraSkillEmblemSlot;
	///> ��������
	std::map<KKSG::BagType, std::pair<UINT32, UINT32>> m_bagExpand; ///> BagType -> pair<num, count>
	///> Ч���ӳ�
	std::unordered_map<UINT32, EffectDataParams> m_buff2EffectData;
	std::unordered_map<UINT32, EffectDataParams> m_skill2EffectData;
	///> GM
	bool m_GMFuseCompensation;
	bool m_GMForgeCompensation;
};

#endif // __BAG_H__
