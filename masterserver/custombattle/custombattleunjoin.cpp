#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "other/matchhandler.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "role/rolesummarymgr.h"
#include "friend/friendop.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "team/teamforceop.h"
#include "role/rolemanager.h"
#include "loghelper/tlogr.h"

void CustomBattleHandleUnJoin::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	CustomBattle* battle = m_manager->GetBattle(req.uid);
	if (NULL == battle)
	{
		SSWarn<<"invalid battle id:"<<req.uid<<END;
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	errorcode = HandleUnJoin(req, battle);	

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

KKSG::ErrorCode CustomBattleHandleUnJoin::HandleUnJoin(const CustomBattleReq& req, CustomBattle* battle)
{
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	///> 没有加入过
	if (!roleinfo->IsJoin(req.uid))
	{
		SSWarn<<"not join uid:"<<req.uid<<" roleid:"<<req.roleid<<END;
		//return KKSG::ERR_FAILED;
	}
	if (battle->GetCreator() == req.roleid)
	{
		if (!battle->CanCreatorUnJoin())
		{
			return KKSG::ERR_CUSTOMBATTLE_CREATORLEAVE;
		}
	}

	///> 离开取消匹配
	CustomBattleMsMatcher::Instance()->DelCustomMatcher(req.roleid);
	if (battle->IsPvpTwo())
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

	///> 离开
	battle->DelRole(req.roleid);
	if (!roleinfo->UnJoin(req.uid, false))
	{
		return KKSG::ERR_FAILED;
	}
	else
	{
		DelayReply(KKSG::ERR_SUCCESS, NULL, req.delayid);

		// join flow
		TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());
		const KKSG::CustomBattleConfig& config = battle->GetData().config();
		joinlog.m_MatchId = battle->GetID();
		joinlog.m_Op = 2;
		joinlog.BuildCustom(config);
		joinlog.m_uRoleID = req.roleid;
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
		joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//帐号;//

		joinlog.Do();

		return KKSG::ERR_SUCCESS;
	}
}

