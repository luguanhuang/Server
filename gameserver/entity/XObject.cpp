#include "pch.h"
#include "XObject.h"
#include "component/XComponent.h"
#include "component/XIdleComponent.h"
#include "component/XRotationComponent.h"
#include "component/XNetComponent.h"
#include "scene/scene.h"
#include "util/XCommon.h"
#include "battle/ptcg2c_ontransferwall.h"
#include "scene/grid.h"
#include "LogInit.h"

XObject::XObject(Unit* pUnit)
	:	_pSoul(pUnit), 
		_pmachine(NULL),
		_pskill(NULL),
		_pbuff(NULL),
		_pnet(NULL),
		_ppresent(NULL),
		_protate(NULL),
		_phit(NULL),
		_pqte(NULL),
		_ai(NULL),
		_pnavigation(NULL),
		_pfly(NULL),
		_position(Vector3::zero), 
		_face_degree(0), 
		_face(Vector3::forward),
		_mob_shieldable(false)
{
	memset(_actions, 0, sizeof(UINT32) * XAction_Max * XComponent_Max);
	memset(_components, NULL, sizeof(XComponent*) * XComponent_Max);
	memset(_components_idx, NULL, sizeof(XComponent*) * XComponent_Max);
}

XObject::~XObject()
{
	for(int i = XComponent_Max - 1; i >= 0; i--)
	{
		if(_components[i]) DetachComponent(_components[i]->ID());
		_components[i] = NULL;

		_components_idx[i] = NULL;
	}

	_pSoul = 0;

	_pmachine = NULL;
	_pskill = NULL;
	_pnet = NULL;
	_pbuff = NULL;
	_ppresent = NULL;
	_phit = NULL;
	_pfly = NULL;
	_pnavigation = NULL;
	_ai = NULL;
	_pqte = NULL;
}

bool XObject::Initilize()
{
	return true;
}

void XObject::Uninitilize()
{
	if(_pskill && _pskill->IsCasting()) _pskill->EndSkill();
	if(_pmachine) _pmachine->ForceToDefaultState(false);
}

void XObject::OnCreated()
{
	for(int i = 0; i < XComponent_Max; i++)
	{
		if(_components[i])
		{
			_components[i]->Attached();
		}
		else
			break;
	}
}

void XObject::OnEnterScene(Scene *scene)
{

}

void XObject::OnSceneReady()
{

}

void XObject::OnLeaveScene()
{

}

void XObject::Update(float fDeltaT)
{
	if(_pnet)
	{
		_pnet->Passive(Has_H_Movement());
	}

	for(int i = 0; i < XComponent_Max; i++)
	{
		if(_components[i])
		{
			if(_components[i]->IsEnabled()) _components[i]->Update(fDeltaT);
		}
		else
			break;
	}
}

/*XComponent* XObject::GetXComponent(UINT32 uuid)
{
	for(int i = 0; i < XComponent_Max; i++)
	{
		if(_components[i])
		{
			if(_components[i]->ID() == uuid)
				return _components[i];
		}
		else
			return NULL;
	}

	return NULL;
}*/

XComponent* XObject::AttachComponent(XComponent* pComponentObject)
{
	if(!pComponentObject) return NULL;

	UINT32 uID = pComponentObject->ID();
	//detached older one if has
	DetachComponent(uID);
	
	int i = 0;
	for(; i < XComponent_Max; i++)
	{
		if(!_components[i])
		{
			_components[i] = pComponentObject;
			_components_idx[uID] = pComponentObject;

			break;
		}
	}
	if(i == XComponent_Max)
	{
		LogError("Entity [%llu] failed to add Component %d due to reach the MAXCOUNT.", GetID(), uID);
	}
	else
	{
		pComponentObject->OnAttachToHost(this);
	}
	
	return pComponentObject;
}

void XObject::DetachComponent(UINT32 uuid)
{
	int i = 0;
	for(; i < XComponent_Max; i++)
	{
		if(_components[i])
		{
			if(_components[i]->ID() == uuid)
			{
				_components_idx[uuid] = NULL;
				break;
			}
		}
		else
			break;
	}

	if(i < XComponent_Max && _components[i])
	{
		_components[i]->OnDetachFromHost();
		delete _components[i];

		for (int j = i; j < XComponent_Max; j++)
		{
			if(j + 1 < XComponent_Max)
			{
				_components[j] = _components[j + 1];
				if(!_components[j]) break;
			}
			else
			{
				_components[j] = NULL;
			}
		}
	}
}

void XObject::AttachAction(XActionDefine action, UINT32 componentid)
{
	XComponent* c = GetXComponent(componentid);
	if(c && action >= 0 && action < XAction_Max)
	{
		int i = 0;
		for(; i < XComponent_Max; i++)
		{
			if(!_actions[action][i])
			{
				_actions[action][i] = componentid;
				break;
			}
			else if(_actions[action][i] == componentid)
			{
				break;
			}
		}
	}
}

void XObject::DetachAction(XActionDefine action, UINT32 componentid)
{
	XComponent* c = GetXComponent(componentid);
	if(c && action >= 0 && action < XAction_Max)
	{
		int i = 0;
		for(; i < XComponent_Max; i++)
		{
			if(_actions[action][i])
			{
				if(_actions[action][i] == componentid)
					break;
			}
			else
				break;
		}

		if(i < XComponent_Max && _actions[action][i])
		{
			for (int j = i; j < XComponent_Max; j++)
			{
				if(j + 1 < XComponent_Max)
				{
					_actions[action][j] = _actions[action][j + 1];
					if(!_actions[action][j]) break;
				}
				else
				{
					_actions[action][j] = NULL;
				}
			}
		}
	}
}

HTIMER XObject::DelayAction(IArgs* pargs, float delay)
{
	return XCommon::SetTimer(delay, DelegateCombination(XObject, OnAction, this), pargs, __FILE__, __LINE__);
}

bool XObject::OnAction(IArgs* pargs, void *)
{ 
	XActionArgs* args = static_cast<XActionArgs*>(pargs);
	return DispatchAction(args);
}

bool XObject::DispatchAction(XActionArgs* args)
{
	bool bSucceed = false;

	int def = args->ArgsDefine();
	if(def < 0 || def >= XAction_Max) return false;

	for(int i = 0; i < XComponent_Max; i++)
	{
		if(!_actions[def][i]) break;

		XComponent* pComponent = GetXComponent(_actions[def][i]);

		if(pComponent != NULL && pComponent->IsEnabled())
		{
			bool ret = pComponent->OnReceiveAction(args);
			bSucceed = ret || bSucceed;
		}
	}

	return bSucceed;
}

void XObject::TransferLocation(const Vector3& pos, float face)
{
	Idled();

	SetPosition(pos);
	SetFace(face);
	
	if(GetUnit()->IsRole())
	{
		///> 更新视野格子
		if (NULL != _pSoul->GetCurrScene())
		{
			_pSoul->GetCurrScene()->UpdateViewGrid(_pSoul);
		}

		if(_pnet) _pnet->BroadcastCorrectLocation(pos, face, true, false);

		PtcG2C_OnTransferWall transfer;
		GetUnit()->Send(transfer);

		if(_pskill)
		{
			//transfer mobbed units
			auto it = _pskill->GetSkillMobs().begin();
			while (it != _pskill->GetSkillMobs().end())
			{
				XEntity* mob = XEntity::ValidEntity(*it);
				if(mob && mob->GetUnit()->GetRunSpeed() > 0)
				{
					mob->Idled();

					mob->SetPosition(pos);
					mob->SetFace(face);
				}

				++it;
			}
		}
	}
}

void XObject::CorrectLocation()
{
	Scene* pScene = GetUnit()->GetCurrScene();
	if(pScene)
	{
		Idled();
		pScene->UpdateViewGrid(_pSoul);

		if(_pnet) _pnet->BroadcastCorrectLocation(_position, _face_degree, false, false);
	}
}

void XObject::CorrectLocation(const Vector3& pos, float face, bool toall)
{
	Scene* pScene = GetUnit()->GetCurrScene();
	if(pScene)
	{
		float y = 0;
		if(pScene->GetGrid()->TryGetHeight(pos.x, pos.z, y))
		{
			if(y >= 0)
			{
				Idled();

				SetPosition(pos.x, y, pos.z);
				SetFace(face);
				
				pScene->UpdateViewGrid(_pSoul);

				if(_pnet) _pnet->BroadcastCorrectLocation(_position, _face_degree, false, toall);
			}
		}
	}
}

void XObject::SetPosition_s(const Vector3& pos)
{
	Scene* pScene = _pSoul->GetCurrScene();
	if(pScene != NULL) pScene->SetPosition(_pSoul, pos);
}

void XObject::SetFace(float degree)
{
	InnerSetFace(degree);

	if(_protate) _protate->Cancel();
}

void XObject::SetFace(const Vector3& face)
{
	InnerSetFace(face);

	if(_protate) _protate->Cancel();
}

void XObject::InnerSetFace(float degree)
{
	_face_degree = XRoundToCircle(degree);
	_face = Vector3::FloatToAngle(_face_degree);
}

void XObject::InnerSetFace(const Vector3& face)
{
	_face = Vector3::Normalized(face);
	if(_face.IsZero()) _face = Vector3::forward;

	_face_degree = Vector3::AngleToFloat(_face);
}