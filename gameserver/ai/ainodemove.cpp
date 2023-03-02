#include "pch.h"
#include <typeinfo>
#include "ainodemove.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "entity/XActionDefine.h"
#include "component/XAIComponent.h"
#include "aibehaviortree.h"
#include "component/XNavigationComponent.h"
#include "scene/scene.h"
#include "ainodefactory.h"
#include "scene/grid.h"
#include "scene/waypoint.h"
#include "LogInit.h"
#include "ainodecondition.h"
#include "aionlinedebugmgr.h"

ActionMove::ActionMove(const tinyxml2::XMLElement* elem)
{
	if (elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_MoveDirName");
		m_moveDirIndex = -1;
		if (attr)
		{
			m_sharedMoveDir = attr->Value();
			if (m_sharedMoveDir != "")
				m_moveDirIndex = AINodeFactory::CreateIndexByName(m_sharedMoveDir.c_str());
		}

		attr = elem->FindAttribute("Shared_MoveDestName");
		m_moveDestIndex = -1;
		if (attr)
		{
			m_sharedMoveDest = attr->Value();
			if (m_sharedMoveDest != "")
				m_moveDestIndex = AINodeFactory::CreateIndexByName(m_sharedMoveDest.c_str());
		}

		attr = elem->FindAttribute("Shared_MoveSpeedName");
		m_moveSpeedIndex = -1;
		if (attr)
		{
			m_sharedMoveSpeed = attr->Value();
			if (m_sharedMoveSpeed != "")
				m_moveSpeedIndex = AINodeFactory::CreateIndexByName(m_sharedMoveSpeed.c_str());
		}

	}
}

bool ActionMove::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("ActionMove", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();

	Vector3 moveDir = data->GetVector3ByIndex(m_moveDirIndex);
	Vector3 moveDest = data->GetVector3ByIndex(m_moveDestIndex);

	if( moveDest.x < 0 || 
		moveDest.x > 1024 || 
		moveDest.z < 0 ||
		moveDest.z > 1024 )
	{
		LogError("Unit [%llu] is searching to an error position x: %.3lf z: %.3lf", unit->GetID(), moveDest.x, moveDest.z);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Dest out of range: " << moveDest);
		AIRETURN("ActionMove", false);
	}

	float moveSpeed = data->GetFloatByIndex(m_moveSpeedIndex) * unit->AIComponent()->MoveSpeed();
	if (moveDest == Vector3::zero)
	{
		moveDest = unit->GetXObject()->GetPosition_p();
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Dest: " << moveDest << "; ");

	if (unit->GetXObject()->NavigationComponent() && unit->GetXObject()->NavigationComponent()->IsOnUse())
		unit->GetXObject()->NavigationComponent()->SetUseState(false);

	if (!unit->GetCurrScene()->TryMove(unit, moveDest-unit->GetXObject()->GetPosition_p()) && unit->GetXObject()->NavigationComponent())
	{
		if (unit->GetCurrScene()->GetWayPoint())	
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Start Navi");
			unit->GetXObject()->NavigationComponent()->SetUseState(true, moveDest);
		}
		else
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Start Move");
			XMoveActionArgs args;
			args.Speed = moveSpeed;
			args.Destination = moveDest;
			unit->GetXObject()->DispatchAction(&args);
		}
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Start Move");
		XMoveActionArgs args;
		args.Speed = moveSpeed;
		args.Destination = moveDest;
		unit->GetXObject()->DispatchAction(&args);
	}

	AIRETURN("ActionMove", true);
}

SetDest::SetDest(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_FinalDestName");
	m_finalDestIndex = -1;
	if (attr)
	{
		m_sharedFinalDest = attr->Value();

		if (m_sharedFinalDest != "")
			m_finalDestIndex = AINodeFactory::CreateIndexByName(m_sharedFinalDest.c_str());
	}
	
	attr = elem->FindAttribute("Shared_TargetName");
	m_targetIndex = -1;
	if (attr)
	{
		m_sharedTarget = attr->Value();
		if (m_sharedTarget != "")
			m_targetIndex = AINodeFactory::CreateIndexByName(m_sharedTarget.c_str());
	}

	attr = elem->FindAttribute("Shared_NavName");
	m_navIndex = -1;
	if (attr)
	{
		m_sharedNav = attr->Value();
		if (m_sharedNav != "")
			m_navIndex = AINodeFactory::CreateIndexByName(m_sharedNav.c_str());
	}

	attr = elem->FindAttribute("Shared_BornPosName");
	m_bornPosIndex = -1;
	if (attr) 
	{
		m_sharedBornPos = attr->Value();
		if (m_sharedBornPos != "")
			m_bornPosIndex = AINodeFactory::CreateIndexByName(m_sharedBornPos.c_str());
	}

	attr = elem->FindAttribute("Shared_BornPosmValue");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), m_BornPos);

	attr = elem->FindAttribute("Shared_TickCountName");
	m_tickCountIndex = -1;
	if (attr)
	{
		m_sharedTickCount = attr->Value();
		if (m_sharedTickCount != "")
			m_tickCountIndex = AINodeFactory::CreateIndexByName(m_sharedTickCount.c_str());
	}

	attr = elem->FindAttribute("RandomMax");
	if (attr) attr->QueryFloatValue(&m_RandomMax);

	attr = elem->FindAttribute("AdjustAngle");
	if (attr) attr->QueryFloatValue(&m_AdjustAngle);

	attr = elem->FindAttribute("Shared_AdjustLengthName");
	m_adjustLengthIndex = -1;
	if (attr)
	{
		m_sharedAdjustLengthName = attr->Value();
		if (m_sharedAdjustLengthName != "")
			m_adjustLengthIndex = AINodeFactory::CreateIndexByName(m_sharedAdjustLengthName.c_str());
	}

	attr = elem->FindAttribute("Shared_AdjustLengthmValue");
	if (attr) attr->QueryFloatValue(&m_sharedAdjustLength);

	attr = elem->FindAttribute("AdjustDir");
	if (attr) attr->QueryIntValue(&m_AdjustDir);

	attr = elem->FindAttribute("SetDestType");
	if (attr) attr->QueryIntValue(&m_AdjustSetDestType);
}

bool SetDest::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("SetDest", false);

	SharedData* data = unit->AIData();

	Vector3 finalDest = data->GetVector3ByIndex(m_finalDestIndex);
	Transform target = data->GetTransformByIndex(m_targetIndex);
	Transform nav = data->GetTransformByIndex(m_navIndex);
	Vector3 bornPos = data->GetVector3ByIndex(m_bornPosIndex);
	if (m_sharedBornPos == "") bornPos = m_BornPos;
	int tickCount = data->GetIntByIndex(m_tickCountIndex);
	float randomMax = m_RandomMax;
	float adjustAngle = m_AdjustAngle;
	float adjustLength = data->GetFloatByIndex(m_adjustLengthIndex, m_sharedAdjustLength);
	AdjustDirection setDestType = (AdjustDirection)m_AdjustDir;
	SetDestWay desttype = (SetDestWay)m_AdjustSetDestType;
	Grid* curGrid = unit->GetCurrScene()->GetGrid();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	switch (desttype)
	{
	case SetDest::Target:
		if (target == NULL)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; Failed");
			AIRETURN("SetDest", false);
		}
		finalDest = target->GetXObject()->GetPosition_p();
		break;
	case SetDest::BornPos:
		finalDest = bornPos;
		break;
	case SetDest::NavPos:
		if (nav == NULL)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "No Nav; Failed");
			AIRETURN("SetDest", false);
		}
		finalDest = nav->GetXObject()->GetPosition_p();
		break;
	default:
		break;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "OriginDest: " << finalDest << "; ");

	if (randomMax > 0)
		finalDest = finalDest + Vector3((float)XRandom::randDouble(-1, 1), 0, (float)XRandom::randDouble(-1, 1)) * randomMax;

	if (adjustLength != 0)
	{
		Vector3 vec = Vector3::zero;
		float height = 0;

		if (setDestType == SetDest::TargetDir)
		{
			vec = unit->GetXObject()->GetPosition_p() - finalDest;
			vec.Horizontal();
		}
		else if (setDestType == SetDest::TargetFace)
			vec = target->GetXObject()->GetFace_p();
		else if (setDestType == SetDest::SelfFace)
			vec = unit->GetXObject()->GetFace_p();


		Vector3 adjustDir = Vector3::HorizontalRotate(vec, (tickCount%2) * adjustAngle * 2 - adjustAngle);
		Vector3 pos = finalDest + Vector3::Normalized(adjustDir) * adjustLength;

		if(curGrid->TryGetHeight(pos.x, pos.z, height))
		{
			if (height < 0)
			{
				for (int i=0; i<9; i++)
				{
					float angle = adjustAngle + 20*i;
					adjustDir = Vector3::HorizontalRotate(vec, (tickCount%2) * angle * 2 - angle);
					pos = finalDest + Vector3::Normalized(adjustDir) * adjustLength;

					if (curGrid->TryGetHeight(pos.x, pos.z, height))
					{
						if (height >= 0)
						{
							break;
						}
					}

					angle = adjustAngle - 20*i;
					adjustDir = Vector3::HorizontalRotate(vec, (tickCount%2) * angle * 2 - angle);
					pos = finalDest + Vector3::Normalized(adjustDir) * adjustLength;

					if (curGrid->TryGetHeight(pos.x, pos.z, height))
					{
						if (height >= 0)
						{
							break;
						}
					}
				}
			}
		}

		finalDest = pos;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "FinalDest: " << finalDest << "; ");

	data->SetVector3ByIndex(m_finalDestIndex, finalDest);

	AIRETURN("SetDest", true);
}


NavToTarget::NavToTarget( const tinyxml2::XMLElement *elem )
{
	if(elem)
	{
		const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");
		m_targetIndex = -1;
		if(attr)
		{
			m_sharedTargetName = attr->Value();

			if (m_sharedTargetName != "")
				m_targetIndex = AINodeFactory::CreateIndexByName(m_sharedTargetName.c_str());
		}

		attr =  elem->FindAttribute("Shared_NavTargetName");
		m_navTargetIndex = -1;
		if(attr)
		{
			m_sharedNavTargetName = attr->Value();

			if (m_sharedNavTargetName != "")
				m_navTargetIndex = AINodeFactory::CreateIndexByName(m_sharedNavTargetName.c_str());
		}

		attr =  elem->FindAttribute("Shared_NavPosName");
		m_navPosIndex = -1;
		if(attr)
		{
			m_sharedNavPosName = attr->Value();

			if (m_sharedNavPosName != "")
				m_navPosIndex = AINodeFactory::CreateIndexByName(m_sharedNavPosName.c_str());
		}

		m_navPos = Vector3::zero;
		attr = elem->FindAttribute("Shared_NavPosmValue");
		if(attr)
			SharedData::ParseVector3FromString(attr->Value(), m_navPos);

	}
}

bool NavToTarget::Update( Unit* unit )
{
	if (!unit->AIComponent())
		return false;

	SharedData* data = unit->AIData();
	Transform target = data->GetTransformByIndex(m_targetIndex);
	
	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (target == NULL || !XEntity::ValidEntity(target->GetID()))
	{
		Transform nav_target = data->GetTransformByIndex(m_navTargetIndex);

		if (nav_target != NULL)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "NavTarget " << nav_target->GetID() << "; TempID " << nav_target->GetTemplateID(); "; ");
			AIRETURN("NavToTarget", _Do(unit, nav_target->GetXObject()->GetPosition_p()));
		}
		else
		{
			const Vector3& navPos = data->GetVector3ByIndex(m_navPosIndex);
			if (navPos == Vector3::zero)
			{
				if (unit->GetXObject()->NavigationComponent() && !unit->GetXObject()->NavigationComponent()->IsOnUse())
				{
					AI_DEBUG_STATEMENT("Just Start Navi");
					unit->GetXObject()->NavigationComponent()->SetUseState(true);
					AIRETURN("NavToTarget", true);
				}
			}
			else
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "NavPos " << navPos << "; ");
				AIRETURN("NavToTarget", _Do(unit, navPos));
			}
		}

		AIRETURN("NavToTarget", false);
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID(); "; ");
		AIRETURN("NavToTarget", _Do(unit, target->GetXObject()->GetPosition_p()));
	}

	//return true;
	AIRETURN("NavToTarget", true);
}

bool NavToTarget::_Do(Unit* pUnit, const Vector3 &des)
{
	AI_DEBUG_STATEMENT(m_DebugInfo << "Dest " << des << "; ");

	if (!pUnit->GetCurrScene()->TryMove(pUnit, des - pUnit->GetXObject()->GetPosition_p()) && pUnit->GetXObject()->NavigationComponent())
	{
		if (!pUnit->GetXObject()->NavigationComponent()->IsOnUse() || !pUnit->GetXObject()->NavigationComponent()->CheckMoveDest(des))
		{
			AI_DEBUG_STATEMENT("Start Navi");
			pUnit->GetXObject()->NavigationComponent()->SetUseState(true, des);
		}
	}
	else
	{
		XAIComponent* ai = pUnit->AIComponent();
		if (ai == NULL)
			return false;
		XMoveActionArgs args;
		args.Speed = ai->MoveSpeed();
		args.Destination = des;
		//return unit->GetXObject()->DispatchAction(&args);
		bool bRes = pUnit->GetXObject()->DispatchAction(&args);
		if (bRes)
		{
			AI_DEBUG_STATEMENT("Start Move");
		}

		if (bRes && pUnit->GetXObject()->NavigationComponent())
		{
			AI_DEBUG_STATEMENT("Stop Navi");
			///> 要把导航关了。不然会来回走
			pUnit->GetXObject()->NavigationComponent()->SetUseState(false);
	}
		return bRes;

	}

	return true;
}

void NavToTarget::Print()
{
	AINodeBase::Print();
	SSInfo << VAR_NAME(m_sharedTargetName) << " : " <<m_sharedTargetName << END;
	SSInfo << VAR_NAME(m_sharedNavTargetName) << " : " <<m_sharedNavTargetName << END;
}

ActionRotate::ActionRotate(const tinyxml2::XMLElement *elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("RotDegree");
	if (attr) attr->QueryFloatValue(&m_RotDegree);

	attr = elem->FindAttribute("RotSpeed");
	if (attr) attr->QueryFloatValue(&m_RotSpeed);

	attr = elem->FindAttribute("RotType");
	if (attr) attr->QueryIntValue(&m_RotType);
	else m_RotType = 0;
}

bool ActionRotate::Update(Unit* unit)
{
	const Vector3& rotate = Vector3::HorizontalRotate(m_RotType == 0 ? unit->GetXObject()->GetFace_p() : Vector3::forward, m_RotDegree);

	XRotationActionArgs rargs;
	rargs.TargetDir = rotate;
	rargs.Palstance = m_RotSpeed;
	unit->GetXObject()->DispatchAction(&rargs);

	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << rotate << ", Current: " << unit->GetXObject()->GetFace_p());
	return true;
}

bool RotateToTarget::Update(Unit* unit)
{
	UINT64 targetid = unit->AIComponent()->GetTarget();
	Unit* target = Unit::FindUnit(targetid);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (!target)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; Failed;");
		return false;
	}

	Vector3 rotate = target->GetXObject()->GetPosition() - unit->GetXObject()->GetPosition();

	XRotationActionArgs rargs;
	rargs.TargetDir = rotate;
	rargs.Palstance = unit->GetRotateSpeed();
	unit->GetXObject()->DispatchAction(&rargs);

	AI_DEBUG_STATEMENT(m_DebugInfo << rotate << "; ");
	AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << target->GetID() << "; TempID " << target->GetTemplateID(); "; ");
	return true;
}

ConditionCanReach::ConditionCanReach(const tinyxml2::XMLElement *elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TemplateidName");
	m_TemplateIdIndex = -1;
	if (attr) 
	{
		string name = attr->Value();
		if (name != "")
			m_TemplateIdIndex = AINodeFactory::CreateIndexByName(name.c_str());
	}

	attr = elem->FindAttribute("Shared_TemplateidmValue");
	if (attr) attr->QueryIntValue(&m_TemplateId);

	attr = elem->FindAttribute("Shared_DestPosName");
	m_DestPosIndex = -1;
	if (attr)
	{
		string posname = attr->Value();
		if (posname != "")
			m_DestPosIndex = AINodeFactory::CreateIndexByName(posname.c_str());
	}

	attr = elem->FindAttribute("Shared_DestPosmValue");
	if (attr)SharedData::ParseVector3FromString(attr->Value(), m_DestPos);

}

bool ConditionCanReach::Update(Unit* unit)
{
	SharedData* data = unit->AIData();
	Vector3 dest = m_DestPos;
	if (m_DestPosIndex != -1) dest = data->GetVector3ByIndex(m_DestPosIndex);

	DEFINE_AI_DEBUG_HELPER(this, unit);
	AI_DEBUG_STATEMENT(m_DebugInfo << dest << "; ");
	AI_DEBUG_RETURN_WITHOUT_DEFINE("ConditionCanReach", unit->GetCurrScene()->TryMove(unit, dest-unit->GetXObject()->GetPosition_p()));
}

Navigation::Navigation(const tinyxml2::XMLElement *elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_MoveDirName");

	m_MoveDirOld = 1;
	m_MoveDirIndex = -1;
	if (attr)
	{
		m_MoveDirName = attr->Value();
		if (m_MoveDirName.length() != 0)
			m_MoveDirIndex = AINodeFactory::CreateIndexByName(m_MoveDirName);
	}

	attr = elem->FindAttribute("IsGoForward");
	if (attr)
		m_IsGoForward = attr->BoolValue();
	else
		m_IsGoForward = false;

	attr = elem->FindAttribute("PatrolPointRadius");
	if (attr)
		m_PatrolPointRadius = attr->FloatValue();
	else
		m_PatrolPointRadius = 0.5f;
	if (m_PatrolPointRadius < 0.5f)
		m_PatrolPointRadius = 0.5f;

	m_PatrolPointRadius *= m_PatrolPointRadius;
}

bool Navigation::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	SharedData* data = unit->AIData();
	if (data == NULL || ai == NULL)
		return false;
	XPatrol& patrol = ai->GetPatrol();
	Vector3 curNavPos = patrol.GetCurNavPos();

	if (curNavPos == Vector3::zero)
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	int oldDir = m_MoveDirOld;
	m_MoveDirOld = data->GetIntByIndex(m_MoveDirIndex, 1);

	const Vector3& moveDir = curNavPos - unit->GetXObject()->GetPosition_p();

	if (oldDir != m_MoveDirOld)
	{
		patrol.ToggleNavDir();
		patrol.GetNextNavPos();

		AI_DEBUG_STATEMENT(m_DebugInfo << "Toggle Dir " << m_MoveDirOld <<"; ");
	}
	else if (Vector3::sqrtMagnitude(moveDir) <= m_PatrolPointRadius || patrol.GetIsInNavGap())
	{
		if (patrol.GetIsInNavGap())
		{
			UINT64 navidletime = TimeUtil::GetMilliSecond() - patrol.GetNavNodeFinishTime();

			if (navidletime < (UINT64)patrol.GetNavGap()*1000)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "In Gap; ");
				return true;
			}
			else
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "End Gap; ");
				curNavPos = patrol.GetNextNavPos();
			}
		}
		else
		{
			if (patrol.GetNavGap() > 0)
			{
				patrol.SetNavNodeFinishTime(TimeUtil::GetMilliSecond());
				patrol.SetIsInNavGap(true);
				
				AI_DEBUG_STATEMENT(m_DebugInfo << "Begin Gap; ");
				return true;
			}
			else
				curNavPos = patrol.GetNextNavPos();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if (m_IsGoForward)
		curNavPos = patrol.GetNextForwardNavPos(unit->GetXObject()->GetPosition_p());
	//////////////////////////////////////////////////////////////////////////

	patrol.SetIsInNavGap(false);

	AI_DEBUG_STATEMENT(m_DebugInfo << "Dest: " << curNavPos << " from" << unit->GetXObject()->GetPosition_p() << "; ");

	if (!unit->GetCurrScene()->TryMove(unit, curNavPos-unit->GetXObject()->GetPosition_p()) && unit->GetXObject()->NavigationComponent())
	{
		if (!unit->GetXObject()->NavigationComponent()->IsOnUse() || !unit->GetXObject()->NavigationComponent()->CheckMoveDest(curNavPos))
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Start Navi; ");

			unit->GetXObject()->NavigationComponent()->SetUseState(true, curNavPos);
		}
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Start Move ");

		XMoveActionArgs args;
		args.Speed = unit->GetXObject()->AIComponent()->MoveSpeed();
		args.Destination = curNavPos;
		args.StopTowards = moveDir;
		//LogInfo("The dest pos[%f, %f, %f]", curNavPos.x, curNavPos.y, curNavPos.z);
		//return unit->GetXObject()->DispatchAction(&args);
		//AIRETURN("NavToTarget", unit->GetXObject()->DispatchAction(&args));
		bool bRes = unit->GetXObject()->DispatchAction(&args);

		AI_DEBUG_STATEMENT(m_DebugInfo << (bRes ? "Success; " : "Failed; "));

		if (bRes && unit->GetXObject()->NavigationComponent())
		{
			///> 要把导航关了。不然会来回走
			unit->GetXObject()->NavigationComponent()->SetUseState(false);

			AI_DEBUG_STATEMENT(m_DebugInfo << "Stop Navi; ");
		}
		AIRETURN("NavToTarget", bRes);
	 }
	 
	return true;
}

MoveStratage::MoveStratage(const tinyxml2::XMLElement *elem)
{
	TargetIndex = -1;
	Stratage = -1;
	DestIndex = -1;
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_TargetName");

	if (attr)
	{
		if (strcmp(attr->Value(), "") != 0)
			TargetIndex = AINodeFactory::CreateIndexByName(attr->Value());
	}

	attr = elem->FindAttribute("StratageIndex");
	if (attr) attr->QueryIntValue(&Stratage);

	attr = elem->FindAttribute("Shared_FinalDestName");
	if (attr)
	{
		if (strcmp(attr->Value(), "") != 0)
			DestIndex = AINodeFactory::CreateIndexByName(attr->Value());
	}
}

bool MoveStratage::Update(Unit* unit)
{
	if (TargetIndex == -1 || Stratage == -1 || DestIndex == -1)
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	WayPoint* wayPoint = unit->GetCurrScene()->GetWayPoint();
	if (!wayPoint)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No WayPoint; False; ");
		return false;
	}

	Transform target = unit->AIData()->GetTransformByIndex(TargetIndex);

	if (!target)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; False; ");
		return false;
	}

	int index = -1;
	float distance = 99999;
	for (int i=0; i<= wayPoint->GetLastPointIndex(); i++)
	{
		Vector3 curPoint = wayPoint->GetPoint(i);

		//走不过去的点略过
		if (!unit->GetCurrScene()->TryMove(unit, curPoint-unit->GetXObject()->GetPosition_p()))
		{
			//LogInfo("Skip index[%d]", i);
			continue;
		}

		Vector3 targetDir = unit->GetXObject()->GetPosition_p() - target->GetXObject()->GetPosition_p();
		Vector3 destDir = curPoint - unit->GetXObject()->GetPosition_p();
		destDir.y = 0; targetDir.y = 0;
		float d = Vector3::Magnitude(destDir);

		targetDir.Normalize();
		destDir.Normalize();

		float dotf = Vector3::Dot(targetDir, destDir);
		if (dotf < 0)
		{
			//LogInfo("Skip dir[%d]", i);
			continue;
		}
		else
			d -= dotf;

		if (d < 0.1f)
		{
			//LogInfo("continue dotf[%d]", i);
			continue;
		}

		if (d < distance)
		{
			distance = d;
			index = i;
		}
	}

	Vector3 pos = unit->GetXObject()->GetPosition_p();
	Vector3 destpos = wayPoint->GetPoint(index);
	//LogInfo("pos[%f, %f, %f], dest[%f, %f, %f], index[%d]", pos.x, pos.y, pos.z, destpos.x, destpos.y, destpos.z, index);

	if (index != -1)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Index " << index << "; " << "Pos " << destpos);
		unit->AIData()->SetVector3ByIndex(DestIndex, destpos);
	}
	else
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "Proper Point Not Found");
	}

	return true;
}