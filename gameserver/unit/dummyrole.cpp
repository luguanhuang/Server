#include "pch.h"
#include "dummyrole.h"
#include "entity/XDummyRole.h"
#include "gamelogic/rolesummarymgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "dummyrolemanager.h"
#include "gamelogic/allyskill.h"
#include "scene/ptcg2c_allymatchroleidnotify.h"
#include "scene/scene.h"
#include "table/globalconfig.h"
#include "table/PlayerLevelManager.h"
#include "util/XRandom.h"
#include "gamelogic/XLevelWave.h"
#include "gamelogic/robotgenerator.h"
#include "rolemanager.h"
#include "table/XSkillEffectMgr.h"


DummyRole::DummyRole()
{
	m_pXAlly = NULL;
	m_srcRoleID = 0;
	m_level = 1;
	m_fightgroup = KKSG::FightRole;
	m_skillLevelMgr = NULL;
	m_statitics.SetUnit(this);
	m_WaveID = 0;
	m_pSpawner = NULL;
	m_DummyType = SCENE_ALLY;
}

DummyRole::~DummyRole()
{
	delete m_skillLevelMgr;
}

void DummyRole::Init(RoleSummary *pSummary, const Vector3& pos, const float face, UINT32 fightGroup, const EnterSceneInfo& info, DummyType type)
{
	m_DummyType = type;
	m_srcRoleID = pSummary->GetID();
	m_uID = pSummary->qwRobotID;
	m_entersceneinfo = info;

	DummyRoleSkillSet *pSkillSet = new DummyRoleSkillSet();
	m_skillLevelMgr = pSkillSet;
	
	m_skillslot.resize(KKSG::Attack_Max);
	for (unsigned i = 0; i < m_skillslot.size(); ++i)
	{
		m_skillslot[i] = 0;
	}

	SetFightGroup(fightGroup);

	InitAllyAttribute(pSummary);
	
	pSkillSet->InitFromRoleSummary(pSummary, m_pXAlly);

	///> 所有技能
	std::vector<skillInfo>& tempskills = pSkillSet->GetSkills();
	for (auto i = tempskills.begin(); i != tempskills.end(); ++i)
	{
		m_skills.push_back(i->skillhash);
	}
	///> 携带技能
	std::vector<UINT32> bindskills;
	for (auto i = m_skills.rbegin(); i != m_skills.rend(); ++i)
	{
		bindskills.push_back(*i);
	}
	for (int i = KKSG::Skill_1_Attack; i <= KKSG::Ultra_Attack; ++i)
	{
		for (auto j = bindskills.begin(); j != bindskills.end(); ++j)
		{
			if (BindSkillSlot(i, *j))
			{
				bindskills.erase(j);
				break;
			}
		}
	}

	m_pXAlly = new XDummyRole(this);
	m_pObject = m_pXAlly;
	m_pObject->SetPosition(pos);
	m_pObject->SetFace(face);
	m_pXAlly->Initilize();
	m_pXAlly->OnCreated();

	pSummary->SetUnitBasic(&m_appearance);
	pSummary->SetUnitFashion(&m_appearance);
	pSummary->SetUnitSkill(&m_appearance);
	Unit::GetAppearance(m_appearance);

	m_appearance.clear_skills();
	for(size_t i = 0; i < tempskills.size(); ++i)
	{
		KKSG::SkillInfo& info = *m_appearance.add_skills();
		info.set_skillhash(tempskills[i].skillhash);
		info.set_skilllevel(tempskills[i].skilllevel);
		info.set_skillpoint(tempskills[i].skillpoint);
		info.set_isbasic(tempskills[i].isbasic);

	}
	m_appearance.clear_bindskills();
	for (auto i = m_skillslot.begin(); i != m_skillslot.end(); ++i)
	{
		m_appearance.add_bindskills(*i);
	}

	pSkillSet->RefreshSelfLinkedLevels(this);
}

void DummyRole::Uninit()
{
	m_pXAlly->Uninitilize();
	delete m_pXAlly;
	m_pXAlly = NULL;
}

void DummyRole::Update(float delta)
{
	if (m_currScene)
	{
		Unit::Update(delta);

		m_pXAlly->Update(delta);
		m_statitics.Update();
	}
}

void DummyRole::GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief)
{
	Unit::GetAppearance(m_appearance, isbrief);//属性会改变
	Appearance = m_appearance;
	Appearance.set_category(KKSG::Category_DummyRole);
}

void DummyRole::InitAllyAttribute(RoleSummary *pSummary)
{
	//m_Name = pSummary->strName;
	SetName(pSummary->GetName());
	m_exp = 0;

	m_prof = pSummary->GetProfession();
	m_level = pSummary->GetLevel();
	m_maxExp = (UINT32)PlayerLevelManager::Instance()->GetLevelupExp(m_level);
	m_exp = XRandom::randInt(0, m_maxExp);

	*m_combatAttr = pSummary->oAttribute;
	double hpMax = pSummary->oAttribute.GetAttr(BASIC_MaxHP);
	m_combatAttr->SetAttr(BASIC_CurrentHP, hpMax);

	m_uTemplateID = pSummary->GetProfession();
	m_uPresentID = XEntityInfoLibrary::Instance()->TemplateID2PresentID_Role(m_uTemplateID);

	auto PresentData = XEntityInfoLibrary::Instance()->PresentID2Data(m_uPresentID);
	float scale = PresentData->Scale;
	m_boundRadius = PresentData->BoundRadius * scale;
	m_boundHeight = PresentData->BoundHeight * scale;
}

void DummyRole::CleanUpInScene()
{
	LeaveScene();
	DummyRoleManager::Instance()->Remove(this);
}

void DummyRole::OnAttack(Unit *pTargetUnit, double value)
{
	m_statitics.AddAttackInfo(pTargetUnit, value);
}

void DummyRole::OnTreat(Unit *pTargetUnit, double value)
{
	m_statitics.AddTreatInfo(pTargetUnit, value);
}

void DummyRole::OnHurt(Unit* pCastUnit, double value)
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
	m_statitics.BeHit();
	m_statitics.BeHurt(pCastUnit, value);
}

void DummyRole::OnDied()
{
	m_statitics.AddDeathCount();

	UINT64 killerID = GetKillerID();	
	if(killerID)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(killerID);
		if(pRole)
		{
			pRole->Statistics().AddKillCount();
		}
		else
		{
			DummyRole* pDummy = DummyRoleManager::Instance()->Find(killerID);
			if(pDummy)
			{
				pDummy->Statistics().AddKillCount();
			}
		}
	}

	if(NULL != m_pSpawner)
	{
		m_pSpawner->OnDummyRoleDie(this);
	}

	if(NULL != m_currScene)
	{
		m_currScene->OnDummyRoleDead(this);
	}
}

void DummyRole::OnEnterScene(Scene *scene)
{
}

void DummyRole::OnLeaveScene()
{
}

std::string DummyRole::GetAIString()
{
	switch(m_DummyType)
	{
	case SCENE_ENEMY:
		return  GetGlobalConfig().GetConfig("EnemyAutoFight");
	case SCENE_ALLY:
		return  GetGlobalConfig().GetConfig("AllyAutoFight");
	case ARENA_ENEMY:
		return  GetGlobalConfig().GetConfig("ArenaServerAI");
	default:
		break;
	}

	return  GetGlobalConfig().GetConfig("AllyAutoFight");
}

void DummyRole::GetAIArgs(AIDataArgs &args)
{
	args.mSpeed = GetRunSpeed();
	args.mNormalAttackProb = 1.0f;
	args.mEnterFightRange = 20.0f;
	args.mIsWander = false;
	args.mActionGap = 1.5f;
	args.mAIStartTime = 1.5f;
	args.mIsFixedInCd = false;
	args.mFightTogetherDis = 30.0f;
	args.mType = 1;

	args.mRatioLeft = 0.2f;
	args.mRatioRight = 0.2f;
	args.mRatioDistance = 0.2f;
	args.mRatioIdle = 0.2f;
	args.mRatioExp = 0.2f;
	args.mRatioSkill = 0.2f;
}

std::ostream & operator<<(std::ostream &stream, DummyRole *pAlly)
{
	stream << "{Enemy present: " << pAlly->GetPresentID() << " uid: " << (pAlly->GetID()) << (pAlly->IsDead() ? " Dead" : " Alive") << "}";
	return stream;
}

void DummyRole::AddExp(UINT32 exp)
{
	m_exp += exp;
	if (m_exp >= m_maxExp)
	{
		while (m_level < PlayerLevelManager::Instance()->MaxLevel())
		{
			PlayerLevelTable::RowData *row = PlayerLevelManager::Instance()->GetNextLevelInfo(m_level);
			if (m_exp < (UINT32)row->Exp)
			{
				m_maxExp = (UINT32)row->Exp;
				break;
			}

			++m_level;
			m_exp -= (UINT32)row->Exp;
		}
	}
}

bool DummyRole::BindSkillSlot(UINT32 slot, UINT32 skillhash)
{
	if (slot >= m_skillslot.size() || 0 == skillhash)
	{
		return false;
	}

	if (m_skillslot[slot] == skillhash)
	{
		return false;
	}

	if (!XSkillEffectMgr::Instance()->CanBindSkillInSlot(skillhash, slot))
	{
		return false;
	}

	///> 设置技能
	m_skillslot[slot] = skillhash;

	LogDebug("bind skill %u to slot %d", skillhash, slot);

	return true;
}

void DummyRole::OnRevive()
{
	if (NULL != m_currScene)
	{
		m_currScene->OnDummyRoleRevive(this);
	}
}
