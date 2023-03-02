#include "pch.h"
#include "network/gslink.h"
#include "role/rolesummarymgr.h"
#include "role/ptcg2m_updaterolesummary.h"
#include "dragonguild/dragonguildmgr.h"

// generate by ProtoGen at date: 2016/7/18 21:58:49

void PtcG2M_UpdateRoleSummary::Process(UINT32 dwConnID)
{
	if (m_Data.datas_size() != m_Data.masks_size())
	{
		SSWarn << "data size != mask size" << END;
		return;
	}

	for (int i = 0; i < m_Data.datas_size(); ++i)
	{
		CRoleSummaryMgr::Instance()->InsertRoleSummary(*m_Data.mutable_datas(i), m_Data.masks(i), m_Data.timestamp(i), false);
		CRoleSummaryMgr::Instance()->SetChanged(m_Data.datas(i).roleid());
	}

	DragonGuildMgr::Instance()->SortByPPT();
}
