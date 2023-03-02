#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teamforceop.h"
#include "network/worldlink.h"
#include "config.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "other/matchhandler.h"
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

void CustomBattleHandleUnJoinWorld::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;

	// check valid
	// to do...

	errorcode = m_manager->TransReqToWorld(req);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

void CustomBattleHandleUnJoinWorld::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& info)
{
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	KKSG::ErrorCode temperrorcode = KKSG::ERR_FAILED;
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	UINT32 configid = roleinfo->GetJoinConfigId();
	if (!roleinfo->UnJoin(req.uid, false))
	{
		temperrorcode = KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	else
	{
		DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);
		if (CCustomBattleConfig::Instance()->IsPvpTwo(configid))
		{
			CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
			if (NULL != role)
			{
				Team* team = TeamMgr::Instance()->FindTeam(role->GetTeamID());
				if (NULL != team && NULL != team->GetConf() && team->GetConf()->Type == TEAM_TYPE_CUSTOMPKTWO)
				{
					TeamForceOp op;
					op.LeaveTeam(role);
				}
				HMatchHandler match;
				match.CheckRoleStop(role, KMT_CUSTOM_PKTWO);
			}
		}
		temperrorcode = KKSG::ERR_SUCCESS;

		// join flow
		TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());

		joinlog.m_MatchId = req.uid;
		joinlog.m_Op = 2;
		joinlog.m_Matchtype = 0;
		joinlog.m_MatchMode = 0;
		joinlog.m_ConfigId = configid;//比赛配置ID;
		joinlog.m_isSystem = 0;//是否系统比赛;//0:否 1：是
		joinlog.m_strMatchName = "";//比赛命名名称;
		joinlog.m_uRoleID = req.roleid;
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
		joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//帐号;//

		joinlog.Do();
	}

	if (KKSG::ERR_SUCCESS != temperrorcode)
	{
		ErrorNotify(temperrorcode, req.delayid);
	}
	return;		
}
