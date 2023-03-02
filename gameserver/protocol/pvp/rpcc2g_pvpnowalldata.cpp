#include "pch.h"
#include "pvp/rpcc2g_pvpnowalldata.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/scenepvp.h"

// generate by ProtoGen at date: 2016/6/27 10:43:57

RPC_SERVER_IMPLEMETION(RpcC2G_PvpNowAllData, roArg, PvpNowGameData)

void RpcC2G_PvpNowAllData::OnCall(const roArg &roArg, PvpNowGameData &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if(pRole->GetCurrScene() && pRole->GetCurrScene()->GetPvpHandler())
	{
		pRole->GetCurrScene()->GetPvpHandler()->FillNowGameData(roRes);
	}
	
}

void RpcC2G_PvpNowAllData::OnDelayReplyRpc(const roArg &roArg, PvpNowGameData &roRes, const CUserData &roUserData)
{
}
