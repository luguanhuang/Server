#include "pch.h"
#include <typeinfo>
#include "ainodecondition.h"
#include "aibehaviortree.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "component/XAIComponent.h"
#include "entity/XEntity.h"
#include "component/XQuickTimeEventComponent.h"
#include "scene/scenefinder.h"
#include "scene/scene.h"
#include "buff/XBuffEnum.h"
#include "component/XBuffComponent.h"
#include "ainodefactory.h"
#include "unit/role.h"
#include "aionlinedebugmgr.h"

#define SKIP_DEAD_OR_ALIVE(deadOrAlive, it) if (((deadOrAlive) == 1 && (*(it))->IsDead()) || ((deadOrAlive) == 2 && !(*(it))->IsDead())) continue;

ValueTarget::ValueTarget(const tinyxml2::XMLElement* elem)
{
	if(elem)
	{
		const tinyxml2::XMLAttribute *attr = elem->FindAttribute("Shared_TargetName");
		if(attr)
		{
			m_sharedTargetName = attr->Value();
		}
	}
}

ValueDistance::ValueDistance(const tinyxml2::XMLElement* elem)
{
	if(elem)
	{
		const tinyxml2::XMLAttribute *attr = elem->FindAttribute("Shared_TargetName");
		if(attr)
		{
			m_sharedTargetName = attr->Value();
		}
		
		attr = elem->FindAttribute("Shared_MaxDistanceName");
		if(attr)
		{
			m_sharedMaxDistanceName = attr->Value();
		}
	}
}

bool ValueDistance::Update(Unit* unit)
{
	AIRETURN("ValueDistance", true);
}

void ValueDistance::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_sharedTargetName) << " : " << m_sharedTargetName << END;
	SSInfo << VAR_NAME(m_sharedMaxDistanceName) << " : " << m_sharedMaxDistanceName << END;
}

bool ValueTarget::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("ValueTarget", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	UINT64 targetid = unit->GetXObject()->AIComponent()->GetTarget();

	if (targetid)
	{
		if (!XEntity::ValidEntity(targetid))
		{
			unit->GetXObject()->AIComponent()->SetTarget(0);

			AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; ");

			AIRETURN("ValueTarget", false);
		}

#ifdef AIDEBUG
		m_DebugInfo << targetid;
		if (debugHelper.ShouldLog())
		{
			Unit* pUnit = Unit::FindUnit(targetid);
			if (pUnit)
				m_DebugInfo << "; TempID: " << pUnit->GetTemplateID();
		}
#endif
		AIRETURN("ValueTarget", true);
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; ");

		AIRETURN("ValueTarget", false);
	}
}


void ValueTarget::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_sharedTargetName) << " : " << m_sharedTargetName <<END;
}

bool StatusIdle::Update( Unit* unit )
{
	AIRETURN("StatusIdle", true);
}

bool StatusMove::Update( Unit* unit )
{
	AIRETURN("StatusMove", true);
}

bool StatusRotate::Update( Unit* unit )
{
	AIRETURN("StatusRotate", true);
}

ValueHP::ValueHP( const tinyxml2::XMLElement* elem )
{
	m_maxPercent = 0;
	m_minPercent = 0;
	if(elem)
	{
		const tinyxml2::XMLAttribute *attr = elem->FindAttribute("MaxPercent");
		if(attr)
		{
			m_maxPercent = attr->IntValue();			
		}

		attr = elem->FindAttribute("MinPercent");
		if(attr)
		{
			m_minPercent = attr->IntValue();			
		}
	}
}

bool ValueHP::Update( Unit* unit )
{
	int nHpPercent = (int) (100 * unit->GetAttr(BASIC_CurrentHP)/unit->GetAttr(TOTAL_MaxHP));
	bool bRes = (nHpPercent >= m_minPercent && nHpPercent <= m_maxPercent);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	AI_DEBUG_STATEMENT(m_DebugInfo << nHpPercent << "; " << (bRes ? "Success" : "Failed"));

	AIRETURN("ValueHP", bRes);
}

void ValueHP::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_maxPercent) << " : " << m_maxPercent <<END;
	SSInfo << VAR_NAME(m_minPercent) << " : " << m_minPercent <<END;
}

ValueMP::ValueMP( const tinyxml2::XMLElement* elem )
{
	m_maxPercent = 0;
	m_minPercent = 0;
	if(elem)
	{
		const tinyxml2::XMLAttribute *attr = elem->FindAttribute("MaxPercent");
		if(attr)
		{
			m_maxPercent = attr->IntValue();			
		}

		attr = elem->FindAttribute("MinPercent");
		if(attr)
		{
			m_minPercent = attr->IntValue();			
		}
	}
}

bool ValueMP::Update( Unit* unit )
{
	AIRETURN("ValueMP", true);
}

void ValueMP::Print()
{

	AINodeBase::Print();

	SSInfo << VAR_NAME(m_maxPercent) << " : " << m_maxPercent <<END;
	SSInfo << VAR_NAME(m_minPercent) << " : " << m_minPercent <<END;
}

bool StatusBehit::Update( Unit* unit )
{
	AIRETURN("StatusBehit", true);
}

bool StatusDeath::Update( Unit* unit )
{

	AIRETURN("StatusDeath", true);
}

bool StatusFreeze::Update( Unit* unit )
{
	AIRETURN("StatusFreeze", true);
}

bool StatusSkill::Update( Unit* unit )
{
	AIRETURN("StatusSkill", true);
}

bool StatusWoozy::Update( Unit* unit )
{
	AIRETURN("StatusWoozy", true);
}

ValueFP::ValueFP( const tinyxml2::XMLElement* elem )
{
	m_maxFP = 0;
	m_minFP = 0;
	if(elem)
	{
		const tinyxml2::XMLAttribute *attr = elem->FindAttribute("MaxFP");
		if(attr)
		{
			m_maxFP = attr->IntValue();			
		}

		attr = elem->FindAttribute("MinFP");
		if(attr)
		{
			m_minFP = attr->IntValue();			
		}
	}
}

bool ValueFP::Update( Unit* unit )
{
	AIRETURN("ValueFP", true);
}

void ValueFP::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_maxFP) << " : " << m_maxFP <<END;
	SSInfo << VAR_NAME(m_minFP) << " : " << m_minFP <<END;
}

bool IsOppoCastingSkill::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		return false;

	//return unit->GetXObject()->AIComponent()->IsOppoCastingSkill();
	AIRETURN("IsOppoCastingSkill", false);
}

bool IsHurtOppo::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsHurtOppo", false);

	AI_DEBUG_RETURN("IsHurtOppo", unit->GetXObject()->AIComponent()->IsHurtOppo());
}

bool IsFixedInCd::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsFixedInCd", false);

	AI_DEBUG_RETURN("IsFixedInCd",  unit->GetXObject()->AIComponent()->IsFixedInCd());
}

bool IsWander::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsWander",  false);

	AI_DEBUG_RETURN("IsWander", unit->GetXObject()->AIComponent()->IsWander());
}

bool IsCastingSkill::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsCastingSkill", false);

	AI_DEBUG_RETURN("IsCastingSkill", unit->GetXObject()->AIComponent()->IsCastingSkill());
}

bool IsFighting::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsFighting",  false);

	AI_DEBUG_RETURN("IsFighting", unit->GetXObject()->AIComponent()->IsFighting());
}

IsQTEState::IsQTEState(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("QTEState");
	if (attr) attr->QueryIntValue(&QTEState);
}

bool IsQTEState::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("IsQTEState",  false);

	if (unit->GetXObject()->QuickTimeEventComponent())
	{
		if (QTEState == 1000)
			AI_DEBUG_RETURN("IsQTEState", unit->GetXObject()->QuickTimeEventComponent()->IsInAnyState())
		else
			AI_DEBUG_RETURN("IsQTEState", unit->GetXObject()->QuickTimeEventComponent()->IsInState((uint)QTEState))
	}
	else
		AI_DEBUG_RETURN("IsQTEState",  false);
}

TargetQTEState::TargetQTEState(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
	if (attr) 
	{
		Shared_Target = attr->Value();

		if (Shared_Target != "")
			TargetIndex = AINodeFactory::CreateIndexByName(Shared_Target.c_str());
		else
			TargetIndex = 0;
	}

	attr = elem->FindAttribute("QTEState");
	if (attr) attr->QueryIntValue(&QTEState);
}


bool TargetQTEState::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("TargetQTEState",  false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	Transform target = unit->AIData()->GetTransformByIndex(TargetIndex);

	if (!target)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; Failed;");
		AIRETURN("TargetQTEState",  false);
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "TargetUID " << target->GetID() << "; TempID " << target->GetTemplateID() << "; ");
	if (target->GetXObject()->QuickTimeEventComponent())
	{
		AI_DEBUG_RETURN_WITHOUT_DEFINE("TargetQTEState", target->GetXObject()->QuickTimeEventComponent()->IsInState((uint)QTEState));
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Target has no QTE");
		AI_DEBUG_RETURN_WITHOUT_DEFINE("TargetQTEState",  false);
	}
}

ConditionMonsterNum::ConditionMonsterNum(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_NumName");
	NumIndex = -1;
	if (attr)
	{
		NumString = attr->Value();
		if (NumString != "")
			NumIndex = AINodeFactory::CreateIndexByName(NumString.c_str());
	}

	attr = elem->FindAttribute("MonsterId");
	if (attr) attr->QueryIntValue(&MonsterId);
}

bool ConditionMonsterNum::Update(Unit* unit)
{
	if (!unit->AIComponent())
		AIRETURN("TargetQTEState",  false);

	FindEnemyTemplateIdCondition cond0(MonsterId);
	std::vector<Unit *> units0;

	if (MonsterId != 0)
		unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

	int number = 0;

	for (UINT32 i=0; i<units0.size(); i++)
		if (XEntity::ValidEntity(units0[i]->GetID()))
			number += 1;
	
	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << number);

	unit->AIData()->SetIntByIndex(NumIndex, number);
	return true;
}

ConditionPlayerNum::ConditionPlayerNum(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_NumName");
	NumIndex = -1;
	if (attr)
	{
		NumString = attr->Value();
		if (NumString != "")
			NumIndex = AINodeFactory::CreateIndexByName(NumString.c_str());
	}

	attr = elem->FindAttribute("PlayerBaseProf");
	if (attr) 
		attr->QueryIntValue(&BaseProf);
	else
		BaseProf = 0;

	attr = elem->FindAttribute("PlayerDetailProf");
	if (attr)
		attr->QueryIntValue(&DetailProf);
	else
		DetailProf = 0;

	attr = elem->FindAttribute("Way");
	if (attr) 
		attr->QueryIntValue(&Way);
	else
		Way = 0;

	attr = elem->FindAttribute("Radius");
	if (attr)
		attr->QueryFloatValue(&SqrRadius);
	else
		SqrRadius = 0.0f;
	SqrRadius *= SqrRadius;

	attr = elem->FindAttribute("Center");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), Center);
}

bool ConditionPlayerNum::Update(Unit* unit)
{
	int number = 0;
	const std::list<Role*>& players = unit->GetCurrScene()->GetAllRoles();

	for (auto i = players.begin(); i != players.end(); ++i)
	{
		Role* role = *i;

		if (BaseProf != 0 || DetailProf != 0)
		{
			if (BaseProf != 0 && role->GetProfession() % 10 != BaseProf)
				continue;
			if (DetailProf != 0 && role->GetProfession() != DetailProf)
				continue;
		}

		SKIP_DEAD_OR_ALIVE(Way, i);

		if (SqrRadius > 0.0f)
		{
			float sqrDis = Vector3::sqrtMagnitude(role->GetXObject()->GetPosition_p() - Center);
			if (sqrDis > SqrRadius)
				continue;
		}

		++number;
	}

	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << number);

	unit->AIData()->SetIntByIndex(NumIndex, number);
	return true;
}

IsTargetImmortal::IsTargetImmortal(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
	if (attr) 
	{
		TargetName = attr->Value();
		if (TargetName != "")
			TargetIndex = AINodeFactory::CreateIndexByName(TargetName.c_str());
		else
			TargetIndex = 0;
	}
}

bool IsTargetImmortal::Update(Unit* unit)
{
	Transform target = unit->AIData()->GetTransformByIndex(TargetIndex);
	
	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (!target)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; False;");

		AIRETURN("TargetQTEState",  false);
	}
	
	AI_DEBUG_STATEMENT(m_DebugInfo << "TargetUID " << target->GetID() << "; TempID " << target->GetTemplateID() << "; ");

	AI_DEBUG_RETURN_WITHOUT_DEFINE("IsTargetImmortal", target->GetXObject()->BuffComponent()->IsBuffStateOn(XBuffType_Immortal));
}