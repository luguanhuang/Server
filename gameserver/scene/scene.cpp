#include "pch.h"
#include <time.h>
#include "scene.h"
#include "scenefinder.h"
#include "sceneconfig.h"
#include "unit/role.h"
#include "entity/XRole.h"
#include "entity/XObject.h"
#include "gamelogic/XLevelWave.h"
#include "scenemortuary.h"
#include "unit/unit.h"
#include "unit/enemy.h"
#include "unit/enemymanager.h"
#include "skill/XBulletMgr.h"
#include "scenecb.h"
#include "scenemanager.h"
#include "unit/rolemanager.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "gamelogic/itemconfig.h"
#include "table/globalconfig.h"
#include "foreach.h"
#include "gamelogic/teamreward.h"
#include "util/XRandom.h"
#include "battle/ptcg2c_enemydorpdoodadntf.h"
#include "battle/ptcg2c_fetchenemydoodadntf.h"
#include "scene/ptcg2c_scenefinishstatechanged.h"
#include "gamelogic/bagtransition.h"
#include "math/XVector3.h"
#include "scenefinishcb.h"
#include "table/BuffTable.h"
#include "buff/XBuffTemplateManager.h"
#include "sceneupdatehandler.h"
#include "scenedoodad.h"
#include "scenepk.h"
#include "scenepvp.h"
#include "state/XStateDefine.hpp"
#include "state/XStateMachine.h"
#include "guild/guildconfig.h"
#include "util/gettimeofday.h"
#include "waypoint.h"
#include "waypointmgr.h"
#include "teleportdoor.h"
#include "unit/dummyrole.h"
#include "unit/dummyrolemanager.h"
#include "camp/camprecord.h"
#include "scenetower.h"
#include "unit/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "scene/scenedragon.h"
#include "crpc.h"
#include "live/liveinfo.h"
#include "live/livemanager.h"
#include "scenegmf.h"
#include "guild/ptcg2c_noticeguildbossend.h"
#include "live/livemanager.h"
#include "battlewatch/ptcg2c_notifyroleempty2watcher.h"
#include "gamelogic/noticemgr.h"
#include "guild/ptcg2m_rolesfinishscenetomaster.h"
#include "scene/ptcg2m_sceneendtomaster.h"
#include "mslink.h"
#include "scene/sceneteam.h"
#include "guild/guildboss.h"
#include "gamelogic/syncskill.h"
#include "gamelogic/skilllevel.h"
#include "sceneworldboss.h"
#include "gamelogic/firstpassmgr.h"
#include "gamelogic/dragonexpbase.h"
#include "scenerisk.h"
#include "utility/UtilMath.h"
#include "scenebossrush.h"
#include "unit/pet.h"
#include "sceneinspire.h"
#include "sceneskycity.h"
#include "sceneskycitywait.h"
#include "scenegarden.h"
#include "scenereswar.h"
#include "scenestatistics.h"
#include "sceneguildcamp.h"
#include "scenereswarpve.h"
#include "scenekillinfo.h"
#include "sceneinherit.h"
#include "scenehorse.h"
#include "scenehorserace.h"
#include "scene/ptcg2m_sceneemptynotify.h"
#include "sceneinvfight.h"
#include "scenecastle.h"
#include "sceneherobattle.h"
#include "scene/ptcg2c_scenestatentf.h"
#include "scene/ptcg2c_sceneprepareinfontf.h"
#include "guild/guildcampparty.h"
#include "scene/scenemorphinfo.h"
#include "gamelogic/mobabattlemgr.h"
#include "scene/bigmeleeready.h"
#include "scene/battlefieldready.h"
#include "gamelogic/rolenethandler.h"
#include "scene/scenewedding.h"
#include "table/DragonNestTable.h"
#include "gamelogic/dragonconfig.h"
#include "unit/attributewatcher.h"
#include "scene/scenecompetedragon.h"
#include "scene/battlefieldfight.h"
#include "scenetransconfig.h"
#include "table/XSkillEffectMgr.h"

Scene::Scene()
:m_isready(false)
,m_ispvpone(false)
,m_ownerID(0)
,m_state(SCENE_RUNNING)
,m_finishState(KKSG::SCENE_FINISH_NONE)
,m_finishStateExpireTime(0)
,m_addedTimeSpan(0)
,m_serialScenesCostTime(0)
{
	memset(&m_sceneStartTime, 0, sizeof(m_sceneStartTime));
	memset(&m_SceneFinishTime, 0, sizeof(m_SceneFinishTime));

	m_pSpawner = NULL;
	m_pMortuary = NULL;
	m_teamSelectReward = NULL;
	m_updatehandler = NULL;
	m_pSceneDoodad = NULL;
	m_way = NULL;
	m_scenegroup = NULL;

	m_pSceneKill = NULL;
	m_pSceneMorphInfo = NULL;
	m_statistics = NULL;
	m_garden = NULL;
	m_globalAIUnit = NULL;

	m_vshandler = NULL;
	m_sceneTeam = NULL;
	m_worldbosshandler = NULL;
	m_pTower = NULL;
	m_pDragon = NULL;
	m_pCompeteDragon = NULL;
	m_pSceneWait = NULL;
	m_pGuildCamp = NULL;
	m_pWedding = NULL;
	m_pSceneResWarPVE = NULL;
	m_pHorseWait = NULL;
	m_DEHandler = NULL;
	m_BRHandler = NULL;	
	m_sceneRisk = NULL;
	m_pInspire = NULL;
	m_inherit = NULL;
	m_pSceneBMReady = NULL;
	m_liveID = 0;
	m_sendLiveNotice = false;
	m_debugAddWatchNum = 0;
	m_abyssID = 0;
	m_stopAITime = 0;
	m_AIMsgChannel.SetHostScene(this);
	m_lastKickRoleTime = 0;
	m_transctrl = NULL;
}

Scene::~Scene()
{

}

void Scene::Init(UINT32 sceneID, SceneInfo *pSceneInfo)
{
	SceneBase::Init(sceneID, pSceneInfo);

	UINT32 scenetype = GetSceneType();

	// group
	if (!SceneConfig::Instance()->IsBattleSceneExcept(scenetype))
	{
		m_scenegroup = new SceneGroup();
		if (!SceneConfig::Instance()->IsMultiBattle(scenetype))
		{
			m_statistics = new SceneStatistics();
			AddEventListener(m_statistics);
		}
	}
	// update
	if (IsSyncScene() && !SceneConfig::Instance()->IsSpawnExcept(scenetype))
	{
		m_pSpawner = new XLevelSpawner(GetSceneTemplateID(), this);
		m_pSpawner->Init();

		m_updatehandler = &SpawnUpdateHandler::m_updatehandler;
	}

	// way
	m_way = WayPointMgr::Instance()->GetWayPointByName(m_pSceneInfo->m_pConf->DynamicScene);

	// dead
	m_pMortuary = new SceneMortuary(this);
	m_pMortuary->SetReviveConfig(m_pSceneInfo->m_pConf->ReviveTime[0], m_pSceneInfo->m_pConf->ReviveTime[1]
	, m_pSceneInfo->m_pConf->ReviveBuff);

	// team box
	m_teamSelectReward = new TeamSelectReward(this);
	// doodad
	m_pSceneDoodad = new SceneDoodad(this);
	//kill
	m_pSceneKill = new SceneKill(this);
	//morph
	m_pSceneMorphInfo = new SceneMorphInfo(this);
	if (m_pSceneMorphInfo->CanMorph() && !GSConfig::Instance()->IsCrossGS())
	{
		AddEventListener(m_pSceneMorphInfo);
	}
	// inspire
	if(SceneInspire::NeedInspire(this))
	{
		m_pInspire = new SceneInspire(this);
	}
	// global AI
	if (IsSyncScene() && m_pSceneInfo && m_pSceneInfo->m_pConf && m_pSceneInfo->m_pConf->SceneAI != "")
	{
		m_globalAIUnit = new GlobalAIUnit();
		m_globalAIUnit->Init(this);
	}

	// vs
	m_vshandler = SceneVsBase::CreateVsHandler(this);
	if(m_vshandler)
	{
		m_updatehandler = &VsUpdateHandler::m_updatehandler;
	}
	// handlers
	InitHandler(scenetype);

	// ready mark
	if (!SceneConfig::Instance()->IsHasReadyScene(scenetype))
	{
		m_isready = true;
		OnSceneReady();
	}

	// pvp one
	if (SceneConfig::Instance()->IsPVPScene(scenetype))
	{
		auto i = GetGlobalConfig().PVPOne.find(m_pSceneInfo->m_pConf->id);
		if (i != GetGlobalConfig().PVPOne.end())
		{
			m_ispvpone = true;
		}
	}

	// trans
	TransConfig* transconfig = SceneTransConfig::Instance()->GetConfig(m_pSceneInfo->m_pConf->id);
	if (NULL != transconfig)
	{
		m_transctrl = new SceneTransCtrl(this, transconfig);
	}

	m_hasPvpBias = GetGlobalConfig().PvpBiasSceneTypes.count(scenetype) > 0;

	SSInfo<<"new scene id:"<<m_SceneID<<" map id:"<<m_pSceneInfo->m_pConf->id<<END;
}

#define SafeDeleate(p) do {if (p != NULL) { delete p; p = NULL; }} while(0)

void Scene::Uninit()
{
	SSInfo<<"destroy scene id:"<<m_SceneID<<" map id:"<<m_pSceneInfo->m_pConf->id<<END;

	SceneBase::Uninit();

	if (m_pSpawner)m_pSpawner->UnInit();
	SafeDeleate(m_pSpawner);

	if (m_globalAIUnit)m_globalAIUnit->Uninit();
	SafeDeleate(m_globalAIUnit);

	SafeDeleate(m_pMortuary);
	SafeDeleate(m_teamSelectReward);
	SafeDeleate(m_vshandler);
	SafeDeleate(m_pSceneDoodad);
	SafeDeleate(m_sceneTeam);
	SafeDeleate(m_worldbosshandler);
	SafeDeleate(m_scenegroup);
	SafeDeleate(m_DEHandler);
	SafeDeleate(m_BRHandler);
	SafeDeleate(m_pInspire);
	SafeDeleate(m_pSceneWait);
	SafeDeleate(m_garden);
	SafeDeleate(m_statistics);
	SafeDeleate(m_pSceneResWarPVE);
	SafeDeleate(m_pTower);
	SafeDeleate(m_pGuildCamp);
	SafeDeleate(m_pWedding);
	SafeDeleate(m_sceneRisk);
	SafeDeleate(m_pDragon);
	SafeDeleate(m_pCompeteDragon);
	SafeDeleate(m_pSceneMorphInfo);
	SafeDeleate(m_pSceneKill);
	SafeDeleate(m_inherit);
	SafeDeleate(m_pHorseWait);
	SafeDeleate(m_pSceneBMReady);
	SafeDeleate(m_transctrl);
	ClearAllTeleport();
	m_updatehandler = NULL;
}

void Scene::InitBattleParam(const KKSG::CreateBattleParam& roParam)
{
	if (roParam.has_teamdata())
	{
		m_sceneTeam = new SceneTeam(this);
		KKSG::SceneSwitchData tmpData;
		m_sceneTeam->Init(roParam.teamdata());
	}
	if (roParam.has_owner())
	{
		m_ownerID = roParam.owner().uid();
		SSInfo<<"new scene id:"<<m_SceneID<<" map id:"<<m_pSceneInfo->m_pConf->id<<" ownerid:"<<m_ownerID<<END;
	}
	for (int i = 0; i < roParam.guildinfo_size(); ++i)
	{
		SceneGuildInfo info;
		info.guildID = roParam.guildinfo(i).guildid();
		info.guildName = roParam.guildinfo(i).guildname();
		info.guildIcon = roParam.guildinfo(i).guildicon();
		m_sceneGuildInfos.push_back(info);
	}
	///> 创建Battle时初始化
	if(m_vshandler)
	{
		m_vshandler->Init(roParam);
	}
	///> 公会boss加入boss
	if (KKSG::SCENE_GUILD_BOSS == GetSceneType())
	{
		GuildBossMgr::Instance()->AddBoss(this, roParam.guildid(), roParam.guildbossinfo().bossindex(), roParam.guildbossinfo().curbossindex(), roParam.guildname(), roParam.guildbossinfo().bosshp(),roParam.guildbossinfo().count());
	}

	if (NULL != m_pSceneResWarPVE && KKSG::SCENE_RESWAR_PVE == GetSceneType())
	{
		UINT32 nMineID = roParam.reswar().mineid();
		UINT32 nTeamID = roParam.reswar().teamid();	
		UINT64 nGuildID = roParam.reswar().selfguildid();
		UINT32 nBuffTime   =  roParam.reswar().selfaddtime();
		if(roParam.reswar().has_buffid())
		{
			m_pSceneResWarPVE->SetRandBuffID(roParam.reswar().buffid());
		}
		m_pSceneResWarPVE->SetGuildID(nGuildID);
		m_pSceneResWarPVE->SetMineID(nMineID);
		m_pSceneResWarPVE->SetTeamID(nTeamID);
		m_pSceneResWarPVE->SetGuildBuffTime(nBuffTime);
		m_pSceneResWarPVE->Spawn();
		LogDebug("-----Scene::InitBattleParam---mineid=%d teamid=%d", nMineID, nTeamID);
	}

	///> 世界boss初始化
	if (NULL != m_worldbosshandler && roParam.has_worldboss())
	{
		const KKSG::WorldBossBornInfo& info = roParam.worldboss();
		m_worldbosshandler->InitBoss(info.enemyid(), info.attackpercent(), info.currenthp(), info.maxhp());
	}
	///> 初始化家园数据
	if (NULL != m_garden && roParam.has_gardeninfo())
	{
		const KKSG::SceneGardenInfo& info = roParam.gardeninfo();
		m_garden->Init(info);
	}
	///> 初始化婚宴数据
	if (NULL != m_pWedding && roParam.has_weddinginfo())
	{
		m_pWedding->Init(roParam.weddinginfo());
	}
	if (NULL != m_pDragon)
	{
		m_pDragon->InitBattle(roParam);
	}
}

void Scene::InitSceneSwitchData(const KKSG::SceneSwitchData& roData)
{
	m_serialScenesCostTime = roData.alreadycosttime();
	///> 自动切换场景，创建场景时调用
	if(roData.has_teamdata())
	{
		m_sceneTeam = new SceneTeam(this);
		m_sceneTeam->Init(roData.teamdata());
	}

	if(m_vshandler)
	{
		m_vshandler->Init(roData);
	}

	if (m_pTower)
	{
		m_pTower->Init(roData);
	}
	if (GetSceneType()==KKSG::SCENE_ABYSS_PARTY)
	{
		if (roData.roleextralist_size()!=0)
		{
			m_abyssID = roData.roleextralist(0).abyssid();
		}
	}

	if (m_pDragon)
	{
		m_pDragon->Init(roData);
	}

	for (int i = 0; i < roData.deadroles_size(); ++i)
	{
		m_switchDeadRoles.insert(roData.deadroles(i));
	}
}

//用来处理玩家进入场景时带的数据(创建场景时不方便带的数据)
void Scene::InitRoleSwitchData(const KKSG::SceneSwitchData& roData, Role* pRole, HEnterSceneType type)
{
	SSInfo << " role = " << pRole->GetID() << " type = " << type << END;
	if (HEST_HAVE_CREATED == type)
	{
		if (m_sceneTeam)
		{
			m_sceneTeam->InitRoleData(roData, pRole);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool Scene::CheckDynamicBlock(const Vector3& from, const Vector3& to)
{
	if(m_pSpawner)
	{
		const std::vector<XLevelInfo>* pInfos = m_pSpawner->GetLevelScript()->GetLevelScriptInfos();
		auto it = pInfos->begin();

		while (it != pInfos->end())
		{
			if((*it).enable)
			{
				float width = (*it).width / 2.0f;

				Vector3 center((*it).x, 0, (*it).z);
				Vector3 face = Vector3::HorizontalRotate(Vector3::FloatToAngle((*it).face), 90, true);

				if(UtilMath::IsLineSegmentCross(from, to, center + width * face, center - width * face))
				{
					if((*it).height > from.y) return false;
				}
			}

			++it;
		}
	}

	return true;
}

bool Scene::CheckSequenceBlock(const Vector3& from, const Vector3& to, const Vector3& center, int& currentIdx, int& crosstype, float& degree)
{
	if(m_pSpawner)
	{
		const std::vector<XLevelInfo>* pInfos = m_pSpawner->GetLevelScript()->GetLevelScriptInfos();
		auto it = pInfos->begin();

		Vector3 currentFace;
		Vector3 move = to - from; move.y = 0;

		if(move.IsZero()) return false;

		while (it != pInfos->end())
		{
			float width = (*it).width / 2.0f;

			Vector3 wall_center((*it).x, 0, (*it).z);
			Vector3 face = Vector3::HorizontalRotate(Vector3::FloatToAngle((*it).face), 90, true);

			if(UtilMath::IsLineSegmentCross(from, to, wall_center + width * face, wall_center - width * face))
			{
				Vector3 rankdir = to - center; rankdir.y = 0;
				if(Vector3::Clockwise(Vector3::forward, face))
					degree = GetSequenceDegree(face, rankdir);
				else
					degree = GetSequenceDegree(-face, rankdir);

				currentIdx = (*it).number;
				crosstype = Vector3::Angle(Vector3::FloatToAngle((*it).face), move) < 90 ? 1 : 0;

				return true;
			}
			else if((*it).number == currentIdx)
			{
				currentFace = face;
			}

			++it;
		}

		if(!currentFace.IsZero())
		{
			Vector3 rankdir = to - center; rankdir.y = 0;
			if(Vector3::Clockwise(Vector3::forward, currentFace))
				degree = GetSequenceDegree(currentFace, rankdir);
			else
				degree = GetSequenceDegree(-currentFace, rankdir);
		}
	}
	
	return false;
}

float Scene::GetSequenceDegree(const Vector3& face, const Vector3& rankdir)
{
	float degree = Vector3::Angle(face, rankdir);

	if(!Vector3::Clockwise(face, rankdir))
		degree = 360 - degree;

	return degree;
}

void Scene::CheckInitDeath(Role* role)
{
	if (role)
	{
		auto find = m_switchDeadRoles.find(role->GetID());
		if (find != m_switchDeadRoles.end())
		{
			AttributeWatcher watcher(role);
			watcher.SetAttr(BASIC_CurrentHP, 0);
			watcher.CheckChangedAttributeAndNotify(true);
			role->TriggerDeath();
			m_switchDeadRoles.erase(find);
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// doodad
void Scene::FillDoodadInfo(KKSG::SceneCfg* pData)
{
	if (m_SyncMode != SERVERMODE)
	{
		m_pSceneDoodad->FillDoodadInfo(pData);
	}
}

void Scene::SetPickedDoodad(UINT32 waveID)
{
	m_pSceneDoodad->SetPickedDoodad(waveID);
}

const std::list<PickDoodadInfo>& Scene::GetDoodadsInfo()
{
	return m_pSceneDoodad->GetDoodadsInfo();
}

std::list<PickServerModeDoodadInfo>& Scene::GetServerDoodadsInfo()
{
	return m_pSceneDoodad->GetServerDoodadsInfo();
}

void Scene::FetchDoodad(Role* role, const KKSG::EnemyDoodadInfo& arg, KKSG::RollInfoRes& roRes)
{
	m_pSceneDoodad->FetchDoodad(role, arg, roRes);
}

void Scene::GenerateEnemyDroppedDoodad(Enemy* pEnemy, bool isLastEnemy)
{
	if (m_SyncMode != SERVERMODE)
		return;

	m_pSceneDoodad->GenerateEnemyDroppedDoodad(pEnemy, isLastEnemy);
}

void Scene::GenerateDoodad(UINT32 doodadID, UINT32 waveID, Vector3& pos)
{
	if (m_SyncMode != SERVERMODE)
		return;

	m_pSceneDoodad->GenerateDoodad(doodadID, waveID, pos);
}

void Scene::NotifyDoodadAppearOnEnter(Role* role)
{
	if (!role)
	{
		return;
	}

	if (SyncMode() == SERVERMODE)
	{
		m_pSceneDoodad->NotifyDoodadAppearOnEnter(role);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void Scene::SyncLevelScriptState(Role* role)
{
	if (GetSpawner())
	{
		GetSpawner()->OnEnterScene(role);
	}
}

TeleportBase* Scene::AddTeleportDoor(const Vector3& sPos, float tr , const Vector3& dPos, float dface)
{
	TeleportBase* teleportDoor = NULL;
	//根据场景类型 决定用那种传送门
	if(GetSceneType() == KKSG::SCENE_CASTLE_FIGHT)
	{
		teleportDoor = new TeleportOne(sPos , tr , dPos , dface , this);
	}
	else
	{
		teleportDoor = new TeleportDoor(sPos , tr , dPos , dface , this);
	}

	if(teleportDoor)
	{
		m_teleports.push_back(teleportDoor);
	}

	return teleportDoor;
}

void Scene::ClearAllTeleport()
{
	for(size_t i = 0; i < m_teleports.size(); ++i)
	{
		delete m_teleports[i];
	}
	m_teleports.clear();
}

bool Scene::HasTeleportDoor()
{
	return !m_teleports.empty();
}

void Scene::UpdateTeleport()
{
	std::vector<TeleportBase*> waitDel;	
	for(size_t i = 0; i < m_teleports.size(); ++i)
	{
		if(NULL == m_teleports[i])
		{
			continue;
		}
		if(m_teleports[i]->IsFinished())
		{
			waitDel.push_back(m_teleports[i]);
		}
		else
		{
			m_teleports[i]->Update();
		}
	}

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		DelTeleportDoor(waitDel[i]);
	}
}

void Scene::DelTeleportDoor(TeleportBase* tele)
{
	for(auto it = m_teleports.begin(); it != m_teleports.end(); ++it)
	{
		if((*it) == tele)
		{
			delete (*it);
			m_teleports.erase(it);
			break;	
		}
	}
}

int Scene::GetKillEnemyScore()
{
	return m_pSceneKill->GetKillMonsterScore();
}

void Scene::SetSceneKillState(bool state)
{
	m_pSceneKill->SetState(state);
}

void Scene::AddKillEnemyInfo(Enemy* enemy)
{
	m_pSceneKill->AddKillEnemyInfo(enemy);
}

void Scene::SetStageRankInfo(UINT64 qwRoleID, const StageRankInfo& info)
{
	m_StageRankInfo[qwRoleID] = info;
}

StageRankInfo* Scene::GetStageRankInfo(UINT64 roleid)
{
	auto it = m_StageRankInfo.find(roleid);
	return it == m_StageRankInfo.end() ? NULL : &it->second;
}

INT32 Scene::GetRoleRank(UINT64 qwRoleID) const
{
	auto it = m_StageRankInfo.find(qwRoleID);
	return it == m_StageRankInfo.end() ? 0 : it->second.rank;
}

UINT32 Scene::ChangeWatchRole(Role* role, UINT64 viewID)
{
	if (!role->IsWatcher())
	{
		return KKSG::ERR_FAILED;
	}

	const std::list<Role*>& roles = GetAllRoles();
	bool find = false;
	foreach(i in roles)
	{
		if ((*i)->GetID() == viewID)
		{
			find = true;
			break;
		}
	}

	if(!find)
	{
		return KKSG::ERR_FAILED;
	}

	m_WatcherViewRole[role->GetID()] = viewID;

	return KKSG::ERR_SUCCESS;
}

UINT32 Scene::GetWatchRoleNum()
{
	return m_pUnitHandler->GetWatcherNum() + m_debugAddWatchNum;
}

UINT32 Scene::GetTotalWatchNum()
{
	if (m_liveID)
	{
		LiveInfo* info = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, m_liveID);
		if (info)
		{
			return info->m_watchNum;
		}
	}

	return 0;
}

UINT32 Scene::GetTotalCommendNum()
{
	if (m_liveID)
	{
		LiveInfo* info = LiveManager::Instance()->GetLiveInfo(KKSG::LIVE_RECOMMEND, m_liveID);
		if (info)
		{
			return info->m_commendNum;
		}
	}

	return 0;
}


SceneGuildInfo* Scene::GetSceneGuildInfo(UINT64 guildID)
{
	foreach(i in m_sceneGuildInfos)
	{
		if (i->guildID == guildID)
		{
			return &(*i);
		}
	}
	return NULL;
}

bool Scene::IsHelper(UINT64 roleid)
{
	if(m_sceneTeam && m_sceneTeam->IsHelper(roleid))
	{
		return true;
	}
	return false;
}

int Scene::UseTicket(UINT64 roleID)
{
	if(m_sceneTeam)
	{
		return m_sceneTeam->UseTicket(roleID);
	}
	return 0;
}

void Scene::GetTeammates(Unit* pUnit, std::vector<Unit*>& teammates)
{
	teammates.clear();
	if(NULL == pUnit)
	{
		return ;
	}

	std::vector<UINT64> IDS;
	if(m_sceneTeam)
	{
		m_sceneTeam->GetTeammates(pUnit->GetID(), IDS);
	}
	else if(GetPvpHandler())
	{
		GetPvpHandler()->GetSamePvpCampRoles(pUnit->GetID(), IDS);
	}
	for(size_t i = 0; i < IDS.size(); ++i)
	{
		Unit* mateUnit = NULL;
		Role* mateRole = RoleManager::Instance()->FindByRoleID(IDS[i]);
		if(mateRole)
		{
			mateUnit = (Unit*)(mateRole);
		}
		else
		{
			DummyRole* mateDummy = DummyRoleManager::Instance()->Find(IDS[i]);
			if(mateDummy)
			{
				mateUnit = (Unit*)(mateDummy);
			}

		}
		if(mateUnit && mateUnit->GetID() != pUnit->GetID() && mateUnit->GetCurrScene() == pUnit->GetCurrScene())
		{
			teammates.push_back(mateUnit);
		}
	}
}

void Scene::AddEnterFight(UINT64 uid)
{
	auto it = m_makeEnemyEenterFight.find(uid);
	if (it == m_makeEnemyEenterFight.end())
	{
		m_makeEnemyEenterFight.insert(std::make_pair(uid, 1));
	}
	else
	{
		it->second++;
	}
}

int Scene::GetEnterFightTime(UINT64 uid)
{
	auto it = m_makeEnemyEenterFight.find(uid);
	if (it == m_makeEnemyEenterFight.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}

UINT32 Scene::GetRankNpcHpPercent()
{
	UINT32 npcId = StageRankMgr::Instance()->GetConcernNpcId(GetSceneTemplateID());
	if (npcId == 0)
	{
		return 0;
	}
	FindEnemyIDCondition cond(npcId);
	std::vector<Unit *> units;
	m_pUnitHandler->GetUnitByCondition(cond, units);
	if (units.empty())
	{
		return 0;
	}
	if (units.size() != 1)
	{
		SSWarn << "npc: " << npcId << " not only one" << END;
	}
	return units[0]->GetHpPercent();

}

Unit* Scene::FindSceneUnit(UINT64 unitID)
{
	Unit* pUnit = Unit::FindUnit(unitID);
	if(pUnit && pUnit->GetCurrScene() == this)
	{
		return pUnit;
	}
	return NULL;
}

Role* Scene::FindSceneRole(UINT64 roleID)
{
	Unit* pUnit = FindSceneUnit(roleID);	
	if(pUnit && pUnit->IsRole())
	{
		return ((Role*)(pUnit));
	}
	return NULL;
}
	
Role* Scene::FindSceneRoleNotWatcher(UINT64 roleID)
{
	Role* pRole = FindSceneRole(roleID);
	if(pRole && !pRole->IsWatcher())
	{
		return pRole;
	}
	return NULL;
}

Role* Scene::FindSceneRoleCond(UINT64 roleID, bool includeLoading, bool includeWatcher)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if(NULL == pRole)
	{
		return NULL;
	}
	if(pRole->GetCurrScene() != this)
	{
		if(!includeLoading || pRole->GetLoadingScene() != this)
		{
			return NULL;
		}
	}

	if(!includeWatcher && pRole->IsWatcher())
	{
		return NULL;
	}
	return pRole;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// update
void Scene::Update(float deltaTime)
{
	if (m_SyncMode == SERVERMODE)
	{
		UpdateTeleport();

		if (m_sceneTeam)
		{
			m_sceneTeam->Update();
		}

		if (NULL != m_worldbosshandler)
		{
			m_worldbosshandler->Update();
		}

		timeval now;
		gettimeofday(&now, NULL);
		UINT64 time = now.tv_sec;
		time *=1000;
		time += now.tv_usec / 1000;
		if (m_stopAITime && time >= m_stopAITime)
		{
			SetStopAITime(0);
		}
		if (!m_stopAITime && m_state != SCENE_RUNNING)
		{
			SetStopAITime(UINT64_MAX);
		}

		///> is scene end 
		if (m_state == SCENE_RUNNING)
		{
			RunningSceneUpdate(deltaTime);
		}

		SceneBase::Update(deltaTime);
	}
	///> end process
	if (m_state != SCENE_RUNNING)
	{
		FinishSceneUpdate(deltaTime);
	}
	CheckKickRole();
}

void Scene::RunningSceneUpdate(float deltaTime)
{
	if (NULL != m_updatehandler)	
	{
		m_state = (SceneState)m_updatehandler->Update(this, deltaTime);

		if (m_state != SCENE_RUNNING)
		{
			if (NULL != m_globalAIUnit)
			{
				XAIDisableEventArgs oArgs;
				m_globalAIUnit->GetXObject()->DispatchAction(&oArgs);
			}
			if (NULL != m_statistics)
			{
				///> 场景结束剩下的怪物
				UINT32 count = 0;
				const std::unordered_map<UINT64, Unit*> units = GetUnits();
				for (auto i = units.begin(); i != units.end(); ++i)
				{
					Unit* unit = i->second;
					if (unit->IsDead())
					{
						continue;
					}
					if (FightGroupMgr::Instance()->CanAttack(KKSG::FightRole, unit->GetFightGroup()))
					{
						++count;
					}
				}
				m_statistics->m_monsterinfo.MonsterEndCount = count; 
			}
		}

		if(m_DEHandler)
		{
			m_DEHandler->OnSceneUpdate(m_state);
		}
		if(m_BRHandler)
		{
			m_BRHandler->Update();
		}
		if (m_pSceneResWarPVE)
		{
			m_pSceneResWarPVE->Update();
		}
	}
	if (NULL != m_transctrl)
	{
		m_transctrl->OnTick(false);
	}
}

void Scene::FinishSceneUpdate(float deltatime)
{
	switch (m_finishState)
	{
		///> 开始结算
	case KKSG::SCENE_FINISH_NONE:
		{
			ChangeFinishState(KKSG::SCENE_FINISH_START, 0);
			break;
		}
		///> 是否需要延迟
	case KKSG::SCENE_FINISH_START:
	case KKSG::SCENE_FINISH_PICK_ITEM:
		{
			EndSyncScene(m_state);
			break;
		}
		///> 是否需要抽宝箱
	case KKSG::SCENE_FINISH_SHOWRESULT:
		{
			if (time(NULL) >= m_finishStateExpireTime)
			{
				if(m_teamSelectReward && IsCanDrawBox())
				{
					ChangeFinishState(KKSG::SCENE_FINISH_DRAW_BOX, m_pSceneInfo->GetSelectBoxTime());
				}
				else
				{
					ChangeFinishState(KKSG::SCENE_FINISH_END, m_pSceneInfo->GetSelectBoxEndTime());
				}
			}
			break;
		}
		///> 抽宝箱
	case KKSG::SCENE_FINISH_DRAW_BOX:
		{
			if (m_teamSelectReward->IsAllSelected())
			{
				m_teamSelectReward->End();
				ChangeFinishState(KKSG::SCENE_FINISH_END, m_pSceneInfo->GetSelectBoxEndTime());
			}
			else if (time(NULL) >= m_finishStateExpireTime)
			{
				m_teamSelectReward->Finish(false);
				m_teamSelectReward->End();
				ChangeFinishState(KKSG::SCENE_FINISH_END, m_pSceneInfo->GetSelectBoxEndTime());
			}
			break;
		}
	default:
		break;
	}
}

void Scene::ChangeFinishState(int newState, int countdown)
{
	m_finishState = newState;
	m_finishStateExpireTime = countdown + (int)time(NULL);

	if (m_finishState == KKSG::SCENE_FINISH_START)
	{
		for (auto iter = m_EventListeners.begin(); iter != m_EventListeners.end(); ++iter)
		{
			(*iter)->OnBeginFinish(this);
		}
	}
}

void Scene::FinishScene()
{
	m_finishState = KKSG::SCENE_FINISH_SHOWRESULT;

    if (m_sceneTeam)
    {
        m_sceneTeam->OnSceneCompleteForDragonStatistics(m_state == SCENE_WIN);
    }

	const std::list<Role*>& temproles = GetAllRoles();
	std::vector<Role*> vecRole;
	for (auto i = temproles.begin(); i != temproles.end(); ++i)
	{
		vecRole.push_back(*i);
	}
	bool noFloor = false;
	if (m_pTower && !vecRole.empty())
	{
		noFloor = m_pTower->FinishNoFloor(vecRole[0]);
	}

	///> dispatch scene finish
	for (auto i = m_Listeners.begin(); i != m_Listeners.end(); ++i)
	{
		ISceneListener* listener  = *i;
		listener->OnSceneFinished(this, vecRole, !m_pTower ? (m_state == SCENE_WIN) : !noFloor);
	}

	if (m_state == SCENE_WIN)
	{
		if (m_pDragon)
		{
			m_pDragon->FinishScene(vecRole);
		}

		if (m_pCompeteDragon)
		{
			m_pCompeteDragon->FinishScene(vecRole);
		}

		OnSceneFinish();
	}

	if (m_sceneTeam)
	{
		m_sceneTeam->OnSceneComplete(m_state == SCENE_WIN);
	}

	FirstPassMgr::Instance()->CheckFirstPass(this);
}

void Scene::EndSoloScene(SceneState state, UINT32 timespan)
{
	if (m_finishState == KKSG::SCENE_FINISH_NONE)
	{
		m_state = state;
		if (!timespan)
		{
			gettimeofday(&m_SceneFinishTime, NULL);
		}
		else
		{
			SetTimeSpan(timespan);
		}

		FinishScene();
	}
}

bool Scene::CheckWinDealy(SceneState state)
{
	if (state == SCENE_WIN)
	{
		if (GetSceneInfo()->m_pConf->WinDelayTime)
		{
			ChangeFinishState(KKSG::SCENE_FINISH_PICK_ITEM, GetSceneInfo()->m_pConf->WinDelayTime);
			return true;
		}
	}

	return false;
}

bool Scene::CheckWinDelayEnd()
{
	if (m_finishState == KKSG::SCENE_FINISH_PICK_ITEM)
	{
		if (m_pTower && m_pTower->CheckWinDelayEnd())
		{
			return true;
		}

		if (time(NULL) >= m_finishStateExpireTime)
		{
			if (!m_pSceneDoodad->IsRollDoodadItemNotPicked())
			{
				return true;
			}

			m_pSceneDoodad->RandLeftDoodad();
			return false;
		}
	}
	return false;
}

bool Scene::CheckFinishEnd(SceneState state)
{
	bool end = false;
	if (m_finishState == KKSG::SCENE_FINISH_NONE || m_finishState == KKSG::SCENE_FINISH_START)
	{
		if (CheckWinDealy(state))
		{
			return end;
		}
		end = true;
	}
	else if (CheckWinDelayEnd())
	{
		end = true;
	}

	return end;
}

void Scene::EndSyncScene(SceneState state)
{
	if (!CheckFinishEnd(state))
	{
		return;
	}

	m_state = state;
	gettimeofday(&m_SceneFinishTime, NULL);

	FinishScene();

	ReviveDeathRole();
}

bool Scene::IsWatchEnd()
{
	if (0 == m_RoleCount || GetSceneState() != SCENE_RUNNING)
	{
		return true;
	}

	auto gwb = GetGWBHandler();
	if(gwb && gwb->IsWatchEnd())//公会擂台战
	{
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scene::KillWave(int waveID)
{
	FindEnemyWaveIDCondition cond(waveID);
	std::vector<Unit *> units;
	m_pUnitHandler->GetUnitByCondition(cond, units);
	for (UINT32 i = 0; i < units.size(); ++i)
	{
		if (!units[i]->IsDead())
		{
			units[i]->TriggerDeath();
		}
	}
}
// to delete 
void Scene::KillAllEnemy(int waveID)
{
	KillWave(waveID);
}
// to delete 
void Scene::KillAllAlly(int waveID)
{
	KillWave(waveID);
}

void Scene::KillAllEnemy(bool isend)
{
	const std::list<Role*>& vec = GetAllRoles();
	if (vec.empty())
	{
		return;
	}

	const std::vector<Unit *>* units = GetOpponents((*vec.begin())->GetFightGroup());
	if (NULL == units)
	{
		return;
	}
	std::vector<Unit*> tempunits = *units;
	for (auto i = tempunits.begin(); i != tempunits.end(); ++i)
	{
		Unit* unit = *i;
		if (!unit->IsDead())
		{
			unit->TriggerDeath();
		}
	}
	if (NULL != m_statistics && isend)
	{
		m_statistics->m_monsterinfo.MonsterCount2 = tempunits.size();
	}
}

void Scene::KickAllRoleOut()
{
	for (auto i = m_RoleWaitLoadScene.begin(); i != m_RoleWaitLoadScene.end(); ++i)
	{
		Role *pRole = i->second;
		pRole->ClearLoadingScene();
		SceneManager::Instance()->ChangeScene(pRole, pRole->GetPositionKeeper().GetLastHallSceneID());
	}
	m_RoleWaitLoadScene.clear();
	m_PrepareList->Clear();

	const std::list<Role*>& vecRole = GetAllRoles();
	for (auto i = vecRole.begin(); i != vecRole.end(); ++i)
	{
		Role* pRole = (*i);
		SceneManager::Instance()->ChangeScene(pRole, pRole->GetPositionKeeper().GetLastHallSceneID());
	}

	auto& watchers = GetWatchers();
	for (auto it = watchers.begin(); it != watchers.end(); ++it)
	{
		Role* pRole = it->second;
		SceneManager::Instance()->ChangeScene(pRole, pRole->GetPositionKeeper().GetLastHallSceneID());
	}

	m_pMortuary->ClearDeathInfo();

	m_pSceneDoodad->Clear();
}

void Scene::ReviveDeathRole()
{
	if (GetSceneType() != KKSG::SCENE_DRAGON || GetSceneState() != SCENE_WIN)
	{
		return;
	}
	if (!m_sceneTeam)
	{
		return;
	}
	UINT32 expID = m_sceneTeam->GetExpID();
	const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(expID);
	if(!data)
	{
		return;
	}
	if (data->DragonNestDifficulty == 2)
	{
		return;
	}
	const std::list<Role*>& roles = GetAllRoles();
	foreach(i in roles)
	{
		if ((*i)->IsDead())
		{
			(*i)->Revive();
		}
	}
}

void Scene::GetPlayersNotHelper(std::vector<Role *> &output)
{
	const std::list<Role*>& vecRole = GetAllRoles();
	for (auto iter = vecRole.begin(); iter != vecRole.end(); ++iter)
	{
		if (!IsHelper((*iter)->GetID()))
		{
			output.push_back(*iter);
		}
	}
}

void Scene::ChangeAttrBeforEnterScene(Unit *pUnit)
{
	if (GetSceneType() == KKSG::SCENE_BOSSRUSH && pUnit->IsEnemy() && ((Enemy*)pUnit)->IsBoss() && GetBRHandler())
	{
		GetBRHandler()->OnBossAppear((Enemy*)pUnit);
	}
	if (GetSceneType() == KKSG::SCENE_RESWAR_PVE && pUnit->IsEnemy() && ((Enemy*)pUnit)->IsBoss() && GetSceneResWarPVE())
	{
		GetSceneResWarPVE()->OnBossAppear((Enemy*)pUnit);
	}
	if(GetDEHandler() && GetSceneType() == KKSG::SCENE_DRAGON_EXP && pUnit->IsEnemy() && ((Enemy*)pUnit)->IsBoss())
	{
		GetDEHandler()->OnBossApper((Enemy*)pUnit);
	}
	if ((GetSceneType() == KKSG::SCENE_BATTLEFIELD_FIGHT && pUnit->IsEnemy()) && GetBFFightHandler())
	{
		GetBFFightHandler()->AddBuff((Enemy*)pUnit);
	}
	if(m_pDragon)
	{
		m_pDragon->AddBuff((Enemy*)pUnit);
	}
}

void Scene::ReviveEnemy(Enemy* pUnit, int waveID, UINT64 lastID)
{
	if(NULL != pUnit && -1 != waveID)
	{
		pUnit->SetWaveID(waveID);
		pUnit->SetSpawner(GetSpawner());
		if (GetSpawner())
		{
			//if (pUnit->IsBoss())
			{
				GetSpawner()->BossRevive(waveID, lastID, pUnit);
			}
		}
	}
}

std::ostream & operator<<(std::ostream &stream, Scene *scene)
{
	stream << "{Scene tid: " << scene->GetSceneTemplateID() << ", sid: " << scene->GetSceneID() << ", " 
		<< scene->SceneTypeStr(scene->GetSceneType()) << "}";
	return stream;
}

Scene * Scene::CreateScene(UINT32 sceneTemplateID, SceneInfo *pSceneInfo, UINT32 dwSceneInstanceID)
{
	Scene *pScene = NULL;
	UINT32 sceneID = dwSceneInstanceID;
	if(sceneID == INVALID_SCENE_ID)
	{
		sceneID = NewSceneID();
	}

	pScene = new Scene();
	pScene->Init(sceneID, pSceneInfo);
	return pScene;
}

Scene * Scene::CreateScene(UINT32 sceneTemplateID, SceneInfo *pSceneInfo, UINT64 ownerID, UINT32 dwSceneInstanceID)
{
	Scene *pScene = NULL;
	UINT32 sceneID = dwSceneInstanceID;
	if(sceneID == INVALID_SCENE_ID)
	{
		sceneID = NewSceneID();
	}

	pScene = new Scene();
	pScene->SetOwnerID(ownerID);
	pScene->Init(sceneID, pSceneInfo);
	return pScene;
}

void Scene::DestroyScene(Scene *scene)
{
	scene->Uninit();
	delete scene;
}

void Scene::OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value)
{
	if (m_vshandler)
	{
		m_vshandler->OnHurt(pTargetUnit, pCastUnit, value);
	}
}

void Scene::DumpScene(std::stringstream &ss)
{
	ss << this << "\n"
		<< "  EnemyCount: " << m_EnemyCount << " , DummyCount: " << m_DummyRoleCount << ", RoleCount: " << m_RoleCount << ", WaitingCount: " << m_RoleWaitLoadScene.size() << "\n\n";

	m_pUnitHandler->DumpLine(ss);
	ss << "\n";
}

bool Scene::IsPVPCombat(Scene* scene)
{
	if(scene == NULL)
		return false;
	if(scene->GetSceneInfo() != NULL)
		return scene->GetSceneInfo()->m_pConf->CombatType == 1;
	return false;
}

void Scene::NotifySceneState()
{
	PtcG2C_SceneStateNtf ntf;
	ntf.m_Data.mutable_state()->set_isready(m_isready);
	ntf.m_Data.mutable_state()->set_runstate(m_state);

	const std::list<Role*>& roles = GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		if (GetSceneType() != KKSG::SCENE_HEROBATTLE && 
			GetSceneType() != KKSG::SCENE_MOBA)
		{
			///> 能动
			role->MakePuppet(false);
		}
		ntf.m_Data.set_rolespecialstate(role->GetSpecialState());
		role->Send(ntf);
	}
}

void Scene::NotifyRoleWait(const std::vector<Role*>& loaded, const std::vector<std::string>& unloaded)
{
	// pack
	PtcG2C_ScenePrepareInfoNtf ntf;
	for (auto i = unloaded.begin(); i != unloaded.end(); ++i)
	{
		ntf.m_Data.add_unreadyroles(*i);
	}
	// notify
	for (auto j = loaded.begin(); j != loaded.end(); ++j)
	{
		(*j)->Send(ntf);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// event
bool Scene::CheckLeaveBeforeFinish(Role* role)
{
	if (role->IsWatcher())
	{
		return false;
	}
	if (m_state != SCENE_RUNNING)
	{
		return false;
	}

	UINT32 sceneType = GetSceneType();
	///> 单机场景
	if (!IsSyncScene())
	{
		return true;
	}
	//  离开公会场景时候清理交易数据
	if (GetSceneType() == KKSG::SCENE_GUILD_HALL && role->getGuildId() == GetOwnerID())
	{
		CGuildCampPartyMgr::Instance()->ClearTradeInfo(role);
	}

	///> 非战斗场景
	if (SceneConfig::Instance()->IsBattleSceneExcept(sceneType))
		return false;

	///> 黑暗神殿
	if (sceneType == KKSG::SCENE_TOWER)
	{
		return false;
	}

	///> 非关卡脚本控制的场景
	if (NULL != m_updatehandler && m_updatehandler->GetHandlerType() != UPDATE_HANDLER_SPAWN)
	{
		return false;
	}
	
	return true;
}

void Scene::OnLeaveBeforeFinish(Role* role)
{
	if (CheckLeaveBeforeFinish(role))
	{
		role->OnSceneFailed(this, false);
	}
}

void Scene::OnPreEnterScene(Scene *pScene, Role* role)
{
	if(m_vshandler)
	{
		m_vshandler->PreEnterScene(role);
	}
}

void Scene::OnPreEnterSceneFinally(Scene* pScene, Role* pRole)
{
	if(m_vshandler)
	{
		m_vshandler->PreDoEnterScene(pRole);
	}

	if (!IsWatchEnd() && pRole->IsWatcher())
	{
		LiveManager::Instance()->EnterSceneNotifyWatchData(pRole, this);
	}

	if (m_sceneStartTime.tv_sec == 0)
	{
		InitSceneStartTime();
	}	
}

void Scene::OnEnterSceneFinally(Scene *pScene, Role* pRole)
{
	if (!IsWatchEnd() && pRole->IsWatcher())
	{
		LiveManager::Instance()->OnEnterWatch(pRole, m_liveID);
	}

	SyncLevelScriptState(pRole);

	NotifyDoodadAppearOnEnter(pRole);

	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = pRole->GetID();
		event.type = VS_EVENT_ROLE_LOADED;
		m_vshandler->VsOnEvent(event);

		// notify wait info
		if (!IsSceneReady())
		{
			std::vector<Role*> loaded;
			std::vector<std::string> unloaded;
			m_vshandler->GetWaitInfo(loaded, unloaded);
			NotifyRoleWait(loaded, unloaded);
		}
	}
	if (m_DEHandler)
	{
		m_DEHandler->OnRoleEnter(pRole);
	}
	if (m_sceneTeam)
	{
		m_sceneTeam->OnClientLoaded(pRole);
	}

	if (GetResWarHandler())
	{
		GetResWarHandler()->InitDataToClient(pRole);
	}

	if (!pRole->IsWatcher() && m_liveID)
	{
		LiveManager::Instance()->OnEnter(pRole, m_liveID);
	}

	if (GetSceneType() == KKSG::SCENE_GUILD_HALL && pRole->getGuildId() == GetOwnerID())
	{
		CGuildCampPartyMgr::Instance()->UpdatePartyStauts(pRole,GetSceneID());
	}
	
	CheckInitDeath(pRole);
}

void Scene::OnLeaveScene(Scene *pScene, Role* role)
{
	if (role->IsWatcher())
	{
		m_WatcherViewRole.erase(role->GetID());
	}
	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = role->GetID();
		event.type = VS_EVENT_ROLE_QUIT;
		m_vshandler->VsOnEvent(event);
	}
	if(m_DEHandler)
	{
		m_DEHandler->OnRoleLeave(role);
	}
}

void Scene::OnAddNonRole(Scene *pScene, Unit* unit)
{
	if (m_isready)
	{
		if (!unit->IsRole())
		{
			unit->OnSceneReady();
		}
		if (m_vshandler)
		{
			m_vshandler->OnNonRoleEnterScene(unit);
		}
	}
}

void Scene::OnRemoveWaitingRole(Scene* pScene, Role* role)
{
	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = role->GetID();
		event.type = VS_EVENT_ROLE_LOADING_QUIT;
		m_vshandler->VsOnEvent(event);
	}
}

void Scene::OnRemoveUnit(Scene *pScene, Unit* pUnit)
{
	if (!pUnit)
	{
		return;
	}
	if (pUnit->IsRole())
	{
		OnLeaveBeforeFinish((Role*)pUnit);
	}
	if (m_liveID && pUnit->IsRole())
	{
		Role* role = (Role*)pUnit;
		if (role->IsWatcher())
		{
			LiveManager::Instance()->LeaveWatchBattle(role, m_liveID);
			role->SetWatcher(false);
		}
		else
		{
			LiveManager::Instance()->OnLeave(role, m_liveID);
		}
	}
}

void Scene::OnRoleReconnect(Role* role)
{
	if (GetSpawner() && role)
	{
		GetSpawner()->PushReconnectRole(role);
	}

	if (!SceneConfig::Instance()->IsSpawnExcept(GetSceneType()))
		m_pSceneDoodad->NotifyDoodadAppearOnEnter(role);

	if (m_vshandler)
	{
		m_vshandler->OnRoleReconnect(role);
	}
	if (NULL != m_worldbosshandler)
	{
		m_worldbosshandler->OnRoleReconnect(role);
	}
}

void Scene::BeforeRoleLoginReconnect(Role* pRole)
{
	if (m_vshandler != NULL)
	{
		m_vshandler->OnRoleReconnectBeforeNtf(pRole);
	}
	pRole->GetNetHandler()->SendEnterScene(this);
}

void Scene::OnSceneReady()
{
	NotifySceneState();

	// scene event
	if (SceneConfig::Instance()->IsHasReadyScene(GetSceneType())) 
	{
		InitSceneStartTime();
	}

	if (GetGlobalAI())
	{
		XAIEnableEventArgs oArgs;
		GetGlobalAI()->GetXObject()->DispatchAction(&oArgs);
	}

	// role event
	const std::list<Role*>& roles = GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		(*i)->OnSceneReady();
	}
	// unit
	const std::unordered_map<UINT64, Unit*>& units = GetUnits();
	for (auto j = units.begin(); j != units.end(); ++j)
	{
		Unit* unit = j->second;
		unit->OnSceneReady();
	}
}

void Scene::OnSceneFinish()
{
	PtcG2M_SceneEndToMaster msg;

	const std::list<Role*>& playersVec = GetAllRoles();
	foreach (i in playersVec)
	{
		if ((*i) != NULL)
		{
			msg.m_Data.add_roles((*i)->GetID());
		}
	}

	SceneInfo* pInfo = GetSceneInfo();
	if (pInfo == NULL || pInfo->m_pConf == NULL)
	{
		return ;
	}
	msg.m_Data.set_scenename(pInfo->m_pConf->Comment);
	msg.m_Data.set_sceneid(GetSceneID());
	msg.m_Data.set_scenetype(pInfo->m_pConf->type);
	msg.m_Data.set_iswin(SCENE_WIN == m_state);

	if (!GSConfig::Instance()->IsCrossGS())
	{
		MSLink::Instance()->SendTo(msg);
	}
	else
	{
		std::unordered_set<UINT32> sids;
		for (auto it = playersVec.begin(); it != playersVec.end(); ++it)
		{
			Role* pRole = (*it);
			UINT32 serverID = pRole->GetFromServerId();
			if(sids.find(serverID) != sids.end())
			{
				continue;
			}
			sids.insert(serverID);
			pRole->SendToMS(msg);
		}
	}
}

void Scene::OnRoleDead(Role *pRole)
{
	if (m_pMortuary)
	{
		m_pMortuary->AddDeathRole(pRole);
	}

	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = pRole->GetID();
		event.type = VS_EVENT_ROLE_DIE;
		m_vshandler->VsOnEvent(event);
	}
	///> dispatch role die
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnRoleDie(this, pRole);
	}
}

void Scene::OnRoleRevive(Role* pRole)
{
	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = pRole->GetID();
		event.type = VS_EVENT_ROLE_REVIVE;
		m_vshandler->VsOnEvent(event);
	}

	if (m_pMortuary)
	{
		m_pMortuary->DeleteDeathRoleInTimer(pRole);
	}
}

void Scene::OnDummyRoleRevive(DummyRole* pDummy)
{
	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = pDummy->GetID();
		event.type = VS_EVENT_ROLE_REVIVE;
		m_vshandler->VsOnEvent(event);
	}
}

void Scene::OnEnemyDead(Enemy* enemy)
{
	if(m_DEHandler && enemy->IsBoss())
	{
		m_DEHandler->OnBossDie(enemy);
	}

	if (NULL != m_vshandler)
	{
		m_vshandler->OnEnemyDead(enemy);
	}

	///> dispatch enemy die
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnEnemyDie(this, enemy);
	}

	AddKillEnemyInfo(enemy);
}

void Scene::OnDummyRoleDead(DummyRole* pdummy)
{
	if (NULL != m_vshandler)
	{
		VsEvent event;
		event.roleid = pdummy->GetID();
		event.type = VS_EVENT_ROLE_DIE;
		m_vshandler->VsOnEvent(event);
	}
	///> dispatch dummy die
	for (auto i = m_EventListeners.begin(); i != m_EventListeners.end(); ++i)
	{
		(*i)->OnDummyDie(this, pdummy);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
time_t Scene::CostTime()
{
	if (m_sceneStartTime.tv_sec == 0)
	{
		return -1;
	}

	return CostTimeMili() / 1000;
}

time_t Scene::CostTimeMili()
{
	if (m_sceneStartTime.tv_sec == 0)
	{
		return -1;
	}

	if (m_SceneFinishTime.tv_sec == 0)
	{
		timeval now;
		gettimeofday(&now, NULL);
		return (now.tv_sec - m_sceneStartTime.tv_sec + m_addedTimeSpan + m_serialScenesCostTime) * 1000 + (now.tv_usec - m_sceneStartTime.tv_usec) / 1000;
	}

	return (m_SceneFinishTime.tv_sec - m_sceneStartTime.tv_sec + m_addedTimeSpan + m_serialScenesCostTime) * 1000 + (m_SceneFinishTime.tv_usec - m_sceneStartTime.tv_usec) / 1000;
}



int Scene::GetTimeSpan()
{
	return (int)(m_SceneFinishTime.tv_sec - m_sceneStartTime.tv_sec + m_addedTimeSpan + m_serialScenesCostTime);
}

int Scene::GetAddedTimeSpan()
{
	return m_addedTimeSpan;
}

void Scene::SetTimeSpan(int time)
{
	m_SceneFinishTime.tv_sec = m_sceneStartTime.tv_sec + time; 
}

void Scene::AddTimeSpan(int addedTime)
{
	m_addedTimeSpan += addedTime;
}

void Scene::InitSceneStartTime()
{
	gettimeofday(&m_sceneStartTime, NULL);
}

UINT64 Scene::GetFightGroupUId(Unit* unit)
{
	if(GetVsHandler())
	{
		return GetVsHandler()->GetFightGroupUId(unit->GetID());
	}
	return (UINT64)(-1);
}

UINT32 Scene::GetFightGroup(Unit* unit)
{
	UINT64 uid = GetFightGroupUId(unit);
	if(GetGWBHandler() && (-1) == uid)//两个擂台战特殊处理
	{
		return (UINT32)(KKSG::FightNeutral);
	}
	int max = FightGroupMgr::Instance()->GetGroupMax();
	if (KKSG::SCENE_MOBA == GetSceneType())
	{
		return uid;
	}
	return m_grouphelper.GetGroup(max, unit->GetFightGroup(), uid);
}

void Scene::OnSetRank(UINT64 roleid, UINT32 rolelevel, UINT32 star, bool isrobot)
{
	if (m_teamSelectReward && IsCanDrawBox())
	{
		m_teamSelectReward->OnSetRank(roleid, rolelevel, star, isrobot);
	}
}

//RoleBoxRewardInfo Scene::FillBoxRobot(UINT64 robotid, UINT32 star)
//{
//	RoleBoxRewardInfo boxinfo;
//	if (m_teamSelectReward && IsCanDrawBox())
//	{
//		boxinfo = TeamSelectReward::FillBox(m_pSceneInfo, star, 0, 0);
//		m_teamSelectReward->FillRobotBox(robotid, boxinfo);
//	}
//	return boxinfo;
//}
//
bool Scene::CanCastSkill()
{
	return m_pSceneInfo->m_pConf->ShowSkill;
}

bool Scene::CanCastNormalSkill()
{
	return m_pSceneInfo->m_pConf->ShowNormalAttack;
}

void Scene::CheckKickRole()
{
	// 玩法结束
	if (m_dontDestory)
	{
		return ;
	}
	if (!m_pSceneInfo->m_pConf->CanReconnect)
	{
		return;
	}
	if (IsSceneNoRole())
	{
		return ;
	}

	//  场景结束
	UINT32 now = TimeUtil::GetTime();
	bool kick = false;
	if (m_SceneFinishTime.tv_sec != 0 && m_finishState == KKSG::SCENE_FINISH_END)
	{
		UINT32 EndTimeOut = m_pSceneInfo->m_pConf->EndTimeOut;
		if (EndTimeOut == 0) EndTimeOut = 120;

		if ((now > (EndTimeOut + m_finishStateExpireTime)))
		{
			kick = true;
		}
	}
	else if (IsAllRoleDisconnect() && now > GetAllDisconnectTime() + GetGlobalConfig().AllDisconnectKickTime)
	{
		kick = true;
	}

	if (kick && now > m_lastKickRoleTime + 10)
	{
		LogInfo("Kick all role out, tid: %u sid: %u", GetSceneTemplateID(), GetSceneID());
		KickAllRoleOut();
		m_lastKickRoleTime = now;
	}
}

std::vector<Unit*> Scene::GetAllDeadSameGroupRoles(UINT32 fightgroup)
{
	std::vector<Unit*> retroles;
	std::list<Role*> roles = GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* temprole = *i;
		if (temprole->IsDead() && temprole->GetFightGroup() == fightgroup)
		{
			retroles.push_back(temprole);	
		}
	}
	return retroles;
}

const Sequence<UINT32, 2>* Scene::GetPeerItem()
{
	return &m_pSceneInfo->m_pConf->PeerBox;
}

void Scene::OnSkillRevive(Unit* firer, Unit* target)
{
	if (NULL == firer || NULL == target)
	{
		return;
	}
	if (!firer->IsRole() || !target->IsRole())
	{
		return;
	}
	RawNotice rawtarget(ReviveSelf);
	rawtarget.Replace("$R", ((Role*)firer)->GetName());
	rawtarget.Send((Role*)target);

	RawNotice rawteam(ReviveOther);
	rawteam.Replace("$R1", ((Role*)target)->GetName());
	rawteam.Replace("$R2", ((Role*)firer)->GetName());

	const std::vector<Unit*>* allys = GetAllys(target->GetFightGroup());
	if (NULL != allys)
	{
		for (auto i = allys->begin(); i != allys->end(); ++i)
		{
			Unit* unit = *i;
			if (unit->IsRole() && unit != target)
			{
				rawteam.Send((Role*)unit);
			}
		}
	}
}

float Scene::GetPvpProfessionBias(Unit* pCaster, Unit* pReceiver)
{
	if (m_hasPvpBias)
		return XSkillEffectMgr::Instance()->GetPvpBias(pCaster, pReceiver);
	return 1.0f;
}

bool Scene::IsSelectBoxExcept(UINT64 roleid, UINT32 rolelevel)
{
	if (GetSceneType() != KKSG::SCENE_DRAGON)
	{
		return false;
	}
	SceneTeam* pteam = GetSceneTeam();
	if (!pteam)
	{
		return false;
	}

	auto data = DragonConfig::Instance()->GetDragonTableData(pteam->GetExpID());
	if (!data)
	{
		return false;
	}

	if (data->DragonNestDifficulty == 0 && data->MaxDragonDropLevel > 0 && rolelevel > data->MaxDragonDropLevel)
	{
		SSInfo<<"sceneid:"<<GetSceneTemplateID()<<" role:"<<roleid<<" rolelevel:"<<rolelevel<<" box except"<<END;
		return true;
	}

	return false;
}
