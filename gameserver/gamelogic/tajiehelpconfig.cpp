#include "pch.h"
#include "tajiehelpconfig.h"

INSTANCE_SINGLETON(TajieHelpConfig)

TajieHelpConfig::TajieHelpConfig()
{
}

TajieHelpConfig::~TajieHelpConfig()
{
}

bool TajieHelpConfig::Init()
{
	if (!m_oTaJieHelpTable.LoadFile("table/TaJieHelpUrl.txt"))
	{
		LogWarn("Load TaJieHelpUrl.txt failed!");
		return false;
	}

	return true;
}

void TajieHelpConfig::Uninit()
{
}

const TaJieHelpUrl::RowData* TajieHelpConfig::GetTaJieHelpDataByScene(UINT32 sceneID)
{
	return m_oTaJieHelpTable.GetBySceneId(sceneID);
}
