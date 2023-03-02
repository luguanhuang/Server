#ifndef __XBUFFTRIGGERBYSTACKING_H__
#define __XBUFFTRIGGERBYSTACKING_H__

#include "XBuffTrigger.h"

class XBuffTriggerByStacking : public XBuffTrigger
{
public:

	XBuffTriggerByStacking(XBuff *pBuff);
	~XBuffTriggerByStacking();

	virtual void OnAppend(XEntity*pEntity);
	virtual bool CheckTriggerCondition();

private:
	UINT32 m_Base;
};


#endif // __XBUFFTRIGGERBYCOMBO_H__