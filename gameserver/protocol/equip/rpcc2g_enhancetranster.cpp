#include "pch.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "unit/rolemanager.h"
#include "gamelogic/enhanceequipmgr.h"
#include "gamelogic/bagtransition.h"
#include "equip/rpcc2g_enhancetranster.h"
#include "table/globalconfig.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/6/27 0:04:58

RPC_SERVER_IMPLEMETION(RpcC2G_EnhanceTranster, EnhanceTransterArg, EnhanceTransterRes)

float GetRate(int pos)
{
	for (auto i =  GetGlobalConfig().EnhanceTransRate.begin(); i !=  GetGlobalConfig().EnhanceTransRate.end(); ++i)
	{
		if (pos == int(i->seq[0]))
		{
			return i->seq[1];
		}
	}
	return 1.0f;
}

void RpcC2G_EnhanceTranster::OnCall(const EnhanceTransterArg &roArg, EnhanceTransterRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	UINT64 originid = roArg.originuid();
	UINT64 destid = roArg.destuid();
	XItem* origin = role->Get<Bag>()->FindUniqueItem(originid);
	XItem* dest = role->Get<Bag>()->FindUniqueItem(destid);
	if (NULL == origin || NULL == dest)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}	
	// pos
	if (XItem::GetPos(origin->itemID) != XItem::GetPos(dest->itemID))
	{
		roRes.set_errorcode(KKSG::ERR_ENHANCE_TRANSPOS);
		return;
	}

	// dest level
	ErrorCode errorCode = (KKSG::ErrorCode)EnhanceEquipMgr::Instance()->DoTransfer(role, origin, dest);	
	roRes.set_errorcode(errorCode);
	if (KKSG::ERR_SUCCESS != errorCode)
	{
		return;
	}

	role->RecalcuateAttributeAndNotify(RecalAttr_Enhance);

	role->OnEnhanceTrans(XItem::GetPos(dest->itemID));

	roRes.set_isperfect(false);
	return;
}

void RpcC2G_EnhanceTranster::OnDelayReplyRpc(const EnhanceTransterArg &roArg, EnhanceTransterRes &roRes, const CUserData &roUserData)
{
}
