#include "pch.h"
#include <time.h>
#include "unit/role.h"
#include "rolecommonhandler.h"
#include "skillchangedrecord.h"
#include "table/PowerPointMgr.h"
#include "utility/loghelper.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "stagemgr.h"
#include "globalconfig.h"
#include "bagtransition.h"
#include "FatigueMgr.h"
#include "rolesummarymgr.h"
#include "checkinmgr.h"
#include "payv2Record.h"
#include "rewardmgr.h"
#include "teamrequest.h"
#include "team.h"
#include "scene/sceneworldboss.h"
#include "buyrecord.h"
#include "activitymgr.h"
#include "rewardmgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildrecord.h"
#include "gamelogic/arenarewardmgr.h"
#include "guild/guildboss.h"
#include "guild/ptcg2c_newguildbonusntf.h"
#include "gamelogic/onlinereward.h"
#include "RandomTaskConfig.h"
#include "teammgr.h"
#include "noticemgr.h"
#include "pkmgr.h"
#include "rolenethandler.h"
#include "gamelogic/rolefashion.h"
#include "mail/mailop.h"
#include "findback/findbackrecord.h"
#include "utility/tlogger.h"
#include "gamelogic/arenamgr.h"
#include "scene/scenetower.h"
#include "designationMgr.h"
#include "levelsealMgr.h"
#include "achievev2Mgr.h"
#include "scene/attributepk.h"
#include "loginrecord.h"
#include "table/PlayerLevelManager.h"
#include "entity/XRole.h"
#include "QAMgr.h"
#include "gamelogic/flowerrecord.h"
#include "bossrushmgr.h"
#include "gamelogic/chatmgr.h"
#include "scene/scenemortuary.h"
#include "pb/project.pb.h"
#include "scene/sceneteam.h"
#include "live/livemanager.h"
#include "gamelogic/payv2Mgr.h"
#include "unit/pet.h"
#include "gamelogic/firstpassrecord.h"
#include "gamelogic/atlas.h"
#include "stageassist.h"
#include "lotteryrecord.h"
#include "gamelogic/ibshopmgr.h"
#include "gamelogic/superriskrecord.h"
#include "scene/scenerisk.h"
#include "camp/camprecord.h"
#include "gamelogic/spriterecord.h"
#include "idip/idiprecord.h"
#include "guild/guildrecord.h"
#include "scene/DragonExpConfig.h"
#include "guild/ptcg2m_synguildcontribute.h"
#include "mslink.h"
#include "pvpmgr.h"
#include "gamelogic/taskrecord.h"
#include "scene/sceneskycitywait.h"
#include "halliconmgr.h"
#include "gamelogic/designationRecord.h"
#include "spactivity/spactivityrecord.h"
#include "gamelogic/levelsealRecord.h"
#include "buff/buffrecord.h"
#include "scene/scenestatistics.h"
#include "event/eventmgr.h"
#include "gamelogic/titlerecord.h"
#include "table/OpenSystemMgr.h"
#include "scene/scenereswar.h"
#include "scene/scenereswarpve.h"
#include "scene/scenehorse.h"
#include "jademgr.h"
#include "antiaddictionmgr.h"
#include "spactivity/spactivitymgr.h"
#include "gamelogic/horsemgr.h"
#include "pkrecord.h"
#include "pvprecord.h"
#include "popwindows.h"
#include "gamelogic/holidaystage.h"
#include "gamelogic/shoprecord.h"
#include "gamelogic/herobattlerecord.h"
#include "gamelogic/horsemgr.h"
#include "gamelogic/halliconmgr.h"
#include "spactivity/spactivityhandler.h"
#include "gamelogic/rolemisc.h"
#include "enhanceequipmgr.h"
#include "gamelogic/reportdatarecord.h"
#include "scene/sceneinvfight.h"
#include "unit/rolemanager.h"
#include "gamelogic/loginactivity.h"
#include "guild/guildcampparty.h"
#include "gamelogic/operatingactivity.h"
#include "gamelogic/abysspartymgr.h"
#include "gamelogic/tarjamgr.h"
#include "scene/scenecustompk.h"
#include "utility/tlogmgr.h"
#include "table/ohmygodmgr.h"
#include "gamelogic/platformshare_record.h"
#include "gamelogic/dragonhelpconfig.h"
#include "weekend4v4mgr.h"
#include "scene/bigmeleeready.h"
#include "gamelogic/tajiehelprecord.h"
#include "scene/scenecustompktwo.h"
#include "teamrecord.h"
#include "gamelogic/mobabattlemgr.h"
#include "gamelogic/attrmodmgr.h"
#include "gamelogic/dragonrecord.h"
#include "rolepartner.h"
#include "dragonguild/dragonguildrecord.h"
#include "gamelogic/competedragonrecord.h"
#include "gamelogic/battlefieldgsmgr.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "worldlevelmgr.h"
#include "define/spactivityiddef.h"
#include "spactivity/spactivitydatasmall.h"
#include "spactivity/spactivitybase.h"
#include "gamelogic/teamreward.h"

RoleCommonHandler RoleCommonHandler::GlobalHandler;

void RoleCommonHandler::OnLevelup(Role *poRole)
{
	poRole->Get<MyLoginRecord>()->UpdateLoginRewardOpenState();

	poRole->Get<CFindBackRecord>()->CheckOpen();

	poRole->Get<CSuperRiskRecord>()->OnLevelUp();

	PayV2Mgr::Instance()->GrowthFundHintNotify(poRole);
	PopWindows::Instance()->PopQuestionire(poRole);

	if (1 != poRole->GetLevel())
	{
		PlayerLevelTable::RowData* data = PlayerLevelManager::Instance()->GetLevelInfo(poRole->GetLevel());
		if (data)
		{
			BagGiveItemTransition transition(poRole);
			transition.SetReason(ItemFlow_Level, ItemFlow_Level_Give);
			transition.GiveItem(SKILL_POINT, data->AddSkillPoint);
			transition.NotifyClient();
		}
	}
	poRole->Get<RoleMisc>()->OnLevelUp();
	LoginActivityMgr::Instance()->OnTriger(poRole,false);
	OperatingActivityMgr::Instance()->OnTriger(poRole);
	CNpcFeelingMgr::Instance()->OnLevelUp(poRole);
}

void RoleCommonHandler::OnTaskComplete(Role *poRole, int TaskID)
{
	if (TaskID == 3000)
	{
		poRole->CheckExp();
		const WorldLevelConf* conf = WorldLevelMgr::Instance()->GetWorldLevelConf(poRole->GetLevel(), poRole->GetID());
		if (conf != NULL)
		{
			poRole->LevelUpRapidly(conf->BackflowLevel);
			poRole->Get<BuffRecord>()->AddBuff(0, conf->BackBattleBuff[0], conf->BackBattleBuff[1]);
		}
	}
	else if (TaskID == 3003)
	{
		SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(poRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
		if (backflow != NULL)
		{
			SpActivityBase::CloseSpActivitySystem(poRole, SYS_BACKFLOW_LEVELUP);
		}
	}
}

void RoleCommonHandler::OnEnterScene(Role *poRole, Scene *poScene)
{
	// modify attr
	AttrModMgr::Instance()->ModifyAttr(poRole, poScene);

	// set group
	UINT32 group = poScene->GetFightGroup(poRole);
	poRole->SetFightGroup(group);
	
	// event
	UINT32 scenetype = poScene->GetSceneType();
	if (KKSG::SCENE_GUILD_BOSS == scenetype)
	{
		GuildBossMgr::Instance()->OnEnterScene(poRole);
	}
	else if (KKSG::SCENE_BOSSRUSH == scenetype)
	{
		BossRushMgr::Instance()->OnRoleEnterScene(poRole);
	}
	else if (KKSG::SCENE_TOWER == scenetype)
	{
		poScene->GetSceneTower()->OnEnterScene(poRole);
	}
	else if (KKSG::SKYCITY_WAITING == scenetype)
	{
		poScene->GetSkyCityWait()->OnEnterScene(poRole);
	}else if (KKSG::SCENE_RESWAR_PVE == scenetype)
	{
		poScene->GetSceneResWarPVE()->OnEnterScene(poRole);
	}else if (KKSG::SCENE_HORSE == scenetype)
	{
		poScene->GetSceneHorseWait()->OnEnterScene(poRole);
	}else if (KKSG::SCENE_RESWAR_PVP == scenetype)
	{
		poScene->GetResWarHandler()->OnEnterScene(poRole);
	}else if (KKSG::SCENE_BIGMELEE_READY == scenetype)
	{
		poScene->GetSceneBMReady()->OnEnterScene(poRole);
	}else if (KKSG::SCENE_BATTLEFIELD_READY == scenetype)
	{
		BattleFieldGSMgr::Instance()->OnEnterScene(poRole);
	}

	if (poRole->GetXRole())
	{
		poRole->GetXRole()->CoolDownAllSkills();
	}

	poRole->GetRoleSkillLevelMgr()->CastEnterSceneSkill(poScene);

	poRole->Get<MyLoginRecord>()->OnEnterScene();
	poRole->Get<StageMgr>()->OnEnterScene(poScene);
	poRole->Get<CSpriteRecord>()->OnEnterScene(poScene);
	//poRole->Get<PetSys>()->OnEnterScene(); ///> 特殊需求（客户端压力），进场景不需要上马

	poRole->Get<LotteryRecord>()->HintNotify();
	poRole->Get<LotteryRecord>()->HintNotifyGoldClick();

	poRole->Get<CPayV2Record>()->Update();		
	poRole->Get<TaskRecord>()->OnEnterScene();

}

void RoleCommonHandler::OnLeaveScene(Role *poRole, Scene *poScene)
{
	///> 双人坐骑离开场景
	poRole->Get<PetSys>()->ResetPairRide(false);

	poRole->Get<BuffRecord>()->OnLeaveScene(poScene);

	///> 关卡消耗和CD放到离开场景时计算
	if(poScene->IsHelper(poRole->GetID()) || poScene->UseTicket(poRole->GetID()))
	{
		//组队助战和用票不消耗
	}
	else
    {
	    SceneManager::Instance()->TakeEnterSceneCost(poRole, poScene->GetSceneInfo(), poScene->GetSceneState() == SCENE_WIN);
    }

	poRole->Get<CSuperRiskRecord>()->m_superRisk.OnLeave(poScene);

	SceneMortuary *pMortuary = poScene->GetMortuary();
	pMortuary->DeleteDeathRoleInTimer(poRole); // 删除计时复活队列中的role

	if  (poScene->GetSceneType() == KKSG::SCENE_GUILD_HALL && poRole->getGuildId() == poScene->GetOwnerID())
	{
		CGuildCampPartyMgr::Instance()->ClearRoleInTransactionStatus(poRole->GetRoleID());
	}

	if (!poRole->IsWatcher() && NULL != poScene && poScene->IsCanDrawBox() && NULL != poScene->GetTeamSelectReward())
	{
		poScene->GetTeamSelectReward()->OnRoleLeave(poRole);
	}
}
static INT32 GetSurviveCount(Scene *poScene)
{
	if (NULL == poScene)
	{
		return 0;
	}
	SceneTeam* pTeam = poScene->GetSceneTeam();
	if (NULL == pTeam)
	{
		return 0;
	}
	SceneMortuary* pMor =  poScene->GetMortuary();
	if (NULL == pMor)
	{
		return 0;
	}
	std::vector<UINT64> vList;
	vList = pTeam->GetAllMemberIDs();
	INT32 Count = vList.size();
	for (UINT32 i = 0; i < vList.size(); i ++)
	{
		//离开或者死亡;
		Role* pRole = RoleManager::Instance()->FindByRoleID(vList[i]);
		if (NULL == pRole || NULL == pRole->GetCurrScene() || pRole->GetCurrScene() != poScene || pRole->IsWatcher() || pMor->GetDeathCount(vList[i]) == (pMor->GetReviveCount(vList[i]) + 1))
		{
			Count --;
		}
	}
	return Count;
}
void RoleCommonHandler::OnCompleteScene(Role *poRole, Scene *poScene)
{
	if (NULL == poRole || NULL == poScene)return;
	///> 完成关卡任务
	poRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_StageID, poScene->GetSceneTemplateID());
	poRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_StageType, poScene->GetSceneType());

	///> 完成新手关卡
	if ((int)poScene->GetSceneTemplateID() == GetGlobalConfig().InitialSceneID || poScene->GetSceneTemplateID() == GetGlobalConfig().BraveReturnServerSceneID)
	{
		poRole->GetRoleAllInfoPtr()->mutable_brief()->set_completeguidestage(true);
	}
	if (poScene->GetSceneTemplateID() == GetGlobalConfig().BackFlowActivitySceneID)
	{
		SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(poRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
		if (backflow != NULL)
		{
			backflow->SetIsFinishBackFlowScene();
		}
	}

	if (poScene->GetSceneTower())
	{
		poScene->GetSceneTower()->OnFinishScene(poRole);

		poScene->GetSceneTower()->OnChangeRankV2(poRole);
	}

	if (poScene->IsHelper(poRole->GetID()))
	{
		poRole->Get<StageMgr>()->AddHelperWinCount();
	}

	if (poScene->GetSceneType() == KKSG::SCENE_GUILD_CAMP && !poScene->IsHelper(poRole->GetID()))
	{
		poRole->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_GuildRisk);
	}

	if (poScene->GetSceneType() == KKSG::SCENE_NEST && poScene->GetSceneTeam())
	{
		UINT32 expID = poScene->GetSceneTeam()->GetExpID();
		poRole->Get<RoleMisc>()->CheckHardestNestExpID(expID);

		poRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::NestBack, 1, 1);
	}

	poRole->Get<CTaJieHelpRecord>()->TajieSceneProcess(poScene, true);

	EventMgr::Instance()->AddEvent(poRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_StageTypeCount, poScene->GetSceneType(), 1);
	
	poRole->Get<StageMgr>()->SetTrophy(poScene);
	
	TRoundFlowLog oLog(poRole);
	// stage assist
	StageRankInfo* stageinfo = poScene->GetStageRankInfo(poRole->GetID());
	if (NULL != stageinfo)
	{
		StageAssistInfo info;
		info.stageid = poScene->GetSceneTemplateID();
		info.time = stageinfo->timespan;
		info.hp = stageinfo->hppercent;
		info.revive = stageinfo->deathcount;
		int pptconf = poScene->GetSceneInfo()->m_pConf->RecommendPower;
		if (0 != pptconf)
		{
			double pptrole = poRole->GetAttr(TOTAL_POWERPOINT);
			info.ppt = (0.0l == pptrole) ? 0.0l : (pptrole - (double)pptconf)/((double)pptconf);
		}
		info.iswin = true;

		int point = StageAssistMgr::Instance()->CalAssistPoint(info);

		bool isfirst = poRole->Get<StageMgr>()->AddStageAssist(info.stageid, point);
		std::string first = isfirst ? "first" : "";
		SSInfo<<"Stage Assist Win "<<first<<", role:"<<poRole->GetID()<<" point:"<<point<<" id:"<<info.stageid<<" time:"<<info.time
			<<" hp:"<<info.hp<<" death:"<<info.revive<<" ppt:"<<info.ppt<<END;

		oLog.m_iStageTime = info.time;
		oLog.m_iHpPercent = info.hp;
		oLog.m_iReviveCount = info.revive;
	}
	oLog.m_Uid = poScene->GetSceneID();
	oLog.m_iReviveCount = poScene->GetMortuary()?poScene->GetMortuary()->GetReviveCount(poRole->GetID()):0;
	oLog.m_DeathCount = poScene->GetMortuary()?poScene->GetMortuary()->GetDeathCount(poRole->GetID()):0;

	if (KKSG::SCENE_BOSSRUSH == poScene->GetSceneType())
	{
		return;
	}

	UINT32 dwCurrTime = TimeUtil::GetTime();
	INT32 nTimeUsed	  = ((dwCurrTime >= (UINT32)poRole->GetEnterSceneTime() && poRole->GetEnterSceneTime()) ? dwCurrTime - (UINT32)poRole->GetEnterSceneTime() : 0);
	oLog.m_nBattleID = poScene->GetSceneTemplateID();
	oLog.m_nBattleType = poScene->GetSceneType();
	oLog.m_nRoundScore = poScene->GetRoleRank(poRole->GetID());
	oLog.m_nRoundTime  = nTimeUsed;

	oLog.m_nResult = TX_BATTLE_SUCCESS;
	if (poScene->GetSceneType() == KKSG::SCENE_INVFIGHT)
	{
		auto handler = poScene->GetInvFightHandler();
		if (!(handler && handler->GetFinalWinRoleID() == poRole->GetID()))
		{
			oLog.m_nResult = TX_BATTLE_FAIL;
		}
		if (NULL != handler)
		{
			UINT64 roleID11 = handler->GetRoleIDByIndex(0);
			UINT64 roleID22 = handler->GetRoleIDByIndex(1);
			Role* pRoleFight = RoleManager::Instance()->FindByRoleID(roleID22);
			if (pRoleFight != NULL && poRole->GetID()==roleID11)
			{
				TInvFightFlow Log(poRole);
				Log.SetFriendRole(pRoleFight);
				Log.m_Result = oLog.m_nResult;
				Log.Do();
			}
		}
	}

	oLog.m_nRank = 0;

	SceneTeam *pTeam = poScene->GetSceneTeam();
	if (NULL != pTeam)
	{
		oLog.m_nTeam = pTeam->GetAllMemberIDs().size()>1?1:0;
		oLog.m_TeamCount = pTeam->GetAllMemberIDs().size();
		oLog.m_TeamID = pTeam->GetTeamID();
	}
	oLog.m_TeamSurviveCount = GetSurviveCount(poScene);

	oLog.m_nStar = poScene->GetRoleRank(poRole->GetID());//星级

	if (KKSG::SCENE_TOWER == poScene->GetSceneType())
	{
		if (NULL != poScene->GetSceneTower())
		{
			//oLog.m_nTopFloor = poScene->GetSceneTower()->GetTopFloor(poRole);
			oLog.m_nTopFloor = poScene->GetSceneTower()->GetFinalReachFloor(poRole);
		}
	}

	if (poScene->GetSceneInfo()->m_pConf->syncMode == 0) // 节日副本结束处理
	{
		poRole->Get<StageMgr>()->DoAfterHolidayEnd(poScene->GetSceneInfo()->m_pConf->id);
	}
	
	oLog.m_iHelper = poScene->IsHelper(poRole->GetID())?1:0;
	oLog.m_iAutoFight = poRole->IsClientAutoFight()?1:0;
	oLog.Do();
}

void RoleCommonHandler::OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw)
{
	if (NULL == poRole || NULL == poScene)return;
	if (poScene->GetSceneType() == KKSG::SCENE_DRAGON_EXP)
	{
		poRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_StageType, poScene->GetSceneType());
	}

	poRole->Get<CTaJieHelpRecord>()->TajieSceneProcess(poScene, false);

	TRoundFlowLog oLog(poRole);
	// stage assist
	StageRankInfo* stageinfo = poScene->GetStageRankInfo(poRole->GetID());
	if (NULL != stageinfo)
	{
		StageAssistInfo info;
		info.stageid = poScene->GetSceneTemplateID();
		info.time = stageinfo->timespan;
		info.hp = stageinfo->hppercent;
		info.revive = stageinfo->deathcount;
		double pptconf = poScene->GetSceneInfo()->m_pConf->RecommendPower;
		double pptrole = poRole->GetAttr(TOTAL_POWERPOINT);
		info.ppt = (0.0l == pptrole) ? 0.0l : (pptrole - pptconf)/pptconf;
		info.iswin = false;

		int point = StageAssistMgr::Instance()->CalAssistPoint(info);

		SSInfo<<"Stage Assist Lose, role:"<<poRole->GetID()<<" point:"<<point<<" id:"<<info.stageid<<" time:"<<info.time
			<<" hp:"<<info.hp<<" death:"<<info.revive<<" ppt:"<<info.ppt<<END;

		poRole->Get<StageMgr>()->AddStageAssist(info.stageid, point);

		oLog.m_iStageTime = info.time;
		oLog.m_iHpPercent = info.hp;
		oLog.m_iReviveCount = info.revive;
	}
	oLog.m_Uid = poScene->GetSceneID();
	oLog.m_iReviveCount = poScene->GetMortuary()?poScene->GetMortuary()->GetReviveCount(poRole->GetID()):0;
	oLog.m_DeathCount = poScene->GetMortuary()?poScene->GetMortuary()->GetDeathCount(poRole->GetID()):0;

	if (KKSG::SCENE_BOSSRUSH == poScene->GetSceneType())
	{
		return;
	}

	UINT32 dwCurrTime = TimeUtil::GetTime();
	INT32 nTimeUsed	  = ((dwCurrTime >= (UINT32)poRole->GetEnterSceneTime() && poRole->GetEnterSceneTime())? dwCurrTime - (UINT32)poRole->GetEnterSceneTime() : 0);
	oLog.m_nBattleID = poScene->GetSceneTemplateID();
	oLog.m_nBattleType = poScene->GetSceneType();
	oLog.m_nRoundScore = poScene->GetRoleRank(poRole->GetID());
	oLog.m_nRoundTime  = nTimeUsed;
	oLog.m_nResult = isdraw?TX_BATTLE_DRAW:TX_BATTLE_FAIL;

	SceneTeam *pTeam = poScene->GetSceneTeam();
	if (NULL != pTeam)
	{
		oLog.m_nTeam = pTeam->GetAllMemberIDs().size()>1?1:0;
		oLog.m_TeamCount = pTeam->GetAllMemberIDs().size();
		oLog.m_TeamID = pTeam->GetTeamID();
	}
	oLog.m_TeamSurviveCount = GetSurviveCount(poScene);

	if (KKSG::SCENE_TOWER == poScene->GetSceneType())
	{
		if (NULL != poScene->GetSceneTower())
		{
			//oLog.m_nTopFloor = poScene->GetSceneTower()->GetTopFloor(poRole);
			oLog.m_nTopFloor = poScene->GetSceneTower()->GetFinalReachFloor(poRole);
		}
	}
	oLog.m_iHelper = poScene->IsHelper(poRole->GetID())?1:0;
	oLog.m_iAutoFight = poRole->IsClientAutoFight()?1:0;
	oLog.Do();
}

void RoleCommonHandler::OnSceneDraw(Role *poRole, Scene* scene)
{
	OnSceneFailed(poRole, scene, true, true);
}

void RoleCommonHandler::OnSweepScene(Role *poRole, SceneInfo *pSceneInfo)
{
	poRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_StageID, pSceneInfo->m_pConf->id);
	poRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_StageType, pSceneInfo->m_pConf->type);
}

void RoleCommonHandler::OnFirstTimeEnterScene(Role *poRole, Scene *poScene)
{
	FatigueMgr::AddLoginFatigue(poRole);
	poRole->Get<BuyRecord>()->LoginCheck();
	DesignationMgr::Instance()->FirstEnterSceneNotify(poRole);
	DesignationMgr::Instance()->SendCoverDesignationNotify(poRole);
	AchieveV2Mgr::Instance()->HintNotify(poRole);
	poRole->Get<CGuildRecord>()->HintNotify(poRole);

	CLevelSealMgr::Instance()->NotifyLevelSealInfo(poRole);
	poRole->Get<CFlowerRecord>()->OnFirstEnterScene();

	poRole->Get<CTowerRecord>()->OnLogin();

	// 工会挑战奖励红点事件;
	//poRole->CheckGuildDareRewardNtf();

	poRole->Get<PetSys>()->DoOnlineChek();

	poRole->Get<CAtlasSys>()->SynAtlasAttr();

	Bag* pBag = poRole->Get<Bag>();
	pBag->RearrangeBag();
	pBag->NotifyEnhanceMaster();

	poRole->Get<CSpriteRecord>()->OnFirstTimeEnterScene();

	EnhanceEquipMgr::Instance()->OnRoleChangeProfession(poRole, false);

	poRole->Get<CIdipRecord>()->CheckZeroProFit();
	poRole->Get<CIdipRecord>()->NotifyMessage();

	poRole->Get<CPayV2Record>()->Update();
	PayV2Mgr::Instance()->PayCheck(poRole);

	DragonExpConfig::Instance()->SynProgressToMS(poRole);

	poRole->Get<DesignationRecord>()->CalcMaxDesignation();

	OpenSystemMgr::Instance()->OnRoleFirstEnterScene(poRole);

	CActivityMgr::Instance()->OnRoleFirstEnterScene(poRole);

	poRole->Get<CShopRecord>()->OnFirstEnterScene();

	poRole->SendServerOpenDay();

	poRole->NoticeNameChange();

	poRole->Get<CHeroBattleRecord>()->UpdateMSRankList();
	poRole->Get<CSpriteRecord>()->UpdateMSRankList();

	poRole->Get<SpActivityRecord>()->OnFirstEnterScene();

	LoginActivityMgr::Instance()->OnTriger(poRole,true);
	OperatingActivityMgr::Instance()->OnTriger(poRole);

	if (poRole->GetRoleAllInfo().extrainfo().logintimes() == 1)
	{
		GMAddItem& items = OhMyGodMgr::Instance()->GetGMItem();
		if (!items.Table.empty())
		{
			BagGiveItemTransition trans(poRole);
			trans.SetReason(ItemFlow_GM, ItemFlow_GM_Give);
			for (int i = 0; i < items.Table.size(); ++i)
			{
				trans.GiveItem(items.Table[i]->itemid, items.Table[i]->itemcounts);
			}
			trans.NotifyClient();
			poRole->Get<TaskRecord>()->GMFinishAll();
		}
	}

	// 第一次登陆到主城，发分享周奖励信息
	if (NULL != poScene)
	{
		if (poScene->GetSceneTemplateID() == KKSG::SCENE_HALL)
		{
			poRole->Get<PlatformShareRecord>()->NotifyPlatShareAward();
		}
	}

	poRole->Get<TaskRecord>()->OnFirstTimeEnterScene();

	poRole->Get<CSuperRiskRecord>()->OnFirstEnterScene();

	poRole->Get<DragonGuildRecord>()->OnLogin();

	///> 更新龙玉合法性
	poRole->Get<Bag>()->ForeachEquipOnBody(std::bind(&CJadeMgr::UpdateJadeOnEquip, CJadeMgr::Instance(), poRole, std::placeholders::_1));
	poRole->Get<Bag>()->ForeachEquip(std::bind(&CJadeMgr::UpdateJadeOnEquip, CJadeMgr::Instance(), poRole, std::placeholders::_1));
	for (int i = 0; i < EquipCount; ++i)
	{
		poRole->OnChangeJadeOnEquip(i);
	}
}

void RoleCommonHandler::OnLoginReconnect(Role* poRole, Scene* poScene)
{
	poRole->Get<BuyRecord>()->LoginCheck();
	DesignationMgr::Instance()->FirstEnterSceneNotify(poRole);
	DesignationMgr::Instance()->SendCoverDesignationNotify(poRole);
	AchieveV2Mgr::Instance()->HintNotify(poRole);

	// 工会挑战奖励红点事件;
	//poRole->CheckGuildDareRewardNtf();

	poRole->Get<PetSys>()->DoOnlineChek();
	poRole->Get<CAtlasSys>()->SynAtlasAttr();
	Bag* pBag = poRole->Get<Bag>();
	pBag->RearrangeBag();
	pBag->NotifyEnhanceMaster();

	poRole->Get<CIdipRecord>()->CheckZeroProFit();
	poRole->Get<CIdipRecord>()->NotifyMessage();
	poRole->Get<CPayV2Record>()->Update();
	PayV2Mgr::Instance()->PayCheck(poRole);
	poRole->Get<CShopRecord>()->OnFirstEnterScene();
	poRole->SendServerOpenDay();
	poRole->NoticeNameChange();
}

void RoleCommonHandler::OnHallGsEnterScene(Role *poRole, Scene *poScene)
{
	if (poRole == NULL)
	{
		SSWarn << " poRole is NULL " << END;
		return;
	}

	SSInfo<<"handler on hall gs enter scene, rolename:"<<poRole->GetName()<<END;

	///> 数据在MainHall，玩家从GSN回到MainHall数据发生变化，需要重新通知客户端
	CRewardMgr::Instance()->OnRoleLogin(poRole);
	CCheckinMgr::Instance()->SendCheckinInfo(poRole);
	CActivityMgr::Instance()->HintNotify(poRole);
	OnlineRewardMgr::Instance()->OnLineNotify(poRole);
	PkMgr::Instance()->HintNotify(poRole);
	LiveManager::Instance()->OnFirstEnterNotifyIconNum(poRole);
	//poRole->CheckGuildDareRewardNtf();

	HallIconMgr::Instance()->OnRoleLogin(poRole);
	HorseRideMgr::Instance()->OnRoleLogin(poRole);

	DesignationMgr::Instance()->FirstEnterSceneNotify(poRole);
	DesignationMgr::Instance()->SendCoverDesignationNotify(poRole);
	AchieveV2Mgr::Instance()->HintNotify(poRole);
	DesignationMgr::Instance()->LoginUpdateFlowerDesignation( poRole );
	ChatMgr::Instance()->SendWorldChannelLeftTimesNotify( poRole );
	IBShopMgr::Instance()->SendIcon2Client(poRole);

	poRole->Get<CFindBackRecord>()->OnFirstEnterScene();
	poRole->Get<CFirstPassRecord>()->OnFirstEnterScene();
	
	SpActivityMgr::Instance()->SendActivityOffsetDayPtc(poRole);

	OpenSystemMgr::Instance()->OnHallEnterScene(poRole);
	poRole->Get<RoleMisc>()->OnHallEnterScene();

	poRole->NotifyBackFlow();
	poRole->Get<BuyRecord>()->HintBackFlow();

	poRole->SendWorldLevelNtf();
}

void RoleCommonHandler::OnLogout(Role *poRole, const char *msg)
{
	poRole->Get<CGuildRecord>()->EndCard(poRole);
	CAntiAddictionMgr::Instance()->LoginOut(poRole);
	TLogMgr::Instance()->Logout(poRole);

	poRole->Get<SpActivityRecord>()->UpdateBackFlowPreData();
}

void RoleCommonHandler::OnEnterGs(Role *poRole)
{
	poRole->Get<StageMgr>()->SyncTarjaMs();
	if (GSConfig::Instance()->IsHallGS())
	{
		OpenSystemMgr::Instance()->OnRoleFirstEnterScene(poRole);
	}
}

void RoleCommonHandler::OnLeaveGs(Role *poRole)
{
	poRole->Get<CGuildRecord>()->EndCard(poRole);
	poRole->Get<SpActivityRecord>()->UpdateBackFlowPreData();
}

void RoleCommonHandler::OnLogin(Role *poRole)
{
	PvpMgr::Instance()->OnRoleLogin(poRole);
	QAMgr::Instance()->OnRoleLogin(poRole);
//	HallIconMgr::Instance()->OnRoleLogin(poRole);
//	HorseRideMgr::Instance()->OnRoleLogin(poRole);
	PopWindows::Instance()->PopQuestionire(poRole);
	CHolidayStage::Instance()->CheckOnline(poRole);
	CGuildCampPartyMgr::Instance()->SystemOpenNotify(poRole);
	DragonHelpConfig::Instance()->OnRoleLogin(poRole);
    WeekEnd4v4Mgr::Instance()->HallIconRoleNtf(poRole, KKSG::HICONS_BEGIN);
	DragonExpConfig::Instance()->OnRoleLogin(poRole);

	TLoginLog oLog(poRole);
	oLog.Do();

	poRole->InitAntiAddictionInfo();
	poRole->Get<RoleMisc>()->OnLogin();
	poRole->Get<CTaJieHelpRecord>()->OnLogin();
	poRole->Get<CDragonRecord>()->OnLogin();
	poRole->Get<RolePartner>()->OnLogin();
	poRole->Get<CCompeteDragonRecord>()->OnLogin();
	CNpcFeelingMgr::Instance()->OnLogin(poRole);
	CAntiAddictionMgr::Instance()->OnLogin(poRole);
}

void RoleCommonHandler::OnChangeAttr(Role* poRole)
{
	DesignationMgr::Instance()->OnPPT(poRole);
}

void RoleCommonHandler::OnAttack(Role *poRole, Unit *pTargetUnit, double value)
{
	if (NULL != poRole->GetCurrScene() && NULL != poRole->GetCurrScene()->Statistics())
	{
		poRole->GetCurrScene()->Statistics()->m_TotalDamage += value;
	}
}

void RoleCommonHandler::OnHurt(Role *poRole, Unit* pCastUnit, double value)
{
	if (NULL != poRole->GetCurrScene() && NULL != poRole->GetCurrScene()->Statistics())
	{
		poRole->GetCurrScene()->Statistics()->m_TotalBehit += value;
	}
}

void RoleCommonHandler::OnGetItem(Role *poRole, XItem* item)
{

}

void RoleCommonHandler::OnChangeVirtualItem(Role* poRole, UINT32 item, INT64 count)
{
	switch(item)
	{
	///> 公会贡献
	case GUILD_CONTRIBUTE:  
		{
			if (count > 0)
			{
				PtcG2M_SynGuildContribute msg;
				msg.m_Data.set_roleid(poRole->GetID());
				msg.m_Data.set_num((UINT32)count);
				MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), msg);
			}
		}
		break;
	case SKILL_POINT:
		{
			if (count > 0)
			{
				BagGiveItemTransition give(poRole);
				give.SetReason(ItemFlow_Skill, ItemFlow_SkillTwo);
				give.GiveItem(SKILL_POINT_TWO, count);
				give.NotifyClient();
			}
		}
	case DRAGON_COIN:
	case DIAMOND:
		{
			poRole->SetFlag(RSF_ISRoleDataNeedSave, true);
		}
		break;
	default:
		break;
	}

}

void RoleCommonHandler::OnBuy(Role* poRole, UINT32 dwItemId)
{
	if (dwItemId == FATIGUE)
	{
		poRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::FATIGUE_BUY, 1, 1);
	}
}

void RoleCommonHandler::OnRecoverFatigue(Role* poRole, int num, int lastRecoverTime, int thisReoverTime)
{
	poRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::FATIGUE_RECOVER, 1, num, lastRecoverTime, thisReoverTime);
}

void RoleCommonHandler::OnChangeVip(Role* poRole)
{
	IBShopMgr::Instance()->SendIcon2Client(poRole);
	
	EventMgr::Instance()->AddGuildBonusEvent(poRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_VIPLEVEL, poRole->Get<CPayV2Record>()->GetVipLevel());
}

void RoleCommonHandler::OnTitleLevelUp(Role* poRole)
{
	EventMgr::Instance()->AddGuildBonusEvent(poRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_TITLE, poRole->Get<TitleRecord>()->GetTitle());
	EventMgr::Instance()->AddEvent(poRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_Title, 0, poRole->Get<TitleRecord>()->GetTitle());
}

void RoleCommonHandler::OnPay(Role* poRole, UINT32 totalPay, UINT32 PayNum)
{
	DesignationMgr::Instance()->OnRechargeAmount(poRole, totalPay);
	AchieveV2Mgr::Instance()->OnRechargeAmount(poRole, totalPay);
}

void RoleCommonHandler::OnFirstPay(Role* poRole)
{
	DesignationMgr::Instance()->OnFirstRecharge(poRole);
	AchieveV2Mgr::Instance()->OnFirstRecharge(poRole);
	EventMgr::Instance()->AddGuildBonusEvent(poRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_CHARGE, 1);
}

void RoleCommonHandler::OnQA(Role* poRole, UINT32 type)
{
	if (type == QATYPE_MULTI)
	{
		poRole->Get<CFindBackRecord>()->AddItemUsedCount(KKSG::QAMULTI_BACK, 1, 1);
	}
}

void RoleCommonHandler::OnBuyPayMember(Role* poRole, INT32 ID)
{
	EventMgr::Instance()->AddGuildBonusEvent(poRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_PRIVILEGE, ID);

	EventMgr::Instance()->AddEvent(poRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_BuyPrivilege, ID, 1);

	poRole->Get<CHeroBattleRecord>()->OnBuyPayMember(ID);
}

void JudgeJadeOnEquip(XItem *pItem, UINT32& minLevel, bool& result, UINT32 roleLevel)
{
	if(result == false)
	{
		return;
	}
	if(pItem == NULL || pItem->jadeInfo == NULL)
	{
		result = false;
		return;
	}
	UINT32 slot = CJadeMgr::Instance()->GetEmptySlot(pItem->jadeInfo, roleLevel, XItem::GetPos(pItem->itemID));
	if(slot != JADEINVALIDSLOT)
	{
		result = false;
		return;
	}
	if(pItem->jadeInfo->vecJadeSingle.empty())
	{
		result = false;
		return;
	}
	for(auto i = pItem->jadeInfo->vecJadeSingle.begin(); i != pItem->jadeInfo->vecJadeSingle.end(); i++)
	{
		JadeConf* conf = ItemConfig::Instance()->GetJadeConf((*i).jadeid);
		if(conf == NULL)
		{
			continue;
		}
		minLevel = std::min(minLevel, conf->JadeLevel);
	}
}

void RoleCommonHandler::OnChangeJadeOnEquip(Role* poRole, UINT32 pos)
{
	UINT32 minLevel = (UINT32)-1;
	bool result = true;
	poRole->Get<Bag>()->ForeachEquipOnBody(std::bind(JudgeJadeOnEquip, std::placeholders::_1, std::ref(minLevel), std::ref(result), poRole->GetLevel()));
	if(result)
	{
		LogDebug("Have full equips and all jades's level on equips >= %u", minLevel);
		EventMgr::Instance()->AddGuildBonusEvent(poRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_ALLDRAGONJADE_LEVEL, minLevel);
	}
}

void RoleCommonHandler::OnWearEquip(Role* pRole, UINT32 pos)
{
	XItem* item = pRole->Get<Bag>()->GetItemOnBody(EQUIP, pos);
	if (item)
	{
		AchieveV2Mgr::Instance()->OnSuitLevel(pRole, item->uid, item->GetEnhanceLevel());
	}

	UINT32 minQuality = -1;
	for (UINT32 i = 0; i < EquipCount; i++)
	{
		XItem* item = pRole->Get<Bag>()->GetItemOnBody(EQUIP, i);
		if (item == NULL)
		{
			minQuality = -1;
			break;
		}
		else
		{
			minQuality = std::min(minQuality, XItem::GetQuality(item->itemID));
		}
	}
	if (minQuality != -1)
	{
		EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_AllEquipQuality, 0, minQuality);
	}
}

void RoleCommonHandler::OnWearEmblem(Role* pRole, UINT32 pos)
{
	UINT32 count = pRole->Get<Bag>()->CountEmblemOnBody();
	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_Emblem, 0, count);
}

void RoleCommonHandler::OnThroughDragon(Role* pRole)
{

	Scene* pScene = pRole->GetCurrScene();
	if (!pScene)
	{
		return;
	}


	SceneTeam* team = pScene->GetSceneTeam();
	if (!team || !team->IsAllCross())
	{
		return;
	}

	PopWindows::Instance()->CheckComment(pRole, KKSG::COMMENT_DRAGON, team->GetExpID());
	if (!team->IsAllCross())
	{
		return;
	}

	EventMgr::Instance()->AddGuildBonusEvent(pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_DRAGON_NEST, team->GetExpID());
	EventMgr::Instance()->AddEventOnlyOnGS(pRole, ACH_EVENT_OFFSET + ACHIEVE_COM_DRAGON_SCENE , team->GetExpID(), team->GetAllMemberIDs().size(), pScene->GetTimeSpan());

}

void RoleCommonHandler::OnBuyAileen(Role* poRole)
{
	EventMgr::Instance()->AddEvent(poRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_BuyGift, 0, 1);
}

void RoleCommonHandler::OnBuyGrouwthFund(Role* pRole)
{
	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_BuyFund, 0, 1);
}
