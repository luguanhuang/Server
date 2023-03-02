#include "pch.h"
#include "npcfeeling/rpcc2m_npcflreqc2m.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "npcfeeling/rpcm2g_npcflreqm2g.h"
#include "npcfeeling/npcfeelingmgr.h"

// generate by ProtoGen at date: 2017/9/11 10:34:55

RPC_SERVER_IMPLEMETION(RpcC2M_NpcFlReqC2M, NpcFlArg, NpcFlRes)

void RpcC2M_NpcFlReqC2M::OnCall(const NpcFlArg &roArg, NpcFlRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	switch(roArg.reqtype())
	{
	case NPCFL_GIVE_GIFT:
		{
		}
		break;
	default:
		break;
	}

	RpcM2G_NpcFlReqM2G* rpc = RpcM2G_NpcFlReqM2G::CreateRpc();
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_delayid(DelayRpc());
	*rpc->m_oArg.mutable_c2marg() = roArg;
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_NpcFlReqC2M::OnDelayReplyRpc(const NpcFlArg &roArg, NpcFlRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)	
	{
		return;
	}
	NpcFlResM2G* m2gRes = (NpcFlResM2G*)(roUserData.m_pUserPtr);
	roRes = m2gRes->c2mres();
	if(KKSG::ERR_SUCCESS != roRes.errorcode())
	{
		return;
	}
	switch(roArg.reqtype())
	{
	case NPCFL_GIVE_GIFT:
		{
			CNpcFeelingMgr::Instance()->UpdateTopAvExp(m2gRes->roleexp());
			CNpcFeelingMgr::Instance()->UpdateNpc2Role(m2gRes->npc2role());
		}
		break;
	default:
		break;
	}
}
