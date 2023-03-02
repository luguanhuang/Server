#include "pch.h"
#include "battlefield/ptck2m_battlefailinfomsntf.h"
#include "battlefield/battlefieldmgr.h"

// generate by ProtoGen at date: 2017/9/12 15:09:36

void PtcK2M_BattleFailInfoMsNtf::Process(UINT32 dwConnID)
{
	BattleFieldMgr::Instance()->SendMatchFail(m_Data);
}
