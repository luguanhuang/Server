#include "pch.h"
#include "misc/ptcm2g_controlfunctionntf.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2017/1/18 21:06:34

void PtcM2G_ControlFunctionNtf::Process(UINT32 dwConnID)
{
	OpenSystemMgr::Instance()->IdipOpSystem(m_Data.systemid(), m_Data.isopen());
}
