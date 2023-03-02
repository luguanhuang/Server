#include "pch.h"
#include "guild/rpcc2g_changeguildcard.h"
#include "guild/guildrecord.h"
#include "unit/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guildcardmgr.h"
#include "guild/guild.h"
#include "guild/guildconfig.h"
#include "gamelogic/takecostmgr.h"
#include "define/tlogenum.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2015/10/2 10:43:56

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeGuildCard, ChangeGuildCardArg, ChangeGuildCardRes)

void RpcC2G_ChangeGuildCard::OnCall(const ChangeGuildCardArg &roArg, ChangeGuildCardRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		return;
	}
	UINT32 guildlevel = guild->GetGuildLvl();
	if (!GuildConfig::Instance()->IsOpen(guildlevel, PokerTimes))
	{
		return;
	}

	UINT32 confchangecount = GuildConfig::Instance()->GetConfig(guildlevel, PokerReplaceTimes);

	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();

	if (record->IsChangeLimit(confchangecount))
	{
		if (!TakeCostManager::Instance()->TakeCost("BuyCardChangeCost", role, record->GetBuyChangeCount(), ItemFlow_Guild, ItemFlow_Guild_Card))
		{
			roRes.set_errorcode(ERR_ITEM_NOT_ENOUGH);
			return;
		}
		record->AddBuyChangeCount();
	}
	record->AddChangeCount();

	UINT32 card = record->ChangeCard(roArg.card());
	if (UINT32(-1) == card)
	{
		SSWarn<<"select card:"<<roArg.card()<<END;
		for (auto i = record->GetCards().begin(); i != record->GetCards().end(); ++i)
		{
			SSWarn<<"cards:"<<*i<<END;
		}
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	roRes.set_card(card);
	
	UINT32 result = GuildCardMgr::Instance()->GetResult(record->GetCards());
	roRes.set_result(result);

	roRes.set_errorcode(ERR_SUCCESS);

	TGuildCardFlow oLog(role);
	oLog.m_iOperType = TX_GUILD_CARD_CHANGE;
	oLog.m_lGuildID = guild->GetGuildId();
	oLog.m_iGuildLevel = guild->GetGuildLvl();
	oLog.Do();
	return;
}

void RpcC2G_ChangeGuildCard::OnDelayReplyRpc(const ChangeGuildCardArg &roArg, ChangeGuildCardRes &roRes, const CUserData &roUserData)
{
}
