#include "pch.h"
#include "battlefield/ptck2m_battlefieldreadyscenentf.h"
#include "battlefield/battlefieldmgr.h"

// generate by ProtoGen at date: 2017/9/6 12:55:55

void PtcK2M_BattleFieldReadySceneNtf::Process(UINT32 dwConnID)
{
	BattleFieldMgr::Instance()->SendReadySceneInfo(m_Data);
}
