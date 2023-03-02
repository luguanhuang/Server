#ifndef __ENHANCEEQUIPMGR_H__
#define __ENHANCEEQUIPMGR_H__

#include "item.h"
#include "table/EnhanceEquipTable.h"
#include "table/EnhanceTransferTable.h"
#include "table/EnhanceTable.h"
#include "table/EnhanceMaster.h"

typedef EnhanceMaster::RowData EnhanceMsConf;

struct EnhanceConf
{
	std::vector<Sequence<UINT32, 2>> needItem;
	UINT32 successRate;
	UINT32 upRate;
	UINT32 isNeedBreak;
	std::map<UINT32, vector<Sequence<UINT32, 2>>> enhanceAttr;
	bool addAttr(UINT32 prof, std::vector<Sequence<UINT32,2>>& attr)
	{
		if(enhanceAttr.find(prof) != enhanceAttr.end())
		{
			return false;
		}
		enhanceAttr.insert(std::make_pair(prof, attr));
		return true;
	}
};

struct EnhanceTransRes
{
	UINT32 destlevel;
	std::vector<ItemDesc> items;

	EnhanceTransRes()
	{
		destlevel = 0;
	}
};

class Role;
class EnhanceEquipMgr
{
	DECLARE_SINGLETON(EnhanceEquipMgr);
public:
	bool Init();
	void Uninit();

	void ChangeAttr(Role* role, UINT32 destlevel, XItem* item);

	void DebugLevel(Role* role, XItem* item, UINT32 level);

	const EnhanceConf* GetEnhanceConf(UINT32 equipPos, UINT32 enhanceLevel);
	const std::vector<Sequence<UINT32, 2>>* GetEnhanceAttr(UINT32 equipPos, UINT32 enhanceLevel, UINT32 profession);

	UINT32 DoTransfer(Role* role, XItem* origin, XItem* dest);

	bool UseEnhanceItem(Role* role, UINT32 equipPos, UINT32 enhanceLvl, std::vector<std::pair<UINT32, UINT32>>& combinVec, std::vector<ItemDesc>& vlist);
	KKSG::ErrorCode EnhanceEquip(Role* pRole, XItem* pItem, UINT32 slot, std::vector<ItemDesc>& vlist);
	bool GetResult(UINT32 equipPos, UINT32 enhanceLvl, UINT32 losTime);
	void CheckEnhanceMaster(Role* role, UINT32 enhanceLvl);
	void ApplySuitEffect(CombatAttribute* combatAttr, Role* pRole);

	bool CanExchange(UINT32 itemId);
	void GetExItemRate(UINT32 itemId, UINT32& exhId, UINT32& rat);

	void OnRoleChangeProfession(Role* pRole, bool isNotify = true);	

	void GMEnhanceAllEquipOnBody(Role* pRole);	

private:
	inline UINT32 _GetEnhanceKey(UINT32 equipPos, UINT32 enhanceLevel);
	inline UINT32 _GetEnhanceMsKey(UINT32 prof, UINT32 enhanceLevel);	

private:
	EnhanceTable m_oTable;	
	EnhanceMaster m_enhanceMaster;

	std::map<UINT32, EnhanceConf> m_enhance;
	std::map<UINT32, EnhanceMsConf*> m_enhanceMs;
};

#endif
