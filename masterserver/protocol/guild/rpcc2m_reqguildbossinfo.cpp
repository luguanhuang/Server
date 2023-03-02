#include "pch.h"
#include "guild/rpcc2m_reqguildbossinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"

// generate by ProtoGen at date: 2016/9/5 16:23:13

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildBossInfo, AskGuildBossInfoArg, AskGuildBossInfoRes)

void RpcC2M_ReqGuildBossInfo::OnCall(const AskGuildBossInfoArg &roArg, AskGuildBossInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn << "RpcC2G_AskGuildBossInfo: pRole is NULL, session id:" << m_sessionID << END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	UINT32 nextBossId = CGuildBossMgr::Instance()->getNextBossIndex(pGuild); 
	roRes.set_bossid(nextBossId);

	UINT32 needKillBossId = CGuildBossMgr::Instance()->getNeedBoosIndex(pGuild);
	if (needKillBossId != 0)
	{
		roRes.set_needkillbossid(needKillBossId);
	}

	UINT32 needGuildLvl = CGuildBossMgr::Instance()->getNeedGuildLvl(pGuild);
	if (needGuildLvl != 0)
	{
		roRes.set_needguildlvl(needGuildLvl);
	}

	if (nextBossId > pGuild->GetBossIndex())
	{
		roRes.set_isfirstkill(true);
	}
	else
	{
		roRes.set_isfirstkill(false);
	}

	if (pGuild->isLeader(pRole->GetID()) || pGuild->isViceLeader(pRole->GetID()))
	{
		roRes.set_isleader(true);
	}
	else
	{
		roRes.set_isleader(false);
	}
}

void RpcC2M_ReqGuildBossInfo::OnDelayReplyRpc(const AskGuildBossInfoArg &roArg, AskGuildBossInfoRes &roRes, const CUserData &roUserData)
{
}
