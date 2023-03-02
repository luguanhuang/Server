#include "pch.h"
#include "common/ptcn2m_updatefunctionopen2ms.h"

// generate by ProtoGen at date: 2016/11/21 16:49:45

void PtcN2M_UpdateFunctionOpen2Ms::Process(UINT32 dwConnID)
{
	SSDebug << "update function open: " << m_Data.flag() << END;
	//FunctionOpenMgr::Instance()->SetOpenFlag(m_Data.flag());
}
