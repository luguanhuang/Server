#include "pch.h"
#include "npcfeeling/rpcm2g_npcflreqm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "npcfeeling/npcfeelingrecord.h"
#include "npcfeeling/npcfeelingmgr.h"

// generate by ProtoGen at date: 2017/9/11 10:37:15

RPC_SERVER_IMPLEMETION(RpcM2G_NpcFlReqM2G, NpcFlArgM2G, NpcFlResM2G)

void RpcM2G_NpcFlReqM2G::OnCall(const NpcFlArgM2G &roArg, NpcFlResM2G &roRes)
{
	auto& c2mArg = roArg.c2marg();
	auto& c2mRes = *roRes.mutable_c2mres();
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2mRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	auto pRecord = pRole->Get<CNpcFeelingRecord>();
	pRecord->Update();//只是为了处理GM调时间

	ErrorCode ret = KKSG::ERR_SUCCESS;

	switch(c2mArg.reqtype())
	{
	case NPCFL_GIVE_GIFT:
		{
			ret = CNpcFeelingMgr::Instance()->HandleGiveNpcItem(pRole, roArg, roRes);
			if(ERR_SUCCESS == ret)
			{
				pRecord->FillOneNpc(c2mRes, c2mArg.npcid());
				pRecord->FillClientCount(c2mRes);
			}
		}
		break;
	case NPCFL_EXCHANGE:
		{
			ret = CNpcFeelingMgr::Instance()->HandleExchange(pRole, c2mArg.npcid(), c2mArg.role2npc(), c2mArg.npc2role());
			if(ERR_SUCCESS == ret)
			{
				pRecord->FillOneNpc(c2mRes, c2mArg.npcid());
			}
		}
		break;
	case NPCFL_NPC_LEVEL_UP:
		{
			ret = CNpcFeelingMgr::Instance()->HandleUpNpcLevel(pRole, c2mArg.npcid());
			if(ERR_SUCCESS == ret)
			{
				pRecord->FillOneNpc(c2mRes, c2mArg.npcid());
			}
		}
		break;
	case NPCFL_UNITE_ACT:
		{
			ret = CNpcFeelingMgr::Instance()->HandleNpcUinteLevel(pRole, c2mArg.uniteid());
			if(ERR_SUCCESS == ret)
			{
				pRecord->FillUniteList(c2mRes);
			}
		}
		break;
	case NPCFL_BUY_GIFT_COUNT:
		{
			ret = CNpcFeelingMgr::Instance()->HandleBuyGiveGiftCount(pRole);	
			if(ERR_SUCCESS == ret)
			{
				pRecord->FillClientCount(c2mRes);
			}
		}
		break;
	case NPCFL_BASE_DATA:
		{
			CNpcFeelingMgr::Instance()->FillBaseData(pRole, c2mRes);
		}
		break;
	default:
		break;
	}

	c2mRes.set_errorcode(ret);
}

void RpcM2G_NpcFlReqM2G::OnDelayReplyRpc(const NpcFlArgM2G &roArg, NpcFlResM2G &roRes, const CUserData &roUserData)
{
}
