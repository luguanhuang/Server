#include "pch.h"
#include "emblem/rpcc2g_buyextraskillemblemslot.h"
#include "globalconfig.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"


// generate by ProtoGen at date: 2017/5/9 15:53:29

RPC_SERVER_IMPLEMETION(RpcC2G_BuyExtraSkillEmblemSlot, BuyExtraSkillEmblemSlotArg, BuyExtraSkillEmblemSlotRes)

void RpcC2G_BuyExtraSkillEmblemSlot::OnCall(const BuyExtraSkillEmblemSlotArg &roArg, BuyExtraSkillEmblemSlotRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Bag* pBag = pRole->Get<Bag>();
	UINT32 curNum = pBag->GetExtraSkillEmblemSlot();
	roRes.set_extraskillemblemslot(curNum);
	if (curNum >= GetGlobalConfig().ExtraSkillEmblemSlotLevelLimit.size()
	 || curNum >= GetGlobalConfig().ExtraSkillEmblemSlotDragonCoin.size())
	{
		roRes.set_result(KKSG::ERR_EXTRA_EMBLEM_SLOT_FULL);
		return;
	}
	if (pRole->GetLevel() < GetGlobalConfig().ExtraSkillEmblemSlotLevelLimit[curNum])
	{
		roRes.set_result(KKSG::ERR_EXTRA_EMBLEM_SLOT_LEVEL);
		return;
	}
	BagTakeItemTransition trans(pRole);
	trans.SetReason(ItemFlow_Emblem, ItemFlow_BuyExtraEmblemSlot);
	if (!trans.TakeItem(DRAGON_COIN, GetGlobalConfig().ExtraSkillEmblemSlotDragonCoin[curNum]))
	{
		trans.RollBack();
		roRes.set_result(KKSG::ERR_DRAGONCOIN_LIMIT);
		return;
	}
	trans.NotifyClient();

	pBag->AddExtraSkillEmblemSlot();

	roRes.set_extraskillemblemslot(curNum + 1);
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_BuyExtraSkillEmblemSlot::OnDelayReplyRpc(const BuyExtraSkillEmblemSlotArg &roArg, BuyExtraSkillEmblemSlotRes &roRes, const CUserData &roUserData)
{
}
