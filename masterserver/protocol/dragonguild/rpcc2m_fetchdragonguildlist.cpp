#include "pch.h"
#include "dragonguild/rpcc2m_fetchdragonguildlist.h"
#include "role/rolesummary.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildsorter.h"
// generate by ProtoGen at date: 2017/9/8 16:40:23

RPC_SERVER_IMPLEMETION(RpcC2M_FetchDragonGuildList, FetchDragonGuildListArg, FetchDragonGuildRes)

static void SetDragonGuildInfo(DragonGuild* pGuild, KKSG::DragonGuildInfo *pInfo, UINT64 qwRoleID)
{
	pInfo->set_id(pGuild->GetID());
	pInfo->set_name(pGuild->GetName());
	pInfo->set_leaderid(pGuild->GetLeaderId());
	CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderId());
	if (pRoleSum != NULL)
	{
		pInfo->set_leadername(pRoleSum->GetName());
	}
	else
	{
		pInfo->set_leadername(pGuild->GetLeaderName());
	}

	pInfo->set_level(pGuild->GetLevel());
	pInfo->set_recruitppt(pGuild->GetRecuitPPT());
	pInfo->set_needapproval(pGuild->GetNeedApproval());
	pInfo->set_membercounts(pGuild->GetMemberCount());
	pInfo->set_issendapplication(false);
	pInfo->set_capacity(pGuild->GetCapacity());
	pInfo->set_announcement(pGuild->GetAnnoucement());
	pInfo->set_totalppt(pGuild->GetTotalPPT());
	pInfo->set_sceneid(pGuild->GetSceneId());
	pInfo->set_scenecnt(pGuild->GetSceneCnt());
	pInfo->set_guildexp(pGuild->GetExp());
}

void RpcC2M_FetchDragonGuildList::OnCall(const FetchDragonGuildListArg &roArg, FetchDragonGuildRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	DragonGuildSorter& sorter = DragonGuildMgr::Instance()->GetSorter();

	if (!sorter.IsSorttypeSupport(roArg.sorttype()))
	{
		LogWarn("DragonGuild sort type %d not support!", roArg.sorttype());
		return ;
	}

	UINT64 roleId = pRole->GetID();
	std::vector<DragonGuild* >& DragonGuildVec = sorter.GetSortResult(roArg.sorttype());
	int count = roArg.count();

	if (roArg.reverse())
	{
		int start = (int)DragonGuildVec.size() - roArg.start() - 1;

		for (int i= start; i >= 0 && count > 0; -- i)
		{
			DragonGuild* pDguild = DragonGuildVec[i];
			if(pDguild == NULL)
				continue;
			if (roArg.has_name())
			{
				if (!pDguild->ContainStr(roArg.name()))
				{
					continue;
				}
			}

			-- count;
			KKSG::DragonGuildInfo* info = roRes.add_dragonguilds();
			SetDragonGuildInfo(pDguild, info, roleId);
		}
	}
	else
	{
		UINT32 start = roArg.start();

		for (int i = start; i < DragonGuildVec.size() && count > 0; ++ i)
		{
			DragonGuild* pDguild = DragonGuildVec[i];
			if(pDguild == NULL)
				continue;
			if (roArg.has_name())
			{
				if (!pDguild->ContainStr(roArg.name()))
				{
					continue;
				}
			}

			-- count;
			KKSG::DragonGuildInfo* info = roRes.add_dragonguilds();
			SetDragonGuildInfo(pDguild, info, roleId);
		}
	}
}

void RpcC2M_FetchDragonGuildList::OnDelayReplyRpc(const FetchDragonGuildListArg &roArg, FetchDragonGuildRes &roRes, const CUserData &roUserData)
{
}
