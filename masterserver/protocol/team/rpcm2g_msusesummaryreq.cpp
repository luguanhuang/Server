#include "pch.h"
#include "team/rpcm2g_msusesummaryreq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "team/robotsummarymgr.h"

// generate by ProtoGen at date: 2016/8/22 21:20:51

RPC_CLIENT_IMPLEMETION(RpcM2G_MSUseSummaryReq, MSRobotSumamryArg, MSRobotSumamryRes)

void RpcM2G_MSUseSummaryReq::OnReply(const MSRobotSumamryArg &roArg, const MSRobotSumamryRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		return ;
	}
	
	if(false == roRes.success())
	{
		SSError << " ms robot sumamry err roleid = " << pRole->GetID() << " retype = " << (int)roArg.type() << END;
		return ;
	}
	
	if(MSUS_GET == roArg.type())
	{
		RobotSummaryMgr::Instance()->AddRobotSummary(pRole->GetID(), roArg.lookupid(), roRes);
	}
}

void RpcM2G_MSUseSummaryReq::OnTimeout(const MSRobotSumamryArg &roArg, const CUserData &roUserData)
{
}
