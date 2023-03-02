#include "pch.h"
#include "bigmelee/ptck2m_bmcrossntfdatantf.h"
#include "mayhem/mayhemmgr.h"
// generate by ProtoGen at date: 2017/6/10 14:15:56

void PtcK2M_BMCrossNtfDataNtf::Process(UINT32 dwConnID)
{
	MayhemMsMgr::Instance()->SetReadyScene(m_Data);
}
