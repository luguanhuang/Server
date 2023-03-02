#include "pch.h"
#include <typeinfo>
#include "ainodetarget.h"
#include "ainode.h"
#include "scene/scene.h"
#include "scene/scenefinder.h"
#include "unit/unit.h"
#include "gamelogic/XEnmityList.h"
#include "entity/XEntity.h"
#include "aibehaviortree.h"
#include "component/XAIComponent.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "scene/scenedoodad.h"
#include "buff/XBuffEnum.h"
#include "component/XBuffComponent.h"
#include "unit/enemy.h"
#include "aionlinedebugmgr.h"

bool ResetTargets::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	if (NULL != unit->GetCurrScene())
	{
		const std::vector<Unit*>* opponent = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());
		if (NULL != opponent)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << opponent->size() << " Targets; ");
			unit->GetXObject()->AIComponent()->SetTargets(*opponent);
		}
		else
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "No Target Found; ");
			std::vector<Unit*>& targets = unit->GetXObject()->AIComponent()->GetTargets();
			targets.clear();
		}
	}
	AIRETURN("ResetTargets", true);
}

FindTargetByDistance::FindTargetByDistance( const tinyxml2::XMLElement* elem )
{
	m_distanceIndex = -1;
	if(elem)
	{
		const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_DistanceName");
		if(attr)
		{
			m_sharedDistanceName = attr->Value();

			if (m_sharedDistanceName != "")
				m_distanceIndex = AINodeFactory::CreateIndexByName(m_sharedDistanceName.c_str());
		}
		
		attr = elem->FindAttribute("Shared_DistancemValue");
		
		if (attr)
		{
			m_distance = attr->FloatValue();
		}

		attr = elem->FindAttribute("FilterImmortal");
		if (attr) attr->QueryBoolValue(&m_filterImmortal);

		attr = elem->FindAttribute("Angle");
		if (attr) attr->QueryFloatValue(&m_angle);
		else m_angle = 180;

		attr = elem->FindAttribute("Delta");
		if (attr) attr->QueryFloatValue(&m_delta);
		else m_delta = 0;

		attr = elem->FindAttribute("TargetType");
		if (attr) attr->QueryIntValue(&m_targetType);
		else m_targetType = 0;
	}
}

bool FindTargetByDistance::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("FindTargetByDistance", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);
	SharedData* data = unit->AIData();
	if(data)
	{
		float distance = data->GetFloatByIndex(m_distanceIndex , m_distance);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Distance " << distance << "; ");

		std::vector<Unit*> indistance;
		const std::vector<Unit*>* opponent = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());
		if (NULL != opponent)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << opponent->size() << " Opponents; ");

			for (UINT32 i = 0; i < opponent->size(); ++i)
			{
				if (m_filterImmortal && (*opponent)[i]->GetXObject()->BuffComponent()->IsBuffStateOn(XBuffType_Immortal))
					continue;

				if (unit->Distance((*opponent)[i]) <= distance)
				{
					bool add = true;

					if (m_targetType == 1 && !(*opponent)[i]->IsRole())
						add = false;

					if (m_targetType == 2 && (*opponent)[i]->IsRole())
						add = false;

					if (m_angle < 180 && m_angle > 0)
					{
						Vector3 vecdir = ((*opponent)[i])->GetXObject()->GetPosition_p() - unit->GetXObject()->GetPosition_p();
						float angle = Vector3::Angle(vecdir, unit->GetXObject()->GetFace_p());
						
						if (angle < m_angle)
						{
							if (add)
								indistance.push_back((*opponent)[i]);
						}
					}
					else
					{
						if (add)
							indistance.push_back((*opponent)[i]);
					}
				}
			}
		}

		AI_DEBUG_STATEMENT(m_DebugInfo << "Set " << indistance.size() << " Targets; ");

		unit->GetXObject()->AIComponent()->SetTargets(indistance);

		AI_DEBUG_RETURN_WITHOUT_DEFINE("FindTargetByDistance", indistance.size() != 0)
	}
	else
	{
		AIRETURN("FindTargetByDistance", false)
	}
}

void FindTargetByDistance::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_sharedDistanceName) << " : " << m_sharedDistanceName <<END;
}

bool SortHitLevel(Unit* a, Unit* b)
{
	int hita = XEntityInfoLibrary::Instance()->TemplateID2HitLevel(a->GetTemplateID());
	int hitb = XEntityInfoLibrary::Instance()->TemplateID2HitLevel(b->GetTemplateID());

	if (hita > hitb)
		return true;
	else
		return false;
}

FindTargetByHitLevel::FindTargetByHitLevel(const tinyxml2::XMLElement *elem)
{
	if (elem)
	{
		const tinyxml2::XMLAttribute* attr = elem->FindAttribute("FilterImmortal");
		if (attr) attr->QueryBoolValue(&m_filterImmortal);
	}
}

bool FindTargetByHitLevel::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("FindTargetByHitLevel", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);
	SharedData* data = unit->AIData();
	if(data)
	{
		std::vector<Unit*> opponent;
		std::vector<Unit*> indistance;
		int hitlevel = -1;

		const std::vector<Unit*>* opponents = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());
		if (NULL != opponents)
		{
			opponent = *opponents;
		}

		std::sort(opponent.begin(), opponent.end(), SortHitLevel);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << opponent.size() << " Opponents; ");

		///> 保底的无敌怪
		Unit* pImmortalBackup = NULL;

		for (UINT32 i = 0; i < opponent.size(); ++i)
		{
			if (XEntity::ValidEntity(opponent[i]->GetID()) && opponent[i]->GetXObject()->AIComponent() && opponent[i]->GetXObject()->AIComponent()->IsFighting())
			{
				int aihit = XEntityInfoLibrary::Instance()->TemplateID2HitLevel(opponent[i]->GetTemplateID());

				if (m_filterImmortal && opponent[i]->GetXObject()->BuffComponent()->IsBuffStateOn(XBuffType_Immortal))
				{
					if (pImmortalBackup == NULL && aihit >= 0)
						pImmortalBackup = opponent[i];
					continue;
				}

				if (aihit >= 0)
				{
					if (hitlevel == -1)
					{
						hitlevel = aihit;
						indistance.push_back(opponent[i]);
					}
					else
					{
						if (aihit == hitlevel)
							indistance.push_back(opponent[i]);
					}
				}
			}
		}

		if (m_filterImmortal && indistance.size() == 0 && pImmortalBackup != NULL)
		{
			indistance.push_back(pImmortalBackup);
			AI_DEBUG_STATEMENT(m_DebugInfo << "Add an immortal target manually; ");
		}

		//// 如果找不到非无敌的怪，就在无敌的对象里面任意找一个
		//if (m_filterImmortal && indistance.size() == 0 && opponent.size() > 0)
		//{
		//	AI_DEBUG_STATEMENT(m_DebugInfo << "Try to find any immortal target");

		//	for (UINT32 i=0; i<opponent.size(); i++)
		//	{
		//		if (opponent[i]->GetXObject()->BuffComponent()->IsBuffStateOn(XBuffType_Immortal))
		//		{
		//			indistance.push_back(opponent[i]);
		//			break;
		//		}
		//	}
		//}

		AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << indistance.size() << " Targets; ");

		unit->GetXObject()->AIComponent()->SetTargets(indistance);

		AI_DEBUG_RETURN_WITHOUT_DEFINE("FindTargetByHitLevel", indistance.size() != 0)
	}
	else
	{
		AIRETURN("FindTargetByHitLevel", false)
	}
}

TargetByHatredList::TargetByHatredList( const tinyxml2::XMLElement* elem )
{
	if(elem)
	{
		const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_DistanceName");
		if(attr)
		{
			m_sharedDistanceName = attr->Value();
		}

		attr = elem->FindAttribute("FilterImmortal");
		if (attr) attr->QueryBoolValue(&m_filterImmortal);
	}
}

bool TargetByHatredList::Update( Unit* unit )
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("TargetByHatredList", false)

	XAIComponent* ai = unit->AIComponent();
	const std::vector<Unit*>& target = ai->EnmityList().GetHateEntity();

	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << target.size() << " Targets; ");
	
	if (target.size() > 0)
	{
		std::vector<Unit*>& targets = ai->GetTargets();
		targets.clear();
		for (UINT32 i=0; i<target.size(); i++)
		{
				targets.push_back(target[i]);
		}
		//unit->GetXObject()->AIComponent()->SetTargets(targets);
		AI_DEBUG_RETURN_WITHOUT_DEFINE("TargetByHatredList", true)
	}

	ai->SetFighting(false); 

	AI_DEBUG_RETURN_WITHOUT_DEFINE("TargetByHatredList", false)
}

void TargetByHatredList::Print()
{
	AINodeBase::Print();

	//SSInfo << VAR_NAME(m_sharedDistanceName) << " : " << m_sharedDistanceName <<END;
}

bool SelectNonHartedList::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	if (!ai)AIRETURN("TargetByHatredList", false)

	std::vector<Unit*> opponent;
	DEFINE_AI_DEBUG_HELPER(this, unit);

	const std::vector<Unit*>& target = ai->EnmityList().GetHateEntity();

	if (target.size() == 0)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "EnmityList Empty; False;");
		return false;
	}

	const std::vector<Unit*>* opponents = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());

	if (opponents != NULL)
		opponent = *opponents;

	UINT32 startIndex = XRandom::randInt(0, opponents->size());

	AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << opponent.size() << " Opponents; ");

	for (UINT32 i=0; i<opponent.size(); i++)
	{
		UINT32 j = (i+startIndex)%opponent.size();
		if (opponent[j] != target[0])
		{
			ai->SetTarget(opponent[j]->GetID());
			AI_DEBUG_STATEMENT(m_DebugInfo << "Set Target" << opponent[j]->GetID() << "; TempID " << opponent[j]->GetTemplateID());

			if (!ai->IsFighting())
				ai->SendEnterFightToClient();

			ai->SetFighting(true);
			AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectNonHartedList", true);
		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectNonHartedList", false);
}

bool FindNavPath::Update(Unit* unit)
{
	AI_DEBUG_RETURN("FindNavPath", unit->GetXObject()->NavigationComponent())
}

DoSelectByUID::DoSelectByUID(const tinyxml2::XMLElement* elem)
{
	m_UIDIndex = -1;
	if(elem)
	{
		const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_UIDName");
		if(attr)
		{
			m_UIDName = attr->Value();	

			if (m_UIDName != "")
				m_UIDIndex = AINodeFactory::CreateIndexByName(m_UIDName.c_str());
		}
	}
}

bool DoSelectByUID::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	if (!ai)
		AIRETURN("DoSelectByUID", false)
	
	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();

	string defaultStr("0");
	const string& strUID = unit->AIData()->GetStringByIndex(m_UIDIndex, defaultStr);

	UINT64 uid = convert<UINT64>(strUID);
	
	AI_DEBUG_STATEMENT(m_DebugInfo << strUID << "; ");

	Unit* pTarget = Unit::FindUnit(uid);
	if (pTarget)
	{
		ai->SetTarget(uid);
		data->SetTransformByIndex(XAIComponent::_target_index, pTarget);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Target TempID " << pTarget->GetTemplateID());
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectByUID", true)
	}

	ai->SetTarget(0);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectByUID", false)
}

void DoSelectByUID::Print()
{
	AINodeBase::Print();

	SSInfo << VAR_NAME(m_UIDName) << " : " << m_UIDName <<END;
}

DoSelectRoleByIndex::DoSelectRoleByIndex(const tinyxml2::XMLElement* elem)
{

	m_IndexIndex = -1;
	m_Index = 0;
	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_IndexmValue");
		if (attr) attr->QueryIntValue(&m_Index);

		attr = elem->FindAttribute("Shared_IndexName");
		if(attr)
		{
			string indexName = attr->Value();	

			if (indexName != "")
				m_IndexIndex = AINodeFactory::CreateIndexByName(indexName.c_str());
		}
	}
}

bool DoSelectRoleByIndex::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	if (!ai)
		AIRETURN("DoSelectRoleByIndex", false)

	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();

	
	int index = unit->AIData()->GetIntByIndex(m_IndexIndex, m_Index);

	AI_DEBUG_STATEMENT(m_DebugInfo << index << "; ");

	const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
	AI_DEBUG_STATEMENT(m_DebugInfo << roles.size() << " Roles; ");
	if (index < 0 || index >= roles.size())
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Out of Range; ");
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectRoleByIndex", false);
	}

	auto it = roles.begin();
	std::advance(it, index);
	Unit* pTarget = *it;
	if (pTarget)
	{
		ai->SetTarget(pTarget->GetID());
		data->SetTransformByIndex(XAIComponent::_target_index, pTarget);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Target Prof " << pTarget->GetTemplateID() << "; UID " << pTarget->GetID() << "; ");
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectRoleByIndex", true)
	}

	ai->SetTarget(0);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectRoleByIndex", false)
}

bool DoSelectNearest::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	if (!XEntity::ValidEntity(unit->GetID()) || !ai)
		AIRETURN("DoSelectNearest", false)

	SharedData* data = unit->AIData();
	
	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<Unit*>& targets = ai->GetTargets();
	float dist = 99999999.0f;
	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << targets.size() << "; ");

	if (targets.size() == 0)
	{
		ai->SetTarget(0);
		ai->SetFighting(false); 
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectNearest", false);
	}

	Unit* target = targets[0];
	for (UINT32 i=0; i<targets.size(); i++)
	{
		if (XEntity::ValidEntity(targets[i]->GetID()) && unit->Distance(targets[i]) <= dist)
		{
			dist = (float)unit->Distance(targets[i]);
			target = targets[i];
		}
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID() << "; dist " << dist << "; ");
	ai->SetTarget(target->GetID());
	data->SetTransformByIndex(XAIComponent::_target_index, target);
	data->SetFloatByIndex(m_targetDisIndex, dist);

	if (!ai->IsFighting())
	{
		XAIEnterFightEventArgs oArgs;
		oArgs.TargetID = target->GetID();
		unit->GetXObject()->DispatchAction(&oArgs);
	}
	ai->SetFighting(true);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectNearest", true)
}

bool DoSelectFarthest::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	if (!XEntity::ValidEntity(unit->GetID()) || !ai)
		AIRETURN("DoSelectNearest", false)

	DEFINE_AI_DEBUG_HELPER(this, unit);
	
	SharedData* data = unit->AIData();

	std::vector<Unit*>& targets = ai->GetTargets();
	float dist = 0.0f;
	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << targets.size() << "; ");

	if (targets.size() == 0)
	{
		ai->SetTarget(0);
		ai->SetFighting(false); 
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectFarthest", false);
	}

	Unit* target = targets[0];
	for (UINT32 i=0; i<targets.size(); i++)
	{
		if (XEntity::ValidEntity(targets[i]->GetID()) && unit->Distance(targets[i]) > dist)
		{
			dist = (float)unit->Distance(targets[i]);
			target = targets[i];
		}
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID() << "; dist " << dist << "; ");

	ai->SetTarget(target->GetID());
	data->SetTransformByIndex(XAIComponent::_target_index, target);
	data->SetFloatByIndex(m_targetDisIndex, dist);
	if (!ai->IsFighting())
	{
		XAIEnterFightEventArgs oArgs;
		oArgs.TargetID = target->GetID();
		unit->GetXObject()->DispatchAction(&oArgs);
	}
	ai->SetFighting(true);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectFarthest", true)
}

bool DetectEnimyInSight::Update( Unit* unit )
{
	XAIComponent* ai = unit->AIComponent();
	float enterFightRange = ai->EnterFightRange();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	bool bRes = false;
	//unit->GetCurrScene()->GetUnitByCondition(Cond, opponent);
	const std::vector<Unit*>* opponent = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());
	if (NULL != opponent)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << opponent->size() << " Opponents; ");
		for (UINT32 i = 0; i < opponent->size(); i++)
		{
			if (!XEntity::ValidEntity((*opponent)[i]->GetID())) continue;

			Vector3 dir = (*opponent)[i]->GetXObject()->GetPosition() - unit->GetXObject()->GetPosition();
			float distance = Vector3::sqrtMagnitude(dir);

			//一旦在视野范围，就激活仇恨列表
			if (distance <= enterFightRange * enterFightRange)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << (*opponent)[i]->GetID() << ", " << (*opponent)[i]->GetTemplateID() << "]; ");
				ai->EnemyDetected((*opponent)[i]);
				///> 只Notify第一个敌人，不然太耗了
				if (!bRes)
					ai->NotifyAllyIntoFight((*opponent)[i]);
				bRes = true;
			}

		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("DetectEnimyInSight", bRes)
}

SelectMoveTargetById::SelectMoveTargetById(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_MoveTarget");
	if (attr)
	{
		MoveTarget = attr->Value();
		if (MoveTarget != "")
			TargetIndex = AINodeFactory::CreateIndexByName(MoveTarget.c_str());
		else
			TargetIndex = 0;
	}

	attr = elem->FindAttribute("ObjectId");
	if (attr) attr->QueryIntValue(&ObjectId);
}

bool SelectMoveTargetById::Update(Unit* unit)
{
	std::vector<Unit*> opponent;
	std::vector<Unit*> targets;
	//FindOpponentCondition Cond(unit);
	FindAllyCondition allyCond(unit); 

	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();

	unit->GetCurrScene()->GetUnitByCondition(allyCond, opponent);

	for (UINT32 i=0; i<opponent.size(); i++)
	{
		if ((int)opponent[i]->GetTemplateID() == ObjectId)
			targets.push_back(opponent[i]);
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << targets.size() << " Opponents; ");
	
	if (targets.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectMoveTargetById", false);

	Unit* target = targets[0];
	float dist = 99999999.0f;
	for (UINT32 i=0; i<targets.size(); i++)
	{
		if (XEntity::ValidEntity(targets[i]->GetID()) && unit->Distance(targets[i]) <= dist)
		{
			dist = (float)unit->Distance(targets[i]);
			target = targets[i];
		}
	}	

	AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID() << "; dist " << dist << "; ");

	data->SetTransformByIndex(TargetIndex, target);

	AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectMoveTargetById", true)
}

SelectBuffTarget::SelectBuffTarget(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_BuffTarget");
	if (attr) 
	{
		BuffTarget = attr->Value();
		if (BuffTarget != "")
			BuffTargetIndex = AINodeFactory::CreateIndexByName(BuffTarget.c_str());
		else
			BuffTargetIndex = 0;
	}
}

bool SelectBuffTarget::Update(Unit* unit)
{
	std::list<PickServerModeDoodadInfo> buffDoodad;
	unit->GetCurrScene()->GetSceneDoodad()->GetServerDoodadsInfo(1, buffDoodad);
	DEFINE_AI_DEBUG_HELPER(this, unit);

	AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << buffDoodad.size() << " Doodads; ");

	if (buffDoodad.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectBuffTarget", false);

	SharedData* data = unit->AIData();

	float dist = 99999.0f;
	Vector3 finalPos;
	for (auto it = buffDoodad.begin(); it != buffDoodad.end(); it++)
	{
		float d = Vector3::Distance(it->pos, unit->GetXObject()->GetPosition_p());
		if ( d < dist)
		{
			dist = d;
			finalPos = it->pos;
		}
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Doodad Pos " << finalPos << "; dist " << dist << "; ");

	data->SetVector3ByIndex(BuffTargetIndex, finalPos);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectBuffTarget", true)
}

SelectItemTarget::SelectItemTarget(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_ItemTarget");
	if (attr) 
	{
		ItemTarget = attr->Value();

		if (ItemTarget != "")
			ItemTargetIndex = AINodeFactory::CreateIndexByName(ItemTarget.c_str());
		else
			ItemTargetIndex = 0;
	}
}

bool SelectItemTarget::Update(Unit* unit)
{
	std::list<PickServerModeDoodadInfo> itemDoodad;
	unit->GetCurrScene()->GetSceneDoodad()->GetServerDoodadsInfo(0, itemDoodad);
	DEFINE_AI_DEBUG_HELPER(this, unit);

	AI_DEBUG_STATEMENT(m_DebugInfo << "Get " << itemDoodad.size() << " Doodads; ");

	if (itemDoodad.size() == 0)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectItemTarget", false);

	float dist = 99999.0f;
	Vector3 finalPos;
	for (auto it = itemDoodad.begin(); it != itemDoodad.end(); it++) 
	{
		float d = Vector3::Distance(it->pos, unit->GetXObject()->GetPosition_p());
		if ( d < dist)
		{
			dist = d;
			finalPos = it->pos;
		}
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Doodad Pos " << finalPos << "; dist " << dist << "; ");

	unit->AIData()->SetVector3ByIndex(ItemTargetIndex, finalPos);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectItemTarget", true);
}

bool ResetHartedList::Update(Unit* unit)
{
	unit->AIComponent()->EnmityList().Reset();
	unit->AIComponent()->SetTarget(0);
	unit->AIComponent()->SetFighting(false);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("ResetHartedList", true);
}

CalDistance::CalDistance(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute("Shared_ObjectName");

	TargetIndex = -1;
	if (attr)
	{
		if (strcmp(attr->Value(), "") != 0)
			TargetIndex = AINodeFactory::CreateIndexByName(attr->Value());
	}

	DistanceIndex = -1;
	attr = elem->FindAttribute("Shared_DistanceName");
	if (attr)
	{
		if (strcmp(attr->Value(), "") != 0)
			DistanceIndex = AINodeFactory::CreateIndexByName(attr->Value());
	}

	attr = elem->FindAttribute("Shared_DistancemValue");
	if (attr)
		attr->QueryFloatValue(&Distance);


	attr = elem->FindAttribute("Shared_DestPointName");
	DestPointIndex = -1;
	if (attr && strcmp(attr->Value(), "") != 0) 
		DestPointIndex = AINodeFactory::CreateIndexByName(attr->Value());

	attr = elem->FindAttribute("Shared_DestPointmValue");

	if (attr)
		SharedData::ParseVector3FromString(attr->Value(), DestPoint);
}

bool CalDistance::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);
	SharedData* aidata = unit->AIData();
	float distance = 0;
	if (TargetIndex != -1)
	{
		Transform target = aidata->GetTransformByIndex(TargetIndex);

		if (target)
		{
			distance = (float)target->Distance(unit);
			AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID() << "; dist " << distance << "; ");
		}
	}
	else
	{
		Vector3 pos = DestPoint;
		if (DestPointIndex != -1)
			pos = aidata->GetVector3ByIndex(DestPointIndex);
		distance = Vector3::Magnitude(unit->GetXObject()->GetPosition() - pos);
		AI_DEBUG_STATEMENT(m_DebugInfo << "TargetPos " << pos << "; dist " << distance << "; ");
	}

	if (DistanceIndex != -1)
		aidata->SetFloatByIndex(DistanceIndex, distance);

	AI_DEBUG_RETURN_WITHOUT_DEFINE("CalDistance", true);
}

bool DoSelectRandomTarget::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();

	if (!ai)
		return false;
	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<Unit*>& targets = ai->GetTargets();
	AI_DEBUG_STATEMENT(m_DebugInfo << "Candidates count " << targets.size() << "; ");

	if (targets.size() == 0)
	{
		ai->SetTarget(0);
		ai->SetFighting(false);
		AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectRandomTarget", false);
	}

	int offset = XRandom::randInt()% targets.size();
	auto i = targets.begin();
	std::advance(i, offset);
	ai->SetTarget((*i)->GetID());

	AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << (*i)->GetID() << "; TempID " << (*i)->GetTemplateID() << "; ");

	if (!ai->IsFighting())
	{
		XAIEnterFightEventArgs oArgs;
		oArgs.TargetID = (*i)->GetID();
		unit->GetXObject()->DispatchAction(&oArgs);
	}
	ai->SetFighting(true);

	AI_DEBUG_RETURN_WITHOUT_DEFINE("DoSelectRandomTarget", true);
}
