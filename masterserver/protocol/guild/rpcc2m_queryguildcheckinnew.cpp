#include "pch.h"
#include "guild/rpcc2m_queryguildcheckinnew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcheckin.h"
#include "table/guildConfig.h"
#include "guild/rpcm2g_reqguildcheckin.h"

// generate by ProtoGen at date: 2016/9/16 21:06:11

RPC_SERVER_IMPLEMETION(RpcC2M_QueryGuildCheckinNew, QueryGuildCheckinArg, QueryGuildCheckinRes)

void RpcC2M_QueryGuildCheckinNew::OnCall(const QueryGuildCheckinArg &roArg, QueryGuildCheckinRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (guild == NULL)
	{
		return;
	}
	if (!CGuildConfig::Instance()->IsOpen(guild->GetLevel(), GuildSign))
	{
		return;
	}

	RpcM2G_ReqGuildCheckin* pMsg = RpcM2G_ReqGuildCheckin::CreateRpc();
	pMsg->m_oArg.set_roleid(pRole->GetID());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pRole->SendMsgToGS(*pMsg);

	/*
	CGuildCheckin* guildcheckin = guild->Get<CGuildCheckin>();
	guildcheckin->Update();

	roRes.set_allcount(guild->GetMemberCount());
	roRes.set_checkincount(guildcheckin->GetCount());

	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();

	roRes.set_checkin(record->GetCheckin());
	roRes.set_boxmask(record->GetBoxMask());

	GuildCheckinMgr::Instance()->NotifyProcess(role, NULL);
	*/
}

void RpcC2M_QueryGuildCheckinNew::OnDelayReplyRpc(const QueryGuildCheckinArg &roArg, QueryGuildCheckinRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (guild == NULL)
	{
		return;
	}

	CGuildCheckin* guildcheckin = guild->Get<CGuildCheckin>();
	guildcheckin->Update();

	roRes.set_allcount(guild->GetMemberCount());
	roRes.set_checkincount(guildcheckin->GetCount());
	roRes.set_checkin(roUserData.m_dwUserData);
	roRes.set_boxmask(*((UINT32*)roUserData.m_pUserPtr));
	GuildCheckinMgr::Instance()->NotifyProcess(pRole, NULL);
}
