#include "pch.h"
#include "team/ptcg2m_tarjainfontf.h"
#include "team/tarjateammgr.h"

// generate by ProtoGen at date: 2017/5/16 13:51:35

void PtcG2M_TarjaInfoNtf::Process(UINT32 dwConnID)
{
	TarjaTeamMgr::Instance()->SetTime(m_Data.role(),m_Data.time());
}
