#include "pch.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildbonus.h"

// generate by ProtoGen at date: 2016/9/16 0:27:41

RPC_SERVER_IMPLEMETION(RpcG2M_MGuildBonusOperateReq, MGuildBonusOperateReqArg, MGuildBonusOperateReqRes)

void RpcG2M_MGuildBonusOperateReq::OnCall(const MGuildBonusOperateReqArg &roArg, MGuildBonusOperateReqRes &roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (!pGuild)
	{
		return;
	}
	auto guildBonusMgr = pGuild->Get<GuildBonusMgr>();
	KKSG::GuildRecord record;
	if (roArg.has_roleguildrecord())
	{
		record.CopyFrom(roArg.roleguildrecord());
	}
	switch(roArg.optype())
	{
	case GBonus_OP_GetBonusReward:
		if (roArg.has_getbonusrewardarg())
		{
			guildBonusMgr->getBonusReward(role, roArg.getbonusrewardarg().bonusid(), roRes, &record);
		}
		break;
	case GBonus_OP_GetBonusDetailInfo:
		if (roArg.has_detailinfoarg())
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			guildBonusMgr->getBonusDetailInfo(role, roArg.detailinfoarg().bonusid(), *roRes.mutable_detailinfo(), &record);
		}
		break;
	case GBonus_OP_GetCheckInBonusInfo:
		if (roArg.has_checkinbonusarg())
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			guildBonusMgr->GetCheckInBonusInfo(role, *roRes.mutable_checkinbonusinfores(), &record);
		}
		break;
	case GBonus_OP_GetBonusList:
		if (roArg.has_bonuslistarg())
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			guildBonusMgr->getBonusList(role, *roRes.mutable_bonuslistres(), &record);
		}
		break;
	case GBonus_OP_GetBonusSend:
		if (roArg.has_sendbonusarg())
		{
			UINT32 error = guildBonusMgr->SendCheckInBonus(role);
			roRes.set_error((KKSG::ErrorCode)error);
		}
		break;
	case GBonus_OP_GetBonusAskFor:
		{
			if (!guildBonusMgr->IsInCheckInBonusTime())
			{
				roRes.set_error(KKSG::ERR_GUILD_CHECKINBONUS_TIMEERROR);
				return;
			}

			roRes.set_error((KKSG::ErrorCode)KKSG::ERR_SUCCESS);
		}
		break;
	case GBonus_OP_GetBonusThank:
		if(roArg.has_thankbonusarg())
		{
			UINT32 error = guildBonusMgr->ThankForBonus(role, roArg.thankbonusarg().bonusid());
			roRes.set_error((KKSG::ErrorCode)error);
			roRes.mutable_thankbonusres()->set_errorcode((KKSG::ErrorCode)error);
		}
		break;
	case GBonus_OP_GetLastCheckinBonusTime:
		{
			roRes.set_error((KKSG::ErrorCode)KKSG::ERR_SUCCESS);
			roRes.set_last_checkin_bonus_time(guildBonusMgr->GetLastCheckinBonusTime());
		}
		break;
	case GBonus_OP_GetBonusLeft:
		if (roArg.has_leftbonusarg())
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			guildBonusMgr->GetBonusLeftList(role, roRes, &record);
		}
		break;
	default:
		break;
	}
}

void RpcG2M_MGuildBonusOperateReq::OnDelayReplyRpc(const MGuildBonusOperateReqArg &roArg, MGuildBonusOperateReqRes &roRes, const CUserData &roUserData)
{
}
