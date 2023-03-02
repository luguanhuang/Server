#include "pch.h"
#include "common/ptcm2g_worldlevelntf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/worldlevelmgr.h"

// generate by ProtoGen at date: 2016/11/15 16:49:51

void PtcM2G_WorldLevelNtf::Process(UINT32 dwConnID)
{
	WorldLevelMgr::Instance()->OnGetMsWorldLevel(m_Data.worldlevel());
}
