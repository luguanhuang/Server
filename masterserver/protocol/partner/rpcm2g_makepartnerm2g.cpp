#include "pch.h"
#include "partner/rpcm2g_makepartnerm2g.h"
#include "partner/partnermgr.h"
#include "partner/ptcm2c_makepartnerresultntf.h"

// generate by ProtoGen at date: 2016/12/8 16:06:49

RPC_CLIENT_IMPLEMETION(RpcM2G_MakePartnerM2G, MakePartnerM2GArg, MakePartnerM2GRes)

void RpcM2G_MakePartnerM2G::OnReply(const MakePartnerM2GArg &roArg, const MakePartnerM2GRes &roRes, const CUserData &roUserData)
{
	std::vector<UINT64> roleIDs;
	for (int i = 0; i < roArg.roleids_size(); ++i)
	{
		roleIDs.push_back(roArg.roleids(i));
	}
	if (roRes.result() != KKSG::ERR_SUCCESS)
	{
		// 推送客户端错误码
		PartnerMgr::NotifyMakePartnerError(roleIDs, roRes.result(), roRes.roleid());
		return;
	}
	PartnerMgr::Instance()->MakePartner(roleIDs);
}

void RpcM2G_MakePartnerM2G::OnTimeout(const MakePartnerM2GArg &roArg, const CUserData &roUserData)
{
	// 失败处理 推送客户端错误码
	std::vector<UINT64> roleIDs;
	for (int i = 0; i < roArg.roleids_size(); ++i)
	{
		roleIDs.push_back(roArg.roleids(i));
	}
	PartnerMgr::NotifyMakePartnerError(roleIDs, KKSG::ERR_TIMEOUT, 0);
}
