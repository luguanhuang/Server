#include "pch.h"
#include "bigmelee/rpcc2m_querybigmeleerank.h"
#include "bigmelee/rpcm2k_querybigmeleerank2k.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "network/worldlink.h"
#include "config.h"
#include "mayhem/mayhemmgr.h"
// generate by ProtoGen at date: 2017/6/20 16:32:47

RPC_SERVER_IMPLEMETION(RpcC2M_QueryBigMeleeRank, QueryMayhemRankArg, QueryMayhemRankRes)

void RpcC2M_QueryBigMeleeRank::OnCall(const QueryMayhemRankArg &roArg, QueryMayhemRankRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_err(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if (roArg.count() <= 0)
	{
		roRes.set_err(ERR_UNKNOWN);
		return;
	}
	if (roArg.count() == 100)
	{
		MayhemMsMgr::Instance()->DelRole(pRole->GetID());
	}
	RpcM2K_QueryBigMeleeRank2k* rpc = RpcM2K_QueryBigMeleeRank2k::CreateRpc();
	rpc->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_roleid(pRole->GetID());
	rpc->m_oArg.set_count(roArg.count());
	WorldLink::Instance()->SendTo(*rpc);
}

void RpcC2M_QueryBigMeleeRank::OnDelayReplyRpc(const QueryMayhemRankArg &roArg, QueryMayhemRankRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS)
	{
		KKSG::QueryMayhemRankRes* pData = (KKSG::QueryMayhemRankRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pData);
	}
	roRes.set_err(KKSG::ErrorCode(roUserData.m_dwUserData));
}
