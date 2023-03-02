#include "pch.h"
#include "XBuffTriggerWhenRemove.h"


XBuffTriggerWhenRemove::XBuffTriggerWhenRemove(XBuff *pBuff) : XBuffTrigger(pBuff)
{
}

XBuffTriggerWhenRemove::~XBuffTriggerWhenRemove()
{
}

void XBuffTriggerWhenRemove::OnRemove(XBuffRemoveReason reason)
{
	if (reason != XBuffRemoveReason_Replace)
		Trigger();
	XBuffTrigger::OnRemove(reason);
}
