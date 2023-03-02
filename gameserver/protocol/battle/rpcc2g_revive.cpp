#include "pch.h"
#include "battle/rpcc2g_revive.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "battle/ptcg2c_revivenotify.h"
#include "scene/scenemortuary.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "scene/sceneconfig.h"
#include "cvsreader.h"
#include "unit/attributewatcher.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "gamelogic/payv2Record.h"
#include "scene/scenedragon.h"

// generate by ProtoGen at date: 2015/7/13 19:17:19

RPC_SERVER_IMPLEMETION(RpcC2G_Revive, ReviveArg, ReviveRes)

void RpcC2G_Revive::OnCall(const ReviveArg &roArg, ReviveRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	Scene *pScene = pRole->GetCurrScene();
	if (NULL == pScene)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	if (pScene->IsSyncScene())
	{
		if (!pRole->IsDead())
		{
			roRes.set_result(KKSG::ERR_SUCCESS);
			return;
		}
	}

	if (KKSG::SCENE_DRAGON == pScene->GetSceneType() && NULL != pScene->GetSceneDragon())
	{
		if (!pScene->GetSceneDragon()->CanRevive())
		{
			roRes.set_result(KKSG::ERR_REVIVE_MAXNUM);
			return;
		}
	}

	SceneMortuary *pMortuary = pScene->GetMortuary();
	SceneInfo* pSceneInfo = SceneConfig::Instance()->FindSceneInfo(pScene->GetSceneTemplateID());
	UINT32 reviveCount = pMortuary->GetReviveLimitCount(pRole->GetID());
	if(reviveCount >= pSceneInfo->m_pConf->ReviveNumb)
	{
		roRes.set_result(KKSG::ERR_REVIVE_MAXNUM);
		return;
	}
	UINT32 itemrevivecount = pMortuary->GetItemReviveCount(pRole->GetID());

	UINT32 costItemID = 0;
	UINT32 costNum = 0;
	bool islimit = true;
	bool iscost = true;
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	if (roArg.type() == KKSG::ReviveItem)
	{
		UINT32 vecSize = pSceneInfo->m_pConf->ReviveCost.size();
		if (0 != vecSize)
		{
			UINT32 index = itemrevivecount >= vecSize - 1 ? vecSize - 1 : itemrevivecount;
			costItemID = pSceneInfo->m_pConf->ReviveCost[index][0];
			costNum = pSceneInfo->m_pConf->ReviveCost[index][1]; 
		}
		else
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		errorcode = KKSG::ERR_REVIVE_ITEMLIMIT;
	}
	else if (roArg.type() == KKSG::ReviveMoney)
	{
		UINT32 vecSize = pSceneInfo->m_pConf->ReviveMoneyCost.size();
		if (0 != vecSize)
		{
			UINT32 index = itemrevivecount >= vecSize - 1 ? vecSize - 1 : itemrevivecount;
			costItemID = pSceneInfo->m_pConf->ReviveMoneyCost[index][0];
			costNum = pSceneInfo->m_pConf->ReviveMoneyCost[index][1]; 
		}
		else
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		errorcode = KKSG::ERR_REVIVE_MONEYLIMIT;
	}
	else if (roArg.type() == KKSG::ReviveSprite)
	{
		if (pScene->IsSyncScene())
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		islimit = roArg.has_clientinfo() ? roArg.clientinfo().islimit() : true;
		iscost = false;
	}
	else if (roArg.type() == KKSG::ReviveVIP)
	{
		if (pScene->GetSceneInfo()->m_pConf->CanVIPRevive)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if (!pMortuary->CanVipRevive(pRole->GetID()))
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if (!pRole->Get<CPayV2Record>()->HasReviveCount())
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		pRole->Get<CPayV2Record>()->UpateReviveCount();
		iscost = false;
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	if (iscost)
	{
		BagTakeItemTransition transition(pRole);
		transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Revive);
		if (transition.TakeItem(costItemID, costNum))
		{	
			transition.NotifyClient();
		}
		else
		{
			transition.RollBack();
			roRes.set_result(errorcode);
			return;
		}	
	}
	
	pMortuary->IncReviveCount(pRole->GetRoleID(), roArg.type(), islimit);

	pRole->Revive();	

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_Revive::OnDelayReplyRpc(const ReviveArg &roArg, ReviveRes &roRes, const CUserData &roUserData)
{
}
