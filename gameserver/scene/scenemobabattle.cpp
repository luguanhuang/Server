#include "pch.h"
#include "scenemobabattle.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "gamelogic/herobattlemgr.h"
#include "herobattle/ptcg2c_herobattlecanusehero.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "herobattle/ptcg2c_closechooseherontf.h"
#include "gamelogic/herobattlerecord.h"
#include "unit/enemy.h"
#include "pb/enum.pb.h"
#include "gamelogic/mobabattlemgr.h"
#include "cmath"
#include "globalconfig.h"
#include "mobabattle/ptcg2c_herokillnotify.h"
#include "mobabattle/ptcg2c_mobabattleteamrolentf.h"
#include "mobabattle/ptcg2c_mobarolechangentf.h"
#include "mobabattle/ptcg2c_mobabattleteammsgntf.h"
#include "unit/attributewatcher.h"
#include "component/XAIComponent.h"
#include "gamelogic/XEnmityList.h"
#include "aimsgchannel.h"
#include "mobabattle/ptcg2m_addmobabattlerecord.h"
#include "mobabattle/ptcg2c_mobaminimapntf.h"
#include "idutil.h"
#include "mslink.h"
#include "servermgr/servermgr.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "utility/tlogger.h"
#include "gamelogic/skilllevel.h"
#include "config/itemdropconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/goalaward.h"

const UINT32 NoBodyDestroyTime = 60 * 5;

const UINT32 OneFrameTime = 100;

SceneMobaBattle::SceneMobaBattle(Scene* scene)
:SceneVsBase(scene)
{
	m_scene = scene;
	m_lastUpdateTime = 0;
	m_lastAddExpTime = 0;
	m_lastUpdateMiniMapTime = 0;
	m_startGameTime = 0;
	m_winTeam = GetTeam1();
	m_mvpid = 0;
	m_loseMvpid = 0;
	m_isSpecialEnd = false;
	m_noOneTime = time(NULL);
}

SceneMobaBattle::~SceneMobaBattle()
{
}

bool SceneMobaBattle::Init(const KKSG::CreateBattleParam& roParam)
{
	for (int i = 0; i < roParam.mobabattle().roleinfo_size(); ++i)
	{
		const KKSG::PvpRoleInfo& tmpdata = roParam.mobabattle().roleinfo(i);
		if (m_roleData.find(tmpdata.roleid()) != m_roleData.end())
		{
			LogError("same role [%llu] enter MobaBattle scene", tmpdata.roleid());
			continue;
		}
		KKSG::PvpRoleInfo data = tmpdata;
		if (data.camp() == 1)
		{
			data.set_camp(GetTeam1());
		}
		else
		{
			data.set_camp(GetTeam2());
		}
		m_roleData.insert(std::make_pair(data.roleid(), MobaRole(data, this)));
	}

	m_team2info[GetTeam1()].eloPoint = roParam.mobabattle().elopoint1();
	m_team2info[GetTeam2()].eloPoint = roParam.mobabattle().elopoint2();

	LogInfo("MobaBattle game start, elopoint %.0lf:%.0lf", roParam.mobabattle().elopoint1(), roParam.mobabattle().elopoint2());

	InitState();

	return true;
}

void SceneMobaBattle::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

void SceneMobaBattle::OnEnterBeforeNtf(Role* role)
{
	///> 隐身
	role->MakeInvisible(true, false);
}

UINT32 SceneMobaBattle::GetGroup(UINT64 roleid)
{
	auto iter = m_roleData.find(roleid);
	if (iter != m_roleData.end())
	{
		return iter->second.GetTeamID();
	}
	return GetTeam1();
}

bool SceneMobaBattle::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if (pRole == NULL)
	{
		return false;
	}
	if (pRole->IsWatcher())
	{
		if (event.type == VS_EVENT_ROLE_LOADED)
		{
			_SyncAllDataToClient(pRole);
		}
		return true;
	}

	auto iter = m_roleData.find(event.roleid);
	if (iter == m_roleData.end())
	{
		LogError("Can't find role [%llu] in MobaBattle", event.roleid);
		return false;
	}

	MobaRole& data = iter->second;

	switch (event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			m_loadmgr.AddRole(event.roleid);

			data.OnEnterGame(pRole);	

			m_noOneTime = 0;	///> 场景内有人

			_SyncAllDataToClient(pRole);

			LogDebug("role [%llu] is loaded in MobaBattle", event.roleid);
			break;
		}
	case VS_EVENT_ROLE_QUIT:
		{
			m_loadmgr.DelRole(event.roleid);
			
			if (m_loadmgr.GetLoadedRoleCount() == 0)
			{
				m_noOneTime = time(NULL);			///> 场景内无人
			}

			data.OnLeaveGame(true);

			break;
		}
	case VS_EVENT_ROLE_DIE:
		{
			_OnRoleDie(pRole, data);
			_SetReviveTime(data);

			break;
		}
	case VS_EVENT_ROLE_REVIVE:
		{
			data.CheckRevive(0, true);
			break;
		}
	default:
		{
			break;
		}
	}

	return true;
}

void SceneMobaBattle::OnNonRoleEnterScene(Unit* pUnit)
{
	if (pUnit->IsEnemy())
	{
		Enemy* pEnemy = static_cast<Enemy*>(pUnit);
		if (pEnemy->GetFightGroup() == GetTeam1() || pEnemy->GetFightGroup() == GetTeam2())
		{
			UINT32 tag = pEnemy->GetTag() & Moba_Tag_All;
			UINT32 num = 1;
			while (tag)
			{
				if (tag & 1)
				{
					m_grouptag2Enemy[std::make_pair(pEnemy->GetFightGroup(), num)].insert(pEnemy->GetID());
				}
				tag >>= 1;
				num <<= 1;
			}
			m_belongTeamEnemy.insert(pEnemy->GetID());

			if (m_grouptag2Enemy.size() >= 200)
			{
				LogError("MobaBattle, GroupTag2Enemy size [%u] >= 200, why?!", m_grouptag2Enemy.size());
			}
			if (m_belongTeamEnemy.size() >= 100)
			{
				LogError("MobaBattle, Group2Enemy size [%u] >= 100, why?!", m_belongTeamEnemy.size());
			}
		}

		const EnemyTemplate* pTemplate = pEnemy->GetTemplate();	
		const MobaMiniMapConf* conf = MobaBattleMgr::Instance()->GetMobaMiniMapConf(pTemplate->PosIndex);
		if (conf != NULL)
		{
			MobaTeamInfo& team1 = m_team2info[GetTeam1()];	
			if (team1.canSeeInMiniMap.find(pTemplate->PosIndex) == team1.canSeeInMiniMap.end())
			{
				team1.canSeeInMiniMap.insert(pTemplate->PosIndex);
				_SendMobaMiniMapToClient(GetTeam1());
			}
			MobaTeamInfo& team2 = m_team2info[GetTeam2()];
			if (team2.canSeeInMiniMap.find(pTemplate->PosIndex) == team2.canSeeInMiniMap.end())
			{
				team2.canSeeInMiniMap.insert(pTemplate->PosIndex);
				_SendMobaMiniMapToClient(GetTeam2());
			}
			if (m_posIndex2KillNum[pTemplate->PosIndex] >= conf->Number)
			{
				m_posIndex2KillNum[pTemplate->PosIndex] = 0;
			}
		}
	}
}

void SceneMobaBattle::Update()
{
	m_statemgr.Update();

	if (m_noOneTime != 0 && m_noOneTime + NoBodyDestroyTime <= time(NULL))
	{
		SetState(VS_STATE_END);
		m_isSpecialEnd = true;
		LogWarn("MobaBattle, game end when nobody in more than %d seconds", NoBodyDestroyTime);
		return;
	}

	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	UINT64 nowTime = TimeUtil::GetMilliSecond();
	if (m_lastUpdateTime + OneFrameTime > nowTime)
	{
		return;
	}
	m_lastUpdateTime = nowTime;

	///> begin ntf
	int span = time(NULL) - m_startGameTime;
	if (10 == span && !m_hinttag.hint10)
	{
		m_hinttag.hint10 = true;
		MobaHintNotify hint(m_scene, 0, MobaHint_Begin10);
		hint.Notify();
	}
	else if (20 == span && !m_hinttag.hint20)
	{
		m_hinttag.hint20 = true;
		MobaHintNotify hint(m_scene, 0, MobaHint_Begin20);
		hint.Notify();
	}

	_CheckRoleRevive(nowTime);
	_CheckChooseHero(nowTime);

	if (m_lastAddExpTime == 0)
	{
		m_lastAddExpTime = nowTime;
	}
	else if (m_lastAddExpTime + 3000 <= nowTime)
	{
		m_lastAddExpTime = nowTime;
		for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
		{
			if (i->second.GetHeroID() != 0)
			{
				i->second.AddExp(GetGlobalConfig().MobaExpPer3Seconds);
			}
		}
		_SendAllMobaRoleChangeToClient();
	}

	///> 更新小地图,从有标识到无标识
	if (m_lastUpdateMiniMapTime + 500 <= nowTime)
	{
		m_lastUpdateMiniMapTime = nowTime;
		_UpdateMiniMap();
	}
}

VsStateType SceneMobaBattle::IsAllLoad()
{
	///> 只要有一个人进来，就进入infight状态
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (m_loadmgr.HaveRole(i->first))
		{
			return VS_STATE_IN_FIGHT;
		}
	}
	return VS_STATE_DUMMY;
}

VsStateType SceneMobaBattle::OnLoadTimeOut()
{
	return VS_STATE_IN_FIGHT;
}

void SceneMobaBattle::OnEnterFight()
{
	m_startGameTime = time(NULL);
}

void SceneMobaBattle::OnStateEnd()
{
	LogDebug("MobaBattle game end, GG");

	///> 结算
	if (!m_isSpecialEnd)
	{
		_MakeEndGameData();
	}

	///> 因为state end后是不会有离开场景的事件触发的，所以需要调一下OnLeaveGame，关键是要把role指针设置为null
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		i->second.OnLeaveGame(false);
	}

	GetScene()->SetDontDestory(false);
	SetEnd();
}

void SceneMobaBattle::OnRoleReconnect(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("role [%llu] reconnected in MobaBattle", pRole->GetID());

	_SyncAllDataToClient(pRole);
	pRole->Get<RoleSkillSet>()->NotifyTransLevel();
}

void SceneMobaBattle::OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	if (pTargetUnit->IsEnemy())
	{
		if (((Enemy*)pTargetUnit)->GetTag() & Moba_Tag_Home)
		{
			time_t nowtime = time(NULL);
			if (nowtime >= m_team2info[pTargetUnit->GetFightGroup()].lasthomebeattacked + 60)
			{
				m_team2info[pTargetUnit->GetFightGroup()].lasthomebeattacked = nowtime;
				MobaHintNotify hint(m_scene, pTargetUnit->GetFightGroup(), MobaHint_SelfBeAttack);
				hint.Notify(false);
			}
		}
	}
	_CheckTowerChangeTarget(pTargetUnit, pCastUnit);
}

void SceneMobaBattle::OnEnemyDead(Enemy* pEnemy)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	if (pEnemy == NULL) return;
	_OnEnemyDie(pEnemy);
}

KKSG::ErrorCode SceneMobaBattle::SetHero(UINT64 roleid, UINT32 heroid)
{	
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_CANTCHOOSEHERO;
	}

	if (!m_loadmgr.HaveRole(roleid))
	{
		LogError("role [%llu] not in this scene, strange error", roleid);
		return KKSG::ERR_UNKNOWN;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		LogError("role [%llu] is in m_loadmgr but not in roleData, what happen??", roleid);
		return KKSG::ERR_UNKNOWN;
	}
	MobaRole& data = iter->second;

	bool alreadyChosen = false;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->first == roleid)
		{
			continue;
		}
		if (i->second.GetTeamID() == data.GetTeamID() && i->second.GetHeroID() == heroid)
		{
			alreadyChosen = true;
			break;
		}
	}
	if (alreadyChosen)
	{
		return KKSG::ERR_HERO_ALREADY_CHOSEN;
	}
	KKSG::ErrorCode code = data.SetHero(heroid);
	if (code == KKSG::ERR_SUCCESS)
	{
		_SendMobaRoleChangeToClient(data);
	}
	return code;
}

void SceneMobaBattle::AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	iter->second.AddExperienceHero(heroid, endTime);
}

KKSG::ErrorCode SceneMobaBattle::AddGroupLevel(UINT32 group, UINT32 addLevel)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_FAILED;
	}
	if (group == GetTeam1() || group == GetTeam2())
	{
		if (addLevel == 0)
		{
			return KKSG::ERR_FAILED;
		}
		MobaTeamInfo& info = m_team2info[group];
		UINT32 mobaMobMaxLevel = MobaBattleMgr::Instance()->GetMobaMobMaxLevel();
		if (info.groupLevel >= mobaMobMaxLevel)
		{
			return KKSG::ERR_MOBA_MOBMAXLEVEL;
		}
		info.groupLevel += addLevel;
		if (info.groupLevel > mobaMobMaxLevel)
		{
			info.groupLevel = mobaMobMaxLevel;
		}
		if (info.groupLevel == 20)
		{
			MobaHintNotify hint(m_scene, group, MobaHint_SelfLevel20);
			hint.Notify();
		}
		else if (info.groupLevel == 50)
		{
			MobaHintNotify hint(m_scene, group, MobaHint_SelfLevel50);
			hint.Notify();
		}

		_SendMobaTeamMsg();

		return KKSG::ERR_SUCCESS;
	}

	return KKSG::ERR_FAILED;
}

UINT32 SceneMobaBattle::GetGroupLevel(UINT32 group)
{
	auto i = m_team2info.find(group);
	if (i == m_team2info.end())
	{
		return 0;
	}
	else
	{
		return i->second.groupLevel;
	}
}

MobaRole* SceneMobaBattle::GetMobaRole(UINT64 roleid)
{
	auto i = m_roleData.find(roleid);
	if (i == m_roleData.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

MobaTeamInfo* SceneMobaBattle::GetMobaTeamInfo(UINT32 teamid)
{
	if (teamid == GetTeam1() || teamid == GetTeam2())
	{
		return &m_team2info[teamid];
	}
	return NULL;
}

void SceneMobaBattle::GetEnemy(UINT32 group, UINT32 tag, MobaEnemySet& enemySet)
{
	UINT32 num = 1;
	while (tag)
	{
		if (tag & 1)
		{
			auto iter = m_grouptag2Enemy.find(std::make_pair(group, num));
			if (iter != m_grouptag2Enemy.end())
			{
				enemySet.AddSet(iter->second);
			}
		}
		tag >>= 1;
		num <<= 1;
	}
}

KKSG::ErrorCode SceneMobaBattle::Upgrade(UINT64 roleid, UINT32 param, UINT32& nowParam)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_FAILED;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		LogError("role [%llu] not in MobaBattle, but run to this, terrible code error", roleid);
		return KKSG::ERR_UNKNOWN;
	}
	return iter->second.Upgrade(param, nowParam);
}

void SceneMobaBattle::FillResult(KKSG::MobaBattleResult* result)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return;
	}
	if (result == NULL)
	{
		return;
	}
	if (m_isSpecialEnd)
	{
		return;
	}
	result->set_mvpid(m_mvpid);
	result->set_losemvpid(m_loseMvpid);
	result->set_damagemaxid(m_damageMaxid);
	result->set_behitdamagemaxid(m_beHitDamageMaxid);
}

void SceneMobaBattle::FillRoleResultAndReward(KKSG::MobaBattleRoleResult* result, UINT64 roleid)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return;
	}
	if (result == NULL)
	{
		return;
	}
	if (m_isSpecialEnd)
	{
		return;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	result->set_iswin(iter->second.GetTeamID() == m_winTeam);
	result->set_heroid(iter->second.GetHeroID());
	result->set_kda(iter->second.GetKDA());
	result->set_isescape(iter->second.GetIsEscape());

	Role* pRole = GetRole(roleid);
	if (pRole != NULL && result->iswin())
	{
		BagGiveItemTransition give(pRole);
		give.SetReason(ItemFlow_Mobabattle, ItemFlow_MobaBattle_Reward);
		for (UINT32 i = 0; i < GetGlobalConfig().MobaBattleReward.size(); ++i)
		{
			UINT32 dropid = GetGlobalConfig().MobaBattleReward[i];
			const ItemDesc* desc = ItemDropConfig::Instance()->GetRandomDrop(dropid);
			if (desc == NULL)
			{
				continue;
			}
			KKSG::ItemBrief* brief = result->add_winreward();
			desc->ToKKSG(brief);
			give.GiveItem(*desc);
		}
		give.NotifyClient();
	}
}

bool SceneMobaBattle::IsWinRole(UINT64 roleid)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return false;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return false;
	}
	return m_winTeam == iter->second.GetTeamID();
}

KKSG::ErrorCode SceneMobaBattle::SendSignal(Role* pRole, UINT32 type)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	auto iter = m_roleData.find(pRole->GetID());
	if (iter == m_roleData.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	return iter->second.SendSignal(type);	
}

void SceneMobaBattle::GetSameTeamRole(UINT64 roleid, std::vector<Role*>& sameGroup)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->second.GetTeamID() == iter->second.GetTeamID())
		{
			Role* pRole = i->second.GetRole();
			if (pRole != NULL)
			{
				sameGroup.push_back(pRole);
			}
		}
	}
}

void SceneMobaBattle::FillChatSource(KKSG::ChatSource* source)
{
	if (source == NULL) return;
	auto iter = m_roleData.find(source->roleid());
	if (iter == m_roleData.end())
	{
		return;
	}
	source->set_heroid(iter->second.GetHeroID());
}

void SceneMobaBattle::SendToTeamRole(UINT32 teamid, const CProtocol& oPtc)
{
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->second.GetTeamID() != teamid)
		{
			continue;
		}
		Role* role = i->second.GetRole();
		if (role != NULL)
		{
			role->Send(oPtc);
		}
	}
}

void SceneMobaBattle::GMEnd(UINT64 roleid)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	m_winTeam = iter->second.GetTeamID();
	SetState(VS_STATE_END);
}

void SceneMobaBattle::GMAddExp(UINT64 roleid, UINT32 addExp)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	iter->second.AddExp(addExp);
	_SendMobaRoleChangeToClient(iter->second);
}

KKSG::ErrorCode SceneMobaBattle::CheckReportPlayer(UINT64 roleid, UINT64 badRoleid, int& num)
{
	if (GetCurrentStateType() != VS_STATE_END)
	{
		return KKSG::ERR_FAILED;
	}
	num = 1;
	MobaRole* pData1 = GetMobaRole(roleid);
	if (pData1 == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	MobaRole* pData2 = GetMobaRole(badRoleid);
	if (pData2 == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pData2->IsAlreadyReport(roleid))
	{
		return KKSG::ERR_ALREADY_REPORTED;
	}
	pData2->AddReporter(roleid);

	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_HeroReportPlayerBack);
	if (conf)
	{
		std::string mailcontent = MailConfig::Instance()->ReplaceString("%s", pData2->GetRoleName(), conf->m_content);
		MailSender sender;
		sender.SetContent(Mail_System, conf->m_title, mailcontent);
		sender.Send(roleid);
	}

	// 1. 对评分大于等于2.0的玩家举报无效
	if (pData2->GetKDA() >= GetGlobalConfig().ReportHeroBattleInvalidKda)
	{
		num = 0;
		return KKSG::ERR_SUCCESS;
	}
	// 2.失败方举报胜利方无效
	if (pData1->GetTeamID() != m_winTeam && pData2->GetTeamID() == m_winTeam)
	{
		num = 0;
		return KKSG::ERR_SUCCESS;
	}
	// 3.失败方玩家举报举报己方评分最低的玩家举报数+1
	if (pData1->GetTeamID() != m_winTeam && pData2->GetTeamID() != m_winTeam)
	{
		float kda = 0.0f;
		UINT64 leastKdaRoleid = 0;
		for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
		{
			if (i->second.GetTeamID() != pData1->GetTeamID())
			{
				continue;
			}
			if (leastKdaRoleid == 0 || i->second.GetKDA() < kda)
			{
				leastKdaRoleid = i->first;
				kda = i->second.GetKDA();
			}
		}
		if (badRoleid == leastKdaRoleid)
		{
			num += GetGlobalConfig().ReportHeroBattleLeastNum;
		}
	}
	// 4.玩家被举报，己方击杀数大于等于2，且自己击杀和助攻都为0的玩家，举报数+2（视为挂机或者消极比赛）
	CombatStatistics& stat = pData2->GetCombatStatistics();
	if (stat.GetKillCount() == 0 && stat.GetAssistNum() == 0)
	{
		if (m_team2info[pData2->GetTeamID()].headCount >= GetGlobalConfig().ReportHeroCampTotalKillNum)
		{
			num += GetGlobalConfig().ReportHeroCampTotalAddNum;
		}
	}

	return KKSG::ERR_SUCCESS;
}

void SceneMobaBattle::_UpdateMiniMap()
{
	///> 更新小地图上本该消失的标识
	MobaTeamInfo& team1 = m_team2info[GetTeam1()];
	MobaTeamInfo& team2 = m_team2info[GetTeam2()];

	std::vector<Vector3> aliveTeam1Pos;
	std::vector<Vector3> aliveTeam2Pos;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		Role* pRole = i->second.GetRole();
		if (pRole != NULL && !pRole->IsDead())
		{
			if (i->second.GetTeamID() == GetTeam1()) aliveTeam1Pos.push_back(pRole->GetXObject()->GetPosition_p());
			if (i->second.GetTeamID() == GetTeam2()) aliveTeam2Pos.push_back(pRole->GetXObject()->GetPosition_p());
		}
	}
	for (auto i = m_belongTeamEnemy.begin(); i != m_belongTeamEnemy.end(); ++i)
	{
		Unit* pUnit = GetUnit(*i);
		if (pUnit != NULL && pUnit->IsEnemy() && !pUnit->IsDead())
		{
			if (pUnit->GetFightGroup() == GetTeam1()) aliveTeam1Pos.push_back(pUnit->GetXObject()->GetPosition_p());
			if (pUnit->GetFightGroup() == GetTeam2()) aliveTeam2Pos.push_back(pUnit->GetXObject()->GetPosition_p());
		}
	}

	for (auto i = m_posIndex2KillNum.begin(); i != m_posIndex2KillNum.end(); ++i)
	{
		const MobaMiniMapConf* conf = MobaBattleMgr::Instance()->GetMobaMiniMapConf(i->first);
		if (conf == NULL)
		{
			LogWarn("PosIndex [%u] not in MobaMiniMap.txt, reload the table??", i->first);
			continue;
		}
		if (i->second < conf->Number)
		{
			continue;
		}
		bool isNeed = team1.canSeeInMiniMap.find(i->first) != team1.canSeeInMiniMap.end();
		for (auto j = aliveTeam1Pos.begin(); j != aliveTeam1Pos.end() && isNeed; ++j)
		{
			float dis = Vector3::Distance(*j, Vector3(conf->Position[0], 0, conf->Position[1]));
			if (dis <= (float)GetGlobalConfig().ViewGroupLen)
			{
				team1.canSeeInMiniMap.erase(i->first);
				_SendMobaMiniMapToClient(GetTeam1());
				break;
			}
		}
		isNeed = team2.canSeeInMiniMap.find(i->first) != team2.canSeeInMiniMap.end();
		for (auto j = aliveTeam2Pos.begin(); j != aliveTeam2Pos.end() && isNeed; ++j)
		{
			float dis = Vector3::Distance(*j, Vector3(conf->Position[0], 0, conf->Position[1]));
			if (dis <= (float)GetGlobalConfig().ViewGroupLen)
			{
				team2.canSeeInMiniMap.erase(i->first);
				_SendMobaMiniMapToClient(GetTeam2());
				break;
			}
		}
	}
}

void SceneMobaBattle::_OnRoleDie(Role* pRole, MobaRole& deadData)
{
	UINT64 killID = pRole->GetKillerID();
	if (m_roleData.find(killID) == m_roleData.end())
	{
		UINT64 newKillID = pRole->Statistics().GetLastAssist();
		if (m_roleData.find(newKillID) == m_roleData.end())
		{
			///> 野怪杀的
			KKSG::EntityCategory unitType = Unit::GetUnitType(killID);
			if (unitType == KKSG::Category_Enemy)
			{
				Unit* killUnit = GetUnit(killID);
				if (killUnit != NULL)
				{
					_SendEnemyKillRoleNtf((Enemy*)killUnit, deadData);
				}
				else
				{
					LogWarn("Wa!!! enemy kill role after it dead!!, roleid [%llu]", pRole->GetID());
				}
			}
			else
			{
				LogError("What!!! uid [%llu], UnitType [%d], not Enemy AND not in this Game Player BUT kill role [%llu] ?!!!", 
							killID, unitType, pRole->GetID());
			}

			return;
		}
		CombatStatistics& statistics = m_roleData[newKillID].GetCombatStatistics();
		statistics.AddKillCount();
		statistics.DelAssistNum();
		killID = newKillID;
	}
	else
	{
		MobaRole& killerData = m_roleData[killID];
		if (killerData.GetRole() == NULL)
		{
			CombatStatistics& statistics = killerData.GetCombatStatistics();
			statistics.AddKillCount();	///> kill的时候role已经离开了
		}
	}
	
	const Vector3& vec3 = pRole->GetXObject()->GetPosition_p();
	UINT32 posxz = (int)(vec3.z * 100 + 0.5f);
	posxz |= ((int)(vec3.x * 100 + 0.5f) << 16);

	MobaRole& killerData = m_roleData[killID];
	m_team2info[killerData.GetTeamID()].headCount++;

	_SendRoleKillRoleNtf(killerData, deadData);

	CombatStatistics& statistics = deadData.GetCombatStatistics();

	std::vector<MobaRole*> assistRole;
	std::vector<UINT64>& assistRoleIDs = statistics.GetAssistRoleIDs();
	for (auto i = assistRoleIDs.begin(); i != assistRoleIDs.end(); ++i)
	{
		auto iter = m_roleData.find(*i);
		if (iter != m_roleData.end())
		{
			assistRole.push_back(&iter->second);
		}
	}
	std::vector<MobaRole*> rangeRole;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->second.GetTeamID() == deadData.GetTeamID()
			|| i->first == deadData.GetRoleID()
			|| i->first == killID)
		{
			continue;
		}
		if (std::find(assistRoleIDs.begin(), assistRoleIDs.end(), i->first) != assistRoleIDs.end())
		{
			continue;
		}
		Role* role = i->second.GetRole();
		if (role != NULL && !role->IsDead())
		{
			float dis = Vector3::Distance(pRole->GetXObject()->GetPosition_p(), role->GetXObject()->GetPosition_p());
			if (dis <= GetGlobalConfig().MobaDeadExpRange)
			{
				rangeRole.push_back(&i->second);
			}
		}
	}
	///> 加经验
	const MobaKillExpConf* expConf = MobaBattleMgr::Instance()->GetMobaKillExpConf(assistRole.size(), rangeRole.size());
	if (expConf == NULL)
	{
		LogError("AssistNum [%u], RangeNum [%u], MobaKillExpConf is NULL!!!!", assistRole.size(), rangeRole.size());
		return;
	}
	else
	{
		double expDiff = _GetExpDiff(killerData.GetTeamID());
		killerData.AddExp(_GetAddExp(deadData.GetLevel(), deadData.GetExp(), expDiff, expConf->KillerExp), posxz);
		for (auto i = assistRole.begin(); i != assistRole.end(); ++i)
		{
			(*i)->AddExp(_GetAddExp(deadData.GetLevel(), deadData.GetExp(), expDiff, expConf->AssistExp), posxz);
		}
		for (auto i = rangeRole.begin(); i != rangeRole.end(); ++i)
		{
			(*i)->AddExp(_GetAddExp(deadData.GetLevel(), deadData.GetExp(), expDiff, expConf->RangeExp), posxz);
		}
	}

	_SendAllMobaRoleChangeToClient(MobaRole_KillStats);
	_SendMobaTeamMsg();
}

void SceneMobaBattle::_SetReviveTime(MobaRole& deadData)
{
	const MobaLevelConf* levelConf = MobaBattleMgr::Instance()->GetMobaLevelConf(deadData.GetLevel());
	if (levelConf == NULL)
	{
		LogError("level [%u], MobaLevelConf is NULL!!!!!", deadData.GetLevel());
		return;
	}

	deadData.SetNextReviveTime(TimeUtil::GetMilliSecond() + levelConf->ReviveTime * 1000);
	_SendMobaRoleChangeToClient(deadData);
}

void SceneMobaBattle::_OnEnemyDie(Enemy* pEnemy)
{
	UINT64 killID = pEnemy->GetKillerID();
	UINT32 killerteam = 0; ///> 0 or team1 or team2, 0是野怪或者是某个怪攻击后死了再杀的
	if (m_roleData.find(killID) == m_roleData.end())
	{
		///> 怪杀的
		KKSG::EntityCategory unitType = Unit::GetUnitType(killID);
		if (unitType == KKSG::Category_Enemy)
		{
			Unit* killUnit = GetUnit(killID);
			if (killUnit != NULL)
			{
				_SendEnemyKillEnemyNtf((Enemy*)killUnit, pEnemy);
				killerteam = killUnit->GetFightGroup();
				if (killerteam != GetTeam1() && killerteam != GetTeam2()) killerteam = 0;
			}
			else
			{
				LogWarn("Wa!!! enemy kill enemy after it dead!!, Enemy templateID [%u]", pEnemy->GetTemplateID());
			}
		}
		else
		{
			LogError("What!!! killID [%llu], UnitType [%d], not Enemy AND not in this Game Player BUT kill Enemy templateID [%u]?!!!", 
						killID, unitType, pEnemy->GetTemplateID());
		}
	}
	else
	{
		///> 人杀的
		MobaRole& killerData = m_roleData[killID];
		killerteam = killerData.GetTeamID();

		_SendRoleKillEnemyNtf(killerData, pEnemy);
		///> 狮蝎
		if (pEnemy->GetTemplateID() == GetGlobalConfig().MobaEnemyId)
		{
			MobaHintNotify hint(m_scene, killerData.GetTeamID(), MobaHint_SelfKill);
			hint.Notify();
		}
	}
	const Vector3& vec3 = pEnemy->GetXObject()->GetPosition_p();
	UINT32 posxz = (int)(vec3.z * 100 + 0.5f);
	posxz |= ((int)(vec3.x * 100 + 0.5f) << 16);
	///> 加经验
	const EnemyTemplate* pTemplate = pEnemy->GetTemplate();
	if (pTemplate->MobaExp > 0)
	{
		for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
		{
			///> 后面的判断是为了判断自杀or杀死自己的召唤物
			if (killID == i->first && i->second.GetTeamID() != pEnemy->GetFightGroup())
			{
				i->second.AddExp(pTemplate->MobaExp, posxz);
				continue;
			}
			Role* pRole = i->second.GetRole();
			if (pRole != NULL && !pRole->IsDead()
				&& (killerteam == 0 || i->second.GetTeamID() == killerteam)
				&& i->second.GetTeamID() != pEnemy->GetFightGroup())
			{
				float dis = Vector3::Distance(pEnemy->GetXObject()->GetPosition_p(), pRole->GetXObject()->GetPosition_p());
				if (dis <= pTemplate->MobaExpRange)
				{
					i->second.AddExp(pTemplate->MobaExp, posxz);
				}
			}
		}
	}
	_SendAllMobaRoleChangeToClient();

	if (pEnemy->GetTag() & Moba_Tag_Tower)
	{
		MobaHintNotify hint(m_scene, pEnemy->GetFightGroup(), MobaHint_SelfDestoryed);
		hint.Notify();
	}

	if (pEnemy->GetTag() & Moba_Tag_Home)
	{
		if (pEnemy->GetFightGroup() == GetTeam1())
		{
			m_winTeam = GetTeam2();
			SetState(VS_STATE_END);
		}
		else if (pEnemy->GetFightGroup() == GetTeam2())
		{
			m_winTeam = GetTeam1();
			SetState(VS_STATE_END);
		}
	}

	_EraseEnemy(pEnemy);

	///> 小地图
	const MobaMiniMapConf* conf = MobaBattleMgr::Instance()->GetMobaMiniMapConf(pTemplate->PosIndex);
	if (conf != NULL)
	{
		UINT32& killNum = m_posIndex2KillNum[pTemplate->PosIndex];
		++killNum;
		if (killNum == conf->Number)
		{
			if (killerteam != 0)
			{
				if (m_team2info[killerteam].canSeeInMiniMap.erase(pTemplate->PosIndex) > 0)
				{
					_SendMobaMiniMapToClient(killerteam);
				}
			}
			///> 如果怪是本方的，需要从本方小地图上消失
			if (pEnemy->GetFightGroup() == GetTeam1() || pEnemy->GetFightGroup() == GetTeam2())
			{
				if (m_team2info[pEnemy->GetFightGroup()].canSeeInMiniMap.erase(pTemplate->PosIndex) > 0)
				{
					_SendMobaMiniMapToClient(pEnemy->GetFightGroup());
				}
			}
		}
	}
}

void SceneMobaBattle::_EraseEnemy(Enemy* pEnemy)
{
	if (pEnemy->GetFightGroup() != GetTeam1() && pEnemy->GetFightGroup() != GetTeam2())
	{
		return;
	}
	UINT32 tag = pEnemy->GetTag() & Moba_Tag_All;
	UINT32 num = 1;
	while (tag)
	{
		if (tag & 1)
		{
			auto iter = m_grouptag2Enemy.find(std::make_pair(pEnemy->GetFightGroup(), num));
			if (iter != m_grouptag2Enemy.end())
			{
				iter->second.erase(pEnemy->GetID());
			}
		}
		tag >>= 1;
		num <<= 1;
	}

	bool eraseSucceed = false;
	if (m_belongTeamEnemy.erase(pEnemy->GetID()) > 0)
	{
		eraseSucceed = true;
	}
	if (!eraseSucceed)
	{
		LogError("enemy [%llu], templateID [%u], can't find in belongTeam2Enemy map"
			, pEnemy->GetID(), pEnemy->GetTemplateID());
	}
}

void SceneMobaBattle::_SendRoleKillRoleNtf(MobaRole& killerData, MobaRole& deadData)
{
	PtcG2C_HeroKillNotify oPtc;
	KKSG::HeroKillUnit* unit = oPtc.m_Data.mutable_killer();
	unit->set_type(KKSG::HeroKillUnit_Hero);
	unit->set_id(killerData.GetHeroID());
	unit->set_teamid(killerData.GetTeamID());
	CombatStatistics& killerStat = killerData.GetCombatStatistics();
	unit->set_continuecounts(killerStat.GetKillContinueCount());
	
	unit = oPtc.m_Data.mutable_dead();
	unit->set_type(KKSG::HeroKillUnit_Hero);
	unit->set_id(deadData.GetHeroID());
	unit->set_teamid(deadData.GetTeamID());
	CombatStatistics& deadStat = deadData.GetCombatStatistics();
	unit->set_continuecounts(deadStat.GetKillContinueCount());
	
	std::vector<UINT64>& assistRoleIDs = deadStat.GetAssistRoleIDs();
	for (auto i = assistRoleIDs.begin(); i != assistRoleIDs.end(); ++i)
	{
		auto iter = m_roleData.find(*i);
		if (iter != m_roleData.end())
		{
			unit = oPtc.m_Data.add_assists();
			unit->set_type(KKSG::HeroKillUnit_Hero);
			unit->set_id(iter->second.GetHeroID());
			unit->set_teamid(iter->second.GetTeamID());
		}
	}
	
	oPtc.m_Data.set_multikill(killerStat.GetMultiKillCount());
	oPtc.m_Data.set_isfirstblood(m_team2info[GetTeam1()].headCount + m_team2info[GetTeam2()].headCount == 1);

	GetScene()->Broadcast(oPtc);
}

void SceneMobaBattle::_SendEnemyKillRoleNtf(Enemy* pKiller, MobaRole& deadData)
{
	PtcG2C_HeroKillNotify oPtc;
	KKSG::HeroKillUnit* unit = oPtc.m_Data.mutable_killer();
	unit->set_type(KKSG::HeroKillUnit_Enemy);
	unit->set_id(pKiller->GetTemplateID());
	unit->set_teamid(pKiller->GetFightGroup());

	unit = oPtc.m_Data.mutable_dead();
	unit->set_type(KKSG::HeroKillUnit_Hero);
	unit->set_id(deadData.GetHeroID());
	unit->set_teamid(deadData.GetTeamID());

	GetScene()->Broadcast(oPtc);
}

void SceneMobaBattle::_SendRoleKillEnemyNtf(MobaRole& killerData, Enemy* pDead)
{
	if (!pDead->GetTemplate()->MobaKillNeedHint)
	{
		return;
	}
	PtcG2C_HeroKillNotify oPtc;
	KKSG::HeroKillUnit* unit = oPtc.m_Data.mutable_killer();
	unit->set_type(KKSG::HeroKillUnit_Hero);
	unit->set_id(killerData.GetHeroID());
	unit->set_teamid(killerData.GetTeamID());

	unit = oPtc.m_Data.mutable_dead();
	unit->set_type(KKSG::HeroKillUnit_Enemy);
	unit->set_id(pDead->GetTemplateID());
	unit->set_teamid(pDead->GetFightGroup());

	GetScene()->Broadcast(oPtc);
}

void SceneMobaBattle::_SendEnemyKillEnemyNtf(Enemy* pKiller, Enemy* pDead)
{
	if (!pDead->GetTemplate()->MobaKillNeedHint)
	{
		return;
	}
	PtcG2C_HeroKillNotify oPtc;
	KKSG::HeroKillUnit* unit = oPtc.m_Data.mutable_killer();
	unit->set_type(KKSG::HeroKillUnit_Enemy);
	unit->set_id(pKiller->GetTemplateID());
	unit->set_teamid(pKiller->GetFightGroup());

	unit = oPtc.m_Data.mutable_dead();
	unit->set_type(KKSG::HeroKillUnit_Enemy);
	unit->set_id(pDead->GetTemplateID());
	unit->set_teamid(pDead->GetFightGroup());

	GetScene()->Broadcast(oPtc);
}

double SceneMobaBattle::_GetExpDiff(UINT32 teamid)
{
	double team1Exp = m_team2info[GetTeam1()].allExp;
	double team2Exp = m_team2info[GetTeam2()].allExp;
	if (teamid == GetTeam1())
	{
		return std::max(0.0, (team2Exp - team1Exp) / (team2Exp + team1Exp));
	}
	return std::max(0.0, (team1Exp - team2Exp) / (team1Exp + team2Exp));
}

double SceneMobaBattle::_GetAddExp(UINT32 level, double exp, double expDiff, const Sequence<double, 2>& param)
{
	return param[0] * level + param[1] * exp * expDiff;
}

void SceneMobaBattle::_CheckRoleRevive(UINT64 nowTime)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		i->second.CheckRevive(nowTime, false);
	}

	_SendAllMobaRoleChangeToClient();
}

void SceneMobaBattle::_CheckChooseHero(UINT64 nowTime)
{
	if (GetCurrentStateType() != VS_STATE_IN_FIGHT)
	{
		return;
	}

	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (!m_loadmgr.HaveRole(i->first))
		{
			continue;
		}
		///> 进入选英雄阶段，一段时间没选英雄，随机选择英雄
		if (i->second.GetCanChooseHeroID() && i->second.GetEnterGameTime() + GetGlobalConfig().MobaBattleChooseHeroTime * 1000 <= nowTime)
		{
			Role* pRole = i->second.GetRole();
			if (pRole == NULL)
			{
				LogError("role [%llu] is null", pRole->GetID());
				continue;
			}

			std::set<UINT32> exceptHero;
			for (auto j = m_roleData.begin(); j != m_roleData.end(); ++j)
			{
				if (j->second.GetTeamID() == i->second.GetTeamID() && j->second.GetHeroID() != 0)
				{
					exceptHero.insert(j->second.GetHeroID());
				}
			}
			KKSG::ErrorCode errorCode = SetHero(pRole->GetID(), pRole->Get<CHeroBattleRecord>()->GetRandomHero(i->second.GetFreeWeekHero(), i->second.GetExperienceHero(), exceptHero));
			if (errorCode != KKSG::ERR_SUCCESS)
			{
				LogError("role [%llu] set hero error [%d]", pRole->GetID(), errorCode);
				continue;
			}
			PtcG2C_CloseChooseHeroNtf oPtc;
			pRole->Send(oPtc);	
		}
	}
}

void SceneMobaBattle::_CheckTowerChangeTarget(Unit* pTarget, Unit* pCaster)
{
	///> 只考虑Role在塔下受Role击
	if (pTarget && pCaster && pTarget->IsRole() && pCaster->IsRole())
	{
		MobaEnemySet enemySet;
		GetEnemy(pTarget->GetFightGroup(), Moba_Tag_TowerLike, enemySet);

		const Vector3& casterPos = pCaster->GetXObject()->GetPosition_p();
		const Vector3& receiverPos = pTarget->GetXObject()->GetPosition_p();

		for (auto it = enemySet.begin(); it != enemySet.end(); ++it)
		{
			UINT64 towerUID = *it;
			Unit* pTowerUnit = Unit::FindUnit(towerUID);
			if (!pTowerUnit || pTowerUnit->IsDead())
				continue;

			XAIComponent* pTowerAI = pTowerUnit->AIComponent();
			if (pTowerAI == NULL)
				continue;

			///> 只有当前塔没有被嘲讽时，才可能转移目标
			if (pTowerAI->EnmityList().IsProvoked())
				continue;

			///> 塔的攻击范围
			float sqrRange = pTowerAI->EnterFightRange() * pTowerAI->EnterFightRange();
			const Vector3& towerPos = pTowerUnit->GetXObject()->GetPosition_p();

			float sqrDis = Vector3::sqrtMagnitude(towerPos - casterPos);
			if (sqrDis > sqrRange)
				continue;

			sqrDis = Vector3::sqrtMagnitude(towerPos - receiverPos);
			if (sqrDis > sqrRange)
				continue;

			///> 都在攻击范围，发受击事件
			XAIEventArgs args;
			args.MsgTargetTemplateId = 0;
			args.EventArg = "heroatkhero";
			args.Templateid = pTarget->GetTemplateID();
			args.Pos = receiverPos;
			args.FloatArg = pCaster->GetXObject()->GetFaceDegree();
			//args.SkillId = skillHurt->SkillId;
			//args.EnemyTemplateId = pCaster->GetTemplateID();
			args.SenderUID = pCaster->GetID();

			if (pTowerUnit->GetCurrScene())
				pTowerUnit->GetCurrScene()->MsgChannel().SendMsgTo(pTowerUnit, AISceneMsgChannel::Cmd, args);
		}
	}
}

void SceneMobaBattle::_SyncAllDataToClient(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	if (!pRole->IsWatcher()) _SendCanUseHeroToClient(pRole);
	_SendTeamRoleToClient(pRole);
	_SendMobaTeamMsg(pRole);
	auto iter = m_roleData.find(pRole->GetID());
	if (iter != m_roleData.end())
	{
		_SendMobaMiniMapToClient(iter->second.GetTeamID(), pRole);
	}
}

void SceneMobaBattle::_SendCanUseHeroToClient(Role* pRole)
{
	if (pRole == NULL || m_roleData.find(pRole->GetID()) == m_roleData.end())
	{
		return;
	}
	PtcG2C_HeroBattleCanUseHero oPtc;

	const std::unordered_set<UINT32>& hero = pRole->Get<CHeroBattleRecord>()->GetHero();
	for (auto i = hero.begin(); i != hero.end(); ++i)
	{
		oPtc.m_Data.add_havehero(*i);
	}

	MobaRole& data = m_roleData[pRole->GetID()];
	for (auto i = data.GetFreeWeekHero().begin(); i != data.GetFreeWeekHero().end(); ++i)
	{
		oPtc.m_Data.add_freehero(*i);
	}
	for (auto i = data.GetExperienceHero().begin(); i != data.GetExperienceHero().end(); ++i)
	{
		oPtc.m_Data.add_experiencehero(i->first);
	}

	oPtc.m_Data.set_leftchoosetime(GetGlobalConfig().MobaBattleChooseHeroTime);

	pRole->Send(oPtc);
}

void SceneMobaBattle::_SendTeamRoleToClient(Role* pRole)
{
	PtcG2C_MobaBattleTeamRoleNtf oPtc;
	oPtc.m_Data.set_team1(GetTeam1());
	oPtc.m_Data.set_team2(GetTeam2());
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		KKSG::MobaRoleData* roleData;
		if (i->second.GetTeamID() == GetTeam1())
		{
			roleData = oPtc.m_Data.add_datalist1();
		}
		else 
		{
			roleData = oPtc.m_Data.add_datalist2();
		}
		i->second.ToKKSG(*roleData, MobaRole_All);
	}

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneMobaBattle::_SendMobaTeamMsg(Role* pRole)
{
	PtcG2C_MobaBattleTeamMsgNtf oPtc;

	KKSG::MobaBattleTeamData* data1 = oPtc.m_Data.add_teamdata();
	data1->set_teamid(GetTeam1());
	MobaTeamInfo& info1 = m_team2info[GetTeam1()];
	data1->set_grouplevel(info1.groupLevel);
	data1->set_headcount(info1.headCount);

	KKSG::MobaBattleTeamData* data2 = oPtc.m_Data.add_teamdata();
	data2->set_teamid(GetTeam2());
	MobaTeamInfo& info2 = m_team2info[GetTeam2()];
	data2->set_grouplevel(info2.groupLevel);
	data2->set_headcount(info2.headCount);

	if (pRole == NULL) GetScene()->Broadcast(oPtc);
	else pRole->Send(oPtc);
}

void SceneMobaBattle::_SendMobaRoleChangeToClient(MobaRole& data, UINT32 field)
{
	PtcG2C_MobaRoleChangeNtf oPtc;
	KKSG::MobaRoleData* roleData = oPtc.m_Data.add_changerole();
	data.ToKKSG(*roleData, field);

	GetScene()->Broadcast(oPtc);
}

void SceneMobaBattle::_SendAllMobaRoleChangeToClient(UINT32 field)
{
	PtcG2C_MobaRoleChangeNtf oPtc;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		if (i->second.GetChangeFlag() > 0 || field > 0)
		{
			KKSG::MobaRoleData* roleData = oPtc.m_Data.add_changerole();
			i->second.ToKKSG(*roleData, field);
		}
	}
	if (oPtc.m_Data.changerole_size() > 0)
	{
		GetScene()->Broadcast(oPtc);
	}
}

void SceneMobaBattle::_SendMobaMiniMapToClient(UINT32 teamid, Role* pRole)
{
	if (teamid != GetTeam1() && teamid != GetTeam2())
	{
		return;
	}
	MobaTeamInfo& info = m_team2info[teamid];
	PtcG2C_MobaMiniMapNtf oPtc;
	for (auto i = info.canSeeInMiniMap.begin(); i != info.canSeeInMiniMap.end(); ++i)
	{
		oPtc.m_Data.add_canseeposindex(*i);
	}
	if (pRole != NULL)
	{
		pRole->Send(oPtc);
	}
	else
	{
		for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
		{
			if (i->second.GetTeamID() == teamid)
			{
				Role* role = i->second.GetRole();
				if (role != NULL)
				{
					role->Send(oPtc);
				}
			}
		}
	}
}

void SceneMobaBattle::_MakeEndGameData()
{
	PtcG2M_AddMobaBattleRecord oPtc;
	KKSG::MobaBattleOneGame& oneGame = *oPtc.m_Data.mutable_game();
	oPtc.m_Data.set_elopoint1(m_team2info[GetTeam1()].eloPoint);
	oPtc.m_Data.set_elopoint2(m_team2info[GetTeam2()].eloPoint);

	oneGame.set_date((UINT32)m_startGameTime);
	oneGame.set_timespan((UINT32)(time(NULL) - m_startGameTime));

	UINT64 mvpid[2] = {0, 0};
	float mvpKda[2] = {0.0f, 0.0f};
	double mvpDamage[2] = {0.0, 0.0};

	double damagemax = 0.0, behitdamagemax = 0.0;
	m_damageMaxid = m_beHitDamageMaxid = 0;

	UINT64 nowTime = TimeUtil::GetMilliSecond();
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		i->second.OnGameEnd(nowTime, oneGame.timespan());

		KKSG::MobaBattleOneGameRole* gamerole = NULL;
		if (i->second.GetTeamID() == GetTeam1())
		{
			gamerole = oneGame.add_team1();
		}
		else
		{
			gamerole = oneGame.add_team2();
		}

		CombatStatistics& stat = i->second.GetCombatStatistics();
		UINT32 tmpkill = stat.GetKillCount();
		UINT32 tmpdead = stat.GetDeathCount();
		UINT32 tmpassist = stat.GetAssistNum();
		double tmpdamage = stat.GetTotalDamage();
		double tmpbehitdamage = stat.GetBeHitDamage();

		gamerole->set_roleid(i->first);
		gamerole->set_name(i->second.GetRoleName());
		gamerole->set_heroid(i->second.GetHeroID());
		gamerole->set_killcount(tmpkill);
		gamerole->set_deathcount(tmpdead);
		gamerole->set_assistcount(tmpassist);
		gamerole->set_multikillcount(stat.GetMultiKillCount());
		gamerole->set_kda(i->second.GetKDA());
		gamerole->set_isescape(i->second.GetIsEscape());

		if (m_damageMaxid == 0 || damagemax < tmpdamage)
		{
			m_damageMaxid = i->first;
			damagemax = tmpdamage;
		}
		if (m_beHitDamageMaxid == 0 || behitdamagemax < tmpbehitdamage)
		{
			m_beHitDamageMaxid = i->first;
			behitdamagemax = tmpbehitdamage;
		}

		int pos = 0;
		if (i->second.GetTeamID() != m_winTeam) pos = 1;
		bool isBetter = false;
		if (mvpid[pos] == 0 || i->second.GetKDA() > mvpKda[pos])
		{
			isBetter = true;
		}
		else if (i->second.GetKDA() == mvpKda[pos])
		{
			if (mvpDamage[pos] < stat.GetTotalDamage() 
			|| (mvpDamage[pos] == stat.GetTotalDamage() && XRandom::randInt(0, 1000) < 500))
			{
				isBetter = true;
			}
		}
		if (isBetter)
		{
			mvpid[pos] = i->first;
			mvpKda[pos] = i->second.GetKDA();
			mvpDamage[pos] = stat.GetTotalDamage();
		}
	}
	m_mvpid = mvpid[0];
	m_loseMvpid = mvpid[1];
	
	oneGame.set_winteamid(m_winTeam == GetTeam1() ? 1 : 2);
	oneGame.set_mvpid(m_mvpid);
	oneGame.set_losemvpid(m_loseMvpid);
	oneGame.set_damagemaxid(m_damageMaxid);
	oneGame.set_behitdamagemaxid(m_beHitDamageMaxid);

	std::string transTag = TTag::Instance()->GetTagString();
	for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
	{
		MobaRole& role = i->second;
		TMobaBattleFlow tlog;
		tlog.m_Result = m_winTeam == role.GetTeamID() ? 1 : 0;
		if (m_mvpid == i->first) tlog.m_MVP = 1;
		else if (m_loseMvpid == i->first) tlog.m_MVP = 2;
		else tlog.m_MVP = 0;
		tlog.m_RoundTime = oneGame.timespan();
		tlog.m_strTransTag = transTag;
		role.FillTLog(tlog);
		tlog.Do();
	}

	oPtc.m_Data.set_transtag(transTag);
	if (GSConfig::Instance()->IsCrossGS())
	{
		std::set<UINT32> serverids;
		for (auto i = m_roleData.begin(); i != m_roleData.end(); ++i)
		{
			serverids.insert(ServerMgr::Instance()->GetFromServerIdByRoleId(i->first));
		}
		for (auto i = serverids.begin(); i != serverids.end(); ++i)
		{
			ServerMgr::Instance()->SendTo(*i, oPtc);
		}
	}
	else
	{
		MSLink::Instance()->SendTo(oPtc);
	}

	if (m_mvpid)
	{
		PtcG2M_ReportBadPlayer2Ms ptc;
		ptc.m_Data.set_roleid(m_mvpid);
		ptc.m_Data.set_scenetype(this->GetScene()->GetSceneType());
		int score = -GetGlobalConfig().ReportHeroBattleMvpDelNum;
		ptc.m_Data.set_score(score);
		ptc.m_Data.set_ismvp(true);
		MSLink::Instance()->SendTo(m_mvpid, ptc);
	}


    // 目标奖励
    {
        std::map<UINT32, double> teamdamage;
        std::map<UINT32, UINT32> teamdeath;
        std::map<UINT32, UINT32> teamkill;
        for (auto itor = m_roleData.begin(); itor != m_roleData.end(); ++itor)
        {
            CombatStatistics& stat = itor->second.GetCombatStatistics();
            teamdamage[itor->second.GetTeamID()] += stat.GetTotalDamage();
            teamdeath[itor->second.GetTeamID()] += stat.GetDeathCount();
            teamkill[itor->second.GetTeamID()] += stat.GetKillCount();
        }


        for (auto itor = m_roleData.begin(); itor != m_roleData.end(); ++itor)
        {
            CombatStatistics& stat = itor->second.GetCombatStatistics();

            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_301, (double)stat.GetKillCount(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_302, (double)stat.GetAssistNum(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_303, teamdamage[itor->second.GetTeamID()], 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_304, stat.GetTotalDamage(), 0);

            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_305, (double)stat.GetKillCount(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_306, (double)stat.GetAssistNum(), 0);
            GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_307, stat.GetTotalDamage(), 0);

            if(m_mvpid == itor->first)
            {
                GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_308, 1.f, 0);
            }

            if(0 == teamdeath[itor->second.GetTeamID()] && 0 != teamkill[itor->second.GetTeamID()])
            {
                GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_309, teamkill[itor->second.GetTeamID()], 0);
            }

            if(itor->second.GetTeamID() == m_winTeam)
            {
                GoalAwardsConfig::Instance()->SetValue(itor->first, GOAL_AWARD_ID_310, 1.f, 0);
            }
            
        }
    }

}