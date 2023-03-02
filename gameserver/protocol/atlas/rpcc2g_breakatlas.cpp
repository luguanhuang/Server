#include "pch.h"
#include "atlas/rpcc2g_breakatlas.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/atlas.h"

// generate by ProtoGen at date: 2016/9/1 16:54:12

RPC_SERVER_IMPLEMETION(RpcC2G_breakAtlas, breakAtlas, breakAtlasRes)

void RpcC2G_breakAtlas::OnCall(const breakAtlas &roArg, breakAtlasRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	CAtlasSys* pCAtlasSys = pRole->Get<CAtlasSys>();
	if (pCAtlasSys == NULL)
	{
		return ;
	}

	pCAtlasSys->BreakCard(roArg.atlaid(), roArg.atlanum());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_breakAtlas::OnDelayReplyRpc(const breakAtlas &roArg, breakAtlasRes &roRes, const CUserData &roUserData)
{
}
