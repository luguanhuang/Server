#include "pch.h"
#include "guild/guildrecord.h"
#include "unit/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildcardmgr.h"
#include "guild/guildconfig.h"
#include "guild/rpcc2g_startguildcard.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "define/tlogenum.h"
#include "utility/tlogger.h"
#include "gamelogic/reportdatarecord.h"

// generate by ProtoGen at date: 2015/10/2 10:40:50

RPC_SERVER_IMPLEMETION(RpcC2G_StartGuildCard, StartGuildCardArg, StartGuildCardRes)

void RpcC2G_StartGuildCard::OnCall(const StartGuildCardArg &roArg, StartGuildCardRes &roRes)
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

	UINT32 confcount = GuildConfig::Instance()->GetConfig(guildlevel, PokerTimes);
	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();
	if (record->IsCountLimit(confcount))
	{
		roRes.set_errorcode(KKSG::ERR_GUILDCARD_COUNTLIMIT);
		return;
	}

	RpcG2M_GuildCardSyncMs* rpc = RpcG2M_GuildCardSyncMs::CreateRpc();
	rpc->m_oArg.set_delayrpc(DelayRpc());
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_op(GuildCard_Start);

	role->SendToMS(*rpc);
	return;
}

void RpcC2G_StartGuildCard::OnDelayReplyRpc(const StartGuildCardArg &roArg, StartGuildCardRes &roRes, const CUserData &roUserData)
{
	const KKSG::GuildCardSyncMsRes* data = (KKSG::GuildCardSyncMsRes*)roUserData.m_pUserPtr;
	Role* role = RoleManager::Instance()->FindByRoleID(data->roleid());
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if (data->errorcode() != ERR_SUCCESS)
	{
		roRes.set_errorcode(ErrorCode(data->errorcode()));
		return;
	}

	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();
	if (record->IsCountLimit(data->confcount()))
	{
		roRes.set_errorcode(KKSG::ERR_GUILDCARD_COUNTLIMIT);
		return;
	}
	record->AddPlayCount();

	UINT32 store = record->RandomStore();
	roRes.set_store(store);
	std::vector<UINT32> cards = record->GetCards();
	for (auto i = cards.begin(); i != cards.end(); ++i)
	{
		roRes.add_card(*i);
	}
	UINT32 result = GuildCardMgr::Instance()->GetResult(record->GetCards());
	roRes.set_result(result);

	role->OnGuildActivity(1);

	roRes.set_errorcode(ERR_SUCCESS);

	TGuildCardFlow oLog(role);
	oLog.m_iOperType = TX_GUILD_CARD_START;
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild)
	{
		oLog.m_lGuildID = guild->GetGuildId();
		oLog.m_iGuildLevel = guild->GetGuildLvl();
	}
	oLog.Do();

	role->Get<CReportDataRecord>()->UpdatePokerCount();
}
