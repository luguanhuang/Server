#include "pch.h"
#include "network/mslink.h"
#include "unit/rolemanager.h"
#include "guild/guildcardmgr.h"
#include "guild/guildrecord.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/bagtransition.h"
#include "guild/rpcc2g_endguildcard.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "define/tlogenum.h"
#include "utility/tlogger.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"

// generate by ProtoGen at date: 2015/10/2 10:45:21

RPC_SERVER_IMPLEMETION(RpcC2G_EndGuildCard, EndGuildCardArg, EndGuildCardRes)

void RpcC2G_EndGuildCard::OnCall(const EndGuildCardArg &roArg, EndGuildCardRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	TGuildCardFlow oLog(role);
	oLog.m_iOperType = TX_GUILD_CARD_END;
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild)
	{
		oLog.m_lGuildID = guild->GetGuildId();
		oLog.m_iGuildLevel = guild->GetGuildLvl();
	}
	oLog.Do();

	CGuildRecord* record = role->Get<CGuildRecord>();
	record->EndCard(role, NULL == guild ? 0 : guild->GetGuildId());

	return;
}

void RpcC2G_EndGuildCard::OnDelayReplyRpc(const EndGuildCardArg &roArg, EndGuildCardRes &roRes, const CUserData &roUserData)
{

}
