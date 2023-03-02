#include "pch.h"
#include "battlewatch/ptcg2m_syncliveinfotoms.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/7/28 17:33:43

void PtcG2M_SyncLiveInfoToMs::Process(UINT32 dwConnID)
{
	//for (size_t i = 0; i < m_Data.deletelist_size(); ++i)
	bool needSort = false;
	UINT32 sceneID = 0;
	if (m_Data.has_sort())
	{
		needSort = m_Data.sort();
	}
	if (m_Data.has_sceneid())
	{
		sceneID = m_Data.sceneid();
	}
	if (m_Data.has_deletelist())
	{
		LiveManager::Instance()->SyncInfoFromGS(NULL, m_Data.deletelist(), 0, needSort);

		LogInfo("remove liveinfo, liveid:%u", m_Data.deletelist());
	}

	//for (size_t i = 0; i < m_Data.updatelist_size(); ++i)
	if (m_Data.has_updatelist())
	{
		LiveManager::Instance()->SyncInfoFromGS(&m_Data.updatelist(), 0, /*sceneID*/ 0, needSort);
	}
}
