#ifndef __DOODADITEMSKILLCONFIG_H__
#define __DOODADITEMSKILLCONFIG_H__

#include "util/utilsingleton.h"
#include "table/DoodadItemSkillTable.h"

class CDoodadItemSkillConfig : public Singleton<CDoodadItemSkillConfig>
{
public:
	CDoodadItemSkillConfig();
	~CDoodadItemSkillConfig();

	bool Init();
	void Uninit();
	bool LoadFile();
	bool CheckConfig();
	void ClearConfig();

	UINT32 GetAllSkillCount(int sceneType);	
	UINT32 GetItemOverlapCount(UINT32 itemID, int sceneType);

	bool CanHaveSameItem(int sceneType);
	bool IsUseNotify(int sceneType);

private:
	DoodadItemSkillTable::RowData* GetConf(int sceneType, UINT32 itemID);

private:
	DoodadItemSkillTable m_table;
	
};

#endif