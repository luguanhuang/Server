#include "pch.h"
#include "network/gslink.h"
#include "role/rolemanager.h"
#include "friend/frienddef.h"
#include "friend/friend.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_friendgiftopnew.h"

#include "define/tlogenum.h"

#include "role/ptcm2g_roleeventsyncgs.h"
#include "network/gslink.h"
#include "common/rpcm2g_msgiveitem.h"

// generate by ProtoGen at date: 2016/7/28 23:50:17

RPC_SERVER_IMPLEMETION(RpcC2M_FriendGiftOpNew, FriendGiftOpArg, FriendGiftOpRes)

#define GIFT_ITEM 160

void RpcC2M_FriendGiftOpNew::OnCall(const FriendGiftOpArg &roArg, FriendGiftOpRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	FriendOp op(role->GetID());
	CFriendRecord* record = op.GetRecord();
	record->Update();
	if (roArg.op() == KKSG::Friend_SendGift)
	{
		for (int i = 0; i < roArg.roleid_size(); ++i)
		{
			if (record->IsSendLimit())
			{
				roRes.set_errorcode(ERR_FRIEND_SENDLIMIT);
				return;
			}
			UINT64 otherid = roArg.roleid(i);
			// check
			op.SetOther(otherid);
			if (op.GetDayDegree(otherid) >= MAX_GIFTDEGREE)
			{
				// self && other notify
				if (!record->AddSend(otherid))
				{
					roRes.set_errorcode(ERR_FRIEND_HASSEND);
					return;
				}
				if(op.SetSend(otherid, KKSG::FriendGift_Sended))
				{
					PtcM2G_RoleEventSyncGS oPtc;
					oPtc.m_Data.set_roleid(role->GetID());
					oPtc.m_Data.set_type(KKSG::OnSendFriendGift);
					GSLink::Instance()->SendToLine(role->GetGsLine(), oPtc);
				}
			}
		}
		roRes.set_errorcode(ERR_SUCCESS);
		return;	
	}
	else if (roArg.op() == KKSG::Friend_TakeGift)
	{
		for (int i = 0; i < roArg.roleid_size(); ++i)
		{
			if (record->IsReceiveLimit())
			{
				roRes.set_errorcode(ERR_FRIEND_TAKENLIMIT);
				return;
			}
			UINT64 otherid = roArg.roleid(i);
			// check
			if (op.GetReceive(otherid) == KKSG::FriendGift_Received)
			{
				// take
				op.SetReceive(otherid, KKSG::FriendGift_ReceiveTaken);

				RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
				rpc->m_oArg.set_roleid(role->GetID());
				rpc->m_oArg.set_reason(ItemFlow_Friend);
				rpc->m_oArg.set_subreason(ItemFlow_FriendGift);
				KKSG::ItemBrief* item = rpc->m_oArg.add_items();
				item->set_itemid(GIFT_ITEM);
				item->set_itemcount(1);
				GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

				record->AddReceive();
			}
		}
		roRes.set_errorcode(ERR_SUCCESS);
		return;
	}
	else
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
}

void RpcC2M_FriendGiftOpNew::OnDelayReplyRpc(const FriendGiftOpArg &roArg, FriendGiftOpRes &roRes, const CUserData &roUserData)
{
}
