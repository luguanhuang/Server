#include "pch.h"
#include "atlas/rpcc2g_activatatlas.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/atlas.h"


// generate by ProtoGen at date: 2016/9/1 16:42:53

RPC_SERVER_IMPLEMETION(RpcC2G_ActivatAtlas, ActivatAtlasArg, ActivatAtlasRes)

void RpcC2G_ActivatAtlas::OnCall(const ActivatAtlasArg &roArg, ActivatAtlasRes &roRes)
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

	roRes.set_errorcode((KKSG::ErrorCode)pCAtlasSys->ActivatAtlas(roArg.teamid()));
}

void RpcC2G_ActivatAtlas::OnDelayReplyRpc(const ActivatAtlasArg &roArg, ActivatAtlasRes &roRes, const CUserData &roUserData)
{
}
