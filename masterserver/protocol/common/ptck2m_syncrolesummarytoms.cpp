#include "pch.h"
#include "common/ptck2m_syncrolesummarytoms.h"
#include "role/crossrolesummarymgr.h"

// generate by ProtoGen at date: 2017/2/2 21:26:12

void PtcK2M_SyncRoleSummaryToMS::Process(UINT32 dwConnID)
{
	for (int i = 0; i < m_Data.rsus_size(); ++i)
	{
		CrossRoleSummaryMgr::Instance()->UpdateRoleSummary(m_Data.rsus(i));
	}
}
