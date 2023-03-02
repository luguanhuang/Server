#include "pch.h"
#include "common/ptcn2g_updatefunctionopen2gs.h"

// generate by ProtoGen at date: 2016/11/21 16:50:14

void PtcN2G_UpdateFunctionOpen2Gs::Process(UINT32 dwConnID)
{
	SSDebug << "update function open: " << m_Data.flag() << END;
	//FunctionOpenMgr::Instance()->SetOpenFlag(m_Data.flag());
}
