#ifndef __XBUFFTRIGGERBYDEATH_H__
#define __XBUFFTRIGGERBYDEATH_H__

#include "XBuffTrigger.h"

class XBuffTriggerByDeath : public XBuffTrigger
{
public:

	XBuffTriggerByDeath(XBuff *pBuff);
	~XBuffTriggerByDeath();

	virtual bool CheckTriggerCondition();
	virtual void OnRealDead(const XRealDeadActionArgs* arg);

protected:
	virtual void _OnTrigger();

private:
	int m_Type;
	int m_Param0;
};


#endif // __XBUFFTRIGGERBYDEATH_H__