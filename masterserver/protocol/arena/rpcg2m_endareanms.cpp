#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "arena/arenamgr.h"
#include "arena/rpcg2m_endareanms.h"

// generate by ProtoGen at date: 2016/9/12 20:26:45

RPC_SERVER_IMPLEMETION(RpcG2M_EndAreanMs, EndAreanMsArg, EndAreanMsRes)

void RpcG2M_EndAreanMs::OnCall(const EndAreanMsArg &roArg, EndAreanMsRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if (NULL == role)
	{
		return;
	}
	if (roArg.isgm())
	{
		UINT64 challengeid = ArenaMgr::Instance()->GetRoleId(roArg.rank());
		UINT32 rank = ArenaMgr::Instance()->GetRank(role->GetID());
		ArenaMgr::Instance()->UpdateRankList(challengeid, roArg.rank(), role->GetID(), rank);
		SSInfo<<"arena gm op, challenge rank:"<<roArg.rank()<<" role rank:"<<rank<<END;
		return;
	}
	KKSG::ArenaEndInfo* info = roRes.mutable_info();
	if (ArenaMgr::Instance()->OnEnd(role->GetID(), roArg.iswin(), *info))
	{
		roRes.set_issucceed(true);	
	}
	else
	{
		roRes.set_issucceed(false);	
	}
}

void RpcG2M_EndAreanMs::OnDelayReplyRpc(const EndAreanMsArg &roArg, EndAreanMsRes &roRes, const CUserData &roUserData)
{
}
