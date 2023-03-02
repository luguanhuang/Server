#include "pch.h"
#include "wedding/ptcg2m_updateweddingstateg2m.h"
#include "marriage/weddingmgr.h"

// generate by ProtoGen at date: 2017/7/27 20:52:01

void PtcG2M_UpdateWeddingStateG2M::Process(UINT32 dwConnID)
{
	Wedding* pWed =  WeddingMgr::Instance()->GetWedding(m_Data.weddingid());
	if (pWed == NULL)
	{
		LogError("not found wedding[%llu]", m_Data.weddingid());
		return;
	}
	pWed->SetState(m_Data.state(), m_Data.statestarttime());
	if (m_Data.is_end())
	{
		pWed->OnEnd();
		WeddingMgr::Instance()->RemoveWedding(m_Data.weddingid());
	}
}
