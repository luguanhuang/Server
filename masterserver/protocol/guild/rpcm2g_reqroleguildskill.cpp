#include "pch.h"
#include "guild/rpcm2g_reqroleguildskill.h"

// generate by ProtoGen at date: 2016/9/13 19:17:09

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqRoleGuildSkill, ReqRoleGuildSkillArg, ReqRoleGuildSkillRes)

void RpcM2G_ReqRoleGuildSkill::OnReply(const ReqRoleGuildSkillArg &roArg, const ReqRoleGuildSkillRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	std::map<UINT32, UINT32> roleSkillMap;
	for (UINT32 cnt = 0; cnt < roRes.roleskills_size(); ++cnt)
	{
		roleSkillMap[roRes.roleskills(cnt).skillid()] = roRes.roleskills(cnt).maxlvl();
	}
	oUser.m_pUserPtr = (void*)&roleSkillMap;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_ReqRoleGuildSkill::OnTimeout(const ReqRoleGuildSkillArg &roArg, const CUserData &roUserData)
{
}
