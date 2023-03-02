#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"

// reward
// flow: client->ms ms->world ms->gs

// clear cd
// flow: client->ms ms->world ms->gs ms->world

void CustomBattleHandleRewardWorld::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (!roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}
	if (KKSG::CustomBattle_Reward == req.op)
	{
		// trans to world
		errorcode = m_manager->TransReqToWorld(req);
	}
	else if (KKSG::CustomBattle_ClearCD == req.op)
	{
		// trans to world
		errorcode = m_manager->TransReqToWorld(req);
	}

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}


void CustomBattleHandleRewardWorld::HandleGsReply(const CustomBattleReq& req)
{
	if (KKSG::CustomBattle_Reward == req.op)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
		if (NULL != role)
		{
			role->HintNotify(CustomBattle_Custom, true);
		}
		SSInfo<<"roleid:"<<req.roleid<<" uid:"<<req.uid<<" rewarded"<<END;
		DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
	}
	else if (KKSG::CustomBattle_ClearCD == req.op)
	{
		// trans to world
		CustomBattleReq tempreq = req;
		tempreq.op = KKSG::CustomBattle_DoClearCD;
		ErrorCode errorcode = m_manager->TransReqToWorld(tempreq);
		if (KKSG::ERR_SUCCESS != errorcode)
		{
			ErrorNotify(errorcode, req.delayid);
		}
	}
	else
	{
		SSWarn<<"invalid op:"<<req.op<<" roleid:"<<req.roleid<<" uid:"<<req.uid<<END;
	}
}

void CustomBattleHandleRewardWorld::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}
	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (roleinfo->GetJoinId() == req.uid)
	{
		if (KKSG::CustomBattle_Reward == req.op)
		{
			if (info.has_rank())
			{
				std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetCustomReward(roleinfo->GetJoinConfigId(), info.rank());
				m_manager->DelayGiveItem(req, items);
				temperrorcode = KKSG::ERR_SUCCESS;
			}
		}
		else if (KKSG::CustomBattle_ClearCD == req.op)
		{
			// trans to gs
			if (info.has_rank())
			{
				std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetClearCustomCD(roleinfo->GetJoinConfigId(), info.rank());
				temperrorcode = m_manager->DelayTakeItem(req, items); 
			}
		}
		else if (KKSG::CustomBattle_DoClearCD == req.op)
		{
			DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
			temperrorcode = KKSG::ERR_SUCCESS;
		}
	}

	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
		return;
	}
}
