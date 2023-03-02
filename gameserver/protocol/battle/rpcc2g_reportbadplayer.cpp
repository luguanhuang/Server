#include "pch.h"
#include "battle/rpcc2g_reportbadplayer.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/sceneherobattle.h"
#include "battle/ptcg2m_reportbadplayer2ms.h"
#include "servermgr/servermgr.h"
#include "timeutil.h"
#include "network/mslink.h"
#include "idutil.h"

// generate by ProtoGen at date: 2017/5/10 16:10:32

RPC_SERVER_IMPLEMETION(RpcC2G_ReportBadPlayer, ReportBadPlayerArg, ReportBadPlayerRes)

void RpcC2G_ReportBadPlayer::OnCall(const ReportBadPlayerArg &roArg, ReportBadPlayerRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_FAILED);
	if (pRole->GetID() == roArg.roleid())
	{
		return;
	}
	std::set<int> reasons;
	for (int i = 0; i < roArg.reason_size(); ++i)
	{
		reasons.insert(roArg.reason(i))	;	
	}
	if (reasons.empty())
	{
		return;
	}
	Scene* curScene = pRole->GetCurrScene();
	if (curScene == NULL)
	{
		return;
	}
	SceneVsBase* pBase = curScene->GetVsHandler();
	if (pBase == NULL)
	{
		LogError("unsupported scenetype: %u", roArg.scenetype());
		return;
	}

	int num = 0;
	KKSG::ErrorCode ret = pBase->CheckReportPlayer(pRole->GetID(), roArg.roleid(), num);
	if (ret != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(ret);
		return;
	}

	if (num != 0)
	{
		PtcG2M_ReportBadPlayer2Ms ptc;
		ptc.m_Data.set_roleid(roArg.roleid());
		ptc.m_Data.set_report_roleid(pRole->GetID());
		ptc.m_Data.set_scenetype(roArg.scenetype());
		ptc.m_Data.set_score(num);
		ptc.m_Data.set_report_openid(pRole->GetAccount());
		ptc.m_Data.set_group(pBase->GetGroup(roArg.roleid()));
		ptc.m_Data.set_report_group(pBase->GetGroup(pRole->GetID()));
		for(INT32 i = 0; i < roArg.reason_size(); i ++)
		{
			ptc.m_Data.add_reason(roArg.reason(i));
		}
		ptc.m_Data.set_ismvp(false);

		if (GSConfig::Instance()->IsCrossGS())
		{
			ServerMgr::Instance()->SendByRoleId(roArg.roleid(), ptc);	
		}
		else
		{
			MSLink::Instance()->SendTo(ptc);
		}
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_ReportBadPlayer::OnDelayReplyRpc(const ReportBadPlayerArg &roArg, ReportBadPlayerRes &roRes, const CUserData &roUserData)
{
}
