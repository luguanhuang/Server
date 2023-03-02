#include "pch.h"
#include "team/rpcm2g_msusesummaryreq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/msusesummarymgr.h"
#include "gamelogic/robotconfig.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/robotgenerator.h"

// generate by ProtoGen at date: 2016/8/22 21:20:51

RPC_SERVER_IMPLEMETION(RpcM2G_MSUseSummaryReq, MSRobotSumamryArg, MSRobotSumamryRes)

void RpcM2G_MSUseSummaryReq::OnCall(const MSRobotSumamryArg &roArg, MSRobotSumamryRes &roRes)
{
	roRes.set_success(true);

	switch(roArg.type())
	{
	case MSUS_GET:
		{
			Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
			if(NULL == pRole)
			{
				roRes.set_success(false);
				break;
			}
			FilterRoleData filter;
			filter.qwRoleID = pRole->GetID();
			filter.level = pRole->GetLevel();
			filter.ppt = (int)pRole->GetAttr(TOTAL_POWERPOINT);
			filter.prof = pRole->GetProfession();

			RoleSummary* pSummary = RobotConfig::Instance()->CreateRobotSummary(filter, roArg.lookupid()); 
			if(NULL == pSummary)
			{
				roRes.set_success(false);
				break;
			}
			roRes.set_robotid(pSummary->GetRobotID());
			RoleSummaryMgr::Instance()->SummaryToDBStored(pSummary, *roRes.mutable_summarystored());
			RobotGenerator::Instance()->FreeRoleSummary(pSummary);
		}
		break;
	default:
		break;
	}
}

void RpcM2G_MSUseSummaryReq::OnDelayReplyRpc(const MSRobotSumamryArg &roArg, MSRobotSumamryRes &roRes, const CUserData &roUserData)
{
}
