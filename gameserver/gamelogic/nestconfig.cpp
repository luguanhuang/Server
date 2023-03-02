#include "pch.h"
#include "nestconfig.h"
#include "timeutil.h"
#include "scene/sceneconfig.h"
#include "network/mslink.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "define/globalconfigtodbdef.h"
#include "unit/role.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/stagemgr.h"
#include "foreach.h"

INSTANCE_SINGLETON(NestConfig)

NestConfig::NestConfig()
{
}

NestConfig::~NestConfig()
{

}

bool NestConfig::Init()
{
	return LoadFile();
}

void NestConfig::Uninit()
{

}

bool NestConfig::LoadFile()
{
	if (!m_oNestListTable.LoadFile("table/NestList.txt"))
	{
		LogWarn("Load NestList.txt failed!");
		return false;
	}

	m_starNestSceneIds.clear();
	for (auto it = m_oNestListTable.Table.begin(); it != m_oNestListTable.Table.end(); ++it)
	{
		NestListTable::RowData* pData = *it;
		if (pData->Difficulty == 4)
		{
			m_starNestSceneIds.insert(pData->NestID);
		}
	}

	return true;
}

const NestListTable& NestConfig::GetNestListTable()
{
	return m_oNestListTable;
}

const NestListTable::RowData* NestConfig::GetNestListTableData(UINT32 nestExpID)
{
	return m_oNestListTable.GetByNestID(nestExpID);
}

UINT32 NestConfig::CalcHardestNestExpID(Role* role)
{
	UINT32 hardestType = 0;
	UINT32 hardestDiff = 0;
	UINT32 hardestExpID = 0;
	for (auto iter = m_oNestListTable.Table.begin(); iter != m_oNestListTable.Table.end(); ++iter)
	{
		if ((*iter)->Difficulty == 4)
		{
			continue;
		}
		UINT32 expID = (*iter)->NestID;
		UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(expID);
		UINT32 tempType = (*iter)->Type;
		UINT32 tempDiff = (*iter)->Difficulty;
		if(role->Get<StageMgr>()->IsPassed(sceneID))
		{
			if (tempType > hardestType || (tempType == hardestType && tempDiff > hardestDiff))
			{
				hardestType = (*iter)->Type;
				hardestDiff = (*iter)->Difficulty;
				hardestExpID = (*iter)->NestID;
			}
		}
	}

	return hardestExpID;
}

bool NestConfig::IsStarNestStart(UINT32 sceneID)
{
	auto it = m_starNestSceneIds.find(sceneID);
	return it != m_starNestSceneIds.end();
}

