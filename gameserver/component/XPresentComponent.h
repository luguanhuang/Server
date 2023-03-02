#ifndef __XPRESENTCOMPONENT_H__
#define __XPRESENTCOMPONENT_H__

#include "XComponent.h"
#include "table/XEntityPresentation.h"

class XObject;

class XPresentComponent : public XComponent
{
public:
	static const UINT32 uuID = XPresent_Component;

	XPresentComponent(void);
	virtual ~XPresentComponent(void);

	virtual UINT32 ID() { return XPresentComponent::uuID; }

	const string& SkillPrefix() const { return _skill_prefix; } 
	const string& CurvePrefix() const { return _curve_prefix; }

	virtual void OnAttachToHost(XObject* pHost);
	virtual void OnDetachFromHost();

	void ShowUp();
	bool OnTransform(UINT32 to);

	const XEntityPresentation::RowData* PresentLib() const { return _present_data; }

private:
	const XEntityPresentation::RowData* _present_data;

	string _skill_prefix;
	string _curve_prefix;
};

#endif	//__XPRESENTCOMPONENT_H__