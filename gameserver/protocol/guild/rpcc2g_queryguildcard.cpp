#include "pch.h"
#include "guild/guildrecord.h"
#include "unit/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildcardmgr.h"
#include "guild/guildconfig.h"
#include "network/mslink.h"
#include "guild/rpcc2g_queryguildcard.h"
#include "guild/rpcg2m_guildcardsyncms.h"

// generate by ProtoGen at date: 2015/10/2 10:42:15

RPC_SERVER_IMPLEMETION(RpcC2G_QueryGuildCard, QueryGuildCardArg, QueryGuildCardRes)

void RpcC2G_QueryGuildCard::OnCall(const QueryGuildCardArg &roArg, QueryGuildCardRes &roRes)
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

	RpcG2M_GuildCardSyncMs* rpc = RpcG2M_GuildCardSyncMs::CreateRpc();
	rpc->m_oArg.set_delayrpc(DelayRpc());
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_op(GuildCard_Query);

	MSLink::Instance()->SendTo(*rpc);

	return;
}

void RpcC2G_QueryGuildCard::OnDelayReplyRpc(const QueryGuildCardArg &roArg, QueryGuildCardRes &roRes, const CUserData &roUserData)
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

	UINT32 guildlevel = data->guildlevel();
	UINT32 confallcount = data->confallcount();
	UINT32 confcount = data->confcount();
	UINT32 confchangecount = data->confchangecount();

	UINT32 result = data->result();
	UINT32 allcount = data->allcount();
	std::string bestrolename = data->bestrolename();

	roRes.set_canallcount(confallcount);
	roRes.set_allcount(confallcount > allcount ? confallcount - allcount : 0);
	roRes.set_bestresult(result);
	roRes.set_bestrole(bestrolename);
	for (int i = 0; i < data->bestcards_size(); ++i)
	{
		roRes.add_bestcards(data->bestcards(i));
	}

	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();
	UINT32 playcount = record->GetPlayCount();
	UINT32 changecount = record->GetChangeCount();
	UINT32 buychangecount = record->GetBuyChangeCount();
	roRes.set_playcount(confcount > playcount ? confcount - playcount : 0);
	roRes.set_changecount(confchangecount > changecount ? confchangecount - changecount : 0);
	roRes.set_canplaycount(confcount);
	roRes.set_canchangecount(confchangecount);
	roRes.set_buychangcount(buychangecount);
}
