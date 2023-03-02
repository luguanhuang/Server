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

void CustomBattleHandleModify::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (req.uid != roleinfo->GetJoinId())
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_NOTJOIN, req.delayid);
		return;
	}
	CustomBattle* battle = CustomBattleMgr::Instance()->GetBattle(roleinfo->GetJoinId());
	if (NULL == battle)
	{
		errorcode = m_manager->TransReqToWorld(req);
	}
	else
	{
		if (battle->GetCreator() == req.roleid)
		{
			if (KKSG::CustomBattle_Modify == req.op)
			{
				errorcode = CCustomBattleConfig::Instance()->ValidPassword(req.password);
				if (KKSG::ERR_SUCCESS == errorcode)
				{
					battle->SetPassword(req.password);
					DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
				}
			}
			else if (KKSG::CustomBattle_StartNow == req.op)
			{
				battle->StartNow();
				errorcode = KKSG::ERR_SUCCESS;
				DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
			}
		}
		else
		{
			errorcode = KKSG::ERR_CUSTOMBATTLE_NOTCREATER;
		}
	}

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleModify::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	ErrorNotify(errorcode, req.delayid);
	return;		
}
