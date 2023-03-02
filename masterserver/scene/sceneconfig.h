#ifndef _H_SCENECONFIG_H__
#define _H_SCENECONFIG_H__

#include "table/SceneTable.h"

typedef SceneTable::RowData SceneConf;

class CSceneConfig
{
	CSceneConfig();
	~CSceneConfig();
	DECLARE_SINGLETON(CSceneConfig)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const SceneConf* GetSceneConf(UINT32 dwTemplateID);

private:
	SceneTable m_oTable;
};

#endif // __SCENECONFIG_H__