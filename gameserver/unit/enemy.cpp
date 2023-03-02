#include "pch.h"
#include "enemy.h"
#include "entity/XEnemy.h"
#include "pb/project.pb.h"
#include "combatattribute.h"
#include "buff/XBuffTraveller.h"
#include "entity/XEntity.h"
#include "component/XBuffComponent.h"
#include "gamelogic/XLevelWave.h"
#include "unit.h"
#include "scene/scene.h"
#include "enemymanager.h"
#include "math/XVector3.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "buff/XBuffTemplateManager.h"
#include "attributewatcher.h"
#include "table/SuperArmorRecoveryCoffTable.h"
#include "table/CombatParamMgr.h"
#include "entity/XObject.h"
#include "state/XStateMachine.h"
#include "scene/ptcg2c_deathnotify.h"
#include "gamelogic/enemyskill.h"
#include "table/globalconfig.h"
#include "gamelogic/levelsealMgr.h"
#include "unit/rolemanager.h"
#include "gamelogic/killerwatcher.h"
#include "scene/securitystatistics.h"
#include "scene/scenedoodad.h"

#define CallBack(Fun, ...)   ForeachCall(m_eventListener, Fun, ## __VA_ARGS__) 

Enemy::Enemy()
{
	m_fightgroup = KKSG::FightEnemy;
	m_level = 1;
	m_EnemyTemplate = NULL;
	m_pXEnemy = NULL;
	m_WaveID = -1;
	m_pSpawner = NULL;
	m_skillLevelMgr = new EnemySkillSet();
	m_delay = 0;
	m_scale = 1;
}

Enemy::~Enemy()
{
	delete m_skillLevelMgr;
	assert(m_pXEnemy == NULL);
}

void Enemy::Update(float delta)
{
	if (m_currScene && m_currScene->SyncMode() == SERVERMODE)
	{
		Unit::Update(delta);

		m_pXEnemy->Update(delta);

		if (GetDeprecated())
		{
			DoDeadDisappear();
		}
	}
}

void Enemy::GetAppearance(KKSG::UnitAppearance &Appearance, bool brief)
{
	Unit::GetAppearance(Appearance, brief);

	Appearance.set_category(KKSG::Category_Enemy);
	Appearance.set_unittype(m_EnemyTemplate->ID);
	Appearance.set_unitname(GetName());
	Appearance.set_isservercontrol(true);
}

void Enemy::OnEnterScene(Scene *scene)
{
	for (auto i = m_EnemyTemplate->InBornBuff.begin(); i != m_EnemyTemplate->InBornBuff.end(); ++i)
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = (*i)[0];
		args.singleBuff.BuffLevel = (*i)[1];
		args.Caster = m_uID;
		m_pXEnemy->DispatchAction(&args);
	}

	if (this->IsBoss())
		GetCurrScene()->MsgChannel().AddSceneBoss(this);
}

void Enemy::OnLeaveScene()
{
	if (this->IsBoss())
		GetCurrScene()->MsgChannel().RemoveSceneBoss(this);
}

void Enemy::Init(const EnemyTemplate *template_, const Vector3& pos, const float face, float scale)
{
	m_uTemplateID = template_->ID;
	m_uPresentID = template_->PresentID;
	m_EnemyTemplate = template_;
	m_collider = m_EnemyTemplate->Block;
	m_ignore_dynamic_collide = m_EnemyTemplate->DynamicBlock;
	m_summongroup = m_EnemyTemplate->SummonGroup;

	_CreateTag();
	const XEntityPresentation::RowData* data = XEntityInfoLibrary::Instance()->PresentID2Data(m_uPresentID);

	assert(data != NULL);

	m_huge = data->Huge;
	m_scale = data->Scale;
	m_boundRadius = data->BoundRadius * m_scale;
	if(data->BoundRadiusOffset.size() > 0) m_boundRadiusOffset.set(data->BoundRadiusOffset[0], 0, data->BoundRadiusOffset[1]);
	m_boundHeight = data->BoundHeight * m_scale;

	if(template_->FloatHeight.size() > 0)
	{
		m_floatingMin = template_->FloatHeight[0];
		m_floatingMax = template_->FloatHeight[1];
	}

	EnemyManager::LoadEnemyAttribute(m_EnemyTemplate, m_combatAttr , scale);

	m_fightgroup = template_->Fightgroup;

	m_securityStatistics = new XSecurityStatistics(this);
	m_securityStatistics->OnStart();

	m_pXEnemy = new XEnemy(this);
	m_pObject = m_pXEnemy;
	m_pObject->SetPosition(pos);
	m_pObject->SetFace(face);
	m_pXEnemy->Initilize();
	m_pXEnemy->OnCreated();
}

void Enemy::Uninit()
{
	LogInfo("Enemy::Uninit UnitId[%llu], templateid[%d], this[%x]", GetID(), GetTemplateID(), this);

	m_pXEnemy->Uninitilize();
	delete m_pXEnemy;
	m_pXEnemy = NULL;

	if (NULL != m_securityStatistics)
	{
		delete m_securityStatistics;
		m_securityStatistics = NULL;
	}
}

const EnemyTemplate *Enemy::GetTemplate()
{
	return m_EnemyTemplate;
}

float Enemy::GetNormalAttackProb()
{
	return m_EnemyTemplate->AttackProb;
}

float Enemy::EnterFightRange()
{
	return m_EnemyTemplate->Sight;
}

void Enemy::SetWaveID(int waveID)
{
	m_WaveID = waveID;
}

int Enemy::GetWaveID()
{
	return m_WaveID;
}

void Enemy::OnHurt(Unit* pCastUnit, double value)
{
	///> 打出的伤害，找它的召唤者
	if (NULL != pCastUnit)
	{
		Unit* hostunit = pCastUnit->GetFinalHostUnit();
		if (NULL != hostunit)
		{
			pCastUnit = hostunit;
		}
	}

	CallBack(OnHurt, this, pCastUnit, value);

	if (GetCurrScene())
	{
		GetCurrScene()->OnHurt(this, pCastUnit, value);
	}
}

void Enemy::OnAttack(Unit *pTargetUnit, double value)
{
	CallBack(OnAttack, this, pTargetUnit, value);

	if (NULL == pTargetUnit || pTargetUnit == this)
	{
		return;
	}
	///> 角色召唤的怪打出的伤害，记在角色身上
	Unit* hostunit = GetHostUnit();
	if (NULL != hostunit && hostunit->IsRole())
	{
		((Role*)hostunit)->Statistics().AddAttackInfo(pTargetUnit, value);

		///> 连击数
		if (m_EnemyTemplate->LinkCombo == 1)
			((Role*)hostunit)->Statistics().AddComboCount();
	}
}

void Enemy::OnTreat(Unit *pTargetUnit, double value)
{
	if (NULL == pTargetUnit)
	{
		return;
	}
	///> 角色召唤的怪治愈，记在角色身上
	Unit* hostunit = GetHostUnit();
	if (NULL != hostunit && hostunit->IsRole())
	{
		((Role*)hostunit)->Statistics().AddTreatInfo(pTargetUnit, value);
	}
}

void Enemy::OnDied()
{
	if (NULL != m_pSpawner)
	{
		m_pSpawner->OnMonsterDie(this);
	}

	if (NULL != m_currScene)
	{
		m_currScene->OnEnemyDead(this);
	}
	CallBack(OnDied, this);

	UINT64 killerID = GetKiller()->GetKillerID();
	if (killerID)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(killerID);
		if (pRole)
		{
			pRole->Statistics().AddKillEnemyCount(GetTemplateID());

			if (m_currScene && m_currScene->GetSceneDoodad())
			{
				m_currScene->GetSceneDoodad()->GenerateEnemyPandoraDroppedDoodad(this, pRole);
			}
		}
	}
}

bool Enemy::HasDeathSlowCamera()
{
	//return IsBoss() && m_pSpawner && m_pSpawner->IsLevelWin();
	return false;
}

void Enemy::SetSpawner(XLevelSpawner *pSpawner)
{
	m_pSpawner = pSpawner;
}

bool Enemy::DoDeadDisappear()
{
	LogInfo("Enemy::OnDeadDisappear UnitId[%llu], templateid[%d], this[%x]", GetID(), GetTemplateID(), this);
	m_currScene->RemoveUnitAndNotify(this);
	EnemyManager::Instance()->RemoveEnemy(GetID());
	return true;
}

void Enemy::EnableAI(bool isenable)
{
	Unit::EnableAI(isenable);
}

void Enemy::Invincible(bool isvincible)
{
	if (isvincible)
	{
		XAddBuffActionArgs oArgs;
		oArgs.singleBuff.buffID = 1;
		oArgs.singleBuff.BuffLevel = 1;
		oArgs.Caster = GetID();
		GetXObject()->DispatchAction(&oArgs);
	}

	else
	{
		XRemoveBuffActionArgs oArgs;
		oArgs.nBuffId = 1;
		GetXObject()->DispatchAction(&oArgs);
	}
}

void Enemy::AddListener(EnemyEventListener* pListener)
{
	m_eventListener.push_back(pListener);
}

std::ostream & operator<<(std::ostream &stream, Enemy *pEnemy)
{
	stream << "{Enemy present: " << pEnemy->GetPresentID() << " template: " << 
		pEnemy->GetTemplateID() << " uid: " << (pEnemy->GetID()) << (pEnemy->IsDead() ? " Dead" : " Alive") << "}";
	return stream;
}

std::string Enemy::GetAIString()
{
	return XEntityInfoLibrary::Instance()->TemplateID2AIBehavior(GetTemplateID());
}

void Enemy::GetAIArgs(AIDataArgs &args)
{
	args.mSpeed = GetRunSpeed();
	args.mNormalAttackProb = GetNormalAttackProb();
	args.mEnterFightRange = EnterFightRange();
	args.mIsWander = GetTemplate()->IsWander;
	args.mActionGap = GetTemplate()->AIActionGap;
	args.mAIStartTime = GetTemplate()->AIStartTime;
	args.mIsFixedInCd = GetTemplate()->IsFixedInCD;
	args.mType = GetTemplate()->Type; 
	args.mFightTogetherDis = XEntityInfoLibrary::Instance()->TemplateID2FightTogetherDis(GetTemplate()->ID);

	const XEntityStatistics::RowData* rowdata =  XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(GetTemplate()->ID);
	args.mRatioLeft = rowdata->ratioleft;
	args.mRatioRight = rowdata->ratioright;
	args.mRatioDistance = rowdata->ratiodistance;
	args.mRatioIdle = rowdata->ratioidle;
	args.mRatioExp = rowdata->ratioexp;
	args.mRatioSkill = rowdata->ratioskill;
}

void Enemy::SetName(const std::string& name)
{
	m_name = name;
}

const std::string& Enemy::GetName()
{
	if (m_name.empty())
	{
		return m_EnemyTemplate->Name; 
	}
	return m_name;
}

void Enemy::CleanUpInScene()
{
	LeaveScene();
	EnemyManager::Instance()->RemoveEnemy(GetID());
}

void Enemy::_CreateTag()
{
	m_Tag = 0;
	for (UINT32 i = 0; i < m_EnemyTemplate->Tag.size(); ++i)
	{
		m_Tag |= (1 << m_EnemyTemplate->Tag[i]);
	}
}
