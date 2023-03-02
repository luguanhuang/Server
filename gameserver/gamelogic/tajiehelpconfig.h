#ifndef __TAJIEHELPCONFIG_H__
#define __TAJIEHELPCONFIG_H__
#include "table/TaJieHelpUrl.h"

class TajieHelpConfig
{
	TajieHelpConfig();
	~TajieHelpConfig();
	DECLARE_SINGLETON(TajieHelpConfig)

	bool Init();
	void Uninit();

	const TaJieHelpUrl::RowData* GetTaJieHelpDataByScene(UINT32 sceneID);

protected:
	TaJieHelpUrl m_oTaJieHelpTable;
};


#endif
