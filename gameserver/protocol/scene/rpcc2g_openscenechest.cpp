#include "pch.h"
#include "scene/rpcc2g_openscenechest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/stagemgr.h"
#include "scene/sceneconfig.h"
#include "gamelogic/bagtransition.h"
#include "config/itemdropconfig.h"

// generate by ProtoGen at date: 2015/10/9 15:23:20

RPC_SERVER_IMPLEMETION(RpcC2G_OpenSceneChest, OpenSceneChestArg, OpenSceneChestRes)

void RpcC2G_OpenSceneChest::OnCall(const OpenSceneChestArg &roArg, OpenSceneChestRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	UINT32 sceneID = roArg.sceneid();
	StageMgr *pStage = pRole->Get<StageMgr>();
	if (pStage->IsSceneChestOpened(sceneID))
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	if (pStage->GetRank(sceneID) <= 0)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneID);
	if (pSceneInfo == NULL)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	if (pSceneInfo->m_pConf->SceneChest == 0)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(pSceneInfo->m_pConf->SceneChest);
	if (NULL != item)
	{
		BagGiveItemTransition transition(pRole);
		transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Chest);
		transition.GiveItem(item->itemID, item->itemCount);
		transition.NotifyClient();
	}

	pStage->SetSceneChestOpened(roArg.sceneid());
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_OpenSceneChest::OnDelayReplyRpc(const OpenSceneChestArg &roArg, OpenSceneChestRes &roRes, const CUserData &roUserData)
{
}
