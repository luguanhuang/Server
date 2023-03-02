#ifndef __XNETACTOINRECEIVER_H__
#define __XNETACTOINRECEIVER_H__

#include "math/XVector3.h"

class Role;

class XActionReceiver
{
public:
	static void OnMoveReceived(Role* pRole, float velocity, float face, float x, float z, bool interia);
	static void OnMoveReceived(Role* pRole, float velocity, float face, float x, float z, float desx, float desz, bool interia);
	static void OnSkillReceived(Role* pRole, UINT64 target, UINT32 skill, INT32 slot = -1, bool hasmanualface = false, float manualface = 0);
	static void OnSkillSlotReceived(Role* pRole, UINT64 target, INT32 slot, bool hasmanualface = false, float manualface = 0);

private:
	static bool IsValid(Role* pRole);
};

#endif	//__XNETACTOINRECEIVER_H__