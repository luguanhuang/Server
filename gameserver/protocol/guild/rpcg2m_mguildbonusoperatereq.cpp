#include "pch.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"

// generate by ProtoGen at date: 2016/9/16 0:27:41

RPC_CLIENT_IMPLEMETION(RpcG2M_MGuildBonusOperateReq, MGuildBonusOperateReqArg, MGuildBonusOperateReqRes)

void RpcG2M_MGuildBonusOperateReq::OnReply(const MGuildBonusOperateReqArg &roArg, const MGuildBonusOperateReqRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = roRes.error();
	UINT32 temp = 0;
	switch(roArg.optype())
	{
	case GBonus_OP_GetBonusReward:
		{
			data.m_pUserPtr = (void*)&roRes;
		}
		break;
	case GBonus_OP_GetBonusDetailInfo:
		if (roRes.has_detailinfo())
		{
			data.m_pUserPtr = (void*)&roRes.detailinfo();
		}
		break;
	case GBonus_OP_GetCheckInBonusInfo:
		if (roRes.has_checkinbonusinfores())
		{
			data.m_pUserPtr = (void*)&roRes.checkinbonusinfores();
		}
		break;
	case GBonus_OP_GetBonusList:
		if (roRes.has_bonuslistres())
		{
			data.m_pUserPtr = (void*)&roRes.bonuslistres();
		}
		break;
	case GBonus_OP_GetBonusSend:
		if (roRes.has_getbonusreward())
		{
			data.m_pUserPtr = (void*)&roRes.getbonusreward();
		}
		break;
	case GBonus_OP_GetBonusAskFor:
		break;
	case GBonus_OP_GetBonusThank:
		if (roRes.has_thankbonusres())
		{
			data.m_pUserPtr = (void*)&roRes.thankbonusres();
		}
		break;
	case GBonus_OP_GetLastCheckinBonusTime:
		{
			temp = roRes.last_checkin_bonus_time();
			data.m_pUserPtr = (void*)&temp;
		}
		break;
	case GBonus_OP_GetBonusLeft:
		if (roRes.has_leftbonus())
		{
			data.m_pUserPtr = (void*)&roRes.leftbonus();
		}
		break;
	default:
		break;
	}

	CRpc::ReplyDelayRpc(roArg.rpcid(), data);
}

void RpcG2M_MGuildBonusOperateReq::OnTimeout(const MGuildBonusOperateReqArg &roArg, const CUserData &roUserData)
{
}
