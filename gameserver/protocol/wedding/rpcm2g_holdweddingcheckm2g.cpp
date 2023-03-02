#include "pch.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/7/29 16:57:07

RPC_SERVER_IMPLEMETION(RpcM2G_Holdweddingcheckm2g, Holdweddingcheckm2gArg, Holdweddingcheckm2gRes)

void RpcM2G_Holdweddingcheckm2g::OnCall(const Holdweddingcheckm2gArg &roArg, Holdweddingcheckm2gRes &roRes)
{
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
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_Holdweddingcheckm2g::OnDelayReplyRpc(const Holdweddingcheckm2gArg &roArg, Holdweddingcheckm2gRes &roRes, const CUserData &roUserData)
{
}
