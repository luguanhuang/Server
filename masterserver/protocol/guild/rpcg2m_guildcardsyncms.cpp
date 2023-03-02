#include "pch.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcard.h"
#include "guild/guildcardmgr.h"
#include "table/guildConfig.h"
#include "guild/guildbonus.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/rpcg2m_guildcardsyncms.h"
#include "guild/ptcm2c_guildbestcardsntfms.h"

// generate by ProtoGen at date: 2016/9/17 19:33:47

RPC_SERVER_IMPLEMETION(RpcG2M_GuildCardSyncMs, GuildCardSyncMsArg, GuildCardSyncMsRes)

void RpcG2M_GuildCardSyncMs::OnCall(const GuildCardSyncMsArg &roArg, GuildCardSyncMsRes &roRes)
{
	UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(roArg.roleid());
	Guild* guild = CGuildMgr::Instance()->GetGuild(guildid);
	if (guild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	UINT32 guildlevel = guild->GetLevel();
	UINT32 confallcount = CGuildConfig::Instance()->GetConfig(guildlevel, GuildPokerTimes);
	UINT32 confcount = CGuildConfig::Instance()->GetConfig(guildlevel, PokerTimes);
	UINT32 confchangecount = CGuildConfig::Instance()->GetConfig(guildlevel, PokerReplaceTimes);

	roRes.set_guildlevel(guildlevel);
	roRes.set_confallcount(confallcount);
	roRes.set_confcount(confcount);
	roRes.set_confchangecount(confchangecount);

	roRes.set_roleid(roArg.roleid());

	CGuildCard* guildcard = guild->Get<CGuildCard>();
	guildcard->Update();

	if (KKSG::GuildCard_Query == roArg.op())
	{
		// best card info
		UINT32 result = guildcard->GetResult();
		UINT32 allcount = guildcard->GetAllCount();
		std::string bestrolename = guildcard->GetBestRoleName();
		roRes.set_bestrolename(bestrolename);
		const std::vector<UINT32>& cards = guildcard->GetCards();
		for (auto i = cards.begin(); i != cards.end(); ++i)
		{
			roRes.add_bestcards(*i);
		}
	}
	else if (KKSG::GuildCard_Start == roArg.op())
	{
		// card limit
		if (guildcard->IsCountLimit(confallcount))
		{
			roRes.set_errorcode(KKSG::ERR_GUILDCARD_ALLCOUNTLIMIT);
			return;
		}
		guildcard->AddCount();
	}
	else if (KKSG::GuildCard_Change== roArg.op())
	{

	}
	else if (KKSG::GuildCard_End == roArg.op())
	{
		// end guild process
		UINT32 exp = GuildCardMgr::Instance()->GetExp(roArg.result());
		guild->AddExp(exp);

		std::vector<UINT32> cards;
		for (int i = 0; i < roArg.cards_size(); ++i)
		{
			cards.push_back(roArg.cards(i));
		}
		CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
		if (guildcard->PushCards(roArg.roleid(), roArg.result(), cards))
		{
			PtcM2C_GuildBestCardsNtfMs ntf;
			ntf.m_Data.set_bestresult(roArg.result());
			for (auto i = cards.begin(); i !=  cards.end(); ++i)
			{
				ntf.m_Data.add_bestcards(*i);
			}
			ntf.m_Data.set_bestrole(NULL == role ? "" : role->GetName());
			guild->BroadCast(ntf);
		}

		GuildBonusMgr* bonus = guild->Get<GuildBonusMgr>();
		bonus->addGuildBonus(KKSG::GBONUS_CARDREWARD, roArg.result() + 1, role ? role->GetID() : 0);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		SSWarn<<"invalid op:"<<roArg.op()<<END;
	}
}

void RpcG2M_GuildCardSyncMs::OnDelayReplyRpc(const GuildCardSyncMsArg &roArg, GuildCardSyncMsRes &roRes, const CUserData &roUserData)
{
}
