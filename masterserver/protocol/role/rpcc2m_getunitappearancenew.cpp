#include "pch.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rpcc2m_getunitappearancenew.h"

// generate by ProtoGen at date: 2016/12/24 20:57:15

RPC_SERVER_IMPLEMETION(RpcC2M_GetUnitAppearanceNew, GetUnitAppearanceArg, GetUnitAppearanceRes)

void RpcC2M_GetUnitAppearanceNew::OnCall(const GetUnitAppearanceArg &roArg, GetUnitAppearanceRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	UINT64 otherid = roArg.roleid();
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(otherid, true);
	if (NULL == summary)
	{
		//SSWarn<<"summary is NULL, roleid:"<<otherid<<END;
		roRes.set_errorcode(ERR_ROLE_IN_OTHER_SERVER);
		return;
	}

	KKSG::UnitAppearance* appearance = roRes.mutable_unitappearance();
	appearance->set_uid(otherid);
	appearance->set_level(summary->GetLevel());
	// build
	int mask = roArg.mask();
	summary->Build(appearance, mask);
	// team (不支持跨服组队的情况)
	CRole* other = CRoleManager::Instance()->GetByRoleID(otherid);
	Team* team = TeamMgr::Instance()->FindTeam(NULL == other ? 0 : other->GetTeamID());
	if (NULL != team)
	{
		appearance->mutable_team()->set_teamid(team->GetID());
		appearance->mutable_team()->set_haspassword(!team->GetPassword().empty());
	}
	// pet
	if (0 != roArg.petid())
	{
		const KKSG::RoleSummaryStored& dbsummary = summary->GetData();
		UINT64 petId = roArg.petid();
		appearance->clear_pet();
		for (int i = 0; i < dbsummary.pets_size(); ++i)
		{
			if (petId == dbsummary.pets(i).uid())
			{
				KKSG::PetSingle* pet = appearance->mutable_pet();
				*pet = dbsummary.pets(i);
				break;
			}
		}
	}
}

void RpcC2M_GetUnitAppearanceNew::OnDelayReplyRpc(const GetUnitAppearanceArg &roArg, GetUnitAppearanceRes &roRes, const CUserData &roUserData)
{
}
