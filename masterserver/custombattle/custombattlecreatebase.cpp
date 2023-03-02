#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "tsssdk/MsTssHandler.h"
#include "forbid/forbidwordmgr.h"

void CustomBattleHandleCreateBase::HandleClientReq(const CustomBattleReq& req)
{
	// level limit
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
	if (NULL == summary)
	{
		SSWarn<<"not find roleid:"<<req.roleid<<END;
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_ROELNOTEXIST, req.delayid);
		return;
	}
	if (CCustomBattleConfig::Instance()->IsLevelLimit(req.conf.configid(), summary->GetLevel()))
	{
		SSWarn<<"create config:"<<req.conf.configid()<<" roleid:"<<req.roleid<<" level limit:"<<summary->GetLevel()<<END;
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_LEVELLIMIT, req.delayid);
		return;
	}

	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(req.roleid);
	if (NULL != roleinfo && NULL != roleinfo->m_battlejoin)
	{
		// has join
		SSWarn<<"roleid:"<<req.roleid<<" has join:"<<roleinfo->GetJoinId()<<END;
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_HASJOIN, req.delayid);
		return;
	}

	// check valid
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	KKSG::CustomBattleConfig serverconfig;
	bool isgmcreate = CCustomBattleConfig::Instance()->IsGMCreate(req.conf.configid());
	if (isgmcreate)
	{
		if (!CustomBattleMgr::Instance()->IsGMCreate(summary->GetAccount()))
		{
			ErrorNotify(KKSG::ERR_CUSTOM_GMLIMIT, req.delayid);
			return;	
		}
	}
	errorcode = CCustomBattleConfig::Instance()->IsValidCreate(req.conf);
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		errorcode = CCustomBattleConfig::Instance()->FillCreate(req.conf, summary->GetID(), summary->GetName(), serverconfig, isgmcreate);
		if (KKSG::ERR_SUCCESS == errorcode)
		{
			errorcode = CTssHandler::Instance()->ValidNameCheck(req.conf.name());
			if (KKSG::ERR_SUCCESS == errorcode)
			{
				//errorcode = CTssHandler::Instance()->ValidNameCheck(req.conf.desc());
				if (ForbidWordMgr::Instance()->HaveForbidWord(req.conf.name()))
				{
					errorcode = KKSG::ERR_INVALID_NAME;
				}
				else
				{
					errorcode = KKSG::ERR_SUCCESS;
				}
			}
		}
	}

	// result
	if (KKSG::ERR_SUCCESS == errorcode)
	{
		CustomBattleReq serverreq = req;
		serverreq.conf = serverconfig;
		// trans to gs, take item 
		std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetGenerateCost(req.conf.configid());
		errorcode = m_manager->DelayTakeItem(serverreq, items);
	}
	else
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}
