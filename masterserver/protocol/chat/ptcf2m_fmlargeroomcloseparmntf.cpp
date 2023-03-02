#include "pch.h"
#include "chat/ptcf2m_fmlargeroomcloseparmntf.h"
#include "chat/fmmgr.h"

// generate by ProtoGen at date: 2016/11/30 20:40:43

void PtcF2M_FMLargeRoomCloseParmNtf::Process(UINT32 dwConnID)
{
	FMMgr::Instance()->CloseRoom();
}
