#include "pch.h"
#include "pvp/ptck2m_updateworldmatchnum.h"
#include "pvp/pvpmgr.h"

// generate by ProtoGen at date: 2016/11/15 19:09:21

void PtcK2M_UpdateWorldMatchNum::Process(UINT32 dwConnID)
{
	PvpMgr::Instance()->SetWorldMatchNum(m_Data.num());
}
