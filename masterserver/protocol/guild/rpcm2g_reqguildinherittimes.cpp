#include "pch.h"
#include "guild/rpcm2g_reqguildinherittimes.h"

// generate by ProtoGen at date: 2016/12/5 16:59:12

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqGuildInheritTimes, ReqGuildInheritTimesArg, ReqGuildInheritTimesRes)

void RpcM2G_ReqGuildInheritTimes::OnReply(const ReqGuildInheritTimesArg &roArg, const ReqGuildInheritTimesRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	//std::map<UINT32, UINT32> roleSkillMap;
	std::set<UINT64> roleSet;
	for (UINT32 cnt = 0; cnt < roRes.roles_size(); ++cnt)
	{
		roleSet.insert(roRes.roles(cnt));
	}
	oUser.m_pUserPtr = (void*)&roleSet;
	//for (UINT32 cnt = 0; cnt < roRes.roleskills_size(); ++cnt)
//	{
	//	roleSkillMap[roRes.roleskills(cnt).skillid()] = roRes.roleskills(cnt).maxlvl();
	//}
	//oUser.m_pUserPtr = (void*)&roleSkillMap;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ReqGuildInheritTimes::OnTimeout(const ReqGuildInheritTimesArg &roArg, const CUserData &roUserData)
{
}
