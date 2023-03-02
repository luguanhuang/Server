#ifndef __XBUFFTRIGGERQTE_H__
#define __XBUFFTRIGGERQTE_H__

#include "XBuffTrigger.h"

class XBuffTriggerByQTE: public XBuffTrigger
{
public:

	XBuffTriggerByQTE(XBuff *pBuff);
	~XBuffTriggerByQTE();

	bool CheckTriggerCondition();

	virtual void OnQTEStateChanged(const XOnQTEOnEventArgs* arg);
private:
	std::set<UINT32> m_QTEStates;
	UINT64 m_PreToken;
};


#endif // __XBUFFTRIGGERQTE_H__