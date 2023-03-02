#include "pch.h"
#include "team/rpcc2m_fetchteamlistc2m.h"
#include "team/team.h"
#include "foreach.h"
#include "team/teammember.h"
#include "role/rolemanager.h"
#include "team/teamrequest.h"
#include "team/teamtranshandler.h"
#include "network/teamlink.h"
#include "table/expeditionconfigmgr.h"
#include "nestweek/nestweekmgr.h"

// generate by ProtoGen at date: 2016/8/16 10:51:34

RPC_SERVER_IMPLEMETION(RpcC2M_FetchTeamListC2M, FetchTeamListArg, FetchTeamListRes)

static void GetTeamListCB(Team *pTeam, void *arg)
{
	FetchTeamListRes *pRes = (FetchTeamListRes *)arg;
	if (pTeam->GetMemberCount() > 0)
	{
		KKSG::TeamFullDataNtf *pFullData = pRes->add_theteams();
		pFullData->set_hasteam(true);
		pTeam->SaveBrief(pFullData->mutable_teambrief(), false);
		const std::list<CTeamMember *> &Members = pTeam->GetMember();
		foreach(i in Members)
		{
			(*i)->SaveToTeamMember(pFullData->add_members());
		}
	}
}

void RpcC2M_FetchTeamListC2M::OnCall(const FetchTeamListArg &roArg, FetchTeamListRes &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return;
	}

	TeamRequest Tq(pRole);
	
	if (roArg.categoryid_size() > 0)
	{
		std::vector<UINT32> Category;
		for (int i = 0; i < roArg.categoryid_size(); ++i)
		{
			Category.push_back(roArg.categoryid(i));
		}

		Tq.GetExpenditionTeam(Category, GetTeamListCB, &roRes);
	}

	if (roArg.expid() != 0)
	{
		Tq.GetExpenditionTeam(roArg.expid(), GetTeamListCB, &roRes);
	}

	//去TeamServer也去取一批
	if(TeamLink::Instance()->IsConnected())
	{
		TeamTransHandler trans;
		trans.HandleFetchTeamReqFromC(DelayRpc(), pRole->GetID(), roArg);
	}
}

void RpcC2M_FetchTeamListC2M::OnDelayReplyRpc(const FetchTeamListArg &roArg, FetchTeamListRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return;
	}
	FetchTeamListRes* tmpRes = (FetchTeamListRes*)(roUserData.m_pUserPtr);	
	//SSDebug << " ms team size = " << roRes.theteams_size() << " ts team size = " << tmpRes->theteams_size() << END;
	for(int i = 0; i < tmpRes->theteams_size(); ++i)
	{
		int expID = tmpRes->theteams(i).teambrief().expid();
		auto pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
		if(NULL == pConf)
		{
			continue;
		}
		if(!TeamTransHandler::NeedInTs(expID))
		{
			//如果本服此关卡不跨服 则屏蔽可见性
			continue;
		}

		*roRes.add_theteams() = tmpRes->theteams(i);
	}
	//roRes.MergeFrom(*tmpRes);

	//筛减发送的数据
	for(int i = 0; i < roRes.theteams_size(); ++i)
	{
		auto& teamData = *roRes.mutable_theteams(i);
		for(int j = 0; j < teamData.members_size(); ++j)
		{
			auto& memberData = *teamData.mutable_members(j);
			memberData.clear_fashion();
			memberData.clear_outlook();
		}
	}

}
