#include "pch.h"
#include "guild/rpcc2m_sendguildbonusinsendlist.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guild/guild.h"
#include "guild/guildbonus.h"

// generate by ProtoGen at date: 2016/11/2 10:28:20

RPC_SERVER_IMPLEMETION(RpcC2M_SendGuildBonusInSendList, SendGuildBonusInSendListArg, SendGuildBonusInSendListRes)

void RpcC2M_SendGuildBonusInSendList::OnCall(const SendGuildBonusInSendListArg &roArg, SendGuildBonusInSendListRes &roRes)
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
	UINT32 error = record->ManualSendBonusInSendList(role, roArg.bonusid());
	roRes.set_error((ErrorCode)error);
}

void RpcC2M_SendGuildBonusInSendList::OnDelayReplyRpc(const SendGuildBonusInSendListArg &roArg, SendGuildBonusInSendListRes &roRes, const CUserData &roUserData)
{
}
