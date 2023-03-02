#ifndef __XBUFFTRIGGERNEARBYMATES_H__
#define __XBUFFTRIGGERNEARBYMATES_H__

#include "XBuffTrigger.h"

class Role;
class Unit;

class XBuffTriggerNearbyMates : public XBuffTrigger, public ITimer
{
public:

	XBuffTriggerNearbyMates(XBuff *pBuff);
	~XBuffTriggerNearbyMates();

	virtual void OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper);
	virtual void OnRemove(XBuffRemoveReason reason);

	void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual bool CheckTriggerCondition();

private:
	HTIMER m_timer;
	float m_Radius;
	float m_SqrRadius;

	Unit* m_pUnit;
	std::vector<Unit*> m_Mates;
};


#endif // __XBUFFTRIGGERBYBEHIT_H__