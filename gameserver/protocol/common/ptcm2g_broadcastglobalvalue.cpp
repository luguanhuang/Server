#include "pch.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/loginactivity.h"
#include "common/ptcm2g_broadcastglobalvalue.h"
#include "define/globalconfigtodbdef.h"
#include "gamelogic/competedragonconfig.h"

// generate by ProtoGen at date: 2017/4/23 22:30:02

void PtcM2G_BroadCastGlobalValue::Process(UINT32 dwConnID)
{
	if (m_Data.id() == eDragonGlobal)
	{
		DragonConfig::Instance()->FillGlobalInfo(m_Data.value());
	}
	else if (m_Data.id() == eLoginActivity)
	{
		KKSG::GlobalLoginActivityInfo info;
		info.ParseFromString(m_Data.value());
		LoginActivityMgr::Instance()->SetWolrdLevel(info.worldlv());
	}
	else if (m_Data.id() == eCompeteDragonInfo)
	{
		CompeteDragonConfig::Instance()->FillGlobalInfo(m_Data.value());
	}
}
