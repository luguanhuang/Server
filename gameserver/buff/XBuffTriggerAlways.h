#ifndef __XBUFFTRIGGERALWAYS_H__
#define __XBUFFTRIGGERALWAYS_H__

#include "XBuffTrigger.h"

class XBuffTriggerAlways : public XBuffTrigger, public ITimer
{
public:

	XBuffTriggerAlways(XBuff *pBuff);
	~XBuffTriggerAlways();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

	void OnTimer(UINT32 dwID, UINT32 dwCount);

	HTIMER m_timer;

};


#endif // __XBUFFTRIGGERBYBEHIT_H__