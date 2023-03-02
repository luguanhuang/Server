#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"

void CustomBattleHandleJoinSystem::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	///> 是否加入过
	if (roleinfo->IsJoin(req.uid))
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_HASJOIN, req.delayid);
		return;
	}

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
	if (NULL == summary)
	{
		SSWarn<<"summary is NULL roleid:"<<req.roleid<<END;
		ErrorNotify(errorcode, req.delayid);
		return;
	}
	if (CCustomBattleConfig::Instance()->IsLevelLimit((UINT32)req.uid, summary->GetLevel()))
	{
		SSWarn<<"join config:"<<req.uid<<" roleid:"<<req.roleid<<" level limit:"<<summary->GetLevel()<<END;
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_LEVELLIMIT, req.delayid);
		return;
	}

	errorcode = m_manager->TransReqToWorld(req);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleJoinSystem::HandleGsReply(const CustomBattleReq& req)
{
	// trans to world
	KKSG::ErrorCode errorcode = m_manager->TransReqToWorld(req);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleJoinSystem::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	switch(req.op)
	{
	case KKSG::CustomBattle_Join:
		{
			std::vector<ItemDesc> items = CCustomBattleConfig::Instance()->GetJoinCost((UINT32)req.uid);
			temperrorcode = m_manager->DelayTakeItem(req, items);
			break;
		}
	case KKSG::CustomBattle_DoJoin:
		{
			if (!info.has_joininfo())
			{
				temperrorcode = KKSG::ERR_CUSTOMBATTLE_JOINFAILED;
			}
			else
			{
				UINT64 uid = info.joininfo().data().uid();
				SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
				if (roleinfo->Join(uid, true, info.joininfo().data().config().configid()))
				{
					// reply
					KKSG::CustomBattleClientInfo tempinfo;
					*tempinfo.mutable_joininfo() = info.joininfo();
					roleinfo->FillSystemRole(uid, tempinfo.mutable_joininfo()->mutable_role());
					DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);

					temperrorcode = KKSG::ERR_SUCCESS;
				}
				else
				{
					temperrorcode = KKSG::ERR_FAILED;
				}
			}
			break;
		}
	default:
		break;
	}
	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
	}
	return;		
}
