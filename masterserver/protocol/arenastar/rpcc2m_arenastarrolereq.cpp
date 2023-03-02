#include "pch.h"
#include "arenastar/rpcc2m_arenastarrolereq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "arenastar/arenastarmgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2017/3/29 20:29:34

RPC_SERVER_IMPLEMETION(RpcC2M_ArenaStarRoleReq, ArenaStarReqArg, ArenaStarReqRes)

void RpcC2M_ArenaStarRoleReq::OnCall(const ArenaStarReqArg &roArg, ArenaStarReqRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if(!pRole->IsSystemOpened(SYS_ARENA_STAR_ID))
	{
		roRes.set_errorcode(ERR_SYS_NOTOPEN);
		return ;
	}

	switch(roArg.reqtype())
	{
	case ASRT_ROLEDATA:
		{
			ArenaStarMgr::Instance()->HandleTopRoleData(roArg.zantype(), roRes);
		}
		break;
	case ASRT_DIANZAN:
		{
			ArenaStarMgr::Instance()->HandleDianZan(pRole, roArg.zantype(), roRes);
		}
		break;
	default:
		break;
	}
}

void RpcC2M_ArenaStarRoleReq::OnDelayReplyRpc(const ArenaStarReqArg &roArg, ArenaStarReqRes &roRes, const CUserData &roUserData)
{
}
