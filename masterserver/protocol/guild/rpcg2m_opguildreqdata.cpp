#include "pch.h"
#include "guild/rpcg2m_opguildreqdata.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/9/9 9:56:30

RPC_SERVER_IMPLEMETION(RpcG2M_OpGuildReqData, OpGuildReqDataArg, OpGuildReqDataRes)

	
//  所有gs需要公会信息的都从这里返回 
void RpcG2M_OpGuildReqData::OnCall(const OpGuildReqDataArg &roArg, OpGuildReqDataRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roArg.roleid());
	if (pGuild == NULL)
	{
		return;
	}

	pGuild->GuildDataToGS(roArg, roRes);

	switch(roArg.optype())
	{
	case (KKSG::STUDY_SKILL): // 学习公会技能
		{
			if (roArg.has_skillid())
			{
				roRes.set_guildskilllvl(pGuild->GetSkillLvl(roArg.skillid()));
			}
		}
		break;
	case (KKSG::GUILD_DARE_INFO): // 公会挑战数据
		{
		}
		break;
	}

}

void RpcG2M_OpGuildReqData::OnDelayReplyRpc(const OpGuildReqDataArg &roArg, OpGuildReqDataRes &roRes, const CUserData &roUserData)
{
}
