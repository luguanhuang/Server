#include "pch.h"
#include "marriage/rpcm2g_givemarriagelvrerewardm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "config/itemdropconfig.h"
#include "tablemgr/marriagetablemgr.h"

// generate by ProtoGen at date: 2017/7/28 23:41:27

RPC_SERVER_IMPLEMETION(RpcM2G_GiveMarriageLvRerewardM2G, GiveMarriageLvRewardM2GArg, GiveMarriageLvRewardRes)

void RpcM2G_GiveMarriageLvRerewardM2G::OnCall(const GiveMarriageLvRewardM2GArg &roArg, GiveMarriageLvRewardRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	auto data = MarriageTableMgr::Instance()->GetLivenessData(roArg.index());
	if (data == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	// 发送奖励
	std::vector<ItemDesc> vecItem;
	for (auto i = 0;  i < data->dropid.size(); ++i)
	{
		UINT32 dropId = data->dropid[i];
		const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropId, pRole->GetLevel());
		if (NULL == pDrop)
		{
			continue;
		}
		ItemDesc stItem(*pDrop);
		vecItem.push_back(stItem);
	}

	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Marriage, ItemFlow_MarriageLiveness);
	for (auto i = vecItem.begin(); i != vecItem.end(); ++i)
	{
		stTransition.GiveItem(*i);
		roRes.add_itemid(i->itemID);
		roRes.add_itemcount(i->itemCount);
	}
	stTransition.NotifyClient();
}

void RpcM2G_GiveMarriageLvRerewardM2G::OnDelayReplyRpc(const GiveMarriageLvRewardM2GArg &roArg, GiveMarriageLvRewardRes &roRes, const CUserData &roUserData)
{
}
