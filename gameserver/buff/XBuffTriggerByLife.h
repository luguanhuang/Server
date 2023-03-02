#ifndef __XBUFFTRIGGERBYLIFE_H__
#define __XBUFFTRIGGERBYLIFE_H__

#include "XBuffTrigger.h"
#include "unit/combatattrdef.h"

class XBuffTriggerByLife : public XBuffTrigger
{
public:
	XBuffTriggerByLife(XBuff *pBuff);
	~XBuffTriggerByLife();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnAttributeChanged(CombatAttrDef attr);
	virtual bool CheckTriggerCondition();

private:
	float m_HPTriggerPercent;
};

#endif // __XBUFFTRIGGERBYLIFE_H__