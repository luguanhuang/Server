#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"

void CustomBattleHandleRewardSystem::HandleClientReq(const CustomBattleReq& req)
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
		errorcode = roleinfo->CanSystemReward(req.uid);
		if (KKSG::ERR_SUCCESS == errorcode)
		{
			std::vector<ItemDesc> rewards = CCustomBattleConfig::Instance()->GetSystemReward((UINT32)req.uid, roleinfo->GetWin(req.uid));
			m_manager->DelayGiveItem(req, rewards);
			roleinfo->ClearSystem(req.uid);

			CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
			if (NULL != role)
			{
				role->HintNotify(CustomBattle_System, true);
			}
		}
	}
	else if (KKSG::CustomBattle_ClearCD == req.op)
	{
		if (roleinfo->IsSystemCD(req.uid))
		{
			std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetClearSystemCD((UINT32)req.uid, roleinfo->GetWin(req.uid));
			errorcode = m_manager->DelayTakeItem(req, items);
		}
	}

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}


void CustomBattleHandleRewardSystem::HandleGsReply(const CustomBattleReq& req)
{
	if (KKSG::CustomBattle_ClearCD == req.op)
	{
		SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
		roleinfo->ClearSystemRewardCD(req.uid);
	}
	DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
	return;
}
