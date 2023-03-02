#include "pch.h"
#include "enemymanager.h"
#include "enemy.h"
#include "role.h"
#include "combatattribute.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "combatattribute.h"
#include "scene/scene.h"
#include "gamelogic/skilldefine.h"

INSTANCE_SINGLETON(EnemyManager)


UINT64 EnemyManager::NewEnemyID()
{
	return (EnemyMask | (++m_EnemyIDCount));
}

EnemyManager::EnemyManager()
{
	m_CreateCount = 0;
	m_DestoryCount = 0;
	m_EnemyIDCount = GetGlobalConfig().ArenaNpcCount;
}

EnemyManager::~EnemyManager()
{

}

bool EnemyManager::Init()
{
	CmdLine::RegistCmd("enemy", CmdLineEnemyManager);
	return true;
}

void EnemyManager::Uninit()
{

}

void EnemyManager::AddEnemy(Enemy *pEnemy)
{
	m_Enemys[pEnemy->GetID()] = pEnemy;
	++m_CreateCount;
}

void EnemyManager::RemoveEnemy(UINT64 uID)
{
	auto i = m_Enemys.find(uID);
	if (i != m_Enemys.end())
	{
		++m_DestoryCount;

		i->second->Uninit();
		delete i->second;
		i->second = NULL;

		m_todelete.push_back(uID);
	}
}

Enemy * EnemyManager::FindEnemy(UINT64 uID)
{
	auto i = m_Enemys.find(uID);
	return i == m_Enemys.end() ? NULL : i->second;
}

void EnemyManager::Update(float delta)
{
	for (auto i = m_Enemys.begin(); i != m_Enemys.end(); ++i)
	{
		Enemy* enemy = i->second;
		if (NULL != enemy)
		{
			i->second->Update(delta);
		}
	}
	for (auto j = m_todelete.begin(); j != m_todelete.end(); ++j)
	{
		auto k = m_Enemys.find(*j);
		if (k != m_Enemys.end())
		{
			m_Enemys.erase(k);
		}
	}
	m_todelete.clear();
}

Enemy* EnemyManager::CreateEnemy(UINT32 uTemplateID, const Vector3& pos, const float face, float scale)
{
	auto i = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(uTemplateID);
	if (i == NULL) return NULL;

	Enemy *pEnemy = new Enemy();
	pEnemy->SetID(NewEnemyID());
	pEnemy->Init(i, pos, face, scale);
	AddEnemy(pEnemy);

	SSInfo<<"create enemy:"<<uTemplateID<<END;

	return pEnemy;
}

Enemy* EnemyManager::CreateEnemyByCaller(Unit* caller, UINT32 uTemplateID, const Vector3& pos, const float face, bool shieldable, float scale)
{
	if (NULL == caller)
	{
		return NULL;
	}
	//////////////////////////////////////////////
	auto i = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(uTemplateID);
	if (i == NULL) return NULL;

	Enemy *pEnemy = new Enemy();
	pEnemy->SetID(NewEnemyID());
	pEnemy->SetHost(caller);
	pEnemy->Init(i, pos, face, scale);
	AddEnemy(pEnemy);
	
	//////////////////////////////////////////////
	if (i->Fightgroup == -1) pEnemy->SetFightGroup(caller->GetFightGroup());
	if (shieldable && pEnemy->GetXObject()) pEnemy->GetXObject()->MobShieldable();

	caller->GetCurrScene()->AddUnitAndNotify(pEnemy);

	SSInfo<<"create enemy:"<<uTemplateID<<" by caller:"<<caller->GetID()<<END;

	return pEnemy;
}

bool EnemyManager::CmdLineEnemyManager(const CmdLineArgs &args, std::string &outputMessage)
{
	EnemyManager *pManager = EnemyManager::Instance();
	SSInfo << "EnemyManager: enemy count = " << pManager->m_Enemys.size() << 
		" create/destory = " << pManager->m_CreateCount << "/" << pManager->m_DestoryCount << END;

	std::stringstream ss;
	int c = 0;
	for (auto i = pManager->m_Enemys.begin(); i != pManager->m_Enemys.end(); ++i, ++c)
	{
		ss << c << ". " << i->second << "\n";
	}

	SSInfo << ss.str() << END;
	return true;
}

bool EnemyManager::LoadEnemyAttribute(UINT32 uTemplateID, CombatAttribute *pCombatAttr, float scale)
{
	auto i = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(uTemplateID);
	if (i == NULL) return false;
	return LoadEnemyAttribute(i, pCombatAttr, scale);
}

bool EnemyManager::LoadEnemyAttribute(const EnemyTemplate *template_, CombatAttribute *pCombatAttr, float scale)
{
	pCombatAttr->SetAttr(BASIC_MaxHP, template_->MaxHP * scale);
	pCombatAttr->SetAttr(BASIC_CurrentHP, template_->MaxHP * scale);
	pCombatAttr->SetAttr(BASIC_MaxMP, 0);
	pCombatAttr->SetAttr(BASIC_CurrentMP, 0);
	pCombatAttr->SetAttr(BASIC_PhysicalAtk, template_->AttackBase * scale);
	pCombatAttr->SetAttr(BASIC_PhysicalAtkMod, template_->AttackBase * scale);
	pCombatAttr->SetAttr(BASIC_ROTATION_SPEED, template_->RotateSpeed);
	pCombatAttr->SetAttr(BASIC_AUTOROTATION_SPEED, template_->RotateSpeed);
	pCombatAttr->SetAttr(BASIC_WALK_SPEED, template_->WalkSpeed);
	pCombatAttr->SetAttr(BASIC_RUN_SPEED, template_->RunSpeed);
	pCombatAttr->SetAttr(BASIC_ATTACK_SPEED, template_->AttackSpeed);
	pCombatAttr->SetAttr(BASIC_SKILL_CD, template_->SkillCD);
	pCombatAttr->SetAttr(BASIC_MaxSuperArmor, template_->MaxSuperArmor);
	pCombatAttr->SetAttr(BASIC_CurrentSuperArmor, template_->MaxSuperArmor);
	pCombatAttr->SetAttr(BASIC_MagicAtk, template_->MagicAttackBase * scale);
	pCombatAttr->SetAttr(BASIC_MagicAtkMod, template_->MagicAttackBase * scale);
	pCombatAttr->SetAttr(BASIC_SuperArmorRecovery, template_->SuperArmorRecoveryValue * template_->MaxSuperArmor / 100);
	pCombatAttr->SetAttr(BASIC_SuperArmorAtk, 1);
	pCombatAttr->SetAttr(BASIC_SuperArmorDef, 0);
	pCombatAttr->SetAttr(BASIC_SuperArmorReg, 0);
	pCombatAttr->SetAttr(BASIC_ENMITY, GetGlobalConfig().GeneralCombatParam);
	pCombatAttr->SetAttr(BASIC_XULI,  GetGlobalConfig().GeneralCombatParam);

	return true;
}

bool EnemyManager::LoadRoleAttribute(Role* role, CombatAttribute *pCombatAttr, UINT32 profession, float scale)
{
	CombatAttribute* roleattrs = role->GetAttrPtr();

	pCombatAttr->SetAttr(BASIC_MaxHP, roleattrs->GetAttr(TOTAL_MaxHP)* scale);
	pCombatAttr->SetAttr(BASIC_CurrentHP, roleattrs->GetAttr(TOTAL_CurrentHP) * scale);

	if (XEntityInfoLibrary::Instance()->GetAttackType(role->GetProfession()) == XEntityInfoLibrary::Instance()->GetAttackType(profession))
	{
		pCombatAttr->SetAttr(BASIC_PhysicalAtk, roleattrs->GetAttr(TOTAL_PhysicalAtk) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalAtkMod, roleattrs->GetAttr(TOTAL_PhysicalAtkMod) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalDef, roleattrs->GetAttr(TOTAL_PhysicalDef) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalDefMod, roleattrs->GetAttr(TOTAL_PhysicalDefMod) * scale);
		pCombatAttr->SetAttr(BASIC_MagicAtk, roleattrs->GetAttr(TOTAL_MagicAtk) * scale);
		pCombatAttr->SetAttr(BASIC_MagicAtkMod, roleattrs->GetAttr(TOTAL_MagicAtkMod) * scale);
		pCombatAttr->SetAttr(BASIC_MagicDef, roleattrs->GetAttr(TOTAL_MagicDef) * scale);
		pCombatAttr->SetAttr(BASIC_MagicDefMod, roleattrs->GetAttr(TOTAL_MagicDefMod) * scale);
	}
	else
	{
		pCombatAttr->SetAttr(BASIC_PhysicalAtk, roleattrs->GetAttr(TOTAL_MagicAtk) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalAtkMod, roleattrs->GetAttr(TOTAL_MagicAtkMod) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalDef, roleattrs->GetAttr(TOTAL_MagicDef) * scale);
		pCombatAttr->SetAttr(BASIC_PhysicalDefMod, roleattrs->GetAttr(TOTAL_MagicDefMod) * scale);
		pCombatAttr->SetAttr(BASIC_MagicAtk, roleattrs->GetAttr(TOTAL_PhysicalAtk) * scale);
		pCombatAttr->SetAttr(BASIC_MagicAtkMod, roleattrs->GetAttr(TOTAL_PhysicalAtkMod) * scale);
		pCombatAttr->SetAttr(BASIC_MagicDef, roleattrs->GetAttr(TOTAL_PhysicalDef) * scale);
		pCombatAttr->SetAttr(BASIC_MagicDefMod, roleattrs->GetAttr(TOTAL_PhysicalDefMod) * scale);
	}
	return true;
}

bool EnemyManager::LoadEnemyAppearance(UINT32 uTemplateID, KKSG::UnitAppearance *pData, bool loadAttr, float scale)
{
	const EnemyTemplate *template_ = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(uTemplateID);
	if (template_ == NULL) return false;

	pData->set_uid(EnemyManager::Instance()->NewEnemyID());
	pData->set_unittype(template_->ID);
	pData->set_unitname(template_->Name);
	pData->set_isservercontrol(false);
	pData->set_fightgroup(template_->Fightgroup);

	if (loadAttr)
	{
		CombatAttribute combatAttr;
		LoadEnemyAttribute(template_, &combatAttr, scale);
		combatAttr.SaveToAttributes(*pData->mutable_attributes());
	}

	return true;
}

