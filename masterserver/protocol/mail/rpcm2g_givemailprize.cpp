#include "pch.h"
#include "mail/rpcm2g_givemailprize.h"
#include "mail/mailmgr.h"

// generate by ProtoGen at date: 2016/11/1 20:20:08

RPC_CLIENT_IMPLEMETION(RpcM2G_GiveMailPrize, GiveMailPrizeArg, GiveMailPrizeRes)

void RpcM2G_GiveMailPrize::OnReply(const GiveMailPrizeArg &roArg, const GiveMailPrizeRes &roRes, const CUserData &roUserData)
{
	bool isSucceed = true;

	for (int i = 0; i < roArg.mail_size(); i++)
	{
		if (i < roRes.issucceed_size()) 
		{
			CMailMgr::Instance()->DoReward(roArg.roleid(), roArg.mail(i).uid(), roRes.issucceed(i));
			isSucceed &= roRes.issucceed(i);
		}
		else
		{
			CMailMgr::Instance()->DoReward(roArg.roleid(), roArg.mail(i).uid(), true);
		}
	}

	if (isSucceed)
	{
		CUserData data;
		data.m_dwUserData = (UINT32)KKSG::ERR_SUCCESS;
		CRpc::ReplyDelayRpc(roArg.delayid(), data);
	}
	else
	{
		CUserData data;
		data.m_dwUserData = (UINT32)KKSG::ERR_BAG_FULL_GIVE_MAIL_REWARD;
		CRpc::ReplyDelayRpc(roArg.delayid(), data);
	}
}

void RpcM2G_GiveMailPrize::OnTimeout(const GiveMailPrizeArg &roArg, const CUserData &roUserData)
{
	for (int i = 0; i < roArg.mail_size(); i++)
	{
		CMailMgr::Instance()->DoReward(roArg.roleid(), roArg.mail(i).uid(), true);
	}

	CUserData data;
	data.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.delayid(), data);
}
