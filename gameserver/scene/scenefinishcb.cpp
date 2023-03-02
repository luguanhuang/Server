#include "pch.h"
#include "scenefinishcb.h"
#include "scene.h"
#include "scenefinder.h"
#include "gamelogic/stagereward.h"
#include "define/itemdef.h"
#include "unit/role.h"
#include "gamelogic/team.h"
#include "table/globalconfig.h"
#include "gamelogic/arenamgr.h"
#include "gamelogic/arenarewardmgr.h"
#include "gamelogic/rolesummarymgr.h"
#include "scene/scenepk.h"
#include "gamelogic/pkmgr.h"
#include "battleresultlistener.h"
#include "scene/scenemortuary.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/arenarecord.h"
#include "guild/guildmgr.h"
#include "unit/dummyrolemanager.h"
#include "unit/dummyrole.h"
#include "unit/rolemanager.h"
#include "table/expeditionconfigmgr.h"
#include "scene/scenetower.h"
#include "gamelogic/designationMgr.h"
#include "mail/mailsender.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "scenepvp.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "gamelogic/pkrecord.h"
#include "guild/guild.h"
#include "guild/guildconfig.h"
#include "scene/sceneteam.h"
#include "live/livemanager.h"
#include "scene/ptcg2m_synscenerewardg2m.h"
#include "arena/rpcg2m_endareanms.h"
#include "scene/scenedragon.h"
#include "gamelogic/XLevelWave.h"
#include "gamelogic/taskrecord.h"
#include "foreach.h"
#include "sceneskycity.h"
#include "scenereswar.h"
#include "network/mslink.h"
#include "scene/scenestatistics.h"
#include "scene/scenereswarpve.h"
#include "scenegwb.h"
#include "sceneinvfight.h"
#include "sceneherobattle.h"
#include "gamelogic/popwindows.h"
#include "scene/scenehorserace.h"
#include "gamelogic/horsemgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "gamelogic/LotteryMgr.h"
#include "gamelogic/rolepartner.h"
#include "custombattle/ptcg2m_reportcustombattleresult.h"
#include "custombattle/rpcg2k_custombattleresultworld.h"
#include "custombattle/rpcg2m_custombattleresultms.h"
#include "network/worldlink.h"
#include "scene/scenecustompk.h"
#include "scenepktwo.h"
#include "gamelogic/tarjamgr.h"
#include "scenemobabattle.h"
#include "sceneweekend4v4monsterfight.h"
#include "sceneweekend4v4ghostaction.h"
#include "sceneweekend4v4livechallenge.h"
#include "sceneweekend4v4crazybomb.h"
#include "sceneweekend4v4horseracing.h"
#include "gamelogic/reportdatarecord.h"
#include "sceneweekend4v4duck.h"
#include "dragonguild/ptcg2m_checkdragonguildtask.h"
#include "scene/scenecompetedragon.h"
#include "util/gametime.h"
#include "scene/battlefieldfight.h"
#include "gamelogic/goalaward.h"
#include "servermgr/servermgr.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CommonSceneFinishCB CommonSceneFinishCB::GlobalCommonSceneFinishCB;

template <typename T>
static void FillStageRankInfo(StageRankInfo& info, T* role, Scene* scene, UINT32 npchp)
{
	info.Reset();
	info.hppercent = role->GetHpPercent();
	info.timespan = scene->GetTimeSpan();
	info.behit = role->GetStatistics().GetBehit();
	info.combo = role->GetStatistics().GetMaxCombo();
	info.killenemyscore = role->GetStatistics().GetKillEnemyScore();
	info.totalkillenemyscore = scene->GetKillEnemyScore();
	info.npchp = npchp;
	info.found = scene->GetEnterFightTime(role->GetID());
	info.deathcount = scene->GetMortuary()->GetDeathCount(role->GetID());
}

static void FillRobotInfo2Listener(Scene* scene, DummyRole* pRobot, NormalBattleResultListener& listener)
{
	StageRankInfo stageinfo;

	listener.SetInitStatus(pRobot);
	if (scene->IsSceneWinState())
	{
		int npchp = scene->GetRankNpcHpPercent();
		FillStageRankInfo(stageinfo, pRobot, scene, npchp);
		StageRankMgr::Instance()->CalculateRankInfo(scene->GetSceneTemplateID(), stageinfo);

		DummyRoleStageReward reward(pRobot);
		reward.SetListener(&listener);
		reward.SetRank(stageinfo.rank);
		listener.SetDummyRoleEndState(pRobot);
		//RoleBoxRewardInfo boxinfo = scene->FillBoxRobot(pRobot->GetID(), stageinfo.rank);
		//listener.SetBox(&boxinfo);
		if (NULL != scene)
		{
			scene->OnSetRank(pRobot->GetID(), pRobot->GetLevel(), stageinfo.rank, true);
		}
	}
	else
	{
		listener.SetScore(0, 0);
	}
	listener.SetStatisticsInfo(pRobot->GetStatistics());
}

static void CalcSyncSceneRoleRank(bool iswin, Role* role, StageRankInfo& stageinfo, Scene* scene, NormalBattleResultListener& listener)
{
	if (!iswin)
	{
		return;
	}
	UINT32 sceneType = scene->GetSceneType();
	if (sceneType == KKSG::SCENE_ABYSSS || sceneType == KKSG::SCENE_GODDESS || sceneType == KKSG::SCENE_ENDLESSABYSS 
		|| sceneType == KKSG::SCENE_GUILD_CAMP || sceneType == KKSG::SCENE_ACTIVITY_ONE || sceneType == KKSG::SCENE_BIOHELL ||  sceneType == KKSG::SCENE_DUCK || sceneType == KKSG::SCENE_COUPLE)
	{
		StageRankMgr::Instance()->CalculateRankInfo(scene->GetSceneTemplateID(), stageinfo);
	}
	else if (sceneType == KKSG::SCENE_NEST || sceneType == KKSG::SCENE_WEEK_NEST || sceneType == KKSG::SCENE_DRAGON || sceneType == KKSG::SCENE_COMPETEDRAGON
		|| sceneType == KKSG::SCENE_ACTIVITY_TWO)
	{
		stageinfo = StageRankMgr::Instance()->CalculateNestRankInfo(scene, role
			, scene->Statistics()->GetRank(role->GetID(), 0) 
			, scene->Statistics()->GetPPTRank(role->GetID())
			, scene->Statistics()->GetRank(role->GetID(), 2));
	}
	else if (sceneType == KKSG::SCENE_TOWER)
	{
		stageinfo.towerfloor = scene->GetSceneTower()->GetFinalReachFloor(role);
		stageinfo.isnewrecord = scene->GetSceneTower()->IsNewRecordV2(role);
		listener.SetTimeSpan(scene->GetSceneTower()->GetFinalReachTime(role));
	}
}

static void OnSceneFinishNotify(NormalBattleResultListener& listener, Scene* pScene, std::vector<Role*>& roRoles, bool win)
{
	// watch data
	listener.SetWatchInfo(pScene);	

	// dragon
	if(pScene->GetSceneType() == KKSG::SCENE_DRAGON && pScene->GetSceneDragon()->IsFinalScene(win))
	{
		listener.SetIsFinalResult(true);
	}

	bool bSendNotifyFlag = (pScene->IsSyncScene() || win);
	for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
	{
		Role *pRole = *i;

		if (pScene->GetSceneTower())
		{
			bSendNotifyFlag = pScene->GetSceneTower()->CheckSendBattleResult(pRole);
		}
		else if (pScene->GetSceneDragon())
		{
			bSendNotifyFlag = pScene->GetSceneDragon()->CheckSendBattleResult();
		}else if (pScene->GetSceneCompeteDragon())
		{
			bSendNotifyFlag = pScene->GetSceneCompeteDragon()->CheckSendBattleResult();
		}

		if (bSendNotifyFlag)
		{
			listener.NotifyClient(pRole);
		}

		if (win)
		{
			pRole->OnSceneComplete(pScene);
		}
		else
		{
			pRole->OnSceneFailed(pScene);
		}
		// 杀怪任务
		XLevelSpawner* spawner = pScene->GetSpawner();
		if (spawner)
		{
			std::map<UINT32, UINT32>& killEnemy = spawner->m_oLevelState.m_killEnemyNum;
			for (auto j = killEnemy.begin(); j != killEnemy.end(); ++j)
			{
				pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_MonsterID, j->first, j->second);	
			}
		}
	}
	///> 通知观战结算
	if (!roRoles.empty())
	{
		listener.NotifyWatcher(pScene);
	}
}

static void CheckPartner(Scene* pScene, std::vector<Role*>& roles, bool isWin)
{
	for (size_t i = 0; i < roles.size(); ++i)
	{
		Role* pRole = roles[i];
		if (pRole->Get<RolePartner>()->GetPartnerID() == 0)
		{
			continue;
		}
		std::vector<Role*> partners;
		for (size_t j = 0; j < roles.size(); ++j)
		{
			Role* pOther = roles[j];
			if (pRole == pOther)
			{
				continue;
			}
			if (pRole->Get<RolePartner>()->GetPartnerID() == pOther->Get<RolePartner>()->GetPartnerID())
			{
				partners.push_back(pOther);
			}
		}
		if (partners.empty())
		{
			continue;
		}
		pRole->OnCompleteSceneWithPartner(pScene, roles, isWin);
	}
}

void CommonSceneFinishCB::_CheckAchievement(Scene* pScene, bool iswin, std::vector<Role *> &roRoles)
{
	if (!iswin)
		return;
	SceneMortuary* pMor = pScene->GetMortuary();
	if (NULL == pMor)
		return;
	UINT32 noDieNum = 0; 
	for (auto i = roRoles.begin(); i != roRoles.end(); i++)
	{
		if (0 == pMor->GetDeathCount((*i)->GetID()))
		{
			noDieNum++;
		}
	}
	SSDebug << "Finish Scene, SceneID:" << pScene->GetSceneID() << ", TemplateID:" << pScene->GetSceneTemplateID() << ", Win:" << iswin << ", NodieNum:" << noDieNum << ", AllRoleNum:" << roRoles.size() << END;
	for (auto i = roRoles.begin(); i != roRoles.end(); i++)
	{
		PopWindows::Instance()->CheckComment(*i,KKSG::COMMENT_NEST,pScene->GetSceneTemplateID());
		if (noDieNum == roRoles.size())
		{
			AchieveV2Mgr::Instance()->OnNoDieFinishRaid(*i, pScene->GetSceneTemplateID());
			DesignationMgr::Instance()->OnNoDieFinishRaid(*i, pScene->GetSceneTemplateID());
			if (noDieNum==4)
			{
				AchieveV2Mgr::Instance()->OnFourNoDieFinishRaid(*i, pScene->GetSceneTemplateID());
				DesignationMgr::Instance()->OnFourNoDieFinishRaid(*i, pScene->GetSceneTemplateID());
			}
		}
	}
	
	//小分队讨伐任务,成就任务
	if(pScene->GetSceneType() == SCENE_DRAGON && iswin)
	{
		SceneTeam *pTeam = pScene->GetSceneTeam();
		if(pTeam)
		{
			int teamStartMembers = pTeam->GetMemberCountWithoutRobot();
			
			PtcG2M_CheckDragonGuildTask ptc;
			ptc.m_Data.set_rolecount(teamStartMembers);
			for (auto i = roRoles.begin(); i != roRoles.end(); i++)
			{
				if(*i)
				{ 
					ptc.m_Data.add_roleid((*i)->GetID());
				}
			}
			ptc.m_Data.set_nodiecount(noDieNum);
			ptc.m_Data.set_time(pScene->GetOneSceneTimeSpan());
			ptc.m_Data.set_sceneid(pScene->GetSceneTemplateID() );
			for (auto i = roRoles.begin(); i != roRoles.end(); i++)
			{
				if(*i)
				{
					(*i)->SendToMS(ptc);
					break;
				}
			}
		}
	}

	///>独自通关
	SceneTeam *pTeam = pScene->GetSceneTeam();
	if (pTeam)
	{
		int nMembers = pTeam->GetMemberCountWithoutRobot();
		if (nMembers == 1 && roRoles.size() >= 1)
		{
			AchieveV2Mgr::Instance()->OnAloneFinishRaid(roRoles[0], pScene->GetSceneTemplateID());
			DesignationMgr::Instance()->OnAloneFinishRaid(roRoles[0], pScene->GetSceneTemplateID(), 1);
		}

		for (auto iter = pTeam->GetAllMemberIDs().begin(); iter!= pTeam->GetAllMemberIDs().end(); iter++)
		{
			for (auto i = roRoles.begin(); i != roRoles.end(); i++)
			{
				Role *pRole = *i;
				if (pRole->GetID()==*iter)
				{
					DesignationMgr::Instance()->OnCommonType(pRole,DESIGNATION_COM_TYPE_HARDCOREDRAGON, pTeam->GetExpID(),pTeam->GetAllMemberIDs().size(),0);  
					break;
				}
			}
		}
		
	}
}

void CommonSceneFinishCB::FillExDataToMs(NormalBattleResultListener* pListener, Scene* pScene, bool iswin, std::vector<Role *> &roRoles)
{
	if (pScene->GetSceneType()==SCENE_RESWAR_PVE && !iswin)
	{
		SceneResWarPVE* pPVEScene = pScene->GetSceneResWarPVE();
		if(!pPVEScene) 
			return;
		KKSG::GuildExpAndConData* pMsg = pListener->GetGuildInfo();
		KKSG::ResWarBase *pInfo = pMsg->mutable_reswar();
		pInfo->set_guildid(pPVEScene->GetGuildID());
		pInfo->set_teamid(pPVEScene->GetTeamID());
		pInfo->set_bwin(false);
	}

	if(pScene->GetSceneType() == SCENE_WEEK_NEST && iswin && pScene->GetSceneTeam())
	{
		const auto& members = pScene->GetSceneTeam()->GetAllMemberIDs();
		if(ServerMgr::Instance()->CheckRoleIdsIsSameServerId(members))
		{
			KKSG::GuildExpAndConData* pMsg = pListener->GetGuildInfo();	
			KKSG::WeekNestSceneInfo &info = *(pMsg->mutable_weeknestinfo());
			info.set_expid(pScene->GetSceneTeam()->GetExpID());
			info.set_iswin(iswin);
			info.set_usetime(pScene->GetTimeSpan());
			const auto& members = pScene->GetSceneTeam()->GetAllMemberIDs();
			for(size_t i = 0; i < members.size(); ++i)
			{
				info.add_roles(members[i]);
			}
		}
	}

	if (pScene->GetSceneType() == SCENE_BIOHELL && iswin && pScene->GetSceneTeam() && GetGlobalConfig().BioHellRankId == pScene->GetSceneTemplateID())
	{
		const auto& members = pScene->GetSceneTeam()->GetAllMemberIDs();
		if (ServerMgr::Instance()->CheckRoleIdsIsSameServerId(members))//该玩法改成跨服玩法，但是只有都是同一个服时才排行;
		{
			KKSG::GuildExpAndConData* pMsg = pListener->GetGuildInfo();	
			KKSG::BioHellSceneInfo* pData = pMsg->mutable_biohellinfo();
			pData->set_usetime(pScene->GetTimeSpan());
			for(size_t i = 0; i < members.size(); ++i)
			{
				pData->add_roles(members[i]);
			}
		}
	}

	if (pScene->GetSceneType() == SCENE_COMPETEDRAGON && iswin && pScene->GetSceneTeam() && !pScene->GetSceneTeam()->HasNextScene())
	{
		KKSG::GuildExpAndConData* pMsg = pListener->GetGuildInfo();	
		KKSG::CompeteDragonSceneInfo* info = pMsg->mutable_competedragoninfo();
		info->set_passtime(GameTime::GetTime());
		const auto& members = pScene->GetSceneTeam()->GetAllMemberIDs();
		for(size_t i = 0; i < members.size(); ++i)
		{
			info->add_roles(members[i]);
		}
		info->set_usetime(pScene->GetTimeSpan());
	}
}

void CommonSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	CheckPartner(pScene, roRoles, win);
	int sceneType = pScene->GetSceneInfo()->m_pConf->type;
	SceneStatistics* statistics = pScene->Statistics();
	if (NULL == statistics)
	{
		SSError<<"statistic is NULL, scenetype:"<<sceneType<<END;
		return;
	}

	NormalBattleResultListener listener(pScene, win);
	// rank 
	statistics->DoRank(sceneType, roRoles);	
	// robot
	SceneTeam* pTeam = pScene->GetSceneTeam();
	if (NULL != pTeam)
	{
		DummyRole* pRobot = statistics->DoRankRobot(sceneType, pTeam);
		if (NULL != pRobot)
		{
			FillRobotInfo2Listener(pScene, pRobot, listener);
		}
	}

	// team cost
	std::map<UINT64, ItemDesc> teamcostreward;
	if (NULL != pTeam)
	{
		pTeam->InitCostTeam(win, roRoles, teamcostreward);
	}

	///> 先算星级
	UINT32 maxrank = 0;
	std::map<UINT64, StageRankInfo> roleid2rankinfo;
	UINT32 npchp = pScene->GetRankNpcHpPercent();
	for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
	{
		Role *pRole = *i;
		StageRankInfo stageinfo;

		///> 联机的rank服务器算
		if (pScene->IsSyncScene())
		{
			FillStageRankInfo(stageinfo, pRole, pScene, npchp);
			CalcSyncSceneRoleRank(win, pRole, stageinfo, pScene, listener);
			pScene->SetStageRankInfo(pRole->GetID(), stageinfo);
		}
		else
		{
			///> 单机在reportbattle时结算星级，这里直接获取
			StageRankInfo* info = pScene->GetStageRankInfo(pRole->GetID());
			if (NULL != info)
			{
				stageinfo = *info;
			}
		}
		if ((UINT32)stageinfo.rank > maxrank)
		{
			maxrank = stageinfo.rank;
		}
		roleid2rankinfo[pRole->GetID()] = stageinfo;
	}

	// give role reward 
	for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
	{
		Role* pRole = *i;

		bool isHelper = pScene->IsHelper(pRole->GetID());
		bool isBoxExcept = pScene->IsSelectBoxExcept(pRole->GetID(), pRole->GetLevel());
		bool IsLeader = (pTeam != NULL && pTeam->GetLeaderID() == pRole->GetID());

		listener.SetInitStatus(pRole, pRole->getGuildId(), IsLeader);
		listener.SetIsHelper(isHelper);
		listener.SetIsSelectBoxExcept(isBoxExcept);
		listener.SetStatisticsInfo(pRole->GetStatistics());
		if (isHelper)
		{
			pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_Help);
		}

		if (win)
		{
			StageRankInfo& stageinfo = roleid2rankinfo[pRole->GetID()];

			StageReward reward(pRole, pScene, pScene->GetSceneType());
			reward.SetListener(&listener);
			///> 赏金团队长和团队最高星级一致(这里判断有问题 影响不大 暂时不改)
			if (IsLeader && !teamcostreward.empty())
			{
				stageinfo.rank = maxrank;	
			}
			reward.SetRank(stageinfo.rank);

			listener.SetScore(stageinfo.score, stageinfo.vipscore);
			listener.SetVipLevel(pRole->Get<CPayV2Record>()->GetVipLevel());
			listener.SetTowerResult(stageinfo.towerfloor, stageinfo.isnewrecord);

			///> 抽宝箱
			pScene->OnSetRank(pRole->GetID(), pRole->GetLevel(), stageinfo.rank, false);
			//RoleBoxRewardInfo boxinfo = pScene->FillBox(pRole, stageinfo.rank, reward.GetBagTrans());
			//listener.SetBox(&boxinfo);

			TarjaMgr::Instance()->GiveTarjaAward(pRole,pScene);
			///> 助战不给奖励
			if (!isHelper)
			{
				reward.GiveReward();
				pRole->Get<StageMgr>()->OnFinishAbyss(pScene->GetAbyssID());
				if (pTeam&&pTeam->IsAllCross())
				{
					DesignationMgr::Instance()->OnSceneFinished(pRole, pScene->GetSceneTemplateID(), stageinfo);
					AchieveV2Mgr::Instance()->OnSceneFinished(pRole, pScene->GetSceneTemplateID(), stageinfo);
				}
				EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_StageStar, pScene->GetSceneTemplateID(), stageinfo.rank);
			}
			///> 赏金团奖励
			auto j = teamcostreward.find(pRole->GetID());
			if (j != teamcostreward.end())
			{
				reward.GiveTeamCostReward(j->second);
			}
			listener.SetRoleEndState(pRole);
		}
		SSInfo<<"role:"<<pRole<<" scene end, iswin:"<<win<<END;
	}

    // 目标奖励
    {
        if(win && pScene->IsAllRolesFromSameServerID())
        {
            if(SCENE_NEST == pScene->GetSceneType())
            {
                int timespan = pScene->GetTimeSpan();
                int nodeathcount = 0;
                double totaldamage = 0.f;
                for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
                {
                    Role* pRole = *i;
                    if(NULL ==  pRole) continue;
                    const CombatStatistics& stat = pRole->GetStatistics();
                    if(0 == stat.GetDeathCount()) nodeathcount++;
                    totaldamage += stat.GetTotalDamage();
                }
                if(timespan != 0) totaldamage = totaldamage/timespan/60.f;
                for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
                {
                    Role* pRole = *i;
                    if(NULL ==  pRole) continue;
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_401, (double)timespan/60.f, pScene->GetSceneTemplateID());
                    if(pScene->GetEnterRoleCount() == nodeathcount)
                    {
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_402, (double)nodeathcount, pScene->GetSceneTemplateID());
                    }
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_403, totaldamage, pScene->GetSceneTemplateID());

                    bool isHelper = pScene->IsHelper(pRole->GetID());
                    if(isHelper)
                    {
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_702, 1.f, pScene->GetSceneTemplateID());
                    }
                }
            }

            if(SCENE_DRAGON == pScene->GetSceneType())
            {
                int timespan = pScene->GetTimeSpan();
                int nodamagecount = 0;
                int nodeathcount = 0;
                double totaldamage = 0.f;
                for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
                {
                    Role* pRole = *i;
                    if(NULL ==  pRole) continue;
                    const CombatStatistics& stat = pRole->GetStatistics();
                    if(pRole->GetAttr(TOTAL_CurrentHP) == pRole->GetAttr(TOTAL_MaxHP)) nodamagecount++;
                    if(0 == stat.GetDeathCount()) nodeathcount++;
                    totaldamage += stat.GetTotalDamage();
                }
                if(timespan != 0) totaldamage = totaldamage/timespan/60.f;
                for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
                {
                    Role* pRole = *i;
                    if(NULL ==  pRole) continue;
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_501, (double)timespan/60.f, pScene->GetSceneTemplateID());
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_502, (double)nodamagecount, pScene->GetSceneTemplateID());
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_503, totaldamage, pScene->GetSceneTemplateID());
                    if(pScene->GetEnterRoleCount() == nodeathcount)
                    {
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_504, (double)nodeathcount, pScene->GetSceneTemplateID());
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_11xx, (double)nodeathcount, pScene->GetSceneTemplateID());
                    }

                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_12xx, (double)timespan/60.f, pScene->GetSceneTemplateID());

                    const CombatStatistics& stat = pRole->GetStatistics();
                    if(stat.GetTotalDamage() == 0 && stat.GetDeathCount() == 0)
                    {
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_505, 1, pScene->GetSceneTemplateID());
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_13xx, 1, pScene->GetSceneTemplateID());
                    }

                    bool isHelper = pScene->IsHelper(pRole->GetID());
                    if(isHelper)
                    {
                        GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_701, 1.f, pScene->GetSceneTemplateID());
                    }
                }
            }
            if(SCENE_NEST == pScene->GetSceneType() || SCENE_DRAGON == pScene->GetSceneType())
            {
                GoalAwardsConfig::Instance()->TeamFriend(roRoles, pScene->GetSceneTemplateID(), pScene->GetSceneType());
            }
            if(SCENE_COUPLE == pScene->GetSceneType())
            {
                for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
                {
                    Role* pRole = *i;
                    if(NULL ==  pRole) continue;
                    GoalAwardsConfig::Instance()->SetValue(pRole->GetID(), GOAL_AWARD_ID_1401, 1.f, pScene->GetSceneTemplateID());
                }
            }
        }
    }

	///> 数据收集结束，通知客户端
	OnSceneFinishNotify(listener, pScene, roRoles, win);
	///>成就检查
	_CheckAchievement(pScene, win, roRoles);

	///> notify ms
	FillExDataToMs(&listener, pScene, win, roRoles);
	listener.NotifyMS(roRoles);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ArenaSceneFinishCB ArenaSceneFinishCB::GlobalArenaSceneFinishCB;

void ArenaSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	if (!roRoles.empty())
	{
		Role* role = roRoles[0];
		RpcG2M_EndAreanMs* rpc = RpcG2M_EndAreanMs::CreateRpc();
		rpc->m_oArg.set_iswin(win);
		rpc->m_oArg.set_roleid(role->GetID());
		role->SendToMS(*rpc);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PkSceneFinishCB PkSceneFinishCB::GlobalPkSceneFinishCB;
int OperTPkLog(Role* pRole, Role* pEmenyRole, SPkResult* tempresult)
{
	if (NULL == pRole || NULL == pEmenyRole)
	{
		return -1;
	}
	TPkLog oLog(pRole);
	oLog.m_nVipLevel = pRole->Get<CPayV2Record>()->GetVipLevel();
	oLog.m_nEmenyRoleID = pEmenyRole->GetID();
	oLog.strEmenyName = pEmenyRole->GetName();
	oLog.m_nEmenyPro = pEmenyRole->GetProfession();
	oLog.m_nEmenyPowerPoint = (INT32)pEmenyRole->GetAttr(TOTAL_POWERPOINT);
	CPkRecord* record = pRole->Get<CPkRecord>();
	record->Update();
	UINT32 point = record->GetPoint();
	oLog.m_nAfterPoint = point;
	if (NULL == tempresult)
	{
		oLog.m_nBeforePoint = point;
		oLog.m_nResultID = KKSG::PkResult_Lose - 1;
	}
	else
	{
		oLog.m_nBeforePoint = point - tempresult->point;
		oLog.m_nResultID = tempresult->result - 1;
	}
	oLog.Do();
	return 0;
}

void PkSceneFinishCB::OnSceneFinished(Scene* scene, std::vector<Role*>& roles, bool win)
{
	ScenePk* pkhandler = scene->GetPkHandler();
	if (NULL == pkhandler)
	{
		return;
	}

	// waiting role
	std::map<UINT64, Role*>& waitingroles = scene->GetWaitingRoles();
	for (auto i = waitingroles.begin(); i != waitingroles.end(); ++i)
	{
		Role* role = i->second;
		if (!role->IsWatcher())
		{
			roles.push_back(role);
		}
	}

	if(roles.size() == 2)
	{
		OperTPkLog(roles[0], roles[1], pkhandler->GetResult(roles[0]->GetID()));
		OperTPkLog(roles[1], roles[0], pkhandler->GetResult(roles[1]->GetID()));
	}

	NormalBattleResultListener listener(scene, win);

	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		listener.SetInitStatus(role);
		listener.SetSceneRank(RANK_SSS);
		
		KKSG::PkResult* result = listener.GetPkResult();
		pkhandler->FillResult(result, role);

		role->SetRecvedReward();
		PkMgr::Instance()->HintNotify(role);

		listener.SetStatisticsInfo(role->GetStatistics());
	}

	for (auto i = pkhandler->GetRobots().begin(); i != pkhandler->GetRobots().end(); ++i)	
	{
		DummyRole* robot = DummyRoleManager::Instance()->Find(*i);
		if (NULL != robot)
		{
			listener.SetInitStatus(robot);
			listener.SetStatisticsInfo(robot->GetStatistics());
		}
	}

	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		listener.NotifyClient(role);
	}

	if (!roles.empty())
	{
		listener.SetWatchPvp(pkhandler->GetWins().empty() ? 0 : *(pkhandler->GetWins().begin()));
		listener.NotifyWatcher(scene);
	}

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_PVP, scene->GetSceneID(), 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PkTwoSceneFinishCB PkTwoSceneFinishCB::GlobalPkTwoSceneFinishCB;
void PkTwoSceneFinishCB::OnSceneFinished(Scene* scene, std::vector<Role*>& roles, bool win)
{
	ScenePkTwo* pkhandler = scene->GetPkTwoHandler();
	if (NULL == pkhandler)
	{
		return;
	}

	// waiting role
	std::map<UINT64, Role*>& waitingroles = scene->GetWaitingRoles();
	for (auto i = waitingroles.begin(); i != waitingroles.end(); ++i)
	{
		Role* role = i->second;
		if (!role->IsWatcher())
		{
			roles.push_back(role);
		}
	}

	//if(roles.size() == 2)
	//{
	//	OperTPkLog(roles[0], roles[1], pkhandler->GetResult(roles[0]->GetID()));
	//	OperTPkLog(roles[1], roles[0], pkhandler->GetResult(roles[1]->GetID()));
	//}

	NormalBattleResultListener listener(scene, win);

	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		listener.SetInitStatus(role);
		listener.SetSceneRank(RANK_SSS);
		
		KKSG::PkResult* result = listener.GetPkResult();
		pkhandler->FillResult(result, role);

		role->SetRecvedReward();
		PkMgr::Instance()->HintNotify(role);

		listener.SetStatisticsInfo(role->GetStatistics());
	}

	std::list<UINT64> robots;
	pkhandler->FillRobots(robots);
	for (auto i = robots.begin(); i != robots.end(); ++i)	
	{
		DummyRole* robot = DummyRoleManager::Instance()->Find(*i);
		if (NULL != robot)
		{
			listener.SetInitStatus(robot);
			listener.SetStatisticsInfo(robot->GetStatistics());
		}
	}

	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		listener.NotifyClient(role);
	}

	if (!roles.empty())
	{
		listener.SetWatchPvp(pkhandler->GetWin());
		listener.NotifyWatcher(scene);
	}

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_PVP2, scene->GetSceneID(), 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CustomPkSceneFinishCB CustomPkSceneFinishCB::GlobalCustomPkSceneFinishCB;
void CustomPkSceneFinishCB::OnSceneFinished(Scene* scene, std::vector<Role*>& roles, bool win)
{
	SceneCustomPk* pkhandler = scene->GetCustomPkHandler();
	if (NULL == pkhandler)
	{
		return;
	}
	// waiting role
	std::map<UINT64, Role*>& waitingroles = scene->GetWaitingRoles();
	for (auto i = waitingroles.begin(); i != waitingroles.end(); ++i)
	{
		Role* role = i->second;
		if (!role->IsWatcher())
		{
			roles.push_back(role);
		}
	}

	const KKSG::CustomBattleMatchBattleInfo& battle = pkhandler->GetCustomBattle();
	///> 系统比赛通知客户端 或者场景中有玩家不在，给客户端结果
	if (battle.issystem() || pkhandler->IsOneQuit())
	{
		UINT64 winroleid = 0;
		NormalBattleResultListener listener(scene, win);
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			listener.SetInitStatus(role);
			listener.SetSceneRank(RANK_SSS);
			
			bool iswin = pkhandler->IsWin(role->GetID());
			if (iswin)
			{
				winroleid = role->GetID();
			}

			KKSG::CustomBattleResult* result= listener.GetCustomBattleResult();
			result->set_result(iswin ? PkResult_Win : PkResult_Lose);
			result->set_fightgroup(role->GetFightGroup());

			SSInfo<<"roleid:"<<role->GetID()<<" battleid:"<<battle.battleuid()<<" is system:"<<battle.issystem()<<" iswin:"<<iswin<<END;

			listener.SetStatisticsInfo(role->GetStatistics());
		}
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			listener.NotifyClient(role);
		}
		if (!roles.empty())
		{
			listener.SetWatchPvp(winroleid);
			listener.NotifyWatcher(scene);
		}
	}

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_CUSTOMPK, scene->GetSceneID(), 0, 0, 0);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CustomPkTwoSceneFinishCB CustomPkTwoSceneFinishCB::GlobalCustomPkTwoSceneFinishCB;
void CustomPkTwoSceneFinishCB::OnSceneFinished(Scene* scene, std::vector<Role*>& roles, bool win)
{
	//SceneCustomPk* pkhandler = scene->GetCustomPkHandler();
	//if (NULL == pkhandler)
	//{
	//	return;
	//}
	//// waiting role
	//std::map<UINT64, Role*>& waitingroles = scene->GetWaitingRoles();
	//for (auto i = waitingroles.begin(); i != waitingroles.end(); ++i)
	//{
	//	Role* role = i->second;
	//	if (!role->IsWatcher())
	//	{
	//		roles.push_back(role);
	//	}
	//}

	//const KKSG::CustomBattleMatchBattleInfo& battle = pkhandler->GetCustomBattle();
	/////> 系统比赛通知客户端 或者场景中有玩家不在，给客户端结果
	//if (battle.issystem() || pkhandler->IsOneQuit())
	//{
	//	NormalBattleResultListener listener(scene, win);
	//	for (auto i = roles.begin(); i != roles.end(); ++i)
	//	{
	//		Role* role = *i;
	//		listener.SetInitStatus(role);
	//		listener.SetSceneRank(RANK_SSS);
	//		
	//		bool iswin = pkhandler->IsWin(role->GetID());

	//		KKSG::CustomBattleResult* result= listener.GetCustomBattleResult();
	//		result->set_result(iswin ? PkResult_Win : PkResult_Lose);
	//		result->set_fightgroup(role->GetFightGroup());

	//		SSInfo<<"roleid:"<<role->GetID()<<" battleid:"<<battle.battleuid()<<" is system:"<<battle.issystem()<<" iswin:"<<iswin<<END;

	//		listener.SetStatisticsInfo(role->GetStatistics());
	//	}
	//	for (auto i = roles.begin(); i != roles.end(); ++i)
	//	{
	//		Role* role = *i;
	//		listener.NotifyClient(role);
	//	}
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
PvpSceneFinishCB PvpSceneFinishCB::GlobalPvpSceneFinishCB;

void PvpSceneFinishCB::OnSceneFinished( Scene *pScene, std::vector<Role *> &roRoles, bool win )
{
	ScenePvp* pvphandler = pScene->GetPvpHandler();
	if(NULL == pvphandler)
	{
		return;
	}
	NormalBattleResultListener listener(pScene, win);
	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		
		listener.SetInitStatus(pRole);
		listener.SetSceneRank(RANK_SSS);

		listener.SetStatisticsInfo(pRole->GetStatistics());

		KKSG::PVPResult* pvpResult = listener.GetPvpResult();
		pvphandler->FillPvpResult(pvpResult, pRole);

		listener.SetRoleEndState(pRole);
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		listener.NotifyClient(pRole);
	}

	if (!roRoles.empty())
	{
		listener.SetWatchPvp(pvphandler->GetWinUId(), pvphandler->GetMVP());
		listener.NotifyWatcher(pScene);
	}	

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_PROTECTCAPTAIN, pScene->GetSceneID(), 0, 0, 0);

	std::vector<Role*> redCampRoles;
	std::vector<Role*> blueCampRoles;
	for (auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = *it;
		PvpCampType camp = pvphandler->GetRoleCamp(pRole->GetID());	
		if (camp == PVP_CAMP_RED)
		{
			redCampRoles.push_back(pRole);
		}
		else if (camp == PVP_CAMP_BLUE)
		{
			blueCampRoles.push_back(pRole);
		}

		if (camp != PVP_CAMP_ERR && pvphandler->GetFinalWiner() == camp)
		{
			pRole->OnSceneComplete(pScene);
		}
		else if (pvphandler->GetFinalWiner() == PVP_CAMP_ERR)
		{
			//平局
			pRole->OnSceneDraw(pScene);
		}
		else
		{
			//输
			pRole->OnSceneFailed(pScene);
		}
	}
	CheckPartner(pScene, redCampRoles, pvphandler->GetFinalWiner() == PVP_CAMP_RED);
	CheckPartner(pScene, blueCampRoles, pvphandler->GetFinalWiner() == PVP_CAMP_BLUE);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GmfSceneFinishCB GmfSceneFinishCB::GlobalGmfSceneFinishCB;

void GmfSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	NormalBattleResultListener listener(pScene, win);
	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		pRole->OnSceneComplete(pScene);
		listener.SetInitStatus(pRole);
		listener.SetStatisticsInfo(pRole->GetStatistics());
	}
	if (!roRoles.empty())
	{
		listener.NotifyWatcher(pScene);
	}

	auto gwb = pScene->GetGWBHandler();
	if(gwb)
	{
		gwb->RemoveLive();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DragonExpSceneFinishCB DragonExpSceneFinishCB::GlobalDragonExpSceneFinishCB;

void DragonExpSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	StageRankInfo stageinfo;

	NormalBattleResultListener listener(pScene, true);
	for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
	{
		Role* pRole = (*i);

		stageinfo.hppercent = pRole->GetHpPercent();
		stageinfo.timespan = pScene->GetTimeSpan();
		stageinfo.deathcount = pScene->GetMortuary()->GetDeathCount(pRole->GetID());
		if(win)
		{
			stageinfo.rank = RANK_SSS;
			pRole->Get<StageMgr>()->SetRank(pScene->GetSceneTemplateID(), stageinfo.rank);
		}

		pScene->SetStageRankInfo(pRole->GetID(), stageinfo);

		listener.SetInitStatus(pRole, 0);
		if(pScene->GetDEHandler())
		{
			KKSG::DragonExpResult* pDEResult = listener.GetDEResult();	
			pScene->GetDEHandler()->FillResult(pDEResult, pRole, win);
		}

		listener.SetStatisticsInfo(pRole->GetStatistics());
		pRole->SetRecvedReward();
	}

	for (auto i = roRoles.begin(); i != roRoles.end(); ++i)
	{
		Role *pRole = (*i);

		listener.NotifyClient(pRole);

		if (win)
		{
			pRole->OnSceneComplete(pScene);
		}
		else
		{
			pRole->OnSceneFailed(pScene);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SkyCitySceneFinishCB SkyCitySceneFinishCB::GlobalSkyCitySceneFinishCB;

void SkyCitySceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	SceneSkyCity* pHandler = pScene->GetSkyCityHandler();
	if(NULL == pHandler)
	{
		return;
	}
	NormalBattleResultListener listener(pScene, win);
	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		
		listener.SetInitStatus(pRole);
		listener.SetSceneRank(RANK_SSS);

		listener.SetStatisticsInfo(pRole->GetStatistics());

		KKSG::SkyCityFinalInfo* result = listener.GetSkyCityResult();
		pHandler->FillLastResult(pRole, *result);

		listener.SetRoleEndState(pRole);
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		listener.NotifyClient(pRole);
		pRole->OnSceneComplete(pScene);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ResWarSceneFinishCB ResWarSceneFinishCB::GlobalResWarSceneFinishCB;

void ResWarSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	SceneResWar* pHandler = pScene->GetResWarHandler();
	if(NULL == pHandler)
	{ 
		return;
	}
	NormalBattleResultListener winlistener(pScene, true);
	NormalBattleResultListener falselistener(pScene, false);
	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		UINT32 nTeamID = pHandler->GetTeamID(pRole->GetID());
		if (nTeamID == pHandler->GetWinTeam())
		{
			winlistener.SetInitStatus(pRole);
			winlistener.SetSceneRank(RANK_SSS);
			StageReward reward(pRole, pScene, pScene->GetSceneType());
			reward.SetListener(&winlistener);
			reward.GiveReward();
			winlistener.SetRoleEndState(pRole);
		}
		else
		{
			falselistener.SetInitStatus(pRole);
			falselistener.SetRoleEndState(pRole);
		}
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		UINT32 nTeamID = pHandler->GetTeamID(pRole->GetID());
		if (nTeamID == pHandler->GetWinTeam())
		{
			winlistener.NotifyClient(pRole);
			pRole->OnSceneComplete(pScene);
		}
		else
		{
			falselistener.NotifyClient(pRole);
			pRole->OnSceneFailed(pScene);
		}
	}
	winlistener.NotifyMS(roRoles);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HorseSceneFinishCB HorseSceneFinishCB::GlobalHorseSceneFinishCB;

void HorseSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	LogDebug("HorseSceneFinishCB::OnSceneFinished");
	SceneHorseRace* pHandler = pScene->GetHorseRaceHandler();
	if(NULL == pHandler)
	{ 
		return;
	}

	NormalBattleResultListener winlistener(pScene, true);
	std::vector<RoleRaceInfo>  vecRoleRaceInfo = pHandler->GetAllRaceInfo();
	
	for(auto iter = vecRoleRaceInfo.begin(); iter != vecRoleRaceInfo.end(); ++iter)
	{
		Role *pRole = NULL;
		for (auto miter = roRoles.begin(); miter!=roRoles.end(); miter++)
		{
			if((*iter).nRoleID==(*miter)->GetID())
			{
				pRole = *miter;
				break;
			}
		}
		if (pRole)
		{
			winlistener.SetInitStatus(pRole);
			StageReward reward(pRole, pScene, pScene->GetSceneType());
			reward.SetListener(&winlistener);
			reward.GiveReward();
			winlistener.SetRoleEndState(pRole);
		}else
		{
			winlistener.SetInitStatus((*iter).nRoleID,0,0,(*iter).szName,(*iter).nLv,(*iter).nExp,(*iter).nJob,0);
			KKSG::HorseAward  *pRank = winlistener.GetHorseResult();
			pRank->set_horse((*iter).nHorse) ;
			pRank->set_rank((*iter).nRank);
			if ((*iter).nState==eFinal)
			{			
				pRank->set_time((*iter).nTime);
			}

			HorseRace::RowData* pRowData = HorseRideMgr::Instance()->GetRank((*iter).nRank,pScene->GetSceneTemplateID());
			if (pRowData)
			{
				for (auto iter = pRowData->Reward.begin(); iter!= pRowData->Reward.end(); iter++)
				{
					winlistener.AddItem((*iter)[0],(*iter)[1],true,IStageRewardListener::NormalStageReward);
				}
			}
		}
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		winlistener.NotifyClient(pRole);
		pRole->OnSceneComplete(pScene);
		pRole->Get<CReportDataRecord>()->UpdateHorseMatchCount();
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HeroBattleSceneFinishCB HeroBattleSceneFinishCB::GlobalHeroBattleSceneFinishCB;

void HeroBattleSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	LogDebug("HeroBattleSceneFinish!");
	SceneHeroBattle* pHandler = pScene->GetHeroBattleHandler();
	if (pHandler == NULL)
	{
		LogError("HeroBattleHandler is null, Sceneid [%u]", pScene->GetSceneID());
		return;
	}
	NormalBattleResultListener listener(pScene, win);
	const std::unordered_map<UINT64, HeroBattleRoleData>& allRoles = pHandler->GetRoleDatas();

	CombatStatistics emptyStat;
	for (auto it = allRoles.begin(); it != allRoles.end(); ++it)
	{
		const CombatStatistics* stat = pHandler->GetCombatStatistics(it->first);
		if (stat == NULL)
		{
			stat = &emptyStat;
		}
		listener.SetInitStatus(it->second.info);
		listener.SetSceneRank(RANK_SSS);
		listener.SetStatisticsInfo(*stat);

		KKSG::HeroBattleResult* result = listener.GetHeroBattleResult();
		pHandler->FillResultAndReward(result, it->first);
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		listener.NotifyClient(pRole);
		if (pHandler->IsWinRole(pRole->GetID()))
		{
			pRole->OnSceneComplete(pScene);
		}
		else
		{
			pRole->OnSceneFailed(pScene);
		}

		if (pHandler->IsWinRole(pRole->GetID()))
		{
			// 手Q宝箱
			if (pRole->GetClientConf().logintype() == KKSG::LOGIN_QQ_PF || pRole->GetClientConf().logintype() == KKSG::LGOIN_WECHAT_PF)
			{
				LotteryMgr::Instance()->GiveShareRandomGift(pRole, KKSG::SCENE_HEROBATTLE);
			}
		}
	}

	if (!roRoles.empty())
	{
		listener.SetWatchPvp(pHandler->GetWinTeam(), pHandler->GetMvpID());
		listener.NotifyWatcher(pScene);
	}

	LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_HEROBATTLE, pScene->GetSceneID(), 0, 0, 0);

	std::vector<Role*> redCampRoles;
	std::vector<Role*> blueCampRoles;
	for (auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = *it;
		UINT32 camp = pHandler->GetRoleCamp(pRole->GetID());	
		if (camp == pHandler->GetTeam1())
		{
			redCampRoles.push_back(pRole);
		}
		else if (camp == pHandler->GetTeam2())
		{
			blueCampRoles.push_back(pRole);
		}
	}
	CheckPartner(pScene, redCampRoles, pHandler->GetWinTeam() == pHandler->GetTeam1());
	CheckPartner(pScene, blueCampRoles, pHandler->GetWinTeam() == pHandler->GetTeam2());
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MobaBattleSceneFinishCB MobaBattleSceneFinishCB::GlobalMobaBattleSceneFinishCB;

void MobaBattleSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	LogDebug("MobaBattleSceneFinish!");
	SceneMobaBattle* pHandler = pScene->GetMobaBattleHandler();
	if (pHandler == NULL)
	{
		LogError("MobaBattleHandler is null, Sceneid [%u]", pScene->GetSceneID());
		return;
	}
	NormalBattleResultListener listener(pScene, win);
	KKSG::MobaBattleResult* result = listener.GetMobaBattleResult();
	pHandler->FillResult(result);	

	std::unordered_map<UINT64, MobaRole>& allRoles = pHandler->GetRoleDatas();
	for (auto it = allRoles.begin(); it != allRoles.end(); ++it)
	{
		const CombatStatistics& stat = it->second.GetCombatStatistics();
		listener.SetInitStatus(it->second.GetPvpRoleInfo());
		listener.SetSceneRank(RANK_SSS);
		listener.SetStatisticsInfo(stat);

		KKSG::MobaBattleRoleResult* roleResult = listener.GetMobaBattleRoleResult();
		pHandler->FillRoleResultAndReward(roleResult, it->first);
	}

	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		listener.NotifyClient(pRole);
		if (pHandler->IsWinRole(pRole->GetID()))
		{
			pRole->OnSceneComplete(pScene);
		}
		else
		{
			pRole->OnSceneFailed(pScene);
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InvFightSceneFinishCB InvFightSceneFinishCB::GlobalInvFightSceneFinishCB;

void InvFightSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	SceneInvFigtht* pInvFight = pScene->GetInvFightHandler();
	if(NULL == pInvFight)
	{
		return;
	}

	std::vector<Role*> tmpRoles(roRoles.begin(), roRoles.end());

	std::map<UINT64, Role*>& waitingroles = pScene->GetWaitingRoles();
	for (auto i = waitingroles.begin(); i != waitingroles.end(); ++i)
	{
		Role* role = i->second;
		if (!role->IsWatcher())
		{
			tmpRoles.push_back(role);
		}
	}
	NormalBattleResultListener listener(pScene, win);
	for(auto it = tmpRoles.begin(); it != tmpRoles.end(); ++it)
	{
		Role* pRole = (*it);
		
		listener.SetInitStatus(pRole);
		listener.SetSceneRank(RANK_SSS);

		listener.SetStatisticsInfo(pRole->GetStatistics());

		KKSG::InvFightBattleResult* invfResult = listener.GetInvFightResult();
		pInvFight->FillResult(pRole->GetID(), *invfResult);

		listener.SetRoleEndState(pRole);
	}

	for(auto it = tmpRoles.begin(); it != tmpRoles.end(); ++it)
	{
		Role* pRole = (*it);
		listener.NotifyClient(pRole);
		pRole->OnSceneComplete(pScene);
	}
}

LeagueSceneFinishCB LeagueSceneFinishCB::GlobalLeagueSceneFinishCB;

void LeagueSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	NormalBattleResultListener listener(pScene, win);
	for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
	{
		Role* pRole = (*it);
		pRole->OnSceneComplete(pScene);
		listener.SetInitStatus(pRole);
		listener.SetStatisticsInfo(pRole->GetStatistics());
	}
	if (!roRoles.empty())
	{
		listener.NotifyWatcher(pScene);
	}
}

WeekEnd4v4MonsterFightSceneFinishCB WeekEnd4v4MonsterFightSceneFinishCB::GlobalMonsterFightSceneFinishCB;

void WeekEnd4v4MonsterFightSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4MonsterFight* pHandler = pScene->GetMonsterFightHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    KKSG::PvpRoleInfo info;
    for(auto itor = pHandler->m_roleResult.begin();itor != pHandler->m_roleResult.end(); ++itor)
    {
        info.set_roleid(itor->m_roleid);
        listener.SetInitStatus(info);
        KKSG::WeekEnd4v4BattleRoleData* roleresult = listener.GetWeekEnd4v4RoleResult();
        pHandler->FillRoleResultData(roleresult, itor->m_roleid);
    }

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

WeekEnd4v4GhostActionSceneFinishCB WeekEnd4v4GhostActionSceneFinishCB::GlobalGhostActionSceneFinishCB;

void WeekEnd4v4GhostActionSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4GhostAction* pHandler = pScene->GetGhostActionHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    KKSG::PvpRoleInfo info;
    for(auto itor = pHandler->m_roleResult.begin();itor != pHandler->m_roleResult.end(); ++itor)
    {
        info.set_roleid(itor->m_roleid);
        listener.SetInitStatus(info);
        KKSG::WeekEnd4v4BattleRoleData* roleresult = listener.GetWeekEnd4v4RoleResult();
        pHandler->FillRoleResultData(roleresult, itor->m_roleid);
    }

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

WeekEnd4v4LiveChallengeSceneFinishCB WeekEnd4v4LiveChallengeSceneFinishCB::GlobalLiveChallengeSceneFinishCB;

void WeekEnd4v4LiveChallengeSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4LiveChallenge* pHandler = pScene->GetLiveChallengeHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    KKSG::PvpRoleInfo info;
    for(auto itor = pHandler->m_roleResult.begin();itor != pHandler->m_roleResult.end(); ++itor)
    {
        info.set_roleid(itor->m_roleid);
        listener.SetInitStatus(info);
        KKSG::WeekEnd4v4BattleRoleData* roleresult = listener.GetWeekEnd4v4RoleResult();
        pHandler->FillRoleResultData(roleresult, itor->m_roleid);
    }

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

WeekEnd4v4CrazyBombSceneFinishCB WeekEnd4v4CrazyBombSceneFinishCB::GlobalCrazyBombSceneFinishCB;

void WeekEnd4v4CrazyBombSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4CrazyBomb* pHandler = pScene->GetCrazyBombHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    KKSG::PvpRoleInfo info;
    for(auto itor = pHandler->m_roleResult.begin();itor != pHandler->m_roleResult.end(); ++itor)
    {
        info.set_roleid(itor->m_roleid);
        listener.SetInitStatus(info);
        KKSG::WeekEnd4v4BattleRoleData* roleresult = listener.GetWeekEnd4v4RoleResult();
        pHandler->FillRoleResultData(roleresult, itor->m_roleid);
    }

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

WeekEnd4v4HorseRacingSceneFinishCB WeekEnd4v4HorseRacingSceneFinishCB::GlobalHorseRacingSceneFinishCB;

void WeekEnd4v4HorseRacingSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4HorseRacing* pHandler = pScene->GetWeekEnd4v4HorseRacingHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    pHandler->FillResult(listener);

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

WeekEnd4v4DuckSceneFinishCB WeekEnd4v4DuckSceneFinishCB::GlobalDuckSceneFinishCB;

void WeekEnd4v4DuckSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
    SceneWeekEnd4v4Duck* pHandler = pScene->GetDuckHandler();
    if(NULL == pHandler)
    {
        return;
    }

    UINT32 redscore = 0;
    UINT32 bluescore = 0;
    pHandler->GetTeamScore(redscore, bluescore);
    NormalBattleResultListener listener(pScene, true);
    KKSG::WeekEnd4v4TeamResult* teamResult = listener.GetWeekEnd4v4TeamResult();
    teamResult->set_redteamscore(redscore);
    teamResult->set_blueteamscore(bluescore);
    teamResult->set_teamseconds(pHandler->GetTeamSeconds());
    pHandler->AddItemList(teamResult, roRoles, redscore, bluescore);

    KKSG::PvpRoleInfo info;
    for(auto itor = pHandler->m_roleResult.begin();itor != pHandler->m_roleResult.end(); ++itor)
    {
        info.set_roleid(itor->m_roleid);
        listener.SetInitStatus(info);
        KKSG::WeekEnd4v4BattleRoleData* roleresult = listener.GetWeekEnd4v4RoleResult();
        pHandler->FillRoleResultData(roleresult, itor->m_roleid);
    }

    for(auto it = roRoles.begin(); it != roRoles.end(); ++it)
    {
        Role* pRole = (*it);
        listener.NotifyClient(pRole);
        if(pHandler->IsWin(pRole, redscore, bluescore))
        {
            pRole->OnSceneComplete(pScene);
        }else{
            pRole->OnSceneFailed(pScene);
        }
    }
}

BattleFieldSceneFinishCB BattleFieldSceneFinishCB::GlobalBattleFieldSceneFinishCB;
void BattleFieldSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	SceneBFFight* pHandle =  pScene->GetBFFightHandler();
	if (pHandle==NULL)
		return;
	NormalBattleResultListener listener(pScene, true);
	listener.SetEnd(pHandle->GetFinal());
	std::vector<stBFRoleInfo> roleList;
	pHandle->GetRoleList(roleList);
	for (auto iter = roleList.begin(); iter!=roleList.end(); iter++)
	{
		Role *pRole = pHandle->GetRole(iter->nRoleID);
		if (pRole)
		{
			listener.SetInitStatus(pRole);
			KKSG::BattleFieldBattleResult *pResult = listener.GetBattleFieldResult();
			pHandle->FillBattleResult(iter->nRoleID,*pResult);
			StageReward reward(pRole, pScene, pScene->GetSceneType());
			reward.SetListener(&listener);
			reward.GiveReward();
		}else
		{
			listener.SetInitStatus((*iter).nRoleID,0,0,(*iter).szName,0,0,(*iter).job,0);
			KKSG::BattleFieldBattleResult *pResult = listener.GetBattleFieldResult();
			pHandle->FillBattleResult(iter->nRoleID,*pResult);
		}
	}
	for(auto iter = roRoles.begin(); iter != roRoles.end(); ++iter)
	{
		Role* pRole = (*iter);
		listener.NotifyClient(pRole);
		pRole->OnSceneComplete(pScene);
	}
}

void SurviveSceneFinishCB::OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win)
{
	SSDebug << __FUNCTION__ << " survive scene end " << END;	
}

SurviveSceneFinishCB SurviveSceneFinishCB::GlobalSurviveSceneFinishCB;
