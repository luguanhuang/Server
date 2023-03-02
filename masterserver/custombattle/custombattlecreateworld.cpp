#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "loghelper/tlogr.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"

void CustomBattleHandleCreateWorld::HandleGsReply(const CustomBattleReq& req)
{
	// trans to world
	KKSG::ErrorCode errorcode = m_manager->TransReqToWorld(req);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleCreateWorld::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	if (KKSG::CustomBattle_DoCreate == req.op)
	{
		if (!info.has_createinfo())
		{
			temperrorcode = KKSG::ERR_CUSTOMBATTLE_CREATFAILED;
		}
		else
		{
			// role data
			SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
			if (NULL != roleinfo->m_battlejoin)
			{
				SSWarn<<"roleid:"<<req.roleid<<" has join:"<<roleinfo->GetJoinId()<<END;
				//temperrorcode = KKSG::ERR_CUSTOMBATTLE_HASJOIN;
			}

			UINT64 uid = info.createinfo().data().uid();
			roleinfo->Join(uid, false, info.createinfo().data().config().configid());

			// reply
			KKSG::CustomBattleClientInfo tempinfo;
			*tempinfo.mutable_createinfo() = info.createinfo();
			DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);

			temperrorcode = KKSG::ERR_SUCCESS;

			// create flow
			TCustomPkFlow createlog(TLogMgr::Instance()->GetTagString());

			const KKSG::CustomBattleConfig& config = info.createinfo().data().config();
			createlog.m_MatchId = uid;
			createlog.m_Op = 0;
			createlog.BuildCustom(config);
			createlog.m_uRoleID = req.roleid;
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
			createlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//’ ∫≈;//

			createlog.Do();

			// join flow
			TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());

			joinlog.m_MatchId = uid;
			joinlog.m_Op = 1;
			joinlog.BuildCustom(config);
			joinlog.m_uRoleID = req.roleid;
			joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//’ ∫≈;//

			joinlog.Do();
		}
	}

	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
	}
}
