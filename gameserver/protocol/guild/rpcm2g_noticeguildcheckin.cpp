#include "pch.h"
#include "guild/rpcm2g_noticeguildcheckin.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "gamelogic/bagtransition.h"
#include "findback/findbackrecord.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "gamelogic/reportdatarecord.h"

// generate by ProtoGen at date: 2016/9/18 11:17:49

RPC_SERVER_IMPLEMETION(RpcM2G_NoticeGuildCheckin, NoticeGuildCheckinArg, NoticeGuildCheckinRes)

void RpcM2G_NoticeGuildCheckin::OnCall(const NoticeGuildCheckinArg &roArg, NoticeGuildCheckinRes &roRes)
{

	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	CGuildRecord* record = pRole->Get<CGuildRecord>();
	record->Update();
	if (record->IsCheckin())
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_LIMIT);
		return;
	}

	BagTakeItemTransition taketransition(pRole); 
	taketransition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Checkin);
	if (!taketransition.TakeItem(roArg.conitemid(), roArg.conitemnum()))
	{
		taketransition.RollBack();
		roRes.set_errorcode(ERR_GUILDCHECKIN_MONEY);
		return;
	}
	taketransition.NotifyClient();

	record->SetCheckin(roArg.type());
	pRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::GUILDCHECKIN_BACK, 1, 1);

	BagGiveItemTransition givetransition(pRole); 
	givetransition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Checkin);
	givetransition.GiveItem(roArg.additemid(), roArg.additemnum());
	givetransition.NotifyClient();

	pRole->OnGuildActivity(2);

	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_GuildCheckIn, 0, 1);

	pRole->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_GuildSign);
}

void RpcM2G_NoticeGuildCheckin::OnDelayReplyRpc(const NoticeGuildCheckinArg &roArg, NoticeGuildCheckinRes &roRes, const CUserData &roUserData)
{
}
