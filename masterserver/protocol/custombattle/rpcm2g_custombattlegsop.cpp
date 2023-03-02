#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/rpcm2g_custombattlegsop.h"

// generate by ProtoGen at date: 2017/5/2 17:00:34

RPC_CLIENT_IMPLEMETION(RpcM2G_CustomBattleGsOp, CustomBattleGsOpArg, CustomBattleGsOpRes)

void RpcM2G_CustomBattleGsOp::OnReply(const CustomBattleGsOpArg &roArg, const CustomBattleGsOpRes &roRes, const CUserData &roUserData)
{
	if (KKSG::ERR_SUCCESS != roRes.errorcode())
	{
		CUserData data;
		data.m_dwUserData = roRes.errorcode();
		data.m_pUserPtr = NULL;
		ReplyDelayRpc(roArg.delayid(), data);

		SSWarn<<"roleid:"<<roArg.roleid()<<" uid:"<<roArg.uid()<<" op:"<<roArg.op()<<" error:"<<roRes.errorcode()<<END;
		return;
	}
	else
	{
		CustomBattleReq req;
		req.op = roArg.op();
		req.uid = roArg.uid();
		req.conf = roArg.config();
		req.roleid = roArg.roleid();
		req.delayid = roArg.delayid();
		if (!CustomBattleMgr::Instance()->HandleGsReply(req))
		{
			CUserData data;
			data.m_dwUserData = KKSG::ERR_FAILED;
			data.m_pUserPtr = NULL;
			ReplyDelayRpc(roArg.delayid(), data);
			return;
		}
	}
}

void RpcM2G_CustomBattleGsOp::OnTimeout(const CustomBattleGsOpArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	data.m_pUserPtr = NULL;
	ReplyDelayRpc(roArg.delayid(), data);
	return;
}
