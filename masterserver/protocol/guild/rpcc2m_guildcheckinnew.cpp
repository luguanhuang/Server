#include "pch.h"
#include "guild/rpcc2m_guildcheckinnew.h"
#include "guild/rpcm2g_noticeguildcheckin.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcheckin.h"
#include "table/guildConfig.h"


// generate by ProtoGen at date: 2016/9/16 21:00:56

RPC_SERVER_IMPLEMETION(RpcC2M_GuildCheckinNew, GuildCheckinArg, GuildCheckinRes)

void RpcC2M_GuildCheckinNew::OnCall(const GuildCheckinArg &roArg, GuildCheckinRes &roRes)
{

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		roRes.set_errorcode(ERR_GUILD_NOT_IN_GUILD);
		return;
	}

	if (!CGuildConfig::Instance()->IsOpen(guild->GetLevel(), GuildSign))
	{
		roRes.set_errorcode(ERR_GUILD_OPENLIMIT);
		return;
	}

	CGuildCheckin* guildcheckin = guild->Get<CGuildCheckin>();
	guildcheckin->Update();
	/*
	UINT32 countlimit = CGuildConfig::Instance()->GetConfig(guild->GetLevel(), AllGuildSign);
	if (guildcheckin->GetCount() >= countlimit)
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_ALLCOUNT);
		return;
	}*/

	UINT32 type = roArg.type();
	ItemDesc itemconsume = GuildCheckinMgr::Instance()->GetConsume(type);
	ItemDesc itemgive = GuildCheckinMgr::Instance()->GetReward(type);
	/*
	if (itemgive.ItemID == 22)
	{
		guild->AddRoleContribute(role->GetID(), itemgive.ItemCount);
	}*/

	RpcM2G_NoticeGuildCheckin* pMsg = RpcM2G_NoticeGuildCheckin::CreateRpc();
	pMsg->m_oArg.set_type(roArg.type());
	pMsg->m_oArg.set_roleid(role->GetID());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pMsg->m_oArg.set_conitemid(itemconsume.itemID);
	pMsg->m_oArg.set_conitemnum(itemconsume.itemCount);
	pMsg->m_oArg.set_additemid(itemgive.itemID);
	pMsg->m_oArg.set_additemnum(itemgive.itemCount);
	role->SendMsgToGS(*pMsg);

	/*
	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();
	if (record->IsCheckin())
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_LIMIT);
		return;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		roRes.set_errorcode(ERR_GUILD_NOT_IN_GUILD);
		return;
	}
	if (!CGuildConfig::Instance()->IsOpen(guild->GetLevel(), GuildSign))
	{
		roRes.set_errorcode(ERR_GUILD_OPENLIMIT);
		return;
	}

	CGuildCheckin* guildcheckin = guild->Get<CGuildCheckin>();
	guildcheckin->Update();
	UINT32 countlimit = CGuildConfig::Instance()->GetConfig(guild->GetLevel(), AllGuildSign);
	if (guildcheckin->GetCount() >= countlimit)
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_ALLCOUNT);
		return;
	}

	UINT32 type = roArg.type();
	ItemDesc itemconsume = GuildCheckinMgr::Instance()->GetConsume(type);
	BagTakeItemTransition taketransition(role); 
	if (!taketransition.TakeItem(itemconsume.ItemID, itemconsume.ItemCount))
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_MONEY);
		return;
	}
	taketransition.NotifyClient();

	UINT32 process = GuildCheckinMgr::Instance()->GetPorcess(type);
	guildcheckin->Checkin(process, type, role->GetID());	

	UINT32 exp = GuildCheckinMgr::Instance()->GetExp(type);
	guild->AddExp(exp);

	record->SetCheckin(type);
	role->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::GUILDACTIVITY, 3, 1);

	ItemDesc itemgive = GuildCheckinMgr::Instance()->GetReward(type);
	BagGiveItemTransition givetransition(role); 
	givetransition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Checkin);
	givetransition.GiveItem(itemgive.ItemID, itemgive.ItemCount);
	givetransition.NotifyClient();

	GuildCheckinMgr::Instance()->NotifyProcess(NULL, guild);

	role->OnGuildActivity(2);
	roRes.set_errorcode(ERR_SUCCESS);
	*/
}

void RpcC2M_GuildCheckinNew::OnDelayReplyRpc(const GuildCheckinArg &roArg, GuildCheckinRes &roRes, const CUserData &roUserData)
{

	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return ;
	}

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		roRes.set_errorcode(ERR_GUILD_NOT_IN_GUILD);
		return;
	}

	CGuildCheckin* guildcheckin = guild->Get<CGuildCheckin>();
	if (guildcheckin == NULL)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	UINT32 process = GuildCheckinMgr::Instance()->GetPorcess(roArg.type());
	guildcheckin->Checkin(process, roArg.type(), role->GetID());	

	
	UINT32 countlimit = CGuildConfig::Instance()->GetConfig(guild->GetLevel(), AllGuildSign);
	if (guildcheckin->GetCount() <= countlimit)
	{
		std::map<UINT32, UINT32> guildReward;
		GuildCheckinMgr::Instance()->GetGuildReward(roArg.type(), guildReward);
		guild->AddGuildReward(guildReward);
	}
	
	GuildCheckinMgr::Instance()->NotifyProcess(NULL, guild);

	roRes.set_errorcode(ERR_SUCCESS);
}
