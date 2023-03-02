#ifndef __MOBABATTLEMGR_H__
#define __MOBABATTLEMGR_H__

#include "util/utilsingleton.h"
#include "table/MobaLevel.h"
#include "table/MobaHero.h"
#include "table/MobaKillExp.h"
#include "table/MobaMobTable.h"
#include "table/MobaMiniMap.h"
#include "scene/scenemobadef.h"

typedef MobaLevel::RowData MobaLevelConf;
typedef MobaHero::RowData MobaHeroConf;
typedef MobaKillExp::RowData MobaKillExpConf;
typedef MobaMiniMap::RowData MobaMiniMapConf;

enum Moba_Tag
{
	Moba_Tag_Home		=	1 << 2,
	Moba_Tag_Tower		=	1 << 15,		///> 原本是3，现改为15

	Moba_Tag_TypeAll	=	0xFFF7,		///> 只包含类型信息，原本为FFF，现改为FFF7

	Moba_Tag_TowerLike	=	1 << 3,	///> 有塔的行为的东西，原本应为15，现改为3	

	Moba_Tag_All		=	0xFFFF,		///> 包含所有信息
};


struct MobaMobInfo
{
	UINT32 group;
	std::map<UINT32, UINT32> mobIDs;
};

class MobaTableData
{
public:
	MobaTableData();
	~MobaTableData();

	bool HandlerTableData();

protected:
	UINT32 m_maxLevel;
	MobaLevel m_levelTable;
	std::map<UINT32, MobaLevelConf*> m_levelMap;

	MobaHero m_heroTable;
	std::map<UINT32, std::map<UINT32, MobaHeroConf*>> m_heroMap;

	MobaKillExp m_killExpTable;
	std::map<std::pair<UINT32, UINT32>, MobaKillExpConf*> m_killExpMap;

	UINT32 m_mobMaxLevel;
	MobaMobTable m_mobtable;
	std::map<UINT32, MobaMobInfo> m_group2mobInfo;

	MobaMiniMap m_miniMapTable;
};


class MobaBattleMgr : public Singleton<MobaBattleMgr>, public MobaTableData
{
public:
	MobaBattleMgr();
	~MobaBattleMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetMobaMaxLevel();

	const std::map<UINT32, MobaLevelConf*>& GetMobaLevelMap() { return m_levelMap; }
	const MobaLevelConf* GetMobaLevelConf(UINT32 level);

	const MobaHeroConf* GetMobaHeroConf(UINT32 heroid, UINT32 level);
	const std::map<UINT32, MobaHeroConf*>* GetOneMobaHeroConf(UINT32 heroid);

	const MobaKillExpConf* GetMobaKillExpConf(UINT32 assistNum, UINT32 rangeNum);

	inline UINT32 GetMobaMobMaxLevel() { return m_mobMaxLevel; }
	UINT32 GetMobID(UINT32 group, UINT32 level, UINT32 type);

	const MobaMiniMapConf* GetMobaMiniMapConf(UINT32 posIndex);
};




#endif