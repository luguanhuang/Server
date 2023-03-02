#include "pch.h"
#include "battlewatch/ptcg2m_syncallliveinfotoms.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/11/24 21:20:41

void PtcG2M_SyncAllLiveInfoToMs::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.infos_size(); ++i)
	{
		LiveManager::Instance()->SyncInfoFromGS(&m_Data.infos(i).updatelist(), 0, 0, false);
	}

	LiveManager::Instance()->AllSyncSort();
}
