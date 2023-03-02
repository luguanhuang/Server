#include "pch.h"
#include "misc/rpcm2g_checknotice.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "idip/idiprecord.h"

// generate by ProtoGen at date: 2017/2/3 20:12:41

RPC_SERVER_IMPLEMETION(RpcM2G_CheckNotice, CheckNoticeArg, CheckNoticeRes)

void RpcM2G_CheckNotice::OnCall(const CheckNoticeArg &roArg, CheckNoticeRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	role->Get<CIdipRecord>()->DoNotice(roArg, roRes);
}

void RpcM2G_CheckNotice::OnDelayReplyRpc(const CheckNoticeArg &roArg, CheckNoticeRes &roRes, const CUserData &roUserData)
{
}
