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
#include "loghelper/tlogr.h"

void CustomBattleHandleCreate::HandleGsReply(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	// local handle
	if (KKSG::CustomBattle_DoCreate == req.op)
	{
		CustomBattle* battle = m_manager->GetGenerator()->Generate(req.conf, req.roleid, false, MSConfig::Instance()->GetServerID());
		if (NULL == battle)
		{
			errorcode = KKSG::ERR_CUSTOMBATTLE_CREATFAILED;
		}
		else
		{
			// role data
			SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->GetInfo(req.roleid);
			if (NULL != roleinfo->m_battlejoin)
			{
				//errorcode = KKSG::ERR_CUSTOMBATTLE_HASJOIN;
				SSWarn<<"roleid:"<<req.roleid<<" has join:"<<roleinfo->GetJoinId()<<END;
			}

			// battle data
			m_manager->AddBattle(battle->GetID(), battle);
			battle->AddRole(req.roleid, CRoleSummaryMgr::Instance()->GetRoleName(req.roleid));
			roleinfo->Join(battle->GetID(), false, battle->GetConfigId());

			// reply
			KKSG::CustomBattleClientInfo info;
			battle->ToKKSGWithRank(info.mutable_createinfo()->mutable_data());
			battle->FillRole(battle->GetID(), req.roleid, *info.mutable_createinfo()->mutable_role());
			m_manager->FillTag(battle, info.mutable_createinfo()->mutable_data(), battle->GetCreator(), req.roleid);
			DelayReply(KKSG::ERR_SUCCESS, &info, req.delayid);

			errorcode = KKSG::ERR_SUCCESS;

			// create flow
			TCustomPkFlow createlog(TLogMgr::Instance()->GetTagString());
			const KKSG::CustomBattleConfig& config = battle->GetData().config();
			createlog.m_MatchId = battle->GetID();
			createlog.m_Op = 0;
			createlog.BuildCustom(config);
			createlog.m_uRoleID = req.roleid;
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
			createlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//’ ∫≈;//

			createlog.Do();

			// join flow
			TCustomPkFlow joinlog(TLogMgr::Instance()->GetTagString());

			joinlog.m_MatchId = battle->GetID();
			joinlog.m_Op = 1;
			joinlog.BuildCustom(config);
			joinlog.m_uRoleID = req.roleid;
			joinlog.m_strOpenId = NULL == summary ? "" : summary->GetAccount();//’ ∫≈;//

			joinlog.Do();
		}
	}
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		ErrorNotify(errorcode, req.delayid);
	}
	return;
}