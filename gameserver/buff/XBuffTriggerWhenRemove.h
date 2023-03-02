#ifndef __XBUFFTRIGGERWHENREMOVE_H__
#define __XBUFFTRIGGERWHENREMOVE_H__

#include "XBuffTrigger.h"

class XBuffTriggerWhenRemove : public XBuffTrigger
{
public:

	XBuffTriggerWhenRemove(XBuff *pBuff);
	~XBuffTriggerWhenRemove();

	virtual void OnRemove(XBuffRemoveReason reason);
};


#endif // __XBUFFTRIGGERWHENREMOVE_H__