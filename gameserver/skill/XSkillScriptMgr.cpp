#include "pch.h"
#include "XSkillScriptMgr.h"

#include "Player_warrior_dash_start.h"
#include "Player_warrior_charge.h"
#include "Player_cleric_attack_chongjizhishou.h"
#include "Player_cleric_attack_chongjizhishou_ex.h"
#include "Player_assassin_dash.h"
#include "Player_academic_buff_fuhuo.h"

#define FuncBind(__classname__, __funcname__)	\
	if(classname == #__classname__ && funcname == #__funcname__)	\
	return __classname__::__funcname__

pSkillScript XSkillScriptMgr::GetScriptFunc(string classname, string funcname)
{
	FuncBind(Player_warrior_charge, checkInput);
	FuncBind(Player_warrior_dash_start, canfirecharge);
	FuncBind(Player_cleric_attack_chongjizhishou, CallBomber);
	FuncBind(Player_cleric_attack_chongjizhishou_ex, CallBomber);
	FuncBind(Player_assassin_dash, PowerDash);
	FuncBind(Player_academic_buff_fuhuo, ReviveAlly);

	return 0;
}