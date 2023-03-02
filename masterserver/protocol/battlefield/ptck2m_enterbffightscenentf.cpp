#include "pch.h"
#include "battlefield/ptck2m_enterbffightscenentf.h"
#include "battlefield/battlefieldmgr.h"

// generate by ProtoGen at date: 2017/8/31 16:51:46

void PtcK2M_EnterBFFightSceneNtf::Process(UINT32 dwConnID)
{
	BattleFieldMgr::Instance()->EnterScene(m_Data);
}
