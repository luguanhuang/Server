#include "pch.h"
#include "XFlyComponent.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "util/XRandom.h"
#include "scene/scene.h"
#include "scene/grid.h"

XFlyComponent::XFlyComponent(void)
	:_max(0),
	 _min(0),
	 _current(0),
	 _target(0)
{

}

XFlyComponent::~XFlyComponent(void)
{

}

void XFlyComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);

	_max = GetEntity()->GetUnit()->GetFloatingMax();
	_min = GetEntity()->GetUnit()->GetFloatingMin();

	_target = (_max - _min) * XRandom::randDouble(0, 1) + _min;
}

void XFlyComponent::Attached()
{
	Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();
	if(pScene)
	{
		_target += pScene->GetGrid()->GetHeight(GetEntity()->GetPosition_p().x, GetEntity()->GetPosition_p().z);
		_current = _target;

		GetEntity()->ApplyMove(0, _current - GetEntity()->GetPosition_p().y, 0);
	}
}

void XFlyComponent::Update(float fDeltaT)
{
	if(GetEntity()->GetUnit()->IsDead()) return;

	GetEntity()->DisableGravity();

	if(GetEntity()->SkillComponent()->IsCasting()) return;

	Scene* pScene = GetEntity()->GetUnit()->GetCurrScene();
	if(pScene)
	{
		float up = (_target - _current) * std::min(1.0f, 3 * fDeltaT);
		_current += up;

		GetEntity()->ApplyMove(0, up, 0);

		float y = pScene->GetGrid()->GetHeight(GetEntity()->GetPosition_p().x, GetEntity()->GetPosition_p().z);

		if (GetEntity()->GetPosition_p().y - y > _max || GetEntity()->GetPosition_p().y - y < _min)
		{
			_target = (_max - _min) * XRandom::randDouble(0, 1) + _min + y;
		}

		_target += (XRandom::randDouble(0, 1) > 0.5f) ? 0.03f : -0.03f;
		_current = GetEntity()->GetPosition_p().y;
	}
}
