#include "pch.h"
#include "util/XCommon.h"
#include "entity/XObject.h"
#include "XManipulationComponent.h"
#include "skill/XSkillCore.h"
#include "XSkillComponent.h"
#include "entity/XEntity.h"
#include "state/XStateDefine.hpp"
#include "state/XStateMachine.h"
#include "XNetComponent.h"
#include "skill/XSkill.h"
#include "scene/scene.h"

XManipulationComponent::XManipulationComponent(void)
{

}

XManipulationComponent::~XManipulationComponent(void)
{

}

void XManipulationComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);
}

void XManipulationComponent::OnDetachFromHost()
{
	_item.clear();
	XComponent::OnDetachFromHost();
}

void XManipulationComponent::ActionSubscribe()
{
	RegisterAction(XAction_Manipulation_On, DelegateCombination(XManipulationComponent, ManipulationOn, this));
	RegisterAction(XAction_Manipulation_Off, DelegateCombination(XManipulationComponent, ManipulationOff, this));
}

bool XManipulationComponent::ManipulationOn(IArgs* pargs, void *)
{
	XManipulationOnArgs* args = static_cast<XManipulationOnArgs*>(pargs);
	if(_item.find((int)(args->GetToken())) == _item.end())
	{
		_item[(int)(args->GetToken())] = args->Data();
	}

	return true;
}

bool XManipulationComponent::ManipulationOff(IArgs* pargs, void *)
{
	XManipulationOffArgs* args = static_cast<XManipulationOffArgs*>(pargs);

	if(args->DenyToken == 0) _item.clear();
	else
	{
		auto it = _item.find(args->DenyToken);
		if(it != _item.end())
		{
			_item.erase(it);
		}
	}

	return true;
}

void XManipulationComponent::Update(float fDeltaT)
{
	if (_item.empty()) return;

	Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();

	if(pScene != NULL)
	{
		//if(pSet)
		{
			auto it = _item.begin();
			while(it != _item.end())
			{
				const std::vector<Unit*>* pSet = it->second->TargetIsOpponent ? 
					pScene->GetOpponents(GetEntity()->GetUnit()->GetFightGroup()) : 
					pScene->GetAllys(GetEntity()->GetUnit()->GetFightGroup());
				if (!pSet)
					continue;

				Vector3 offset(it->second->OffsetX, 0, it->second->OffsetZ);

				float yRot = Vector3::Angle(Vector3::forward, GetEntity()->GetFace_p());
				if(!Vector3::Clockwise(Vector3::forward, GetEntity()->GetFace_p())) yRot = -yRot;

				const Vector3& center = GetEntity()->GetPosition_p() + Vector3::HorizontalRotate(offset, yRot, false);

				auto itor = (*pSet).begin();
				while(itor != (*pSet).end())
				{
					XEntity* x = XEntity::ValidEntity((*itor)->GetID());
					if(x && x != GetEntity())
					{
						Vector3 gap = center - x->GetPosition_p(); gap.y = 0;
						float dis = Vector3::Magnitude(gap);

						if (dis < it->second->Radius && (dis == 0 || Vector3::Angle(-gap, GetEntity()->GetFace()) <= it->second->Degree * 0.5f))
						{
							float len = it->second->Force * fDeltaT;

							x->ApplyMove(Vector3::Normalized(gap) * std::min(dis, len));
							if(x->NetComponent()) x->NetComponent()->Passive(true);
						}
					}

					++itor;
				}

				++it;
			}
		}
	}
}