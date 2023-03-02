#include "pch.h"
#include "atlas/rpcc2g_autobreakatlas.h"
#include "gamelogic/atlas.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/9/1 16:52:57

RPC_SERVER_IMPLEMETION(RpcC2G_AutoBreakAtlas, AutoBreakAtlasArg, AutoBreakAtlasRes)

void RpcC2G_AutoBreakAtlas::OnCall(const AutoBreakAtlasArg &roArg, AutoBreakAtlasRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	CAtlasSys* pCAtlasSys = pRole->Get<CAtlasSys>();
	if (pCAtlasSys == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return ;
	}

	std::set<UINT32> qualitSet;
	for (UINT32 cnt = 0; cnt < roArg.quilts_size(); ++cnt)
	{
		qualitSet.insert(roArg.quilts(cnt));
	}

	//pCAtlasSys->AutoBreakCard(qualitSet, roArg.groupid());
	roRes.set_errorcode((KKSG::ErrorCode)pCAtlasSys->AutoBreakCard(qualitSet, roArg.groupid()));
}

void RpcC2G_AutoBreakAtlas::OnDelayReplyRpc(const AutoBreakAtlasArg &roArg, AutoBreakAtlasRes &roRes, const CUserData &roUserData)
{
}
