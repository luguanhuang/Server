#include "pch.h"
#include "task/rpcm2g_querydonateinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/dailytaskrecord.h"
#include "table/globalconfig.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/11/16 19:01:33

RPC_SERVER_IMPLEMETION(RpcM2G_QueryDonateInfo, QueryDonateInfoArg, QueryDonateInfoRes)

void RpcM2G_QueryDonateInfo::OnCall(const QueryDonateInfoArg &roArg, QueryDonateInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		return;
	}

	TaskRecord* record = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = record->GetDailyRecord();
	roRes.set_todaydonatecount(dailyRecord.GetTodayDonateCount());
	roRes.set_totaldonatecount(dailyRecord.GetTotalDonateCount());
}

void RpcM2G_QueryDonateInfo::OnDelayReplyRpc(const QueryDonateInfoArg &roArg, QueryDonateInfoRes &roRes, const CUserData &roUserData)
{
}
