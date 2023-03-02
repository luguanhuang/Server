#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"

void CustomBattleHandleDrop::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	if (!CCustomBattleConfig::Instance()->IsSystem(req.uid))
	{
		ErrorNotify(KKSG::ERR_FAILED, req.delayid);
		return;
	}
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	///> 没有加入过
	if (!roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}
	if (!roleinfo->DropSystem(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
	}
	else
	{
		DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
	}
	return;
}


