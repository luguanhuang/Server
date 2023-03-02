#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "role/rolesummarymgr.h"

KKSG::ErrorCode CustomBattleMgr::DelayTakeItem(const CustomBattleReq& req, const std::vector<ItemDesc>& items)
{
	RpcM2G_CustomBattleGsOp* gsop = RpcM2G_CustomBattleGsOp::CreateRpc();

	KKSG::CustomBattleOp op;
	if (KKSG::CustomBattle_Create == req.op)
	{
		op = KKSG::CustomBattle_DoCreate;
	}
	else if (KKSG::CustomBattle_Join == req.op)
	{
		op = KKSG::CustomBattle_DoJoin;
	}
	else if (KKSG::CustomBattle_ClearCD == req.op)
	{
		op = KKSG::CustomBattle_ClearCD;
	}
	else
	{
		SSWarn<<"invalid op:"<<req.op<<END;
		return KKSG::ERR_FAILED;
	}

	if (items.empty())
	{
		SSWarn<<"items is empty, configid:"<<req.conf.configid()<<END;
		return KKSG::ERR_FAILED;
	}
	gsop->m_oArg.set_op(op);
	gsop->m_oArg.set_roleid(req.roleid);
	gsop->m_oArg.set_delayid(req.delayid);
	gsop->m_oArg.set_uid(req.uid);
	*gsop->m_oArg.mutable_config() = req.conf;
	gsop->m_oArg.set_issystem(CCustomBattleConfig::Instance()->IsSystem(req.uid));

	std::stringstream ss;
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		KKSG::ItemBrief* item = gsop->m_oArg.add_itemcost();	
		item->set_itemid(i->itemID);
		item->set_itemcount(i->itemCount);

		ss<<i->itemID<<":"<<i->itemCount<<",";
	}

	LogInfo("roleid:%llu op:%d uid:%llu take item:%s", req.roleid, req.op, req.uid, ss.str().c_str());

	CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
	if (NULL != role)
	{
		role->SendMsgToGS(*gsop);
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		return KKSG::ERR_FAILED;
	}
}

void CustomBattleMgr::DelayGiveItem(const CustomBattleReq& req, const std::vector<ItemDesc>& items)
{
	RpcM2G_CustomBattleGsOp* gsop = RpcM2G_CustomBattleGsOp::CreateRpc();

	gsop->m_oArg.set_op(req.op);
	gsop->m_oArg.set_roleid(req.roleid);
	gsop->m_oArg.set_delayid(req.delayid);
	gsop->m_oArg.set_uid(req.uid);
	*gsop->m_oArg.mutable_config() = req.conf;
	gsop->m_oArg.set_issystem(CCustomBattleConfig::Instance()->IsSystem(req.uid));

	std::stringstream ss;
	for (auto i = items.begin(); i != items.end(); ++i)
	{
		KKSG::ItemBrief* item = gsop->m_oArg.add_itemcost();	
		item->set_itemid(i->itemID);
		item->set_itemcount(i->itemCount);

		ss<<i->itemID<<":"<<i->itemCount<<",";
	}
	
	LogInfo("roleid:%llu op:%d uid:%llu give item:%s", req.roleid, req.op, req.uid, ss.str().c_str());

	CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
	if (NULL != role)
	{
		role->SendMsgToGS(*gsop);
	}
}

bool CustomBattleMgr::HandleGsReply(const CustomBattleReq& req)
{
	switch(req.op)
	{
	case KKSG::CustomBattle_DoCreate:
		{
			if (CustomBattle::IsLocalCreate(req.conf))
			{
				m_create.HandleGsReply(req);
			}
			else
			{
				m_createworld.HandleGsReply(req);
			}
			break;
		}
	case KKSG::CustomBattle_DoJoin:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_joinsystem.HandleGsReply(req);
			}
			else
			{
				SSWarn<<"invalid req, op:"<<req.op<<" roleid"<<req.roleid<<END;
			}
			break;
		}
	case KKSG::CustomBattle_ClearCD:
	case KKSG::CustomBattle_Reward:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_rewardsystem.HandleGsReply(req);
			}
			else if (IsLocal(req.uid))
			{
				m_reward.HandleGsReply(req);
			}
			else
			{
				m_rewardworld.HandleGsReply(req);
			}
			break;
		}
	default:
		{
			SSWarn<<"invalid req, op:"<<req.op<<" roleid"<<req.roleid<<END;
			return false;
		}
		break;
	}
	return true;
}
