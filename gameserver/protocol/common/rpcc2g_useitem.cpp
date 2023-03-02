#include "pch.h"
#include "common/rpcc2g_useitem.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2015/6/12 10:21:12

RPC_SERVER_IMPLEMETION(RpcC2G_UseItem, UseItemArg, UseItemRes)

void RpcC2G_UseItem::OnCall(const UseItemArg &roArg, UseItemRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	KKSG::ErrorCode dwErrorCode = ERR_SUCCESS;
	std::vector<UINT64> uids;
	for (int i = 0; i < roArg.uids_size(); ++i)
	{
		uids.push_back(roArg.uids(i));
	}
	
	BagUseItemTransition transition(pRole);
	if (roArg.optype() == KKSG::FashionSuitWear || roArg.optype() == KKSG::FashionSuitOff)
	{	
		dwErrorCode = transition.UseItem(uids, roArg.optype());
	}	
	else if (roArg.optype() == KKSG::FashionSuitDisplayWear || roArg.optype() == KKSG::FashionSuitDisplayOff) //　外显套装
	{
		dwErrorCode = transition.DisplaySuitFashion(roArg.suit_id(),roArg.optype());
	}
	else if (roArg.optype() == KKSG::FashionDisplayWear || roArg.optype() == KKSG::FashionDisplayOff) //　外显时装
	{
		dwErrorCode = transition.DisplayFashion(roArg.itemid(),roArg.optype());	
	}
	else if (roArg.optype() == KKSG::ActivationFashion)
	{
		dwErrorCode = transition.ActivationFashion(roArg.uid());	
	}	
	else
	{
		dwErrorCode = transition.UseItem(roArg.uid(), uids, roArg.count(), roArg.optype(), roRes, roArg.petid());
	}
	roRes.set_errorcode(dwErrorCode);
}

void RpcC2G_UseItem::OnDelayReplyRpc(const UseItemArg &roArg, UseItemRes &roRes, const CUserData &roUserData)
{
}
