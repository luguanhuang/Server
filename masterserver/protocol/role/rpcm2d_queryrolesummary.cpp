#include "pch.h"
#include "role/rolesummarymgr.h"
#include "role/rpcm2d_queryrolesummary.h"

// generate by ProtoGen at date: 2017/4/26 19:27:56

RPC_CLIENT_IMPLEMETION(RpcM2D_QueryRolesummary, QueryRolesummaryArg, QueryRolesummaryRes)

void RpcM2D_QueryRolesummary::OnReply(const QueryRolesummaryArg &roArg, const QueryRolesummaryRes &roRes, const CUserData &roUserData)
{
	if (KKSG::ERR_SUCCESS == roRes.errorcode())
	{
		KKSG::RoleSummaryStored data = roRes.data();
		CRoleSummaryMgr::Instance()->InsertRoleSummary(data, SUMMARY_ALL, 0, false);
		SSInfo<<"read rolesummary, roleid:"<<roArg.roleid()<<END;
	}
	else
	{
		SSWarn<<"read rolesummary failed, roleid"<<roArg.roleid()<<END;
	}
}

void RpcM2D_QueryRolesummary::OnTimeout(const QueryRolesummaryArg &roArg, const CUserData &roUserData)
{
}
