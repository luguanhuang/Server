#include "pch.h"
#include "db/rpcm2d_updaterolereportdata.h"

// generate by ProtoGen at date: 2017/5/14 22:11:05

RPC_CLIENT_IMPLEMETION(RpcM2D_UpdateRoleReportData, UpdateRoleReportDataArg, UpdateRoleReportDataRes)

void RpcM2D_UpdateRoleReportData::OnReply(const UpdateRoleReportDataArg &roArg, const UpdateRoleReportDataRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() != KKSG::ERR_SUCCESS)
	{
		LogError("update rolereport failed roleid[%llu] error[%u]", roArg.roleid(), roRes.result());
	}
}

void RpcM2D_UpdateRoleReportData::OnTimeout(const UpdateRoleReportDataArg &roArg, const CUserData &roUserData)
{
	LogError("update rolereport timeout[%llu]", roArg.roleid());
}
