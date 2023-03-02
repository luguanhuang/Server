#ifndef __XSUPERARMORCOMPONENT_H__
#define __XSUPERARMORCOMPONENT_H__

#include "XComponent.h"
#include "timer.h"

class Enemy;

class XSuperArmorComponent : public XComponent, ITimer
{
public:

	static const UINT32 uuID = XSuperArmor_Component;

	XSuperArmorComponent();
	~XSuperArmorComponent();

	void SetRecoveryTimeLimit(double second) { m_recoveryTimeLimit = (float)second; }

	virtual UINT32 ID() { return XSuperArmorComponent::uuID; }
	virtual void Attached();

	virtual void OnDetachFromHost();
	virtual void Update(float fDeltaT);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool OnWoozyOffEvent(IArgs* pargs, void *);
	bool OnWoozyEvent(IArgs* pargs, void *);

protected:
	virtual void ActionSubscribe();

private:
	void CheckSuperArmorBroken();
	void RecoverySuperArmor(float fDeltaT);
	double GetRateByState();

	HTIMER m_handler;
	bool   m_WoozyOn;
	float  m_recoveryTimeLimit;
	Enemy *m_pEnemy;
};

#endif // __XSUPERARMORCOMPONENT_H__