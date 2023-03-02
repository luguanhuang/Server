#include "pch.h"
#include "levelseal/rpcc2g_getlevelsealselfgift.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/levelsealMgr.h"

// generate by ProtoGen at date: 2016/10/17 15:56:07

RPC_SERVER_IMPLEMETION(RpcC2G_GetLevelSealSelfGift, GetLevelSealSealGiftArg, GetLevelSealSelfGiftRes)

void RpcC2G_GetLevelSealSelfGift::OnCall(const GetLevelSealSealGiftArg &roArg, GetLevelSealSelfGiftRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	KKSG::ErrorCode errcode = (KKSG::ErrorCode)CLevelSealMgr::Instance()->GetSelfGift(pRole, roArg.count());
	roRes.set_errcode(errcode);
}

void RpcC2G_GetLevelSealSelfGift::OnDelayReplyRpc(const GetLevelSealSealGiftArg &roArg, GetLevelSealSelfGiftRes &roRes, const CUserData &roUserData)
{
}
