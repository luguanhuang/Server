#include "pch.h"
#include "gslink.h"
#include "ranklist/ptcm2d_saveranklistchangetodb.h"

// generate by ProtoGen at date: 2016/8/21 19:17:41

void PtcM2D_SaveRankListChangeToDB::Process(UINT32 dwConnID)
{
	SSInfo<<"save changed ranklist, rank type:"<<m_Data.ranktype()<<END;

	//CRankListDataSourceMgr::Instance()->UpdateChange(m_Data.ranktype(), m_Data.mutable_ranklistchange());
}
