#include "pch.h"
#include "stagereward.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "bagtransition.h"
#include "itemconfig.h"
#include "pb/project.pb.h"
#include "stagemgr.h"
#include "globalconfig.h"
#include "scene/scenefinishcb.h"
#include "foreach.h"
#include "scene/scenedoodad.h"
#include "table/expeditionconfigmgr.h"
#include "rolesummarymgr.h"
#include "scene/scenetower.h"
#include "scene/scenereswarpve.h"
#include "scene/scenehorserace.h"
#include "gamelogic/team.h"
#include "gamelogic/teammgr.h"
#include "guild/guildconfig.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "gamelogic/noticemgr.h"
#include "table/PlayerLevelManager.h"
#include "bossrushmgr.h"
#include "util/XCommon.h"
#include "scene/sceneteam.h"
#include "gamelogic/firstpassrecord.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/superriskrecord.h"
#include "unit/dummyrole.h"
#include "config/itemdropconfig.h"
#include "gamelogic/designationMgr.h"
#include "reswar/reswarpvemgr.h"
#include "scene/battleresultlistener.h"
#include "buff/buffrecord.h"
#include "gamelogic/levelsealMgr.h"
#include "scene/scenereswar.h"
#include "scene/scenestatistics.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "gamelogic/qqviprecord.h"
#include "gamelogic/horsemgr.h"
#include "teamrecord.h"
#include "reportdatamgr.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivityhandler.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivityrecord.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/mailsender.h"
#include "gamelogic/platformshare_record.h"
#include "scene/scenemorphinfo.h"
#include "scene/sceneconfig.h"
#include "servermgr/servercrossdatamgr.h"
#include "scene/battlefieldfight.h"
#include "define/itemdef.h"
#include "gamelogic/dragonconfig.h"

static int SceneTypeToReason(int scenetype)
{
	switch (scenetype)
	{
	case KKSG::SCENE_BATTLE:
		return ItemFlow_Stage_Battle;
	case KKSG::SCENE_NEST:
		return ItemFlow_Stage_Nest;
	case KKSG::SCENE_ARENA:
		return ItemFlow_Stage_Arena;
	case KKSG::SCENE_BOSSRUSH:
		return ItemFlow_Stage_BossRush;
	case KKSG::SCENE_PK:
	case KKSG::SCENE_PKTWO://记录成一个
		return ItemFlow_Stage_Pk;
	case KKSG::SCENE_ABYSSS:
		return ItemFlow_Stage_Abysss;
	case KKSG::SCENE_TOWER:
		return ItemFlow_Stage_Tower;
	case KKSG::SCENE_WEEK_NEST:
		return ItemFlow_WeekNest;
	case KKSG::SCENE_ACTIVITY_ONE:
		return ItemFlow_Stage_ActivityOne;
	case KKSG::SCENE_ACTIVITY_TWO:
		return ItemFlow_Stage_ActivityTwo;
	case KKSG::SCENE_ACTIVITY_THREE:
		return ItemFlow_Stage_ActivityThree;
	case KKSG::SCENE_ABYSS_PARTY:
		return ItemFlow_ABYSS_PARTY;
	case KKSG::SCENE_BIOHELL:
		return ItemFlow_BioHell;
	case KKSG::SCENE_DUCK:
		return ItemFlow_DuckAward;
	default:
		return ItemFlow_Stage_Unknown;
	}
}

StageReward::StageReward(Role *pRole, Scene* scene, UINT32 scenetype) : m_transition(pRole)
{
	m_scene = scene;
	m_pRole = pRole;
	m_Rank = RANK_EMPTY;
	m_pListener = NULL;
	m_exptransition = NULL;
	m_coinper = 1.0l;

	m_subreason = SceneTypeToReason(scenetype);

	if (scenetype == KKSG::SCENE_BATTLE || scenetype == KKSG::SCENE_ABYSSS)
	{
		if (m_pRole->Get<QQVipRecord>()->IsSVip())
		{
			m_coinper += ((double)GetGlobalConfig().QQSVipLevelReward.seq[1])/100;
		}
		else if (m_pRole->Get<QQVipRecord>()->IsVip())
		{
			m_coinper += ((double)GetGlobalConfig().QQVipLevelReward.seq[1])/100;
		}
	}
	if (scenetype == KKSG::SCENE_BATTLE)
	{
		KKSG::StartUpType type = m_pRole->GetStartUpType();
		if (KKSG::StartUp_QQ == type)
		{
			m_coinper += ((double)GetGlobalConfig().QQGameCenterLevelReward.seq[1])/100;
		}
		else if (KKSG::StartUp_WX == type)
		{
			m_coinper += ((double)GetGlobalConfig().WXGameCenterLevelReward.seq[1])/100;
		}
	}
}

StageReward::~StageReward()
{
	if (NULL != m_exptransition)
	{
		delete m_exptransition;
	}
}


void StageReward::GiveReward()
{
	Scene *pScene = m_pRole->GetCurrScene();
	if (pScene == NULL)
	{
		return;
	}

	if(!WeekNestRewardCount(pScene))//每周巢穴
	{
		return;
	}

	if (NotGiveSmallDragonReward(pScene))
	{
		return;
	}

	GiveScenePickedDoodad(pScene);
	GiveSyncScenePickedDoodad(pScene);
	// 
	if (!GiveFirstReward(pScene->GetSceneInfo(), pScene))
	{
		GiveCommonReward(pScene->GetSceneInfo());
	}

	///> 公会挑战奖励
	_GiveGuildReward(pScene);

	switch(pScene->GetSceneType())
	{
	case KKSG::SCENE_BOSSRUSH:
		GiveBossRushReward(pScene);
		break;
	case KKSG::SCENE_TOWER:
		GiveTowerReward(pScene);
		break;
	case KKSG::SCENE_RISK:
		GiveRiskReward(pScene);
		break;
	case KKSG::SCENE_RESWAR_PVE:
		GiveResWarPVEReward(pScene);
		break;
	case KKSG::SCENE_RESWAR_PVP:
		GiveResWarPVPReward(pScene);
		break;
	case KKSG::SCENE_HORSE_RACE:
		GiveHorseRaceReward(pScene);
		break;
	default:
		break;
	}
	///> 等级封印补偿
	//GiveSealExpItem(pScene->GetSceneInfo());

	// 假日活动掉落
	GiveSpActivityReward(pScene->GetSceneInfo());

	// 平台分享
	PlatShareResult(pScene->GetSceneTemplateID());

	m_pRole->SetRecvedReward();
	m_pRole->SetRewardString(m_rewardStream.str());
	m_transition.NotifyClient();
	m_pListener->ModifyExp(m_pRole);
}

void StageReward::GiveTeamCostReward(ItemDesc item)
{
	SSInfo << " role = " << m_pRole->GetID() << " itemid = " << item.itemID << " count = " << item.itemCount << END;

	if(0 == item.itemID)
	{
		SSError << " teamcost give itemid = 0 " << END;
		return;
	}
	if(0 == item.itemCount)
	{
		return;
	}
	// level seal
	UINT32 sceneseal = ExpeditionConfigMgr::Instance()->GetLevelSealTeamCost(m_scene->GetSceneTemplateID());//need expid TODO
	UINT32 currentseal = 1;
    if(GSConfig::Instance()->IsCrossGS())
    {
        currentseal = ServerCrossDataMgr::Instance()->GetSealTypeByRoleID(m_pRole->GetID());
    }
    else
    {
        currentseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
    }
	int giveType = 1;//0.不给 1.正常给 2.减半给
	UINT32 mailConfID = MailConf_TeamCostNormal;//正常给

	if(currentseal > (sceneseal + GetGlobalConfig().TeamLevelSealGap))
	{
		item.itemCount = (UINT32)(item.itemCount * 0.5f);//收益减半
		giveType = 2;//减半
		mailConfID = MailConf_TeamCostHalf;
	}

	if(DIAMOND == item.itemID)//钻石
	{
		if(m_pRole->Get<CTeamRecord>()->GetDiamondCostCountLeft() > 0)
		{
			m_pRole->Get<CTeamRecord>()->AddDiamondCostCount();
			SSInfo << " role get team diamond cost role = " << m_pRole->GetID() << END;

			//收税
			UINT32 dueCount = UINT32(item.itemCount * GetGlobalConfig().TeamDiamondRevenue / 100.0);
			item.itemCount = (item.itemCount >= dueCount)? (item.itemCount - dueCount): 0;
		}
		else//次数达到上线
		{
			giveType = 0;//不给
			mailConfID = MailConf_TeamDiamondCostCount;
		}
	}

	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(mailConfID);
	if(NULL == pMailConf)
	{
		SSError << " mail conf = null confid = " << mailConfID << END;
		return ;
		
	}
	MailSender mailsender;
	mailsender.SetContent(Mail_System, pMailConf->m_title, pMailConf->m_content);

	if(0 != giveType)
	{
		if(0 == item.itemCount)
		{
			item.itemCount = 1;//以防万一
		}
		mailsender.AddItem(item);
		mailsender.SetReason(ItemFlow_TeamCost, ItemFlow_TeamCostGive);

		m_pListener->AddItem(item.itemID, item.itemCount, true, IStageRewardListener::TeamCostReward);
	}

	mailsender.Send(m_pRole->GetID());
}

void StageReward::GiveTowerReward(Scene *pScene)
{
	if (pScene->GetSceneTower())
	{
		if (pScene->IsSceneWinState())
		{
			std::map<int, int> rewards;
			//pScene->GetSceneTower()->GetReward(m_pRole, rewards);
			pScene->GetSceneTower()->GetFloorReward(m_pRole, rewards);
			foreach(i in rewards)
			{
				AddItem(i->first, i->second);
			}
		}

		if(pScene->GetSceneTower()->CheckSendBattleResult(m_pRole))
		{
			std::map<int, int> hisrewards;
			pScene->GetSceneTower()->GetHisFloorReward(m_pRole, hisrewards);
			foreach(i in hisrewards)
			{
				m_pListener->AddItem(i->first, i->second, true, IStageRewardListener::NormalStageReward);
			}
		}
	}
}

void StageReward::GiveRiskReward(Scene *pScene)
{
	if (pScene->GetSceneRisk())
	{
		std::map<int, int> rewards;
		m_pRole->Get<CSuperRiskRecord>()->m_superRisk.GiveReward(rewards);
		foreach(i in rewards)
		{
			AddItem(i->first, i->second);
		}
	}
}



void StageReward::GiveHorseRaceReward(Scene *pScene)
{
	SceneHorseRace* pHorseRace = pScene->GetHorseRaceHandler();
	if (!pHorseRace) 
		return;
	UINT32 nRank	=0;
	UINT32 nHorse	=0;
	UINT32 nTime	=0;
	pHorseRace->GetRoleRank(m_pRole,nTime,nRank,nHorse);
	HorseRace::RowData* pRowData = HorseRideMgr::Instance()->GetRank(nRank,pScene->GetSceneTemplateID());
	if(!pRowData)
		return;
	NormalBattleResultListener* listener = dynamic_cast<NormalBattleResultListener*>(m_pListener);
	KKSG::HorseAward  *pRank = listener->GetHorseResult();
	pRank->set_horse(nHorse);
	pRank->set_rank(nRank);
	pRank->set_time(nTime);
	for (auto iter = pRowData->Reward.begin(); iter!= pRowData->Reward.end(); iter++)
	{
		AddItem((*iter)[0],(*iter)[1]);
	}
}


void StageReward::GiveResWarPVPReward(Scene *pScene)
{
	SceneResWar* pPVPScene = pScene->GetResWarHandler();
	if (!pPVPScene) 
		return;
	NormalBattleResultListener* listener = dynamic_cast<NormalBattleResultListener*>(m_pListener);
	UINT32 nLevel = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	GuildArenaBattlePvpReward::RowData *pCfg  =  ResWarAwardMgr::Instance()->GetPVPAward(nLevel);
	if (!pCfg) 
		return;
	for (auto iter = pCfg->MineralDrop.begin(); iter!=pCfg->MineralDrop.end();iter++)
	{
		const ItemDesc* pDropConf = ItemDropConfig::Instance()->GetRandomDrop(*iter, m_pRole->GetLevel());
		if(!pDropConf) 
			return;
		AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
	}
	listener->SetResWar(pCfg->MineralReward);
	KKSG::GuildExpAndConData* pMsg = listener->GetGuildInfo();
	KKSG::ResWarBase *pInfo = pMsg->mutable_reswar();
	UINT32 nTeamID			= pPVPScene->GetTeamID(m_pRole->GetID());
	UINT64 nGuildID			= pPVPScene->GetGuildID(m_pRole->GetID());
	UINT32 nTime = TimeUtil::GetTime();
	UINT32 nBuffTime = pPVPScene->GetGuildBuffTime(nGuildID);
	UINT32 nRes    = 0;
	if (nBuffTime > nTime)
	{
		nRes = pCfg->MineralReward*2;
	}else
	{
		nRes = pCfg->MineralReward;
	}
	pInfo->set_guildid(nGuildID);
	pInfo->set_teamid(nTeamID);
	pInfo->set_res(nRes);
	pInfo->set_bwin(true);
//调整一下代码的
	UINT32 randomall = 0;
	for (auto i = pCfg->MineralBuffReward.begin(); i != pCfg->MineralBuffReward.end(); ++i)
	{
		randomall += i->seq[1];
	}
	UINT32 random = XRandom::randInt(0, randomall + 1);
	UINT32 temp = 0;
	UINT32 itemid = 0;
	for (auto i = pCfg->MineralBuffReward.begin(); i != pCfg->MineralBuffReward.end(); ++i)
	{
		temp += i->seq[1];
		if (random <= temp)
		{
			itemid = i->seq[0];
			break;
		}
	}
	KKSG::ResWarRole *pWarRole = pInfo->add_rolelist();
	pWarRole->set_role(m_pRole->GetID());
	pWarRole->set_res(nRes);
	bool bRet = ResWarAwardMgr::Instance()->IsItemBuff(itemid);
	if (bRet)
	{
		KKSG::GuildBuffItem *pBuffItem = pInfo->add_itemlist();
		pBuffItem->set_roleid(m_pRole->GetID());
		pBuffItem->set_itemid(itemid);
		pBuffItem->set_count(1);
		listener->AddItem(itemid, 1, true);
		LogDebug("--GiveResWarPVPReward------GuildID=%llu Role=%llu-itemid =%d  itemcount=%d",nGuildID,m_pRole->GetID() ,itemid,1);
	}else
	{
		if (0 != itemid)
		{
			m_pRole->Get<BuffRecord>()->AddBuffItem(itemid);
			listener->AddItem(itemid, 1, true);
		}
	}
}



void StageReward::GiveResWarPVEReward(Scene *pScene)
{
	NormalBattleResultListener* listener = dynamic_cast<NormalBattleResultListener*>(m_pListener);
	if (!listener) 
		return;
	SceneResWarPVE* pPVEScene = pScene->GetSceneResWarPVE();
	if(!pPVEScene)
		return;
	UINT32 nMineID		=  pPVEScene->GetMineID();
	GuildMineralBattle::RowData *pCfg  =  ResWarAwardMgr::Instance()->GetPVEAward(nMineID);
	if (!pCfg)
		return;
	for (auto iter = pCfg->MineralDrop.begin(); iter!=pCfg->MineralDrop.end();iter++)
	{
		const ItemDesc* pDropConf = ItemDropConfig::Instance()->GetRandomDrop(*iter, m_pRole->GetLevel());
		if(!pDropConf) 
			return;
		AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
	}
	UINT32 nTime = TimeUtil::GetTime();
	UINT32 nBuffTime = pPVEScene->GetGuildBuffTime();
	UINT32 nRes    = 0;
	UINT32 nRBuffID = pPVEScene->GetRandBuffID();
	GuildMineralBufflist::RowData* pRowData = ResWarAwardMgr::Instance()->GetGuildMineralBufflist(nRBuffID);
	if (nBuffTime > nTime)
	{
		if (pRowData)
		{
			nRes = pCfg->MineralReward*(2+pRowData->RewardBuff);
		}else
		{
			nRes = pCfg->MineralReward*2;
		}
	}else
	{
		if (pRowData&&pRowData->RewardBuff)
		{
			nRes = pCfg->MineralReward*(pRowData->RewardBuff);
		}else
		{
			nRes = pCfg->MineralReward;
		}
	}
	LogDebug("GiveResWarPVEReward-----guildid=%llu teamid=%d cfg=%d nres=%d",pPVEScene->GetGuildID(),pPVEScene->GetTeamID(),pCfg->MineralReward,nRes);
	listener->SetResWar(nRes);
	KKSG::GuildExpAndConData* pMsg = listener->GetGuildInfo();
	KKSG::ResWarBase *pInfo = pMsg->mutable_reswar();
	pInfo->set_guildid(pPVEScene->GetGuildID());
	pInfo->set_teamid(pPVEScene->GetTeamID());
	pInfo->set_bwin(true);
	pInfo->set_res(nRes);
	UINT32 randomall = 0;
	for (auto i = pCfg->MineralBuffReward.begin(); i != pCfg->MineralBuffReward.end(); ++i)
	{
		randomall += i->seq[1];
	}
	UINT32 random = XRandom::randInt(0, randomall + 1);
	UINT32 temp = 0;
	UINT32 itemid = 0;
	for (auto i = pCfg->MineralBuffReward.begin(); i != pCfg->MineralBuffReward.end(); ++i)
	{
		temp += i->seq[1];
		if (random <= temp)
		{
			itemid = i->seq[0];
			break;
		}
	}
	KKSG::ResWarRole *pRoleWar = pInfo->add_rolelist();
	pRoleWar->set_role(m_pRole->GetID());
	pRoleWar->set_res(nRes);
	bool bRet = ResWarAwardMgr::Instance()->IsItemBuff(itemid);
	if (bRet)
	{
		KKSG::GuildBuffItem *pBuffItem = pInfo->add_itemlist();
		pBuffItem->set_roleid(m_pRole->GetID());
		pBuffItem->set_itemid(itemid);
		pBuffItem->set_count(1);
		listener->AddItem(itemid, 1, true);
		LogDebug("--GiveResWarPVEReward------GuildID=%llu Role=%llu-itemid =%d  itemcount=%d",pPVEScene->GetGuildID(),m_pRole->GetID() ,itemid,1);
	}else
	{
		if (0 != itemid)
		{
			m_pRole->Get<BuffRecord>()->AddBuffItem(itemid);
			listener->AddItem(itemid, 1, true);
		}
	}

}

void StageReward::GiveBossRushReward(Scene *pScene)
{
	std::vector<Sequence<UINT32,2>> rewards;
	int succeedcount = 0;

	BossRushMgr::Instance()->OnBattleFinish(m_pRole, rewards, succeedcount);

	for(size_t i = 0; i < rewards.size(); ++i)
	{
		AddItem(rewards[i][0], rewards[i][1], false);
	}


	StageMgr* record = m_pRole->Get<StageMgr>();
	if (m_pListener)
	{
		m_pListener->SetBossRush(succeedcount, record->GetBossRushMax());
	}

	if (succeedcount > (int)record->GetBossRushMax())
	{
		record->SetBossRushMax(succeedcount);
	}
}

void StageReward::AddItem(UINT32 itemID, UINT32 Count, bool isbind, int AddReason)
{
	if (itemID == 0)
	{
		return;
	}

	if (m_pRole && m_scene && EXP == itemID && m_scene->UseTicket(m_pRole->GetID()))//组队用票进入不给经验
	{
		return;
	}

	//巢穴使用门票后掉落物品不在清单中的无法获得
	if (NULL != m_scene && m_scene->GetSceneType() == KKSG::SCENE_NEST)
	{
		if(!XItem::IsVirtualItem(itemID) && m_scene->UseTicket(m_pRole->GetID()) && GetGlobalConfig().NestDropItemsOnUseTicket.find(itemID) == GetGlobalConfig().NestDropItemsOnUseTicket.end())
		{
			return;
		}
	}

	m_transition.SetReason(ItemFlow_Stage, m_subreason);
	m_transition.GiveItem(itemID, Count, isbind);

	if (m_pListener)
	{
		m_pListener->AddItem(itemID, Count, isbind, AddReason);
	}

	// record 
	if (NULL != m_scene && NULL != m_scene->Statistics())
	{
		m_scene->Statistics()->AddItem(m_pRole->GetID(), itemID, Count);
	}
	m_rewardStream << itemID << ":" << Count << "|";
}

void StageReward::AddSweepExp(UINT32 Count, bool isbind, int AddReason)
{
	m_exptransition->SetReason(ItemFlow_Stage, m_subreason);
	m_exptransition->GiveItem(EXP, Count, isbind);

	if (m_pListener)
	{
		m_pListener->AddItem(EXP, Count, isbind, AddReason);
	}

	// record 
	if (NULL != m_scene && NULL != m_scene->Statistics())
	{
		m_scene->Statistics()->AddItem(m_pRole->GetID(), EXP, Count);
	}
	m_rewardStream << EXP << ":" << Count << "|";
}

void StageReward::GiveScenePickedDoodad(Scene *pScene)
{
	const std::list<PickDoodadInfo> &Doodads = pScene->GetDoodadsInfo();
	foreach (i in Doodads)
	{
		if (i->IsPicked && i->Type == 0)
		{
			AddItem(i->ItemID, i->Count);
		}
	}
}

void StageReward::GiveSyncScenePickedDoodad(Scene *pScene)
{
	if (pScene->GetSceneType() == KKSG::SCENE_TOWER)
	{
		/*SceneTeam* team = pScene->GetSceneTeam();
		if (!team)
		{
		return;
		}
		std::map<int, int>* reward = team->GetSyncSceneFetchReward(m_pRole);
		if (!reward)
		{
		return;
		}*/

		//单人版 V2
		if (pScene->IsSceneWinState())
		{
			const std::map<int, int>* reward = pScene->GetSceneTower()->GetCurFloorFetchReward();
			foreach(j in (*reward))
			{
				AddItem(j->first, j->second);
			}
		}
		if(pScene->GetSceneTower()->CheckSendBattleResult(m_pRole))
		{
			const std::map<int, int>* hisReward = pScene->GetSceneTower()->GetHisFloorFetchReward();
			foreach(j in (*hisReward))
			{
				m_pListener->AddItem(j->first, j->second, true, IStageRewardListener::NormalStageReward);
			}

		}

		return;
	}
	std::list<PickServerModeDoodadInfo> &Doodads = pScene->GetServerDoodadsInfo();
	foreach (i in Doodads)
	{
		if (i->roleID == m_pRole->GetID() && i->Type == 0 && !i->bRewarded)
		{
			m_rewardStream << i->ItemID<< ":" << i->Count << "|";
			AddItem(i->ItemID, i->Count, i->IsBind);
			UINT32 noticeID = ItemConfig::Instance()->GetDoodadNoticeID(pScene->GetSceneType(), i->ItemID, i->Count);
			Notice notice(noticeID);
			notice.Add(m_pRole);
			notice.Add(ItemDesc(i->ItemID, i->Count));
			if (GSConfig::Instance()->IsCrossGS())
			{
				notice.SendInCrossGs(m_pRole->GetFromServerId());
			}
			else
			{
				notice.Send();
			}
			i->bRewarded = true;
		}
	}
}

bool StageReward::CheckNotGiveTowerReward(Scene* pScene)
{
	if (pScene->GetSceneTower() && !pScene->IsSceneWinState())
	{
		return true;
	}

	return false;
}

bool StageReward::GiveFirstReward(SceneInfo* sceneinfo, Scene *scene)
{
	if (CheckNotGiveTowerReward(scene))
	{
		return true;
	}
	int sceneID = scene->GetSceneTemplateID();
	StageMgr *stagerecord = m_pRole->Get<StageMgr>();
	int oldRank = stagerecord->GetRank(sceneID);
	int firststar = 0;
	if (oldRank == RANK_SSS)
	{
		firststar = 2;
	}
	else
	{
		if (m_Rank == RANK_SSS)
		{
			firststar = 1;
		}
	}
	///> 记录玩家关卡星级
	if (StageRankMgr::Instance()->Compare(m_Rank, oldRank))
	{
		///> 这里不需要判断助战 因为助战走不到这里
		stagerecord->SetRank(sceneID, m_Rank);
	}
	if (1 == firststar)
	{
		foreach (i in sceneinfo->m_pConf->FirstSSS)
		{
			AddItem(i->seq[0], i->seq[1], IStageRewardListener::FirstStar);
		}
	}

	bool isfirsttime = false;
	if (scene->GetSceneType() == KKSG::SCENE_BIOHELL)
	{
		if (m_pRole->Get<SpActivityRecord>()->CheckIsFirstCompleteScene(SpActivity_BioHell, scene->GetSceneType(), scene->GetSceneTemplateID()))
		{
			isfirsttime = true;
			m_pRole->Get<SpActivityRecord>()->UpdateFirstCompleteScene(SpActivity_BioHell, scene->GetSceneType(), scene->GetSceneTemplateID());
		}
	}
	else
	{
		isfirsttime = (oldRank == INVALID_STAGERANK);
	}
	if (isfirsttime)
	{
		if (sceneinfo->m_pConf->FirstDownMoney > 0)
		{
			AddItem(GOLD,  GetStageFirstCoin(sceneinfo), false);
		}
		if (sceneinfo->m_pConf->FirstDownExp > 0)
		{
			AddItem(EXP, GetStageFirstExp(sceneinfo), false);
		}
		foreach (i in sceneinfo->m_pConf->FirstDownDrop)
		{
			const ItemDesc* pDrop = NULL;
			if ((pDrop = ItemDropConfig::Instance()->GetRandomDrop(*i, m_pRole->GetLevel())) != NULL)
			{
				AddItem(pDrop->itemID, pDrop->itemCount, pDrop->isbind);
			}
		}

		OnFirstPass(scene);
	}

	if (m_pListener)
	{
		m_pListener->SetSceneRank(m_Rank, firststar);
	}

	return isfirsttime;
}

void StageReward::OnFirstPass(Scene* scene)
{
	m_pRole->Get<CFirstPassRecord>()->OnFirstPass(scene);
	if (scene->GetSceneTower())
	{
		auto data = TowerConfig::Instance()->GetTowerTableData(DEFAULT_HARDLEVEL, scene->GetSceneTower()->GetCurFloor());
		if (data && !data->preward)
		{
			for (auto iter = data->FirstPassReward.begin(); iter != data->FirstPassReward.end(); ++iter)
			{
				AddItem(iter->seq[0], iter->seq[1]);
				if (!scene->GetSceneTower()->CheckSendBattleResult(m_pRole))
				{
					scene->GetSceneTower()->AddHisFetchReward(m_pRole, iter->seq[0], iter->seq[1]);
				}
			}
		}
	}

	if (scene->GetSceneType() == KKSG::SCENE_NEST || scene->GetSceneType() == KKSG::SCENE_DRAGON)
	{
		ReportDataMgr::Instance()->NestFirstPass(m_pRole, scene->GetSceneTemplateID());
	}
	TRoundUnlockLog oLog(m_pRole);
	oLog.m_BattleType = scene->GetSceneType();
	oLog.m_BattleID = scene->GetSceneTemplateID();
	oLog.Do();
}

void StageReward::GiveCommonReward(SceneInfo *sceneinfo)
{
	if (sceneinfo->m_pConf->Money > 0)
	{
		AddItem(GOLD, GetStageCoin(sceneinfo), false);
	}
	if (sceneinfo->m_pConf->Exp > 0)
	{
		AddItem(EXP, GetStageExp(sceneinfo), false);
	}

	std::vector<int> *pDrop = SceneConfig::Instance()->GetDrop(sceneinfo, m_Rank);
	if (pDrop != NULL)
	{
		std::vector<int> &Vec = *pDrop;
		foreach(i in Vec)
		{
			const ItemDesc* pDropConf = NULL;
			if ((pDropConf = ItemDropConfig::Instance()->GetRandomDrop(*i, m_pRole->GetLevel())) != NULL)
			{
				AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
			}
		}
	}

	if (m_scene && m_scene->GetSceneMorphInfo()->CanMorph())
	{
		const SceneMorphTable::RowData* data = SceneConfig::Instance()->GetMorphTableByID(m_scene->GetSceneTemplateID());
		if (data)
		{
			foreach(i in data->DropList)
			{
				const ItemDesc* pDropConf = NULL;
				if ((pDropConf = ItemDropConfig::Instance()->GetRandomDrop(*i, m_pRole->GetLevel())) != NULL)
				{
					AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
				}
			}
		}
	}
}

void StageReward::GiveSweepReward(SceneInfo *pSceneInfo)
{
	_GiveSweep(pSceneInfo, RANK_SSS, false);
	//GiveSealExpItem(pSceneInfo);
}


void StageReward::GiveSpActivityReward(SceneInfo *sceneinfo)
{
	std::vector<Sequence<UINT32, 2>> vecDrop = sceneinfo->m_pConf->SpActivityDrop;
	for (auto i = vecDrop.begin(); i != vecDrop.end(); i++)
	{
		if (m_pRole->Get<SpActivityRecord>()->GetSpActStage((*i)[0]) != 0)
		{
			const ItemDesc* pDropConf = ItemDropConfig::Instance()->GetRandomDrop((*i)[1], m_pRole->GetLevel());
			if (pDropConf != NULL)
			{
				AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
			}
		}
	}
}

void StageReward::PlatShareResult(UINT32 scene_id)
{
	m_pRole->Get<PlatformShareRecord>()->NotifyFirstShare(scene_id);
}

void StageReward::GiveSweepExp(SceneInfo *pSceneInfo)
{
	_GiveSweep(pSceneInfo, RANK_SSS, true);
}

void StageReward::_GiveSweep(SceneInfo *sceneinfo, int Rank, bool exponly)
{
	if (!exponly && sceneinfo->m_pConf->Money > 0)
	{
		AddItem(GOLD,  GetStageCoin(sceneinfo), false);
	}
	if (exponly && sceneinfo->m_pConf->Exp > 0)
	{
		AddSweepExp(GetStageExp(sceneinfo), false);
	}

	// 扫荡，集字活动奖励掉落
	if (!exponly)
	{
		GiveSpActivityReward(sceneinfo);
	}

	std::vector<int> *pDrop = SceneConfig::Instance()->GetDrop(sceneinfo, RANK_SSS);
	if (pDrop == NULL)
	{
		return;
	}
	std::vector<int> &Vec = *pDrop;
	foreach(i in Vec)
	{
		const ItemDesc *pDropConf = NULL;
		if ((pDropConf = ItemDropConfig::Instance()->GetRandomDrop(*i, m_pRole->GetLevel())) != NULL)
		{
			if (exponly)
			{
				if (pDropConf->itemID == EXP)
				{
					AddSweepExp(pDropConf->itemCount, pDropConf->isbind);
				}
			}
			else 
			{
				if (pDropConf->itemID != EXP)
				{
					AddItem(pDropConf->itemID, pDropConf->itemCount, pDropConf->isbind);
				}
			}
		}
	}

	if(!exponly)
	{
		//抽宝箱 
		RoleBoxRewardInfo info = TeamSelectReward::FillBox(sceneinfo, Rank, m_pRole->GetID(), m_pRole->GetLevel());
		if(!info.boxs.empty())
		{
			const auto& item = info.boxs[0];
			AddItem(item.item.itemID, item.item.itemCount, item.item.isbind);
		}
	}
}

void StageReward::NotifyClient(INT32 nReason, INT32 nSubReason)
{
	m_transition.SetReason(nReason, nSubReason);
	m_transition.NotifyClient();
}

void StageReward::NotifyExpClient(INT32 nReason, INT32 nSubReason)
{
	m_exptransition->SetReason(nReason, nSubReason);
	m_exptransition->NotifyClient();
}

void StageReward::SetExpTrans()
{
	m_exptransition = new BagGiveItemTransition(m_pRole);
}

void StageReward::_GiveGuildReward(Scene *pScene)
{
	if (pScene == NULL)
	{
		return ;
	}

	SceneTeam* pSceneTeam = pScene->GetSceneTeam();
	if (pSceneTeam != NULL && !pSceneTeam->IsCostCount() && pScene->GetSceneType() != KKSG::SCENE_DRAGON) // 龙本不检查次数
	{
		return ;
	}


	NormalBattleResultListener* listener = dynamic_cast<NormalBattleResultListener*>(m_pListener);
	if (NULL == listener)
	{
		return;
	}
	if(pScene->GetSceneType() != KKSG::SCENE_NEST && pScene->GetSceneType() != KKSG::SCENE_ABYSSS && pScene->GetSceneType() != KKSG::SCENE_DRAGON)
	{
		return; 
	}

	///> 判断是否工会组队
	UINT64 myGuildID = m_pRole->getGuildId();
	if (0 == myGuildID)
	{
		return;
	}
	if (pScene->IsHelper(m_pRole->GetID()))//助战不加
	{
		return;
	}

	const std::list<Role*>& roleVec = pScene->GetAllRoles();
	UINT32 cnt = 0;
	for (auto iter = roleVec.begin(); iter != roleVec.end(); ++iter)
	{
		Role* temprole = *iter;
		if (temprole != NULL && !pScene->IsHelper(temprole->GetID()) && temprole != m_pRole)
		{
			if (temprole->getGuildId() == myGuildID)
			{
				++cnt;
			}
		}
	}
	///> 2人以上算组队
	if (0 == cnt)  
	{
		return;
	}
	// guild contribute
	KKSG::GuildExpAndConData* msg = listener->GetGuildInfo();
	UINT32 conNum = GuildConfig::Instance()->GetContributeNum(m_pRole->GetGuildLevel(), pScene->GetSceneType()); 
	if (conNum > 0)
	{
		AddItem(GUILD_CONTRIBUTE, conNum, false, IStageRewardListener::GuildReward);

		KKSG::GuildConData* pdata = msg->add_guildcons();
		pdata->set_roleid(m_pRole->GetID());
		pdata->set_guildcon(conNum);

		listener->SetGuildCon(conNum);
	}
	// guild dragon coin
	ItemDesc item = GuildConfig::Instance()->GetStageRoleContribute(m_pRole->GetGuildLevel(), pScene->GetSceneType());
	if (item.itemID == DRAGON_COIN)
	{
		//BagGiveItemTransition bagGive(m_pRole);
		//bagGive.GiveItem(item.itemID, item.itemCount);
		//bagGive.SetReason(ItemFlow_Guild, ItemFlow_Guild_Dare_Reward);
		//bagGive.NotifyClient();

		//listener->SetGuildDargon((UINT32)item.itemCount);
		AddItem(item.itemID, item.itemCount, true, IStageRewardListener::GuildDragonReward);
	}

	///> 公会挑战事件
	m_pRole->OnGuildDare();  

	// guild exp
	std::map<UINT32, UINT32> guildreward;
	GuildConfig::Instance()->GetStageGuildExp(m_pRole->GetGuildLevel(), pScene->GetSceneType(), guildreward);
	if (!guildreward.empty())
	{
		KKSG::GuildExpData* pData = msg->add_guildexps();
		pData->set_guildid(m_pRole->getGuildId());
		pData->set_guildexp(guildreward[1]);
		pData->set_guildprestige(guildreward[2]);

		listener->SetGuildExp(guildreward[1]);
	}
}

bool StageReward::WeekNestRewardCount(Scene* pScene)
{
	if(pScene->GetSceneType() != KKSG::SCENE_WEEK_NEST)
	{
		return true;
	}
	if(0 == m_pRole->Get<CTeamRecord>()->GetWNRewardLeftCount())	
	{
		return false;
	}

	m_pRole->Get<CTeamRecord>()->AddWNRewardCount();	

	return true;
}

bool StageReward::NotGiveSmallDragonReward(Scene* pScene)
{
	if (pScene->GetSceneType() != KKSG::SCENE_DRAGON)
	{
		return false;
	}
	SceneTeam* pteam = pScene->GetSceneTeam();
	if (!pteam)
	{
		return false;
	}

	auto data = DragonConfig::Instance()->GetDragonTableData(pteam->GetExpID());
	if (!data)
	{
		return false;
	}

	if (m_pRole->Get<StageMgr>()->GetRank(pScene->GetSceneTemplateID()) <= 0)
	{
		return false;
	}

	if (data->DragonNestDifficulty == 0 && data->MaxDragonDropLevel > 0 && m_pRole->GetLevel() > data->MaxDragonDropLevel)
	{
		return true;
	}

	return false;
}
