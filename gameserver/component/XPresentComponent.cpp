#include "pch.h"
#include "XPresentComponent.h"
#include "entity/XObject.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "skill/XSkillMgr.h"
#include "XSkillComponent.h"
#include "unit/unit.h"

XPresentComponent::XPresentComponent(void)
	:_present_data(NULL),
	 _skill_prefix(""),
	 _curve_prefix("")
{

}

XPresentComponent::~XPresentComponent(void)
{
	_present_data = NULL;
}

void XPresentComponent::OnAttachToHost(XObject* pHost)
{
	XComponent::OnAttachToHost(pHost);

	_present_data = XEntityInfoLibrary::Instance()->PresentID2Data(GetHost()->GetUnit()->GetPresentID());

	_skill_prefix = "SkillPackage/" + _present_data->SkillLocation;
	_curve_prefix = "Curve/" + _present_data->CurveLocation;
}

bool XPresentComponent::OnTransform(UINT32 to)
{
	/*if(to == 0)
	{
		_skill_prefix = "SkillPackage/" + _present_data->SkillLocation;
	}
	else
	{
		const XEntityStatistics::RowData* data = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(to);

		if(data)
		{
			const XEntityPresentation::RowData* present_data = XEntityInfoLibrary::Instance()->PresentID2Data(data->PresentID);
			_skill_prefix = "SkillPackage/" + present_data->SkillLocation;
		}
		else
			return false;
	}*/

	return true;
}

void XPresentComponent::OnDetachFromHost()
{
	_present_data = NULL;
}

void XPresentComponent::ShowUp()
{
	if(!GetEntity()->GetUnit()->IsRole() && GetEntity()->SkillComponent()->SkillManager()->GetAppearIdentity() != 0)
	{
		XAttackActionArgs attck_arg;

		attck_arg.Identify = GetEntity()->SkillComponent()->SkillManager()->GetAppearIdentity();

		attck_arg.Target = NULL;

		GetEntity()->DispatchAction(&attck_arg);
	}
}