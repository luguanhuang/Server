#ifndef __EMBLEMMGR_H__
#define __EMBLEMMGR_H__

#include "table/EmblemLevelTable.h"
#include "table/AttributeEmblem.h"
#include "table/SkillEmblem.h"
#include "itemsuffix.h"
#include "define/itemdef.h"

typedef SkillEmblem::RowData SkillEmblemConf;


#define EMBLEMATTRNUM 3

struct AttrEmblemSingle
{	
	UINT32 prob;
	Sequence<UINT32, 2> initAttr;
	Sequence<UINT32, 2> attrRange;
	UINT32 smeltAllProb[3];
	std::map<UINT32, std::pair<int, UINT32>> smeltAttr[3]; ///> pair, first--> ÆðÊ¼£¬ second--> ·¶Î§
	UINT32 smeltExpectation;
};

struct AttrEmblemSlot
{
	AttrEmblemSlot() { allprob = 0; }
	UINT32 allprob;
	std::unordered_map<UINT32, AttrEmblemSingle> singles;	
};

struct AttrEmblem
{
	AttrEmblemSlot slot[EMBLEMATTRNUM];
};

struct XItem;
class Role;
struct SkillEffect;

class CEmblemMgr
{
	CEmblemMgr();
	~CEmblemMgr();
	DECLARE_SINGLETON(CEmblemMgr)
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	void InitAttrEmblemAttr(XItem* pItem);
	KKSG::ErrorCode IdentifyAttrEmblem(XItem* pItem, Role* pRole);

	const SkEmblem* GetSkillEmblemByID(UINT32 itemID);
	float GetSkillEmblemEffect(UINT32 skillHash, Role* pRole);
	float GetSkillCDRatio(UINT32 skillHash, Role* pRole);

	bool IsActOnSameSkill(UINT32 type1, UINT32 type2);
	
	const AttrEmblemSingle* GetAttrEmblemSingle(UINT32 itemID, UINT32 slot, UINT32 attrID);

	KKSG::ErrorCode SmeltAttrEmblem(XItem* pItem, UINT32 slot, UINT32& newAttrValue, Role* pRole, std::vector<ItemDesc>& vlist);

	// return smelt stone
	void InitSmeltCountForOldItem(XItem* pItem);
	void ResetAttr(XItem* pItem);

	void FixSmeltCount(XItem* pItem);

	UINT32 GetAttributeEmblemSlotLevelLimit(int level);
	UINT32 GetSkillEmblemSlotLevelLimit(int level);

	KKSG::EmblemSlotType GetSlotTypeByIndex(UINT32 index);

private:
	UINT32 _GetSmeltAttrPos(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax);
	double _GetPercent(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax);

	void _GetCanReplaceEmblemSmeltItem(UINT32 itemID, std::vector<UINT32>& items);

private:
	AttributeEmblem m_oAttrEmblem;
	std::unordered_map<UINT32, AttrEmblem> m_mapAttrEmblem;
	SkillEmblem m_oSkillEmblem;
	std::unordered_map<UINT32, SkEmblem> m_mapSkill;
};

#endif