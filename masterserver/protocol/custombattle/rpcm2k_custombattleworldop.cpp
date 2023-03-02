#include "pch.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/rpcm2k_custombattleworldop.h"

// generate by ProtoGen at date: 2017/4/7 21:32:50

RPC_CLIENT_IMPLEMETION(RpcM2K_CustomBattleWorldOp, CustomBattleWorldOpArg, CustomBattleWorldOpRes)

void RpcM2K_CustomBattleWorldOp::OnReply(const CustomBattleWorldOpArg &roArg, const CustomBattleWorldOpRes &roRes, const CUserData &roUserData)
{
	CustomBattleReq req;
	req.op = roArg.op();
	req.uid = roArg.uid();
	req.conf = roArg.config();
	req.roleid = roArg.roleid();
	req.delayid = roArg.delayid();
	req.battlename = roArg.battlename();
	req.token = roArg.token();
	req.password = roArg.password();
	req.iscross = roArg.querycross();
	if (!CustomBattleMgr::Instance()->HandleWorldReply(req, roRes.errorcode(), roRes.info()))
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_FAILED;
		data.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(roArg.delayid(), data);
	}
}

void RpcM2K_CustomBattleWorldOp::OnTimeout(const CustomBattleWorldOpArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.delayid(), data);
}
