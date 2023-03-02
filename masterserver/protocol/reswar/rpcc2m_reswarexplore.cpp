#include "pch.h"
#include "reswar/rpcc2m_reswarexplore.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "reswar/reswarmgr.h"
// generate by ProtoGen at date: 2016/10/28 19:10:07

RPC_SERVER_IMPLEMETION(RpcC2M_ResWarExplore, ResWarExploreArg, ResWarExploreRes)

void RpcC2M_ResWarExplore::OnCall(const ResWarExploreArg &roArg, ResWarExploreRes &roRes)
{
	ErrorCode error = ERR_SUCCESS;
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole) 
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (ResWarMgr::Instance()->GetState()==false)
	{
		roRes.set_error(KKSG::ERR_RESWAR_ACTIVITY);
		return;
	}
	Team *pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	if (!pTeam)
	{
		roRes.set_error(KKSG::ERR_RESWAR_TEAM);
		return;
	}
	if(pTeam->GetLeader()!= pRole->GetID())
	{
		roRes.set_error(ERR_RESWAR_LEADER);
		return;
	}
	if (pTeam->GetMemberCount()<3)
	{
		roRes.set_error(ERR_RESWAR_LACKPLAYER);
		return;
	}
	bool bSameGuild = ResWarMgr::Instance()->CheckTeam(pTeam);
	if (bSameGuild==false)
	{
		roRes.set_error(ERR_RESWAR_DIFF_GUILD);
		return;
	}

	UINT32 nDNEID = ResWarMgr::Instance()->GetResWarPVDNEID();
	if (nDNEID == pTeam->GetConf()->DNExpeditionID)
	{
		if (roArg.iscancel())
		{
			error = ResWarMgr::Instance()->CancelExplore(pTeam);
		}else
		{
			error = ResWarMgr::Instance()->Explore(pTeam);
		}
		roRes.set_error(error);
	}else
	{
		roRes.set_error(KKSG::ERR_RESWAR_TEAM);
	}
}

void RpcC2M_ResWarExplore::OnDelayReplyRpc(const ResWarExploreArg &roArg, ResWarExploreRes &roRes, const CUserData &roUserData)
{
}
