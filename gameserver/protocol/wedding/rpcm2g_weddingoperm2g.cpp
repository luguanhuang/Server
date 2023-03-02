#include "pch.h"
#include "wedding/rpcm2g_weddingoperm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "unit/pet.h"
#include "unit/roleoutlook.h"
#include "wedding/ptcg2c_weddingcarntf.h"
#include "gamelogic/notice.h"

// generate by ProtoGen at date: 2017/7/31 15:27:03

RPC_SERVER_IMPLEMETION(RpcM2G_WeddingOperM2G, WeddingOperM2GArg, WeddingOperM2GRes)

void RpcM2G_WeddingOperM2G::OnCall(const WeddingOperM2GArg &roArg, WeddingOperM2GRes &roRes)
{
	if (roArg.roleid1() == roArg.roleid2())
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	Role* role1 = RoleManager::Instance()->FindByRoleID(roArg.roleid1());
	Role* role2 = RoleManager::Instance()->FindByRoleID(roArg.roleid2());
	if (!role1 || !role2 || !role1->GetCurrScene() || !role2->GetCurrScene() || role1->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID || role2->GetCurrScene()->GetSceneTemplateID() != HALL_MAP_ID)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
		return;
	}
	auto pos1 = role1->GetXRole()->GetPosition_p();
	auto pos2 = role2->GetXRole()->GetPosition_p();

	float dis = Vector3::Distance(pos1, pos2);
	if (dis > GetGlobalConfig().ApplyMarriageDistance)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
		return;
	}

	// 骑马时不允许婚车
	if (role1->GetState().statetype() == KKSG::OutLook_RidePet || role1->GetState().statetype() == KKSG::OutLook_RidePetCopilot
	 || role2->GetState().statetype() == KKSG::OutLook_RidePet || role2->GetState().statetype() == KKSG::OutLook_RidePetCopilot)
	{
		roRes.set_result(KKSG::ERR_WEDDINGCAR_CANT_IN_PET);
	}
	if (!role1->StateCanChangeTo(KKSG::OutLook_RidePet))
	{
		roRes.set_result(KKSG::ERR_STATE_CANTCHANGE);
		return;
	}
	if (!role2->StateCanChangeTo(KKSG::OutLook_RidePetCopilot))
	{
		roRes.set_result(KKSG::ERR_STATE_CANTCHANGE);
		return;
	}

	// role can see otherrole
	bool cansee = false;
	std::vector<Role*> roles;
	role1->GetCurrScene()->GetRolesCanSee(role1, roles);
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		if (*i == role2)
		{
			cansee = true;
			break;
		}
	}
	if (!cansee)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
		return;
	}

	bool canseeother = false;
	std::vector<Role*> rolesother;
	role2->GetCurrScene()->GetRolesCanSee(role2, rolesother);
	for (auto i = rolesother.begin(); i != rolesother.end(); ++i)
	{
		if (*i == role1)
		{
			canseeother = true;
			break;
		}
	}
	if (!canseeother)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_DISTANCE_TOOFAR);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	PtcG2C_WeddingCarNtf ntf;
	role1->GetAppearance(*ntf.m_Data.mutable_role1());
	role2->GetAppearance(*ntf.m_Data.mutable_role2());
	for (auto it = RoleManager::Instance()->Begin(); it != RoleManager::Instance()->End(); ++it)
	{
		Role* pRole = *it;
		pRole->Send(ntf);
	}

	Notice notice(WeddingCarNotice);
	notice.Add(role1);
	notice.Add(role2);
	notice.Send();
}

void RpcM2G_WeddingOperM2G::OnDelayReplyRpc(const WeddingOperM2GArg &roArg, WeddingOperM2GRes &roRes, const CUserData &roUserData)
{
}
