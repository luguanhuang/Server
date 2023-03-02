#include "pch.h"
#include "battle/rpcc2g_sweep.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "scene/sceneconfig.h"
#include "table/globalconfig.h"
#include "gamelogic/stagemgr.h"
#include "scene/scenemanager.h"
#include "gamelogic/stagereward.h"
#include "foreach.h"
#include "gamelogic/stagerankmgr.h"
#include "utility/loghelper.h"
#include "gamelogic/team.h"
#include "utility/tlogger.h"
#include "gamelogic/teamrecord.h"
#include "event/eventmgr.h"
#include "gamelogic/teamsynextradata.h"

// generate by ProtoGen at date: 2015/7/13 14:28:32

RPC_SERVER_IMPLEMETION(RpcC2G_Sweep, SweepArg, SweepRes)

class SweepResultListener : public IStageRewardListener
{
public:
	KKSG::SweepResult *m_pResult;

	SweepResultListener()
	{
		m_pResult = NULL;
	}

	virtual void SetSceneRank(int Rank, UINT32 firststar)
	{
	}

	virtual void AddItem(int itemID, int itemCount, bool isbind, int AddReason)
	{
		if (EXP == itemID)
		{
			return;
		}
		KKSG::ItemBrief *pItemBrief = m_pResult->add_items();
		pItemBrief->set_itemid(itemID);
		pItemBrief->set_itemcount(itemCount);

		ItemConf* pConf = ItemConfig::Instance()->GetItemConf(itemID);
		if (pConf != NULL)
		{
			isbind = ItemConfig::Instance()->GetBind(pConf, isbind);
		}
		pItemBrief->set_isbind(isbind);
	}

	virtual void ModifyExp(Role* role)
	{
		int count = (int)role->GetRealExp();
		if (0 != count)
		{
			KKSG::ItemBrief *pItemBrief = m_pResult->add_items();
			pItemBrief->set_itemid(EXP);
			pItemBrief->set_itemcount(count);
		}
	}
	virtual void SetExpSeal(bool isexpseal)
	{
	}
};


void RpcC2G_Sweep::OnCall(const SweepArg &roArg, SweepRes &roRes)
{
	roRes.set_result((ErrorCode)KKSG::ERR_INVALID_REQUEST);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	if (roArg.count() > 10)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	if (pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	int SceneID = roArg.sceneid();
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(SceneID);
	if (pSceneInfo == NULL || pSceneInfo->m_pConf == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	if(pSceneInfo->m_pConf->type != KKSG::SCENE_ABYSSS && pSceneInfo->m_pConf->type != KKSG::SCENE_BATTLE && pSceneInfo->m_pConf->type != KKSG::SCENE_TOWER)
	{
		LogError("Role %llu try sweep scene id %d which was not allowed", pRole->GetID(), SceneID);
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	bool IsAbysss = false;
	if(KKSG::SCENE_ABYSSS == pSceneInfo->m_pConf->type)
	{
		IsAbysss = true;
	}

	if(IsAbysss && pRole->GetAttr(TOTAL_POWERPOINT) < (double)pSceneInfo->m_pConf->SweepNeedPPT)
	{
		roRes.set_result(KKSG::ERR_SWEEP_POWERPOINT_LESS);
		return ;
	}

	StageMgr *pStage = pRole->Get<StageMgr>();
	if (pStage->GetRank(SceneID) != RANK_SSS)
	{
		roRes.set_result(KKSG::ERR_SWEEP_NOT_THREE_STAR);
		return;
	}

	const UINT32 SweepTicketItemID = 87;
	UINT32 ticketcount = (UINT32)pRole->Get<Bag>()->CountItemInBag(SweepTicketItemID);

	UINT32 count = roArg.count();
	UINT32 realcount = 0;
	UINT32 errorcode = KKSG::ERR_SUCCESS;
	for (; realcount < count; ++realcount)
	{
		
		errorcode = SceneManager::Instance()->CheckEnterSceneCondition(pRole, pSceneInfo);
		if (errorcode != KKSG::ERR_SUCCESS)
		{
			break;
		}

		if(IsAbysss && 0 == pRole->Get<CTeamRecord>()->GetTeamLeftCountToday(ABYSS_TEAM_SCENE))
		{
			errorcode = ERR_SCENE_TODYCOUNTLIMIT;
			break;
		}

		if(IsAbysss && realcount + 1 > ticketcount)
		{
			errorcode = KKSG::ERR_SWEEP_TICKET_LESS;
			break;
		}

		SceneManager::Instance()->TakeEnterSceneCost(pRole, pSceneInfo, true);
		if(IsAbysss)
		{
			pRole->Get<CTeamRecord>()->AddTeamFinishCountToday(ABYSS_TEAM_SCENE);
			HTeamSynExtraData syn;
			syn.TrySynExtraDataToTs(pRole, ABYSS_TEAM_SCENE);
		}
	}

	//BagTakeItemTransition bagtake(pRole);

	if (0 == realcount)
	{
		roRes.set_result((KKSG::ErrorCode)errorcode);
		return;
	}
	else
	{
		roRes.set_result(ERR_SUCCESS);
	}

	if(IsAbysss)
	{
		BagTakeItemTransition bagTake(pRole);
		bagTake.SetReason(ItemFlow_Stage, ItemFlow_Stage_Sweep);

		if(!bagTake.TakeItem(SweepTicketItemID, realcount))
		{
			bagTake.RollBack();
		}
		else
		{
			bagTake.NotifyClient();
		}

		roRes.set_abyssleftcount(pRole->Get<CTeamRecord>()->GetTeamLeftCountToday(ABYSS_TEAM_SCENE));
	}
		
	SweepResultListener listener;
	StageReward reward(pRole, NULL, pSceneInfo->m_pConf->type);
	reward.SetListener(&listener);
	for (UINT32 i = 0; i < realcount; ++i)
	{
		listener.m_pResult = roRes.add_rewards();
		pRole->OnSweepScene(pSceneInfo);
		reward.GiveSweepReward(pSceneInfo);
	}
	reward.NotifyClient(ItemFlow_Stage, ItemFlow_Stage_Sweep);

	reward.SetExpTrans();
	for (UINT32 i = 0; i < realcount; ++i)
	{
		listener.m_pResult = roRes.mutable_rewards(i);
		reward.GiveSweepExp(pSceneInfo);
		reward.NotifyExpClient(ItemFlow_Stage, ItemFlow_Stage_Sweep);
		listener.ModifyExp(pRole);
	}

	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_StageTypeCount, pSceneInfo->m_pConf->type, realcount);

	TRoundFlowLog oLog(pRole);
	oLog.m_nBattleID = SceneID;
	oLog.m_nBattleType = pSceneInfo->m_pConf->type;
	oLog.m_nResult = TX_BATTLE_SWEEP;
	oLog.m_iCount = realcount;
	oLog.Do();

}

void RpcC2G_Sweep::OnDelayReplyRpc(const SweepArg &roArg, SweepRes &roRes, const CUserData &roUserData)
{
}
