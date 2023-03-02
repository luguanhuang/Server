#include "pch.h"
#include "marriage/rpcc2m_getmarriagerelation.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"

// generate by ProtoGen at date: 2017/7/24 20:59:09

RPC_SERVER_IMPLEMETION(RpcC2M_GetMarriageRelation, GetMarriageRelationArg, GetMarriageRelationRes)

void RpcC2M_GetMarriageRelation::OnCall(const GetMarriageRelationArg &roArg, GetMarriageRelationRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);

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
	
	marriage->FillMarrigeRelation2Client(roRes);
}

void RpcC2M_GetMarriageRelation::OnDelayReplyRpc(const GetMarriageRelationArg &roArg, GetMarriageRelationRes &roRes, const CUserData &roUserData)
{
}
