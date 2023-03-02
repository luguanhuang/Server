#include "pch.h"
#include "marriage/rpcc2m_getmarriageliveness.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"

// generate by ProtoGen at date: 2017/7/28 15:59:07

RPC_SERVER_IMPLEMETION(RpcC2M_GetMarriageLiveness, GetMarriageLivenessArg, GetMarriageLivenessRes)

void RpcC2M_GetMarriageLiveness::OnCall(const GetMarriageLivenessArg &roArg, GetMarriageLivenessRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (!marriage)
	{
		return;
	}
	marriage->FillLivenessRecord(roRes);
	roRes.set_liveness(marriage->GetLiveNess());
	roRes.set_takedchest(marriage->GetTakedChest());
}

void RpcC2M_GetMarriageLiveness::OnDelayReplyRpc(const GetMarriageLivenessArg &roArg, GetMarriageLivenessRes &roRes, const CUserData &roUserData)
{
}
