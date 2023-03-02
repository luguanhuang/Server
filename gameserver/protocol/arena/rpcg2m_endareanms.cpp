#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/arenamgr.h"
#include "gamelogic/arenarecord.h"
#include "scene/scene.h"
#include "scene/battleresultlistener.h"
#include "arena/rpcg2m_endareanms.h"
#include "gamelogic/arenarewardmgr.h"

// generate by ProtoGen at date: 2016/9/12 20:26:45

RPC_CLIENT_IMPLEMETION(RpcG2M_EndAreanMs, EndAreanMsArg, EndAreanMsRes)

void RpcG2M_EndAreanMs::OnReply(const EndAreanMsArg &roArg, const EndAreanMsRes &roRes, const CUserData &roUserData)
{
	if (roArg.isgm())
	{
		SSInfo<<"gm op rank:"<<roArg.rank()<<END;
		return;
	}
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		return;
	}
	if (!roRes.issucceed())
	{
		return;
	}
	Scene* scene = role->GetCurrScene();
	if (NULL == scene)
	{
		return;
	}
	CArenaRecord* record = role->Get<CArenaRecord>();
	UINT32 lastoprank = record->GetOptimalRank();
	if (UINT32(-1) == lastoprank)
	{
		lastoprank = roRes.info().npccount();
	}
	bool ismissed = false;
	CArenaMgr::Instance()->OnEnd(role, roArg.iswin(), roRes.info().isnpc(), roRes.info().challengeid());
	UINT32 lastrank = roRes.info().lastrank();
	UINT32 currentrank = roRes.info().currentrank();
	UINT32 arenaup = lastrank > currentrank ? lastrank - currentrank : 0;
	if (lastoprank > currentrank)
	{
		record->SetOptimalRank(currentrank);
	}
	if (roArg.iswin())
	{
		UINT32 currentoprank = record->GetOptimalRank();
		NormalBattleResultListener listener(scene, roArg.iswin());

		listener.SetInitStatus(role);
		listener.SetArenaup(arenaup, ismissed);
		if (ismissed && arenaup == 0)
		{
			//scene->PushCostExcept(role->GetID());
		}
		if (lastoprank > currentoprank)
		{
			int count = CArenaMgr::Instance()->GetUpReward(lastoprank, currentoprank);

			BagGiveItemTransition give(role);
			give.SetReason(ItemFlow_Stage, ItemFlow_Stage_Arena);
			give.GiveItem(DRAGON_COIN, count);
			give.NotifyClient();

			listener.AddItem(DRAGON_COIN, count, true);
		}
		listener.NotifyClient(role);

		ArenaRewardMgr::Instance()->NotifyRankReward(role);

		role->OnSceneComplete(scene);
	}
	else
	{
		role->OnSceneFailed(scene);
	}

	ArenaRewardMgr::Instance()->NotifyPointReward(role);

	CArenaMgr::Instance()->PopBattleInfo(role->GetID());
}

void RpcG2M_EndAreanMs::OnTimeout(const EndAreanMsArg &roArg, const CUserData &roUserData)
{
}
