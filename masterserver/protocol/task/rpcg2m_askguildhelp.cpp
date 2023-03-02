#include "pch.h"
#include "task/rpcg2m_askguildhelp.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildarena.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "guild/guilddonate.h"

// generate by ProtoGen at date: 2016/11/16 11:19:49

RPC_SERVER_IMPLEMETION(RpcG2M_AskGuildHelp, AskGuildHelpReq, AskGuildHelpRes)

void RpcG2M_AskGuildHelp::OnCall(const AskGuildHelpReq &roArg, AskGuildHelpRes &roRes)
{
	/*
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_code(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_code(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}
	GuildDonate* donate = pGuild->Get<GuildDonate>();
	UINT32 uid = donate->AddAskItemInfo(pRole->GetID(), roArg.itemid(), roArg.itemcount(), roArg.level(), roArg.quality(), roArg.taskid());

	roRes.set_code(KKSG::ERR_SUCCESS);
	roRes.set_ask_uid(uid);
	*/
}

void RpcG2M_AskGuildHelp::OnDelayReplyRpc(const AskGuildHelpReq &roArg, AskGuildHelpRes &roRes, const CUserData &roUserData)
{
}
