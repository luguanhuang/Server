#ifndef __RANDOMATTRMGR_H__
#define __RANDOMATTRMGR_H__

#include "util/utilsingleton.h"
#include "table/RandomAttributes.h"
#include "table/ForgeAttributes.h"
#include "table/ForgeTransAttr.h"
#include "define/itemdef.h"

typedef RandomAttributes::RowData RandomAttrConf;
typedef ForgeAttributes::RowData ForgeAttrConf;
typedef ForgeTransAttr::RowData ForgeTransAttrConf;


struct RandAttrSingle
{
	UINT32 prob;
	Sequence<UINT32, 2> initAttr;
	Sequence<UINT32, 2> attrRange;
	UINT32 smeltAllProb[3];
	std::map<UINT32, std::pair<int, UINT32>> smeltAttr[3];  ///> pair, first--> ÆðÊ¼£¬ second--> ·¶Î§
	UINT32 smeltExpectation;
};

struct RandAttrAll
{
	RandAttrAll() { allprob = 0; }
	UINT32 allprob;
	std::unordered_map<UINT32, RandAttrSingle> singles;	///> key : attribute
	std::pair<UINT32, UINT32> GetInitRandomAttr(RandAttrSingle** nowSingle = NULL);
	std::pair<UINT32, UINT32> GetForgeRandomAttr(RandAttrSingle** nowSingle, std::unordered_set<UINT32>& haveAttrs);
};


struct RandomAttr;
struct XItem;
class Role;
class RandomAttrMgr : public Singleton<RandomAttrMgr>
{
public:
	RandomAttrMgr();
	~RandomAttrMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const RandAttrSingle* GetRandomAttrSingle(UINT32 itemID, UINT32 slot, UINT32 attrID);
	const RandAttrSingle* GetForgeAttrSingle(UINT32 itemID, UINT32 slot, UINT32 attrID);

	void InitRandomAttr(XItem* pItem);

	KKSG::ErrorCode SmeltAttr(XItem* pItem, UINT32 slot, bool isForge, Role* pRole, UINT32& newAttrValue, std::vector<ItemDesc>& vlist);	

	KKSG::ErrorCode ForgeEquip(XItem* pItem, bool isUsedStone, Role* pRole, std::vector<ItemDesc>& vlist);

	KKSG::ErrorCode ForgeReplace(XItem* pItem, Role* pRole);

	KKSG::ErrorCode ForgeRetain(XItem* pItem, Role* pRole);

	// for change profession
	void RandomAttrTransfer(XItem* pItem, UINT32 preItemID);
	void ForgeEquipTransfer(Role* pRole, XItem* pItem, UINT32 preItemID, UINT32 from, UINT32 to);

	// return smelt stone
	void InitSmeltCountForOldItem(XItem* pItem);
	void ResetRandomAttrAndForgeAttr(XItem* pItem);

	void FixSmeltCount(XItem* pItem);
	
private:
	double _GetPercent(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax);
	UINT32 _GetSmeltAttrPos(UINT32 attrValue, UINT32 attrMin, UINT32 attrMax);

	void _GetCanReplaceEquipSmeltItem(UINT32 itemID, std::vector<UINT32>& items);	

private:
	RandomAttributes m_oRandTable;
	ForgeAttributes m_oForgeTable;
	
	std::unordered_map<UINT32, std::map<UINT32, RandAttrAll>> m_mapRandAttr;
	std::unordered_map<UINT32, std::map<UINT32, RandAttrAll>> m_mapForgeAttr;

	ForgeTransAttr m_oForgeTransAttr;
};



#endif