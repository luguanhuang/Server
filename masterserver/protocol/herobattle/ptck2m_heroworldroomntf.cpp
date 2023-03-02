#include "pch.h"
#include "herobattle/ptck2m_heroworldroomntf.h"
#include "herobattle/herobattlemgr.h"

// generate by ProtoGen at date: 2017/2/4 23:04:27

void PtcK2M_HeroWorldRoomNtf::Process(UINT32 dwConnID)
{
	HeroBattleMgr::Instance()->MatchSucceedFromWorld(m_Data);
}
