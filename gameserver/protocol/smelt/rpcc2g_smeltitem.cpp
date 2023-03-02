#include "pch.h"
#include "smelt/rpcc2g_smeltitem.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/emblemmgr.h"
#include "gamelogic/randomattrmgr.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2016/9/10 14:34:41

RPC_SERVER_IMPLEMETION(RpcC2G_SmeltItem, SmeltItemArg, SmeltItemRes)

void RpcC2G_SmeltItem::OnCall(const SmeltItemArg &roArg, SmeltItemRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	if (!roArg.has_uid() && !roArg.has_slot())
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	
	Bag* pBag = pRole->Get<Bag>();
	int slot = -1;
	XItem* item = pBag->FindUniqueItemOnBody(roArg.uid(), &slot);
	if (item == NULL)
	{
		LogWarn("smelt item not in body, uid:%llu", roArg.uid());
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	std::vector<ItemDesc> vlist;
	switch (item->type)
	{
	case EQUIP:
		{
			UINT32 result;
			KKSG::ErrorCode errorCode = RandomAttrMgr::Instance()->SmeltAttr(item, roArg.slot(), roArg.isforge(), pRole, result, vlist);
			roRes.set_errorcode(errorCode);
			if(KKSG::ERR_SUCCESS == errorCode)
			{
				pRole->OnSmeltEquip(slot);

				roRes.set_result(result);
				XItem::PrintEquipTLog(pRole, item, TX_Item_Smelt, vlist);
			}
			break;
		}
	case EMBLEM:
		{
			UINT32 result;
			KKSG::ErrorCode errorCode = CEmblemMgr::Instance()->SmeltAttrEmblem(item, roArg.slot(), result, pRole, vlist);
			roRes.set_errorcode(errorCode);
			if(KKSG::ERR_SUCCESS == errorCode)
			{
				pRole->OnSmeltEmblem(slot);

				roRes.set_result(result);
				XItem::PrintEmblemTLog(pRole, item, TX_Emblem_Smelt, roArg.slot(), vlist);
			}
			break;
		}
	default:
		{
			LogWarn("This item:%u, uid:%llu can't smelt", item->type, roArg.uid());
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			break;
		}
	}
}

void RpcC2G_SmeltItem::OnDelayReplyRpc(const SmeltItemArg &roArg, SmeltItemRes &roRes, const CUserData &roUserData)
{
}
