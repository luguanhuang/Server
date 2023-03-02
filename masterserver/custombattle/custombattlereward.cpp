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

void CustomBattleHandleReward::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (!roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}

	CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(req.uid);
	if (NULL != battle)
	{
		if (KKSG::CustomBattle_Reward == req.op)
		{
			errorcode = battle->CanReward(req.roleid);
			if (KKSG::ERR_SUCCESS == errorcode)	
			{
				std::vector<ItemDesc> rewards = CCustomBattleConfig::Instance()->GetCustomReward(battle->GetConfigId(), battle->GetRank(req.roleid));
				battle->DoReward(req.roleid);
				m_manager->DelayGiveItem(req, rewards);
			}
		}
		else if (KKSG::CustomBattle_ClearCD == req.op)
		{
			if (!battle->IsCD(req.roleid))
			{
				std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetClearCustomCD(battle->GetConfigId(), battle->GetRank(req.roleid));
				errorcode = m_manager->DelayTakeItem(req, items);
			}
		}
	}

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleReward::HandleGsReply(const CustomBattleReq& req)
{
	if (KKSG::CustomBattle_ClearCD == req.op)
	{
		CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(req.uid);
		if (NULL != battle)
		{
			battle->ClearCD(req.roleid);
		}
	}
	else if (KKSG::CustomBattle_Reward == req.op)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
		if (NULL != role)
		{
			role->HintNotify(CustomBattle_Custom, true);
		}
	}
	DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
	return;
}
