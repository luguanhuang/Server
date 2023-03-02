#ifndef __EFFECTCONFIG_H__
#define __EFFECTCONFIG_H__

#include "table/EffectTable.h"
#include "util/utilsingleton.h"
#include "table/EffectRandomList.h"
#include "table/EffectDesTable.h"
#include "table/EffectTypeTable.h"

typedef EffectTable::RowData EffectConf;
typedef EffectRandomList::RowData EffectRandomConf;
typedef EffectDesTable::RowData EffectDesConf;
typedef EffectTypeTable::RowData EffectTypeConf;


class EffectTableData
{
public:
	EffectTableData();
	~EffectTableData();

	bool HandlerTableData();

protected:
	EffectTable m_oTable;
	std::unordered_map<UINT32, std::vector<EffectConf*>> m_effect2Conf;
	std::map<std::pair<UINT32, UINT32>, EffectConf*> m_effectBuff2Conf;
	std::map<std::pair<UINT32, UINT32>, EffectConf*> m_effectSkill2Conf;

	EffectRandomList m_randomTable;
	std::unordered_map<UINT32, std::unordered_map<UINT32, std::vector<EffectRandomConf*>>> m_mapRandomConf;

	EffectTypeTable m_typeTable;
	EffectDesTable m_desTable;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class EffectConfig : public Singleton<EffectConfig>, public EffectTableData
{
public:
	EffectConfig();
	~EffectConfig();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const EffectConf* GetBuffEffectConf(UINT32 effectID, UINT32 buffID);
	const EffectConf* GetSkillEffectConf(UINT32 effectID, UINT32 skillID);

	const std::vector<EffectConf*>* GetEffectConfByEffectID(UINT32 effectID);

	bool GetEffectData(UINT32 effectID, KKSG::EffectData& data);

	void UpdateEffectData(std::vector<KKSG::EffectData>& vecEffectData);
	void UpdateEffectData(std::vector<std::vector<KKSG::EffectData>*>& vecAllEffectData);

	const std::vector<EffectRandomConf*>* GetRandomConf(UINT32 effectRandomID, UINT32 profID);

	void GetRandomEffectIDs(const std::vector<UINT32>& effectRandomIDs, UINT32 profID, UINT32 num, std::vector<UINT32>& effectIDs);

	const EffectDesConf* GetEffectDesConf(UINT32 effectID) { return m_desTable.GetByEffectID(effectID); }
	const EffectTypeConf* GetEffectTypeConf(UINT32 effectType) { return m_typeTable.GetByEffectType(effectType); }
};



#endif
