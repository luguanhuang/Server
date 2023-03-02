#ifndef __SCENELEISURECONFIG_H__
#define __SCENELEISURECONFIG_H__

#include "table/YorozuyaTable.h"

class SceneLeisureConfig
{
	DECLARE_SINGLETON(SceneLeisureConfig);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetSceneId(UINT32 index, UINT32 exceptsceneid);
	bool IsOpen(UINT32 sceneid);

private:
	YorozuyaTable m_table;
	std::set<UINT32> m_notopen;
};

#endif