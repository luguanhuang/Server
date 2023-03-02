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

void CustomBattleHandleUnJoinSystem::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	///> 没有加入过
	if (!roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}

	// check valid
	errorcode = m_manager->TransReqToWorld(req);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleUnJoinSystem::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = HandleUnJoin(req);
	
	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
	}
	return;		
}

KKSG::ErrorCode CustomBattleHandleUnJoinSystem::HandleUnJoin(const CustomBattleReq& req)
{
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	///> 没有加入过
	if (!roleinfo->UnJoin(req.uid, true))
	{
		SSWarn<<"try unjoin uid:"<<req.uid<<" roleid"<<req.roleid<<END;
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	else
	{
		DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
		return KKSG::ERR_SUCCESS;
	}
}

