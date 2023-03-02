#include "pch.h"
#include "chat/ptcf2m_fmwhitelistreloadntf.h"
#include "chat/fmmgr.h"

// generate by ProtoGen at date: 2017/1/16 14:25:56

void PtcF2M_FMWhiteListReloadNtf::Process(UINT32 dwConnID)
{
	FMMgr::Instance()->ReloadWhiteList(&m_Data);
	//
}
