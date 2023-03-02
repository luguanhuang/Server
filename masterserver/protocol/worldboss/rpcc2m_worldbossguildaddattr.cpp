#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "worldboss/worldbossmgr.h"
#include "table/globalconfig.h"
#include "network/gslink.h"
#include "worldboss/rpcc2m_worldbossguildaddattr.h"
#include "worldboss/rpcm2g_worldbossguildaddattrgs.h"
#include "worldboss/ptcm2g_worldbossguildaddattrsyncgsntf.h"

// generate by ProtoGen at date: 2017/4/10 14:49:54

RPC_SERVER_IMPLEMETION(RpcC2M_WorldBossGuildAddAttr, WorldBossGuildAddAttrArg, WorldBossGuildAddAttrRes)


static UINT32 GetGuildCost(UINT32 count)
{
	UINT32 size = GetGlobalConfig().WorldBossGuildConsume.size();
	return GetGlobalConfig().WorldBossGuildConsume[count >= size ? (size - 1) : (count - 1)];
}

void RpcC2M_WorldBossGuildAddAttr::OnCall(const WorldBossGuildAddAttrArg &roArg, WorldBossGuildAddAttrRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID());
	if (INVALID_GUILDID == guildid)
	{
		roRes.set_errorcode(ERR_GUILD_NOT_IN_GUILD);
		return;
	}

	GuildAttr& guildattr = WorldBossMgr::Instance()->GetGuildAttr(guildid);

	SSInfo<<"client count can match, client count:"<<roArg.count()<<" server count:"<<guildattr.count<<" delay count:"<<guildattr.m_delayrpcs.size()
		<<" role:"<<role->GetID()<<" guild:"<<guildid<<END;

	if (0 != GetGlobalConfig().WorldBossGuildAttrCount && (guildattr.count + guildattr.m_delayrpcs.size()) > GetGlobalConfig().WorldBossGuildAttrCount)
	{
		roRes.set_errorcode(ERR_WORLDBOSSGUILD_COUNTLIMIT);
		return;
	}

	///> 没有鼓舞过
	UINT32 delayid = 0;
	if (0 == roArg.count() && guildattr.isempty())
	{
		delayid = DelayRpc();
	}
	else
	{
		///> 客户端扣的钱和服务器一致
		if (GetGuildCost(roArg.count() + 1) == GetGuildCost(guildattr.count + 1 + guildattr.m_delayrpcs.size()))
		{
			delayid = DelayRpc();
		}
		else
		{
			if (guildattr.m_delayrpcs.empty() && roArg.count() != guildattr.count)
			{
				///> 客户端和服务器不一致，需要校验
				PtcM2G_WorldBossGuildAddAttrSyncGsNtf ntf;
				ntf.m_Data.set_guildid(guildid);
				ntf.m_Data.set_count(guildattr.count);
				ntf.m_Data.set_roleid(role->GetID());
				role->SendMsgToGS(ntf);
			}
		}
	}
	
	if (0 != delayid)
	{
		RpcM2G_WorldBossGuildAddAttrGs* rpc = RpcM2G_WorldBossGuildAddAttrGs::CreateRpc();
		rpc->m_oArg.set_count(roArg.count());
		rpc->m_oArg.set_delayid(delayid);
		rpc->m_oArg.set_roleid(role->GetID());
		rpc->m_oArg.set_guildid(guildid);
		role->SendMsgToGS(*rpc);

		guildattr.m_delayrpcs.insert(delayid);

		SSInfo<<"push delayid:"<<delayid<<" guildid:"<<guildid<<" roleid:"<<role->GetID()<<END;
	}
	else
	{
		roRes.set_errorcode(ERR_WORLDBOSSGUILD_UNMATCH);
	}
}

void RpcC2M_WorldBossGuildAddAttr::OnDelayReplyRpc(const WorldBossGuildAddAttrArg &roArg, WorldBossGuildAddAttrRes &roRes, const CUserData &roUserData)
{
	WorldBossGuildAddAttrGsArg* lastarg = (WorldBossGuildAddAttrGsArg*)roUserData.m_pUserPtr; 
	if (NULL == lastarg)
	{
		SSError<<"lastarg is NULL"<<END;
		return;
	}
	UINT32 errorcode = roUserData.m_dwUserData;

	SSInfo<<"delayid:"<<lastarg->delayid()<<" error:"<<errorcode<<" guildid:"<<lastarg->guildid()<<" roleid:"<<lastarg->roleid()<<END;

	GuildAttr& guildattr = WorldBossMgr::Instance()->GetGuildAttr(lastarg->guildid());
	auto i = guildattr.m_delayrpcs.find(lastarg->delayid());
	if (i == guildattr.m_delayrpcs.end())
	{
		SSError<<"not find delayid:"<<lastarg->delayid()<<END;
	}
	else
	{
		if (0 == errorcode)
		{
			++guildattr.count;
			///> 广播所有的GS
			PtcM2G_WorldBossGuildAddAttrSyncGsNtf ntf;
			ntf.m_Data.set_guildid(lastarg->guildid());
			ntf.m_Data.set_count(guildattr.count);
			GSLink::Instance()->SendToAllLine(ntf);
		}
		guildattr.m_delayrpcs.erase(i);
	}

	roRes.set_errorcode((KKSG::ErrorCode)errorcode);
}
