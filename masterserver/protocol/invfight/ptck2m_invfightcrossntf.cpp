#include "pch.h"
#include "invfight/ptck2m_invfightcrossntf.h"
#include "table/globalconfig.h"
#include "invfight/invfightcrossmgr.h"

// generate by ProtoGen at date: 2017/4/15 15:32:54

void PtcK2M_InvFightCrossNtf::Process(UINT32 dwConnID)
{
	InvFightCrossMgr::Instance()->EnterBattle(m_Data.sceneid(), m_Data.roleid(), m_Data.gsline(), GetGlobalConfig().InvFightMapID);
}
