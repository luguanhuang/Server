#include "pch.h"
#include "common/ptcf2m_fmmasterinfontf.h"
#include "network/fmlink.h"

// generate by ProtoGen at date: 2017/3/11 11:58:36

void PtcF2M_FmMasterInfoNtf::Process(UINT32 dwConnID)
{
	FMLink::Instance()->SetMasterFmConnId(dwConnID);
	LogInfo("master world  connId[%u]",  dwConnID);
}
