#include "pch.h"
#include "activityhandler.h"
#include "activitymgr.h"
#include "activityrecord.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "define/itemdef.h"
#include "team.h"
#include "define/qadef.h"
#include "table/TaskMgr.h"
#include "payv2Mgr.h"
#include "unit/rolemanager.h"
#include "scene/sceneteam.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/stagemgr.h"
CActivityHandler CActivityHandler::GlobalHandler;

const UINT32 BossRushNum = 4;
const UINT32 PKContinueWin = 2;
const UINT32 ActivityHeroMOBA = 979;
const UINT32 ActivityRisk = 49;
const UINT32 eActivityHeroBattle3GuildMembers = eActivityHeroBattle + 10000;

void CActivityHandler::FinishScene(Role* poRole, UINT32 sceneType)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	switch (sceneType)
	{
	case KKSG::SCENE_BATTLE:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityStage);
			break;
		}
	case KKSG::SCENE_ABYSSS:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivitySpecialStage);
			break;
		}
	case KKSG::SCENE_ARENA: //竞技场
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityArena);
			break;
		}
	case KKSG::SCENE_BOSSRUSH: //bossrush
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityBossRush);
			break;
		}
	case KKSG::SCENE_PK: //天梯
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityPk);
			break;
		}
	case KKSG::SCENE_TOWER: //黑暗神殿
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityTower);
			break;
		}
	case KKSG::SCENE_DRAGON: //龙穴
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityDragon);
			break;
		}
	case KKSG::SCENE_GODDESS: //女神试炼
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityGoddess);
			break;
		}
	case KKSG::SCENE_DRAGON_EXP: //龙之远征
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityDragonExp);
			break;
		}
	case KKSG::SCENE_ENDLESSABYSS: //无尽深渊
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityEndLessAbyss);
			break;
		}
	case KKSG::SCENE_HEROBATTLE:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityHeroBattle);
			break;
		}
	default:
		break;
	}
}

void CActivityHandler::OnCompleteScene(Role *poRole, Scene *poScene)
{
	FinishScene(poRole, poScene->GetSceneType());

	//处理随机的活动
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	switch (poScene->GetSceneType())
	{
	case KKSG::SCENE_NEST:			// 巢穴
		{	
			if (pRecord->CheckHaveRandomActivityId(eActivityNest))
			{
				bool guild_relation = CActivityMgr::Instance()->CheckPlayerGuileRelation(poRole,poScene);
				if (guild_relation)
				{
					CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityNest,true);
				}
			}
			break;
		}
	case KKSG::SCENE_HEROBATTLE:	// 英雄战场
		{
			if (pRecord->CheckHaveRandomActivityId(eActivityHeroBattle))
			{
				CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityHeroBattle,true);
			}
			else if (pRecord->CheckHaveRandomActivityId(eActivityHeroBattle3GuildMembers))
			{
				bool guild_relation = CActivityMgr::Instance()->CheckPlayerGuileRelation(poRole,poScene);
				if (guild_relation)
				{
					CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityHeroBattle3GuildMembers,true);
				}
			}
			break;
		}
	case KKSG::SCENE_MOBA:   // 英雄峡谷
		{
			if (pRecord->CheckHaveRandomActivityId(ActivityHeroMOBA))
			{
				CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, ActivityHeroMOBA,true);
			}

			break;	
		}
	case  KKSG::SCENE_PK:			// 天梯
		{
			if (pRecord->CheckHaveRandomActivityId(eActivityPk))
			{
				if (poRole->Get<CPkRecord>()->GetContWinDay() >= PKContinueWin)
				{
					CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityPk,true);
				}
			}
			break;
		}
	case  KKSG::SCENE_BOSSRUSH:
		{
			if (pRecord->CheckHaveRandomActivityId(eActivityBossRush))
			{
				BRRoleState* state = poRole->Get<StageMgr>()->GetBRRoleState();
				if ( state->rank >= BossRushNum)
				{
					CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityBossRush,true);
				}
			}
			break;
		}
	case KKSG::SCENE_RISK:
		{
			if (pRecord->CheckHaveRandomActivityId(ActivityRisk))
			{
				CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, ActivityRisk,true);
			}
			break;
		}
	default:
		break;
	}

}

void CActivityHandler::OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw)
{
	UINT32 sceneType = poScene->GetSceneType();
	if (false == isEndScene)
	{
		if(sceneType != KKSG::SCENE_BOSSRUSH  && sceneType != KKSG::SCENE_TOWER 
			&& sceneType != KKSG::SCENE_GODDESS && sceneType != KKSG::SCENE_DRAGON_EXP
			&& sceneType != KKSG::SCENE_DRAGON)
		{
			//这些都是参加就给, 中途退出也算
			return;
		}
	}
	FinishScene(poRole, sceneType);

	if (sceneType == KKSG::SCENE_BOSSRUSH)
	{
		OnCompleteScene(poRole,poScene);
	}
}

void CActivityHandler::OnSweepScene(Role *poRole, SceneInfo *pSceneInfo)
{
	FinishScene(poRole, pSceneInfo->m_pConf->type);
}

void CActivityHandler::OnLevelup(Role *poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->UpdateActiviyId(pRecord, poRole);
}

void CActivityHandler::OnTaskComplete(Role *poRole, int TaskID)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->UpdateActiviyId(pRecord, poRole);

	TaskConfig* config = TaskMgr::Instance()->GetTaskConfig(TaskID);
	if(config == NULL) return;
	if(config->type == TaskType_Daily)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityDailyTask);
	}
}

void CActivityHandler::OnEnhanceEquip(Role* poRole, UINT32 pos)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityEnhance);
}

void CActivityHandler::OnComposeJade(Role* poRole, UINT32 jadeLevel, UINT32 count)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityJade);
}

void CActivityHandler::OnCompose(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityCompose);
}

void CActivityHandler::OnDecompose(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityDecompose);
}

void CActivityHandler::OnBuy(Role* poRole, UINT32 dwItemId)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	if (GOLD == dwItemId)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityBuyGold);
	}
	else if (FATIGUE == dwItemId)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityBuyFatigue);
	}
}

void CActivityHandler::OnShopBuy(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityShopBuy);
}

void CActivityHandler::OnAuction(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityAuction);
}

void CActivityHandler::OnHitWorldBoss(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();	
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityWorldBoss);
}

void CActivityHandler::OnHitGuildBoss(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityGuildBoss);
}

/*void CActivityHandler::OnReinforce(Role* poRole)
{
CActivityReocrd* pRecord = poRole->Get<CActivityReocrd>();
if (NULL == pRecord)
{
return;
}
//CActivityMgr::Instance()->UpdateRecord(pRecord, poRole);	
//CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityReinforce);
}*/

void CActivityHandler::OnTeamStage(Role* poRole, int type)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	switch (type)
	{
	case EXP_TEAM_SCENE:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityExpedtion);
			break;
		}
	case GUILD_TEAM_SCENE:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityGuildStage);
			break;
		}
	case NEST_TEAM_SCENE:
		{
			CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityNest);
			break;
		}
	default:
		break;
	}
}

void CActivityHandler::OnGuildActivity(Role* poRole, int type)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	if (1 == type)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityCard);
	}
	else if (2 == type)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityGuildCheckin);
	}
}

void CActivityHandler::OnSendFlower(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityFlower);
}

void CActivityHandler::OnEnhanceFashion(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityEnhanceFashion);
}

void CActivityHandler::OnSkillUp(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivitySkillUp);
}

void CActivityHandler::OnSmelting(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivitySmelting);
}

void CActivityHandler::OnSlotAttr(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivitySlotAttr);
}

void CActivityHandler::OnQA(Role* poRole, UINT32 type)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	if(QATYPE_MULTI == type)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityQAMulti);
	}
	else if(QATYPE_GUILD == type)
	{
		CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityQAGuild);
	}
}

void CActivityHandler::OnBuyAileen(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityBuyAileen);
}

void CActivityHandler::OnBuyIBShopItem(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityMallBuy);
}

void CActivityHandler::OnGuildInherit(Role* poRole)
{
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityGuildInherit);
}

void CActivityHandler::OnBuyPayMember(Role* poRole, int ID)
{
	if(ID != pay_member_3) return;
	CActivityRecord* pRecord = poRole->Get<CActivityRecord>();
	CActivityMgr::Instance()->OnFinishActivityId(pRecord, poRole, eActivityPayMember);
}