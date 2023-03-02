#include "pch.h"
#include "scene/rpcc2g_reportbattle.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "util/XRandom.h"
#include "scene/scenemortuary.h"
#include "gamelogic/bossrushmgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "anticheat/anticheat.h"
#include "config.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/arenamgr.h"
#include "scene/scenestatistics.h"

// generate by ProtoGen at date: 2015/10/20 17:51:08

RPC_SERVER_IMPLEMETION(RpcC2G_ReportBattle, ReportBattleArg, ReportBattleRes)

void RpcC2G_ReportBattle::OnCall(const ReportBattleArg &roArg, ReportBattleRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);

	Scene *pScene = pRole->GetCurrScene();
	if (pScene == NULL )
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pRole->GetFlag(RSF_ISGiveReward))
	{
		pRole->Send(pRole->BattlePtc);
		return;
	}
	// solo scene
	if (pScene->IsSyncScene())
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	// failed
	if (roArg.battledata().isfailed() && roArg.battledata().has_failedinfo())
	{
		const KKSG::BattleFailedData& data = roArg.battledata().failedinfo();
		StageRankInfo info;
		info.timespan = data.timespan();
		info.hppercent = data.hppercent();
		info.deathcount = data.deathcount();
		pScene->SetStageRankInfo(pRole->GetID(), info);
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	// anti cheat
	if(!GSConfig::Instance()->IsDebug() && AntiCheat::IsCheat(pRole, pScene->GetSceneTemplateID(), (UINT32)pRole->GetEnterSceneTime(), roArg.battledata().anticheatinfo()))
	{
		if (NULL != pScene->Statistics())
		{
			pScene->Statistics()->SetCheat();
		}
		roRes.set_result(KKSG::ERR_ANTI_CHEAT_DETECTED);
		return;
	}

	UINT32 timespan = roArg.battledata().timespan();
	UINT32 hp = roArg.battledata().hppercent();
	StageRankInfo info;
	info.hppercent = hp;
	info.timespan = timespan;
	info.behit = roArg.battledata().behit();
	info.found = roArg.battledata().found();
	info.npchp = roArg.battledata().npchp();
	info.combo = roArg.battledata().combo();
	StageRankMgr::Instance()->CalculateRankInfo(pScene->GetSceneTemplateID(), info);

	pScene->SetStageRankInfo(pRole->GetID(), info);
	pScene->SetTimeSpan(timespan);

	for (int i = 0; i < roArg.battledata().pickdoodadwaveid_size(); ++i)
	{
		pScene->SetPickedDoodad(roArg.battledata().pickdoodadwaveid(i));
	}

	pRole->Statistics().SetMaxCombo(roArg.battledata().combo());
	pRole->Statistics().SetBehit(roArg.battledata().behit());
	pRole->Statistics().SetOpenChestCount(roArg.battledata().openchest());
	SSInfo << "Report Battle, RoleID=" << pRole->GetID() << ", SceneID=" << pScene->GetSceneTemplateID() <<  ", HpPercent=" << hp << END;

	SceneState winstate = SCENE_WIN;
	// remove arena
	///> 竞技场反外挂
	//if (pScene->GetSceneType() == KKSG::SCENE_ARENA)
	//{
	//	const KKSG::ArenaBattleInfo* info = CArenaMgr::Instance()->GetBattleInfo(pRole->GetID());
	//	if (NULL != info)
	//	{
	//		int npcppt = info->ppt();
	//		int ppt = pRole->GetAttr(TOTAL_POWERPOINT);
	//		if (0 != npcppt && npcppt > ppt)
	//		{
	//			float coef = (float)(npcppt - ppt)/float(npcppt);
	//			if (coef > 0.05f)
	//			{
	//				winstate = SCENE_LOSE;
	//				SSWarn<<"anti arena failed, role:"<<pRole<<" npcppt:"<<npcppt<<" ppt:"<<ppt<<END;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		winstate = SCENE_LOSE;
	//		SSWarn<<"anti arena failed, role:"<<pRole<<END;
	//	}
	//}
	pScene->EndSoloScene(winstate, timespan);

	///> 单机模式杀怪任务
	if (roArg.battledata().monster_id_size() == roArg.battledata().monster_num_size())
	{
		for (INT32 i = 0; i < roArg.battledata().monster_id_size(); i++)		
		{
			pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_MonsterID, roArg.battledata().monster_id(i),
												 roArg.battledata().monster_num(i));
		}
	}
}

void RpcC2G_ReportBattle::OnDelayReplyRpc(const ReportBattleArg &roArg, ReportBattleRes &roRes, const CUserData &roUserData)
{
}
