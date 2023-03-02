#include "pch.h"
#include "guild/ptcm2g_synguildbossdoend.h"
#include "guild/guildboss.h"

// generate by ProtoGen at date: 2017/1/14 19:34:52

void PtcM2G_SynGuildBossDoEnd::Process(UINT32 dwConnID)
{
	GuildBossMgr::Instance()->DoGiveReward();  // 时间结束发放参与奖
	GuildBossMgr::Instance()->ClearAfterEnd(); // 清理
	SSInfo << "guild boss PtcM2G_SynGuildBossDoEnd !" << END;
}
