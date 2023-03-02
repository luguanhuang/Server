#include "pch.h"
#include "marriage/rpcc2m_takemarriagechest.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"
#include "tablemgr/marriagetablemgr.h"
#include "marriage/rpcm2g_givemarriagelvrerewardm2g.h"

// generate by ProtoGen at date: 2017/7/28 16:06:09

RPC_SERVER_IMPLEMETION(RpcC2M_TakeMarriageChest, TakeMarriageChestArg, TakeMarriageChestRes)

void RpcC2M_TakeMarriageChest::OnCall(const TakeMarriageChestArg &roArg, TakeMarriageChestRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Marriage* pMr = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (pMr == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	WeddingLoverLiveness::RowData* pData = MarriageTableMgr::Instance()->GetLivenessData(roArg.index());
	if (pData == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pMr->GetLiveNess() < pData->liveness)
	{
		roRes.set_result(KKSG::ERR_LIVENESS_NOT_ENOUGH);
		return;
	}
	if (pMr->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_CHEST_TAKED);
		return;
	}
	RpcM2G_GiveMarriageLvRerewardM2G* rpc = RpcM2G_GiveMarriageLvRerewardM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_index(roArg.index());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_TakeMarriageChest::OnDelayReplyRpc(const TakeMarriageChestArg &roArg, TakeMarriageChestRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Marriage* pMr = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (pMr == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	WeddingLoverLiveness::RowData* pData = MarriageTableMgr::Instance()->GetLivenessData(roArg.index());
	if (pData == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pMr->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_CHEST_TAKED);
		return;
	}
	pMr->SetChestTaked(roArg.index());
	KKSG::GiveMarriageLvRewardRes* res = (KKSG::GiveMarriageLvRewardRes*)roUserData.m_pUserPtr;
	if (res)
	{
		roRes.mutable_itemid()->CopyFrom(res->itemid());
		roRes.mutable_itemcount()->CopyFrom(res->itemcount());
	}
	roRes.set_takedchest(pMr->GetTakedChest());
}
