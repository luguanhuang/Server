#include "pch.h"
#include "opensystemhandler.h"
#include "table/PlayerLevelManager.h"
#include "rolenethandler.h"
#include "scene/scene.h"
#include "table/OpenSystemMgr.h"
#include "unit/role.h"
#include "activitymgr.h"
#include "activityrecord.h"
#include "role/ptcg2c_opensystemntf.h"
#include "define/systemiddef.h"
#include "gamelogic/payv2Mgr.h"
#include "titlerecord.h"
#include "utility/tlogger.h"
#include "scene/DragonExpConfig.h"
#include "gamelogic/superriskrecord.h"
#include "lotteryrecord.h"
#include "gamelogic/firstpassrecord.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/payconfig.h"
#include "findback/itemfindback.h"
#include "findback/findbackrecord.h"
#include "gamelogic/rolemisc.h"
#include "npcfeeling/npcfeelingmgr.h"

OpenSystemHandler OpenSystemHandler::GlobaHandler;


void OpenSystemHandler::OnLevelup(Role *poRole)
{
	std::vector<int> systemids;
	OpenSystemMgr::Instance()->CheckOpenSystem(poRole->GetLevel(), 0, systemids);
	if (systemids.empty())
	{
		return;
	}
	if (!DoOpenSystem(poRole, systemids))
	{
		return;
	}
}

void OpenSystemHandler::OnTaskComplete(Role *poRole, int TaskID)
{
	std::vector<int> systemids;
	OpenSystemMgr::Instance()->CheckOpenSystem(poRole->GetLevel(), TaskID, systemids);
	if (systemids.empty())
	{
		return;
	}
	if (!DoOpenSystem(poRole, systemids))
	{
		return;
	}
}

bool OpenSystemHandler::DoOpenSystem(Role* role, const std::vector<int> &systemids)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return false;
	}
	int OpenCount = 0;
	std::vector<UINT32> openIDs;
	for (UINT32 i = 0; i < systemids.size(); ++i)
	{
		UINT32 systemId = systemids[i];
		if (PayV2Mgr::Instance()->IsNeedOpenPaySystemID(role, systemids[i]))
		{
			continue;
		}
		if(!role->Get<CRoleSystem>()->IsSystemOpenedAtRole(systemids[i]))
		{
			role->Get<CRoleSystem>()->SetSystemOpened(systemids[i]);
			openIDs.push_back(systemids[i]);
			++OpenCount;
			LogInfo("role [%llu], open system [%d]", role->GetID(), systemids[i]);

			TOpenSystemLog log(role);
			log.m_nSystemID = systemids[i];
			log.Do();
		}
	}
	if (OpenCount <= 0)
	{
		return false;
	}	
	OpenSystemMgr::Instance()->NotifyClientSystemOpen(role, &openIDs, NULL);
	OpenSystemMgr::Instance()->NotifyMsSystemOpen(role, &openIDs);

	for (auto i = 0; i < openIDs.size(); ++i)
	{
		OnSystemOpened(role, openIDs[i]);
	}

	return true;
}

void OpenSystemHandler::OnSystemOpened(Role* role, INT32 nSystemID)
{
	switch(nSystemID)
	{
	case DragonExpSysID:
		{
			DragonExpConfig::Instance()->OnSystemOpened(role);
			break;
		}
	case SYS_SUPERRISK:
		{
			role->Get<CSuperRiskRecord>()->OnOpenSystem();
			break;
		}	
	case SYS_SPRITE_SHOP:
		{
			role->Get<LotteryRecord>()->OnSystemOpened(1);
			break;
		}
	case SysGoldClick:
		{
			role->Get<LotteryRecord>()->OnSystemOpened(2);
			break;
		}
	case SYS_FIRSTPASS:
		{
			role->Get<CFirstPassRecord>()->OnOpenSystem();
			break;
		}	
	case SysItemFindBack:
		{
			role->Get<CFindBackRecord>()->GetItemFindBackInfo().CheckOpen();
			break;
		}	
	case SYS_PET:
		{
			role->Get<PetSys>()->CheckOpen();
			break;
		}
	case SYS_MENTOR:
		{
			break;
		}	
	case SYS_EGame:
		{
			role->Get<RoleMisc>()->CheckEGameHint();
			break;
		}
	case SYS_XINYUEVIP:
		{
			role->Get<RoleMisc>()->CheckXinYueHint();
			break;
		}
	case SYS_NPCFEELING:
		{
			CNpcFeelingMgr::Instance()->OnSystemOpen(role);
			break;;
		}
	}
	// reward
	const std::vector<ItemDesc>* items = OpenSystemMgr::Instance()->GetReward(nSystemID);
	if (NULL != items)
	{
		BagGiveItemTransition trans(role);
		trans.SetReason(ItemFlow_OpenSystem, ItemFlow_OpenSystemGive);
		for (auto i = items->begin(); i != items->end(); ++i)
		{
			trans.GiveItem(*i);
		}
		trans.NotifyClient();
	}

	PayV2Mgr::Instance()->CheckPaySystemID(role, nSystemID);
}

