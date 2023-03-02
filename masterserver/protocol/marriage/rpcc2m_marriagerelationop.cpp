#include "pch.h"
#include "marriage/rpcc2m_marriagerelationop.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "marriage/marriagerelationmgr.h"
#include "marriage/rpcm2g_checkmarriagecouple.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2017/7/24 21:06:10

RPC_SERVER_IMPLEMETION(RpcC2M_MarriageRelationOp, MarriageRelationOpArg, MarriageRelationOpRes)

void RpcC2M_MarriageRelationOp::OnCall(const MarriageRelationOpArg &roArg, MarriageRelationOpRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);

	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	UINT64 destRoleID = roArg.destroleid();

	DelayMarriageProcessInfo info;
	UINT32 error = MarriageRelationMgr::Instance()->HandleRelationOp(role, roArg, info);
	if (error != KKSG::ERR_SUCCESS)
	{
		roRes.set_error((ErrorCode)error);
		return;
	}
	
	if(roArg.optype() == KKSG::MarriageOpType_MarryApply)
	{
		RpcM2G_CheckMarriageCouple* rpc = RpcM2G_CheckMarriageCouple::CreateRpc();
		rpc->m_oArg.set_role1(role->GetID());
		rpc->m_oArg.set_role2(info.roleID);
		rpc->m_oArg.set_delayrpc(DelayRpc());
		rpc->m_oArg.set_optype(roArg.optype());
		rpc->m_oArg.set_type(roArg.type());
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
	if(roArg.optype() == KKSG::MarriageOpType_MarryAgree)
	{
		RpcM2G_CheckMarriageCouple* rpc = RpcM2G_CheckMarriageCouple::CreateRpc();
		rpc->m_oArg.set_role1(roArg.destroleid());
		rpc->m_oArg.set_role2(role->GetID());
		rpc->m_oArg.set_delayrpc(DelayRpc());
		rpc->m_oArg.set_optype(roArg.optype());
		rpc->m_oArg.set_type(info.type);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
	if(roArg.optype() == KKSG::MarriageOpType_Divorce && info.toGs)
	{
		RpcM2G_CheckMarriageCouple* rpc = RpcM2G_CheckMarriageCouple::CreateRpc();
		rpc->m_oArg.set_role1(role->GetID());
		rpc->m_oArg.set_role2(role->GetID());
		rpc->m_oArg.set_delayrpc(DelayRpc());
		rpc->m_oArg.set_optype(roArg.optype());
		rpc->m_oArg.set_type(info.type);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
}

void RpcC2M_MarriageRelationOp::OnDelayReplyRpc(const MarriageRelationOpArg &roArg, MarriageRelationOpRes &roRes, const CUserData &roUserData)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	if (roArg.optype() != KKSG::MarriageOpType_MarryApply && roArg.optype() != KKSG::MarriageOpType_MarryAgree && roArg.optype() != KKSG::MarriageOpType_Divorce)
	{
		return;
	}
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS || !roUserData.m_pUserPtr)
	{
		roRes.set_error((ErrorCode)roUserData.m_dwUserData);
		return;
	}

	auto ptr = (KKSG::CheckMarriageCoupleRes*)roUserData.m_pUserPtr;

	UINT32 error = MarriageRelationMgr::Instance()->DelayProcess(role, ptr->role1(), ptr->role2(), roArg, roRes);

	roRes.set_error((ErrorCode)error);
}
