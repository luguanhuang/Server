#include "pch.h"
#include "guild/rpcc2m_getguildbonussendlist.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guild/guild.h"
#include "guild/guildbonus.h"

// generate by ProtoGen at date: 2016/11/2 10:26:17

RPC_SERVER_IMPLEMETION(RpcC2M_GetGuildBonusSendList, GetGuildBonusSendListArg, GetGuildBonusSendListRes)

void RpcC2M_GetGuildBonusSendList::OnCall(const GetGuildBonusSendListArg &roArg, GetGuildBonusSendListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!guild)
	{
		return;
	}

	auto record = guild->Get<GuildBonusMgr>();
	UINT32 error = record->GetGuildBonusInSendList(role, roRes);
	roRes.set_error((ErrorCode)error);
}

void RpcC2M_GetGuildBonusSendList::OnDelayReplyRpc(const GetGuildBonusSendListArg &roArg, GetGuildBonusSendListRes &roRes, const CUserData &roUserData)
{
}
