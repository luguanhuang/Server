#ifndef _DRAGONGUILD_TABLE_MGR_H_
#define _DRAGONGUILD_TABLE_MGR_H_

#include "util/utilsingleton.h"
#include "table/DragonGuildTable.h"
#include "table/DragonGuildLivenessTable.h"
#include "table/DragonGuildAchieveTable.h"
#include "table/DragonGuildTaskTable.h"
#include "table/ShopTable.h"
#include "table/ShopTypeTable.h"
#include "table/DragonGuildConfigTable.h"
#include "table/DragonGuildPermissionTable.h"

#include "define/itemdef.h"

class DragonGuildTableMgr : public Singleton<DragonGuildTableMgr>
{
public:
	DragonGuildTableMgr();
	~DragonGuildTableMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetLevel(UINT32 degress);
	bool HasNewCanTakChest(UINT32 level, UINT32 oldValue, UINT32 newValue);
	DragonGuildLivenessTable::RowData* GetLivenessTable(UINT32 level, UINT32 index);
	std::vector<UINT32> GetTaskChestList(KKSG::DragonGuildTaskType taskType, UINT32 taskID);
	DragonGuildTaskTable::RowData* GetTaskRow(UINT32 taskID);
	DragonGuildAchieveTable::RowData* GetAchiveRow(UINT32 taskID);
	void GetCanTakeChestIndex(UINT32 level, UINT32 liveness, std::vector<UINT32>& index);

	void RandomShop(UINT32 level, std::vector<UINT32>& shopItems);
	void RandomTask(UINT32 worldLevel, std::vector<UINT32>& taskList);
	void GetAchiveList(std::vector<UINT32>& achiveList);
	void GetTaskList(std::vector<UINT32>& achiveList);
	vector<DragonGuildTaskTable::RowData*>& GetSceneTaskList(UINT32 sceneID){ return m_taskScene2id[sceneID]; }
	vector<DragonGuildAchieveTable::RowData*>& GetSceneAcheveList(UINT32 sceneID){ return m_achieveScene2id[sceneID]; }
	UINT32 RandomOne(UINT32 level, UINT32 slot);
	ShopTable::RowData* GetDragonGuildShopItem(UINT32 id);
	std::vector<UINT32>& GetShopRefreshTime() { return m_shopRefreshTime; }
	UINT32 GetCostNum(ShopTable::RowData* pData);

	DragonGuildPermissionTable& GetDragonGuildPermission() { return m_permissionconfig; }
	DragonGuildConfigTable::RowData* GetDragonGuildConfig(UINT32 level);

	DragonGuildTable::RowData* GetDragonGuildData(UINT32 level) { return m_dragonguild.GetBylevel(level); }
	UINT32 GetDragonGuildMaxLevel() { return m_dragonguildconfig.Table.size(); }

private:
	void AddRandomItem(UINT32 level, UINT32 slot, UINT32 rate, UINT32 shopId);
	UINT32 RandomOne(std::map<UINT32, UINT32>& randMap);

private:
	DragonGuildTable m_dragonguild;
	DragonGuildConfigTable m_dragonguildconfig;
	DragonGuildPermissionTable m_permissionconfig;
	DragonGuildTaskTable m_task;
	DragonGuildAchieveTable m_achivement;
	DragonGuildLivenessTable m_liveness;
	ShopTypeTable m_shopType;
	ShopTable m_shop;

	std::vector<UINT32> m_shopRefreshTime;
	std::map<UINT32, UINT32> m_degree2level;	// 亲密度对应等级
	std::map<UINT32, std::map<UINT32, DragonGuildLivenessTable::RowData*>> m_livenessChest; // key1: 等级 key2: 宝箱编号
	std::map<UINT32, std::map<UINT32, UINT32>> m_shopRandMap;							// key: level * 100 + slot key2 概率 value: id
	std::map<UINT32, vector<DragonGuildTaskTable::RowData*>>m_taskScene2id;
	std::map<UINT32, vector<DragonGuildAchieveTable::RowData*>>m_achieveScene2id;
};

#endif