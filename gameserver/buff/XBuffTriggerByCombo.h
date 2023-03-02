#ifndef __XBUFFTRIGGERBYCOMBO_H__
#define __XBUFFTRIGGERBYCOMBO_H__

#include "XBuffTrigger.h"

class XBuffTriggerByCombo : public XBuffTrigger
{
public:

	XBuffTriggerByCombo(XBuff *pBuff);
	~XBuffTriggerByCombo();

	virtual void OnComboChange(UINT32 comboCount);
	virtual bool CheckTriggerCondition();

private:
	UINT32 m_Base;
	UINT32 m_Count;
};


#endif // __XBUFFTRIGGERBYCOMBO_H__