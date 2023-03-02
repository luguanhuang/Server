#include "pch.h"
#include "spactivity/rpcc2g_getspactivityreward.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivity//spactivityrecord.h"
#include "spactivity//spactivitybase.h"
#include "spactivity//spactivitymgr.h"

// generate by ProtoGen at date: 2016/10/11 15:18:04

RPC_SERVER_IMPLEMETION(RpcC2G_GetSpActivityReward, GetSpActivityRewardArg, GetSpActivityRewardRes)

void RpcC2G_GetSpActivityReward::OnCall(const GetSpActivityRewardArg &roArg, GetSpActivityRewardRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return; 
	}
	SpActivityBase* pSpActBase = SpActivityMgr::Instance()->GetSpActivity(roArg.actid());
	if (pSpActBase == NULL)
	{
		LogWarn("actid [%u] is not exist, role [%llu]", roArg.actid(), pRole->GetID());
		roRes.set_errorcode(KKSG::ERR_SPACTIVITY_NOTOPEN);
		return;
	}	
	KKSG::ErrorCode errorCode = pSpActBase->GetReward(pRole, roArg.taskid());
	roRes.set_errorcode(errorCode);
}

void RpcC2G_GetSpActivityReward::OnDelayReplyRpc(const GetSpActivityRewardArg &roArg, GetSpActivityRewardRes &roRes, const CUserData &roUserData)
{
}
