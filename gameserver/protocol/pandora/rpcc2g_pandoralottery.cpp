#include "pch.h"
#include "pandora/rpcc2g_pandoralottery.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "table/ProfessionMgr.h"
#include "gamelogic/pandoraconfig.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/popwindows.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
// generate by ProtoGen at date: 2016/12/17 12:18:05

RPC_SERVER_IMPLEMETION(RpcC2G_PandoraLottery, PandoraLotteryArg, PandoraLotteryRes)

void RpcC2G_PandoraLottery::OnCall(const PandoraLotteryArg &roArg, PandoraLotteryRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT32 profID = pRole->GetProfession();
	profID = ProfessionMgr::Instance()->GetBasicProfession(profID);
	const PandoraHeartConf* conf = PandoraConfig::Instance()->GetPandoraHeartConf(roArg.pandoraid(), profID);
	if (conf == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	UINT32 cnt = roArg.isonelottery() == true ? 1 : 10;
	UINT32 pandoraCount = pRole->Get<Bag>()->CountItemInBag(roArg.pandoraid());
	if (cnt > pandoraCount) cnt = pandoraCount;
	if (cnt == 0)
	{
		roRes.set_errorcode(KKSG::ERR_PANDORA_LACKOF_HEART);
		return;
	}

	UINT32 fireCount = pRole->Get<Bag>()->CountItemInBag(conf->FireID);
	if (cnt > fireCount) cnt = fireCount;
	if (cnt == 0)
	{
		roRes.set_errorcode(KKSG::ERR_PANDORA_LACKOF_FIRE);
		return;
	}
	BagTakeItemTransition takeTrans(pRole);
	takeTrans.SetReason(ItemFlow_UseItem, ItemFlow_UsePandora);
	if (!takeTrans.TakeItem(roArg.pandoraid(), cnt))
	{
		takeTrans.RollBack();
		roRes.set_errorcode(KKSG::ERR_PANDORA_LACKOF_HEART);
		return;
	}
	if (!takeTrans.TakeItem(conf->FireID, cnt))
	{
		takeTrans.RollBack();
		roRes.set_errorcode(KKSG::ERR_PANDORA_LACKOF_FIRE);
		return;
	}
	takeTrans.NotifyClient();

	std::vector<const ItemDesc*> giveItem;
	pRole->Get<LotteryRecord>()->GetPandoraDropItem(conf->PandoraID, cnt, giveItem);

	BagGiveItemTransition giveTrans(pRole);
	giveTrans.SetReason(ItemFlow_UseItem, ItemFlow_UsePandora);
	for (auto i = conf->Reward.begin(); i != conf->Reward.end(); i++)
	{
		giveTrans.GiveItem((*i)[0], (*i)[1] * cnt);
	}
	for (UINT32 i = 0; i < giveItem.size(); i++)
	{
		giveTrans.GiveItem(*giveItem[i]);
		giveItem[i]->ToKKSG(roRes.add_items());
		const PandoraHeartRewardConf* conf = PandoraConfig::Instance()->GetRewardConf(roArg.pandoraid(), *giveItem[i]);
		if (conf == NULL)
		{
			continue;
		}
		if (conf->notice)
		{
			NoticeMgr::Instance()->PandoraNotice(pRole, *giveItem[i], conf->quality);
		}
		if (conf->praise)
		{
			PopWindows::Instance()->CheckComment(pRole, KKSG::COMMENT_PANDORA,giveItem[i]->itemID);
		}
	}
	giveTrans.NotifyClient();
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_Pandora, 0, cnt);
}

void RpcC2G_PandoraLottery::OnDelayReplyRpc(const PandoraLotteryArg &roArg, PandoraLotteryRes &roRes, const CUserData &roUserData)
{
}
