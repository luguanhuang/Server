#ifndef __FUSEMGR_H__
#define __FUSEMGR_H__


#include "util/utilsingleton.h"
#include "itemconfig.h"
#include "table/EquipFusionTable.h"
#include "table/EquipFusionExpTable.h"

class Role;
struct XItem;

typedef EquipFusionTable::RowData EquipFusionConf;
typedef EquipFusionExpTable::RowData EquipFusionExpConf;

class FuseMgr : public Singleton<FuseMgr>
{
public:
	FuseMgr();
	~FuseMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	KKSG::ErrorCode FuseEquipAddExp(Role* pRole, XItem* pItem, UINT32 itemID);
	KKSG::ErrorCode FuseEquipBreak(Role* pRole, XItem* pItem, UINT32 itemID);

	const EquipFusionConf* GetFusionConf(UINT32 profession, UINT32 slot, UINT32 equipType, UINT32 breakNum);

	UINT64 MakeKey(UINT32 profession, UINT32 slot, UINT32 equipType, UINT32 breakNum);

private:
	EquipFusionTable m_oTable;
	EquipFusionExpTable m_oExpTable;
	std::unordered_map<UINT64, EquipFusionConf*> m_mapFusionConf;
};



#endif