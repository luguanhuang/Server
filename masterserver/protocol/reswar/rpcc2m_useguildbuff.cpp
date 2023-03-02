#include "pch.h"
#include "reswar/rpcc2m_useguildbuff.h"
#include "reswar/ptcm2c_guildbuffcdparamntf.h"
#include "reswar/guildbuffmgr.h"
#include "reswar/reswarmgr.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "util/gametime.h"
#include "reswar/reswarpvemgr.h"

// generate by ProtoGen at date: 2016/11/2 20:24:36

RPC_SERVER_IMPLEMETION(RpcC2M_UseGuildBuff, UseGuildBuffArg, UseGuildBuffRes)

void RpcC2M_UseGuildBuff::OnCall(const UseGuildBuffArg &roArg, UseGuildBuffRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(!pRole)
	{
		roRes.set_error(ERR_UNKNOWN);
		SSError << "Cannot Find Role, RoleID:" << m_sessionID << END;
		return; 
	}
	UINT32 bState = ResWarMgr::Instance()->GetState();
	if (!bState)
	{
		roRes.set_error(ERR_RESWAR_ACTIVITY);
		return;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild) 
	{
		roRes.set_error(ERR_GUILDBUFF_GUILD);
		return ;
	}
	GuildTeam *pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(pGuild->GetID());
	if (!pGuildTeam) 
	{
		roRes.set_error(ERR_RESWAR_GROUP);
		return;
	}
	GuildPosition nPosition = pGuild->GetPosition(pRole->GetID());
	if (nPosition==GPOS_LEADER||nPosition==GPOS_VICELEADER)
	{
		if(GameTime::GetTime() > pGuildTeam->nNextUseBuffTime)
		{
			GuildBuffTable::RowData *pRowData =  ResWarAwardMgr::Instance()->GetGuildBuffItemID(roArg.itemid());
			if (pRowData)
			{
				bool bRet =  ResWarMgr::Instance()->UseBuffItem(pGuild->GetID(), roArg.itemid());
				if(bRet)
				{
					GuildBuffMgr::Instance()->AddBuff(pGuild->GetID(), roArg.guildid(), pRowData->id);
					GuildBuffMgr::Instance()->AddRoleID(pRole->GetID());
					UINT32 nUseBuffCD = ResWarMgr::Instance()->GetRiteTime();
					pGuildTeam->nNextUseBuffTime = GameTime::GetTime() + nUseBuffCD;
					roRes.set_cd(nUseBuffCD);
					ResWarMgr::Instance()->SendGuildBuffCD(pGuild,pRole,nUseBuffCD);					
				}else
				{
					roRes.set_error(ERR_GUILDBUFF_ITEM);
				}
			}

		}else
		{
			roRes.set_error(ERR_GUILDBUFF_CD);
		}
	}else
	{
		roRes.set_error(ERR_GUILDBUFF_POS);
	}
}

void RpcC2M_UseGuildBuff::OnDelayReplyRpc(const UseGuildBuffArg &roArg, UseGuildBuffRes &roRes, const CUserData &roUserData)
{
}
