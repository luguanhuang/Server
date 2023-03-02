#ifndef __FASHIONMGR_H__
#define __FASHIONMGR_H__

#include "table/FashionSuitTable.h"
#include "table/FashionComposeTable.h"
#include "unit/combatattribute.h"
#include "table/FashionComposeSet.h"
#include "table/FashionEffectTable.h"
#include "table/FashionCharm.h"
#include "table/FashionHair.h"
#include "define/itemdef.h"
#include "table/FashionSynthesis.h"
#include "table/FashionSuitSpecialEffects.h"
class Bag;
class Role;
struct ItemChangeAttr;
struct FashionSuitEffect
{
	std::map<UINT32, std::vector<ItemChangeAttr>> m_effect;
};


struct FashionSet
{
	UINT32 type;
	UINT32 levelseal;
	UINT32 timebegin;
	UINT32 timeend;
	Sequence<UINT32, 2> fashion;

	bool Valid(UINT32 _levelseal, UINT32 _time)
	{
		if (2 == type)
		{
			return levelseal == _levelseal;
		}
		else if (3 == type)
		{
			return (_time >= timebegin && _time <= timeend);
		}
		else
		{
			return true;
		}
	}

	UINT32 Fill(std::vector<Sequence<UINT32, 2>>& _fashions)
	{
		_fashions.push_back(fashion);
		return fashion.seq[1];
	}
	FashionSet()
	{
		type = 0;
		levelseal = 0;
		timebegin = 0;
		timeend = 0;
	}
};


struct SuitDisplayStore
{
	UINT32 suit_id;
	std::unordered_set<UINT32> items;
	UINT32 count;
	SuitDisplayStore()
	{
		suit_id = 0;
		count = 0;
		items.clear();
	}
};

struct FashionInfo;
class CombatAttribute;
class FashionMgr
{
	DECLARE_SINGLETON(FashionMgr);
	FashionMgr();
	~FashionMgr();

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void BuildDesignation();
	void ClearFile();

	UINT32 RandomComposeFashion();

	void SuitEffect(FashionInfo* fashions[], UINT32 count, CombatAttribute* attrs);

	UINT32 GetFashionSuitID(UINT32 dwFashionID);
	FashionSuitTable::RowData *GetFashionSuitConf(UINT32 dwSuitID);
	bool GetFashionSuit(UINT32 nFashionID,UINT32 nSuitID);
	bool HaveEntireFashion(Role* pRole, UINT32 suitId);
	FashionComposeTable::RowData *GetFashionComposeConf(UINT32 dwFashionID, int Level);
	FashionCharm::RowData* GetFashionCharmConf(UINT32 dwSuitID);

	void FashionActivateEffect(std::vector<SuitDisplayStore>& active_charm,CombatAttribute* attrs);
	void AddFashionCharmAttrs(vector<Sequence<uint, 2>>& add_attribute, CombatAttribute* attrs);
	UINT32 GetSuitActiveLevel(UINT32 suit_id);
	bool NeedActiveFashionSuit(UINT32 suit_id);
	UINT32 GetActiveCount(UINT32 suit_id,UINT32 have_items);
	void GetSuitItems(UINT32 suit_id, std::unordered_set<UINT32>& items);
	UINT32 GetSuitIdForCharm(UINT32 item_id);

	void GetColorByHair(UINT32 hair_id,std::unordered_set<UINT32>& colors);
	void GetHairColorCost(UINT32 hair_id,UINT32 hair_color,std::vector<ItemDesc>& items);

	UINT32 GetDefaultHairColor(UINT32 hair_id);
	UINT32 GetFashionCountBySuitID(Role* pRole, UINT32 suitId);
	UINT32 GetFashionCount(Role* pRole);
	bool GetFashionQuality(Role* pRole, UINT32 fashionId, UINT32& quality);
	FashionSynthesis::RowData* GetFashinSynthesisinfo(const UINT32 fashion_id);
	UINT32 GetSpecialEffectsIDBySuitID(UINT32 suit_id);
	UINT32 GetSuitIDBySpecialEffectsID(UINT32 special_effects_id);
private:	
	void SuitEffect(UINT32 suitid, UINT32 count, CombatAttribute* attrs);
	void QualityEffect(UINT32 quality, UINT32 count, CombatAttribute* attrs, bool isthree);
	bool InitSuitEffect(std::vector<ItemChangeAttr>& attrs, std::vector<Sequence<UINT32, 2>>& conf);

private:
	FashionSuitTable m_SuitTable;
	FashionComposeTable m_ComposeTable;
	FashionComposeSet m_composeset;
	FashionEffectTable m_effecttable;
	FashionCharm  fashion_charm_;
	FashionHair  fashion_hair_;

	std::unordered_map<UINT32,std::vector<UINT32>> charm_count_;
	std::unordered_map<UINT32,UINT32> suilt_items_;
	std::vector<FashionSet> m_type2composeset;
	std::unordered_map<UINT32, UINT32> m_SuitSetID;
	std::map<UINT32, FashionSuitEffect> m_suiteffects;
	std::map<UINT32, FashionSuitEffect> m_qualityeffects;
	std::map<UINT32, FashionSuitEffect> m_threequalityeffects;


	//装备ID---时装ID MAP KEY:装备ID  VALUE:装备所在的时装ID
	std::map<UINT32, UINT32> m_mapEquipFashion;


	// 合成    
	FashionSynthesis	fashion_synthesis_;

	// 特效
	FashionSuitSpecialEffects  fashionsuit_specialeffects_;
};

#endif // __FASHIONMGR_H__