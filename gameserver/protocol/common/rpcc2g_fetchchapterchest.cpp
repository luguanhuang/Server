#include "pch.h"
#include "common/rpcc2g_fetchchapterchest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/stagemgr.h"
#include "scene/sceneconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/itemconfig.h"
#include "config/itemdropconfig.h"
#include "gamelogic/stagerankmgr.h"

// generate by ProtoGen at date: 2015/9/25 14:08:23

RPC_SERVER_IMPLEMETION(RpcC2G_FetchChapterChest, FetchChapterChestArg, FetchChapterChestRes)

void RpcC2G_FetchChapterChest::OnCall(const FetchChapterChestArg &roArg, FetchChapterChestRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}

	StageMgr *pStage = role->Get<StageMgr>();
	if (pStage->IsFetchedChapterChest(roArg.chapterid(), roArg.chestid()))
	{
		roRes.set_result(KKSG::ERR_CHAPTERCHEST_ALREADY_FETCHED);
		return;
	}

	std::vector<UINT32> *pSceneIDS = SceneConfig::Instance()->GetChapterScene(roArg.chapterid());
	if (pSceneIDS == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	ChapterChestInfo *pInfo = SceneConfig::Instance()->GetChapterChestInfo(roArg.chapterid(), roArg.chestid());
	if (pInfo == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	int star = 0;
	for (auto i = pSceneIDS->begin(); i != pSceneIDS->end(); ++i)
	{
		int t = pStage->GetRank(*i);
		switch (t)
		{
		case RANK_S:
			star += 1;
			break;
		case RANK_SS_1:
		case RANK_SS_2:
			star += 2;
			break;
		case RANK_SSS:
			star += 3;
			break;
		default:
			break;
		}
	}
	
	if (star < pInfo->startNeed)
	{
		roRes.set_result(KKSG::ERR_CHAPTERCHEST_NEEDSTAR);
		return;
	}

	const ItemDesc* item = ItemDropConfig::Instance()->GetRandomDrop(pInfo->dropID);
	if (NULL != item)
	{
		BagGiveItemTransition transition(role);
		transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Chest);
		transition.GiveItem(item->itemID, item->itemCount);
		transition.NotifyClient();
	}

	pStage->SetFetchedChapterChest(roArg.chapterid(), roArg.chestid());
	roRes.set_result(KKSG::ERR_SUCCESS);
	return;
}

void RpcC2G_FetchChapterChest::OnDelayReplyRpc(const FetchChapterChestArg &roArg, FetchChapterChestRes &roRes, const CUserData &roUserData)
{
}
