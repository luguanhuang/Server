#include "pch.h"
#include "scene.h"
#include "scenefinishcb.h"
#include "scenegarden.h"
#include "scenedragon.h"
#include "sceneskycity.h"
#include "sceneskycitywait.h"
#include "sceneinherit.h"
#include "scenereswarpve.h"
#include "sceneworldboss.h"
#include "scenetower.h"
#include "scenehorse.h"
#include "scenebossrush.h"
#include "sceneguildcamp.h"
#include "scenerisk.h"
#include "sceneleague.h"
#include "gamelogic/dragonexpbase.h"
#include "scenepktwo.h"
#include "scenecustompktwo.h"
#include "bigmeleeready.h"
#include "battlefieldready.h"
#include "scenewedding.h"
#include "sceneweekend4v4duck.h"
#include "scene/scenecompetedragon.h"

void Scene::InitHandler(UINT32 scenetype)
{
	if (scenetype != KKSG::SCENE_HALL)
	{
		AddEventListener(this);
	}

	switch(scenetype)
	{
	case KKSG::SCENE_HALL:
		{
			m_inherit = new SceneInherit();
			AddEventListener(m_inherit);
			break;
		}
		///> ������� 10����ս����
	case KKSG::SCENE_GUILD_HALL:
		{
			m_garden = new SceneGarden(this);
			m_inherit = new SceneInherit();
			AddEventListener(m_inherit);
			break;
		}
		///> ��԰ 15����ս����
	case KKSG::SCENE_FAMILYGARDEN:
		{
			m_garden = new SceneGarden(this);
			break;
		}
		///> ��վ������ȴ����� 24����ս����
	case KKSG::SKYCITY_WAITING:
		{
			m_pSceneWait = SceneSkyCityWait::CreateSceneSkyWait(this);
			break;
		}
		///> ���г���
	case  KKSG::SCENE_LEISURE:
		{
			break;
		}
		///> ������ 5��������
	case KKSG::SCENE_ARENA:
		{
			AddFinishListener(&ArenaSceneFinishCB::GlobalArenaSceneFinishCB);
			break;
		}
		///> ����boss 7
	case KKSG::SCENE_WORLDBOSS:
		{
			m_worldbosshandler = WorldBossHandler::CreateHandler(this);
			AddEventListener(m_worldbosshandler);
			break;
		}
		///> bossrush 9
	case KKSG::SCENE_BOSSRUSH:
		{
			m_BRHandler = new CSceneBossRush(this);
			AddEventListener(m_BRHandler);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
		///> �ڰ���� 16
	case KKSG::SCENE_TOWER:
		{
			m_pTower = SceneTower::CreateSceneTower(this);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			AddEventListener(m_pTower);
			break;
		}
		///> ���� 18
	case KKSG::SCENE_DRAGON:
		{
			m_pDragon = SceneDragon::CreateSceneDragon(this);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
		///> ��֮Զ�� 21
	case KKSG::SCENE_DRAGON_EXP:
		{
			m_DEHandler = new CDESceneHandler(this);
			AddFinishListener(&DragonExpSceneFinishCB::GlobalDragonExpSceneFinishCB);
			break;
		}
		///> ��ð�� 22
	case KKSG::SCENE_RISK:
		{
			m_sceneRisk = SceneRisk::CreateSceneRisk(this);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
		///> ������ 12 PVP
	case KKSG::SCENE_PK:
		{
			AddFinishListener(&PkSceneFinishCB::GlobalPkSceneFinishCB);
			break;
		}
		///> ������2 46 PVP
	case KKSG::SCENE_PKTWO:
		{
			AddFinishListener(&PkTwoSceneFinishCB::GlobalPkTwoSceneFinishCB);
			break;
		}
		///> ȫ��PK 45 PVP
	case KKSG::SCENE_CUSTOMPK:
		{
			AddFinishListener(&CustomPkSceneFinishCB::GlobalCustomPkSceneFinishCB);
			break;
		}
		///> ȫ��������2 53 PVP
	case KKSG::SCENE_CUSTOMPKTWO:
		{
			AddFinishListener(&CustomPkTwoSceneFinishCB::GlobalCustomPkTwoSceneFinishCB);
			break;
		}
		///> �����ӳ� 17 PVP
	case KKSG::SCENE_PVP:
		{
			AddFinishListener(&PvpSceneFinishCB::GlobalPvpSceneFinishCB);
			break;
		}
		///> ������̨ս 19 PVP
	case KKSG::SCENE_GMF:
		///> ������̨����ս 27 PVP
	case KKSG::SCENE_GPR:
		{
			AddFinishListener(&GmfSceneFinishCB::GlobalGmfSceneFinishCB);
			break;
		}
		///> ��վ����� 25 PVP
	case KKSG::SKYCITY_FIGHTING:
		{
			AddFinishListener(&SkyCitySceneFinishCB::GlobalSkyCitySceneFinishCB);
			break;
		}
		///> ��������ս 29 
	case KKSG::SCENE_RESWAR_PVE:
		{
			m_pSceneResWarPVE = SceneResWarPVE::CreateSceneResWarPVE(this);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
		///> ��������ս 28 
	case KKSG::SCENE_RESWAR_PVP:
		{
			AddFinishListener(&ResWarSceneFinishCB::GlobalResWarSceneFinishCB);
			break;
		}
		///> 30 
	case KKSG::SCENE_GUILD_CAMP:
		{
			m_pGuildCamp = SceneGuildCamp::CreateSceneGuildCamp(this);
			AddEventListener(m_pGuildCamp);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
		///> 34
	case KKSG::SCENE_HORSE:
		{
			m_pHorseWait = SceneHorse::CreateSceneHorse(this);
			break;
		}
		///> 35
	case KKSG::SCENE_HORSE_RACE:
		{
			AddFinishListener(&HorseSceneFinishCB::GlobalHorseSceneFinishCB);
			break;;
		}
		///> 36
	case KKSG::SCENE_HEROBATTLE:
		{
			AddFinishListener(&HeroBattleSceneFinishCB::GlobalHeroBattleSceneFinishCB);
			break;
		}
		///> 37
	case KKSG::SCENE_INVFIGHT:
		{
			AddFinishListener(&InvFightSceneFinishCB::GlobalInvFightSceneFinishCB);
			break;
		}
		///> ����boss 11
	case KKSG::SCENE_GUILD_BOSS:
		{
			break;
		}
		///> �ع鳡����������
	case KKSG::SCENE_CALLBACK:
		///> �����ؿ� 2��������
	case KKSG::SCENE_BATTLE:
		///> ��Ѩ 3
	case KKSG::SCENE_NEST:
		///> ��Ԩ 13
	case KKSG::SCENE_ABYSSS:
		///> Ů������ 20
	case KKSG::SCENE_GODDESS:
		///> �޾���Ԩ 23
	case KKSG::SCENE_ENDLESSABYSS:
		///> ÿ�ܳ�Ѩ
	case KKSG::SCENE_WEEK_NEST:
		///> �������
	case KKSG::SCENE_CASTLE_WAIT:
	case KKSG::SCENE_CASTLE_FIGHT:
		///> תְ���� 26
	case KKSG::SCENE_PROF_TRIALS:
		///> ��ͧ 31 
	case KKSG::SCENE_AIRSHIP:
	case KKSG::SCENE_ACTIVITY_ONE:
	case KKSG::SCENE_ACTIVITY_TWO:
	case KKSG::SCENE_ACTIVITY_THREE:
	case KKSG::SCENE_ABYSS_PARTY:
	case KKSG::SCENE_BIOHELL:
	case KKSG::SCENE_DUCK:
	case KKSG::SCENE_COUPLE:
		{
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
	case KKSG::SCENE_LEAGUE_BATTLE:
		{
			AddFinishListener(&LeagueSceneFinishCB::GlobalLeagueSceneFinishCB);
			break;
		}
	case KKSG::SCENE_MOBA:
		{
			AddFinishListener(&MobaBattleSceneFinishCB::GlobalMobaBattleSceneFinishCB);
			break;
		}
    case KKSG::SCENE_WEEKEND4V4_MONSTERFIGHT:
        {
            AddFinishListener(&WeekEnd4v4MonsterFightSceneFinishCB::GlobalMonsterFightSceneFinishCB);
            break;
        }
    case KKSG::SCENE_WEEKEND4V4_GHOSTACTION:
        {
            AddFinishListener(&WeekEnd4v4GhostActionSceneFinishCB::GlobalGhostActionSceneFinishCB);
            break;
        }
    case KKSG::SCENE_WEEKEND4V4_DUCK:
        {
            AddFinishListener(&WeekEnd4v4DuckSceneFinishCB::GlobalDuckSceneFinishCB);
            break;
        }
    case KKSG::SCENE_WEEKEND4V4_LIVECHALLENGE:
        {
            AddFinishListener(&WeekEnd4v4LiveChallengeSceneFinishCB::GlobalLiveChallengeSceneFinishCB);
            break;
        }
    case KKSG::SCENE_WEEKEND4V4_CRAZYBOMB:
        {
            AddFinishListener(&WeekEnd4v4CrazyBombSceneFinishCB::GlobalCrazyBombSceneFinishCB);
            break;
        }
    case KKSG::SCENE_WEEKEND4V4_HORSERACING:
        {
            AddFinishListener(&WeekEnd4v4HorseRacingSceneFinishCB::GlobalHorseRacingSceneFinishCB);
            break;
        }
	case KKSG::SCENE_BIGMELEE_READY:
		{
			m_pSceneBMReady = SceneBMReady::CreateSceneBMReady(this);
			break;
		}
	case KKSG::SCENE_BIGMELEE_FIGHT:
		{
			break;
		}
	case KKSG::SCENE_WEDDING:
		{
			m_pWedding = new SceneWedding(this);
			AddEventListener(m_pWedding);
			break;
		}
	case KKSG::SCENE_BATTLEFIELD_READY:
		{
			m_pSceneBFReady = SceneBFReady::CreateSceneBFReady(this);
			break;
		}
	case KKSG::SCENE_BATTLEFIELD_FIGHT:
		{
			AddFinishListener(&BattleFieldSceneFinishCB::GlobalBattleFieldSceneFinishCB);
			break;
		}
		///> �������� 
	case KKSG::SCENE_COMPETEDRAGON:
		{
			m_pCompeteDragon = SceneCompeteDragon::CreateSceneCompeteDragon(this);
			AddFinishListener(&CommonSceneFinishCB::GlobalCommonSceneFinishCB);
			break;
		}
	case KKSG::SCENE_SURVIVE://��������
		{
			AddFinishListener(&SurviveSceneFinishCB::GlobalSurviveSceneFinishCB);
			break;
		}
	default:
		{
			SSError<<"invalid scene type:"<<scenetype<<END;
			break;
		}
	}
}

ScenePk* Scene::GetPkHandler()
{
	if(m_vshandler && KKSG::SCENE_PK == GetSceneType())
	{
		return (ScenePk*)(m_vshandler);
	}
	return NULL;
}

ScenePkTwo* Scene::GetPkTwoHandler()
{
	if(m_vshandler && KKSG::SCENE_PKTWO == GetSceneType())
	{
		return (ScenePkTwo*)(m_vshandler);
	}
	return NULL;
}

SceneCustomPkTwo* Scene::GetCustomPkTwoHandler()
{
	if(m_vshandler && KKSG::SCENE_CUSTOMPKTWO == GetSceneType())
	{
		return (SceneCustomPkTwo*)(m_vshandler);
	}
	return NULL;
}

SceneCustomPk* Scene::GetCustomPkHandler()
{
	if(m_vshandler && KKSG::SCENE_CUSTOMPK == GetSceneType())
	{
		return (SceneCustomPk*)(m_vshandler);
	}
	return NULL;
}

SceneInvFigtht* Scene::GetInvFightHandler()
{
	if(m_vshandler && KKSG::SCENE_INVFIGHT == GetSceneType())
	{
		return (SceneInvFigtht*)(m_vshandler);
	}
	return NULL;
}

ScenePvp* Scene::GetPvpHandler()
{
	if(m_vshandler && KKSG::SCENE_PVP == GetSceneType())
	{
		return (ScenePvp*)(m_vshandler);
	}
	return NULL;
}

SceneGWB* Scene::GetGWBHandler()
{
	if(m_vshandler && (KKSG::SCENE_GMF == GetSceneType() || KKSG::SCENE_GPR == GetSceneType()))
	{
		return (SceneGWB*)(m_vshandler);
	}
	return NULL;
}

SceneCastle* Scene::GetCastleHandler()
{
	if(m_vshandler && (KKSG::SCENE_CASTLE_WAIT == GetSceneType() || KKSG::SCENE_CASTLE_FIGHT == GetSceneType()))
	{
		return (SceneCastle*)(m_vshandler);
	}
	return NULL;
}

SceneSkyCity* Scene::GetSkyCityHandler()
{
	if(m_vshandler && KKSG::SKYCITY_FIGHTING == GetSceneType())
	{
		return (SceneSkyCity*)(m_vshandler);
	}
	return NULL;
}

SceneHorseRace* Scene::GetHorseRaceHandler()
{
	if(m_vshandler && KKSG::SCENE_HORSE_RACE == GetSceneType())
	{
		return (SceneHorseRace*)(m_vshandler);
	}
	return NULL;
}

SceneResWar* Scene::GetResWarHandler()
{
	if(m_vshandler && KKSG::SCENE_RESWAR_PVP == GetSceneType())
	{
		return (SceneResWar*)(m_vshandler);
	}
	return NULL;
}

SceneHeroBattle* Scene::GetHeroBattleHandler()
{
	if(m_vshandler && KKSG::SCENE_HEROBATTLE == GetSceneType())
	{
		return (SceneHeroBattle*)(m_vshandler);
	}
	return NULL;
}

SceneLeague* Scene::GetLeagueBattleHandler()
{
	if (m_vshandler && KKSG::SCENE_LEAGUE_BATTLE == GetSceneType())
	{
		return (SceneLeague*)(m_vshandler);
	}
	return NULL;
}

SceneMobaBattle* Scene::GetMobaBattleHandler()
{
	if (m_vshandler && KKSG::SCENE_MOBA == GetSceneType())
	{
		return (SceneMobaBattle*)(m_vshandler);
	}
	return NULL;
}

SceneWeekEnd4v4MonsterFight* Scene::GetMonsterFightHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_MONSTERFIGHT == GetSceneType())
    {
        return (SceneWeekEnd4v4MonsterFight*)(m_vshandler);
    }
    return NULL;
}

SceneWeekEnd4v4GhostAction* Scene::GetGhostActionHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_GHOSTACTION == GetSceneType())
    {
        return (SceneWeekEnd4v4GhostAction*)(m_vshandler);
    }
    return NULL;
}

SceneWeekEnd4v4Duck* Scene::GetDuckHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_DUCK == GetSceneType())
    {
        return (SceneWeekEnd4v4Duck*)(m_vshandler);
    }
    return NULL;
}

SceneWeekEnd4v4LiveChallenge* Scene::GetLiveChallengeHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_LIVECHALLENGE == GetSceneType())
    {
        return (SceneWeekEnd4v4LiveChallenge*)(m_vshandler);
    }
    return NULL;
}

SceneWeekEnd4v4CrazyBomb* Scene::GetCrazyBombHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_CRAZYBOMB == GetSceneType())
    {
        return (SceneWeekEnd4v4CrazyBomb*)(m_vshandler);
    }
    return NULL;
}

SceneWeekEnd4v4HorseRacing* Scene::GetWeekEnd4v4HorseRacingHandler()
{
    if (m_vshandler && KKSG::SCENE_WEEKEND4V4_HORSERACING == GetSceneType())
    {
        return (SceneWeekEnd4v4HorseRacing*)(m_vshandler);
    }
    return NULL;
}


SceneBMFight* Scene::GetBMFightHandler()
{
	if (m_vshandler && KKSG::SCENE_BIGMELEE_FIGHT == GetSceneType())
	{
		return (SceneBMFight*)(m_vshandler);
	}
	return NULL;
}



SceneBFFight* Scene::GetBFFightHandler()
{
	if (m_vshandler && KKSG::SCENE_BATTLEFIELD_FIGHT == GetSceneType())
	{
		return (SceneBFFight*)(m_vshandler);
	}
	return NULL;
}

