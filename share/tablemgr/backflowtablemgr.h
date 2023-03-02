#ifndef _BACKFLOW_TABLE_MGR_H_
#define _BACKFLOW_TABLE_MGR_H_

#include "util/utilsingleton.h"
#include "table/BackFlowServerConfig.h"
#include "table/BackFlowEquip.h"
#include "table/BackFlowEmblem.h"
#include "table/BackFlowSprite.h"

typedef BackFlowEquip::RowData BackFlowEquipConf;
typedef BackFlowEmblem::RowData BackFlowEmblemConf;
typedef BackFlowSprite::RowData BackFlowSpriteConf;

class BackFlowTableData
{
public:
	BackFlowTableData() {}
	~BackFlowTableData() {}

	bool HandlerTableData();

protected:
	BackFlowServerConfig m_serverConfig;
	BackFlowEquip m_equipTable;
	std::map<std::pair<UINT32, UINT32>, std::vector<BackFlowEquipConf*>> m_equipMap;
	BackFlowEmblem m_emblemTable;
	BackFlowSprite m_spriteTable;
};

class BackFlowTableMgr : public Singleton<BackFlowTableMgr>, public BackFlowTableData
{
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	BackFlowServerConfig::RowData* GetServerConfig(UINT32 profession, UINT32 level);
	BackFlowServerConfig::RowData* GetServerConfigFloor(UINT32 profession, UINT32 level);
	BackFlowServerConfig::RowData* GetServerConfigByLevel(UINT32 level);

	const std::vector<BackFlowEquipConf*>* GetEquipConf(UINT32 level, UINT32 profession);
	const BackFlowEmblemConf* GetEmblemConf(UINT32 level, UINT32 profession);
	const BackFlowSpriteConf* GetSpriteConf(UINT32 level, UINT32 profession);
};

#endif