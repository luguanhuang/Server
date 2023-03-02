#include "pch.h"
#include "atlas/rpcc2g_atlasupstar.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/atlas.h"

// generate by ProtoGen at date: 2017/1/7 19:45:56

RPC_SERVER_IMPLEMETION(RpcC2G_AtlasUpStar, AtlasUpStarArg, AtlasUpStarRes)

void RpcC2G_AtlasUpStar::OnCall(const AtlasUpStarArg &roArg, AtlasUpStarRes &roRes)
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

	if (pCAtlasSys->UpStar(roArg.groupid()))
	{
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
	}

	roRes.set_groupid(roArg.groupid());
	roRes.set_star(pCAtlasSys->GetFinishId(roArg.groupid()));
}

void RpcC2G_AtlasUpStar::OnDelayReplyRpc(const AtlasUpStarArg &roArg, AtlasUpStarRes &roRes, const CUserData &roUserData)
{
}
