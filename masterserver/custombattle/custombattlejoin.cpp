#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "friend/friendop.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "loghelper/tlogr.h"

void CustomBattleHandleJoin::HandleClientReq(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	CustomBattle* battle = m_manager->GetBattle(req.uid);
	if (NULL == battle)
	{
		SSWarn<<"invalid battle id:"<<req.uid<<END;
		ErrorNotify(errorcode, req.delayid);
		return;
	}

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
	if (NULL == summary)
	{
		SSWarn<<"summary is NULL roleid:"<<req.roleid<<END;
		ErrorNotify(errorcode, req.delayid);
		return;
	}
	if (CCustomBattleConfig::Instance()->IsLevelLimit(battle->GetConfigId(), summary->GetLevel()))
	{
		SSWarn<<"join config:"<<req.conf.configid()<<" roleid:"<<req.roleid<<" level limit:"<<summary->GetLevel()<<END;
		ErrorNotify(KKSG::ERR_CUSTOMBATTLE_LEVELLIMIT, req.delayid);
		return;
	}

	///> 白名单加入
	if (CCustomBattleConfig::Instance()->IsGMJoin(battle->GetConfigId()))
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(req.roleid);
		if (NULL == role)
		{
			ErrorNotify(errorcode, req.delayid);
			return;
		}
		else
		{
			if (!m_manager->IsGMJoin(role->GetAccount()))
			{
				ErrorNotify(KKSG::ERR_CUSTOM_GMLIMIT, req.delayid);
				return;
			}
		}
	}

	errorcode = HandleJoin(req, battle);	

	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}

KKSG::ErrorCode CustomBattleHandleJoin::HandleJoin(const CustomBattleReq& req, CustomBattle* battle)
{
	SCustomBattleRole* temproleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(req.roleid);
	if (NULL != temproleinfo && NULL != temproleinfo->m_battlejoin)
	{
		SSWarn<<"try join:"<<req.conf.configid()<<" has join:"<<temproleinfo->GetJoinId()<<" roleid:"<<req.roleid<<END;
		return KKSG::ERR_CUSTOMBATTLE_HASJOIN;
	}

	KKSG::ErrorCode errorcode = battle->CanJoin(req.password);
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		return errorcode;
	}
	// check relation
	if (battle->IsFriendOnly() || battle->IsGuildOnly())
	{
		bool isrelated = false;
		if (battle->GetCreator() == req.roleid)
		{
			isrelated = true;
		}
		///> 同好友
		if (!isrelated && battle->IsFriendOnly())
		{
			FriendOp op(req.roleid);
			if (op.IsFriend(battle->GetCreator()))
			{
				isrelated = true;
			}
		}
		///> 同公会
		if (!isrelated && battle->IsGuildOnly())
		{
			Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(battle->GetCreator());
			if (NULL != guild)
			{
				if (guild->IsPlayerInGuild(req.roleid))
				{
					isrelated = true;
				}
			}
		}
		if (!isrelated)
		{
			return KKSG::ERR_CUSTOMBATTLE_JOINNOTRELATED;
		}
	}

	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
	if (NULL != roleinfo->m_battlejoin)
	{
		SSWarn<<"try join:"<<req.conf.configid()<<" has join:"<<roleinfo->GetJoinId()<<" roleid:"<<req.roleid<<END;
		return KKSG::ERR_CUSTOMBATTLE_HASJOIN;
	}
	else
	{
		// battle data
		battle->AddRole(req.roleid, CRoleSummaryMgr::Instance()->GetRoleName(req.roleid));
		roleinfo->Join(battle->GetID(), false, battle->GetConfigId());

		// reply
		KKSG::CustomBattleClientInfo info;
		battle->ToKKSGWithRank(info.mutable_joininfo()->mutable_data());
		battle->FillRole(battle->GetID(), req.roleid, *info.mutable_joininfo()->mutable_role());
		m_manager->FillTag(battle, info.mutable_joininfo()->mutable_data(), battle->GetCreator(), req.roleid);
		DelayReply(KKSG::ERR_SUCCESS, &info, req.delayid);

		// join flow
		TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());

		const KKSG::CustomBattleConfig& config = battle->GetData().config();
		joinlog.m_MatchId = battle->GetID();
		joinlog.m_Op = 1;
		joinlog.BuildCustom(config);
		joinlog.m_uRoleID = req.roleid;
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
		joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//帐号;//

		joinlog.Do();

		return KKSG::ERR_SUCCESS;
	}
}

