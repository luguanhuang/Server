#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "loghelper/tlogr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"

void CustomBattleHandleJoinWorld::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);

	CustomBattleReq tempreq = req;
	///> 已经加入过了
	if (!roleinfo->CanJoin())
	{
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_HASJOIN, req.delayid);
		return;
	}

	///> 直接向world请求dojoin
	tempreq.op = KKSG::CustomBattle_DoJoin;
	errorcode = m_manager->TransReqToWorld(tempreq);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleJoinWorld::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	switch(req.op)
	{
	case KKSG::CustomBattle_DoJoin:
		{
			if (!info.has_joininfo())
			{
				temperrorcode = KKSG::ERR_CUSTOMBATTLE_JOINFAILED;
			}
			else
			{
				SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
				if (NULL != roleinfo->m_battlejoin)
				{
					SSWarn<<"roleid:"<<req.roleid<<" has join:"<<roleinfo->GetJoinId()<<END;
					//temperrorcode = KKSG::ERR_CUSTOMBATTLE_HASJOIN;
				}

				UINT64 uid = info.joininfo().data().uid();
				roleinfo->Join(uid, false, info.joininfo().data().config().configid());

				// reply
				KKSG::CustomBattleClientInfo tempinfo;
				*tempinfo.mutable_joininfo() = info.joininfo();
				DelayReply(KKSG::ERR_SUCCESS, &tempinfo, req.delayid);

				temperrorcode = KKSG::ERR_SUCCESS;

				// join flow
				TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());

				const KKSG::CustomBattleConfig& config = info.joininfo().data().config();
				joinlog.m_MatchId = uid;
				joinlog.m_Op = 1;
				joinlog.BuildCustom(config);
				joinlog.m_uRoleID = req.roleid;
				CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
				joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//帐号;//

				joinlog.Do();

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
