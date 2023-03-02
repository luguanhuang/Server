#include "pch.h"
#include "role/rpcg2d_saveroledata.h"
#include "scene/leavescenecontrol.h"

// generate by ProtoGen at date: 2016/7/12 13:58:58

RPC_CLIENT_IMPLEMETION(RpcG2D_SaveRoleData, SaveRoleDataArg, SaveRoleDataRes)

void RpcG2D_SaveRoleData::OnReply(const SaveRoleDataArg &roArg, const SaveRoleDataRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		LogError("Role %llu save data to db failed, error %d", roArg.roleid(), roRes.result());
	}
	if(roArg.param() != 0)
	{
		CLeaveSceneControl::Instance()->OnSaveRoleDataReply(roArg.param(), roArg.roleid(), roRes.result());
	}
}

void RpcG2D_SaveRoleData::OnTimeout(const SaveRoleDataArg &roArg, const CUserData &roUserData)
{
	LogWarn("Role %llu save data to db timeout", roArg.roleid());
	if(roArg.param() != 0)
	{
		CLeaveSceneControl::Instance()->OnSaveRoleDataReply(roArg.param(), roArg.roleid(), KKSG::ERR_SUCCESS);
	}
}
