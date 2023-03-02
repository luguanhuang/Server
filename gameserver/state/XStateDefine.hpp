#ifndef __IXSTATEDEFINE_H__
#define __IXSTATEDEFINE_H__

#include "pch.h"

enum XStateDefine
{
	XState_Idle = 0,
	XState_Move,
	XState_Jump,
	XState_Fall,
	XState_Freeze,
	XState_BeHit,
	XState_Death,
	XState_Charge,
	XState_Skill,
	XState_PushOut,

	XState_Max
};

class XStateMgr
{
public:
	static bool IsMoveState(XStateDefine state)
	{
		return state == XState_Move;
	}

	static bool IsUnBattleState(XStateDefine state)
	{
		return state == XState_Idle ||
			IsMoveState(state);
	}

	static bool IsAirState(XStateDefine state)
	{
		return state == XState_Fall || 
			state == XState_Jump;
	}

	static bool IsStunState(XStateDefine state)
	{
		return state == XState_Freeze;
	}

	static bool IsUnControlledState(XStateDefine state)
	{
		return IsStunState(state) || 
			state == XState_BeHit ||
			state == XState_Death;
	}
};

#endif	//__IXSTATEDEFINE_H__