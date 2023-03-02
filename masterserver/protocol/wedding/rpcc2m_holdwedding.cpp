#include "pch.h"
#include "wedding/rpcc2m_holdwedding.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "team/teammember.h"
#include "marriage/weddingmgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"
#include "define/tlogenum.h"
#include "marriage/marriagerelationmgr.h"
#include "wedding/ptcm2c_weddinginvitentf.h"

// generate by ProtoGen at date: 2017/7/17 16:01:47

RPC_SERVER_IMPLEMETION(RpcC2M_HoldWedding, HoldWeddingReq, HoldWeddingRes)

void RpcC2M_HoldWedding::OnCall(const HoldWeddingReq &roArg, HoldWeddingRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Team* pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());	
	if (pTeam == NULL)
	{
		if (!pRole->HasTeamInTs())
		{
			roRes.set_result(KKSG::ERR_WEDDING_NEED_TWO_MARRIAGE);
		}
		else
		{
			roRes.set_result(KKSG::ERR_NEED_MARRIAGE_TEAM);
		}
		return;
	}
	if (pTeam->GetExpeditionType() != TEAM_TYPE_MARRIAGE)
	{
		roRes.set_result(KKSG::ERR_NEED_MARRIAGE_TEAM);
		return;
	}
	if (pTeam->GetMemberCount() != 2)
	{
		roRes.set_result(KKSG::ERR_WEDDING_NEED_TWO_MARRIAGE);
		return;
	}
	const std::list<CTeamMember *>& members = pTeam->GetMember();
	UINT64 roleId1 = (*members.begin())->m_qwRoleID;
	UINT64 roleId2 = (*members.rbegin())->m_qwRoleID;
	Marriage* pMarr = MarriageMgr::Instance()->GetMarriage(roleId1);
	if (pMarr == NULL)
	{
		roRes.set_result(KKSG::ERR_WEDDING_NEED_TWO_MARRIAGE);
		return;
	}
	if (pMarr->GetCoupleID() != roleId2)
	{
		roRes.set_result(KKSG::ERR_WEDDING_NEED_TWO_MARRIAGE);
		return;
	}
	KKSG::WeddingType type;
	UINT32 code = pMarr->CheckHoldWedding(type);
	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)code);
		return;
	}
	// 玩家婚礼进行中
	if (WeddingMgr::Instance()->GetWeddingByRoleId(roleId1) || WeddingMgr::Instance()->GetWeddingByRoleId(roleId2))
	{
		roRes.set_result(KKSG::ERR_WEDDING_IS_HOLDING);
		return;
	}
	RpcM2G_Holdweddingcheckm2g* rpc = RpcM2G_Holdweddingcheckm2g::CreateRpc();
	rpc->m_oArg.set_delayid(DelayRpc());
	if (pMarr->GetMarriagePos() == KKSG::MarriagePos_Husband)
	{
		rpc->m_oArg.set_roleid1(roleId1);
		rpc->m_oArg.set_roleid2(roleId2);
	}
	else
	{
		rpc->m_oArg.set_roleid1(roleId2);
		rpc->m_oArg.set_roleid2(roleId1);
	}
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_HoldWedding::OnDelayReplyRpc(const HoldWeddingReq &roArg, HoldWeddingRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	Holdweddingcheckm2gArg* pArg = (Holdweddingcheckm2gArg*)roUserData.m_pUserPtr;
	if (pArg == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	UINT64 roleId1 = pArg->roleid1();
	UINT64 roleId2 = pArg->roleid2();
	Marriage* pMarr = MarriageMgr::Instance()->GetMarriage(roleId1);
	if (pMarr == NULL)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_NOTINMARRIAGE);
		return;
	}
	if (pMarr->GetCoupleID() != roleId2)
	{
		roRes.set_result(KKSG::ERR_MARRIAGE_NOTINMARRIAGE);
		return;
	}
	KKSG::WeddingType type;
	UINT32 code = pMarr->CheckHoldWedding(type);
	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)code);
		return;
	}
	// 玩家婚礼进行中
	if (WeddingMgr::Instance()->GetWeddingByRoleId(roleId1) || WeddingMgr::Instance()->GetWeddingByRoleId(roleId2))
	{
		roRes.set_result(KKSG::ERR_WEDDING_IS_HOLDING);
		return;
	}

	Wedding* pWed = WeddingMgr::Instance()->CreateWedding(type, roleId1, roleId2);
	if (pWed == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	pWed->FillWeddingBrief(roRes.mutable_briedf());

	PtcM2C_WeddingInviteNtf ntf;
	ntf.m_Data.set_type(KKSG::Wedding_Start);
	pWed->FillWeddingBrief(ntf.m_Data.mutable_weddinginfo());
	CRole* pRole1 = CRoleManager::Instance()->GetByRoleID(roleId1);
	CRole* pRole2 = CRoleManager::Instance()->GetByRoleID(roleId2);
	if (pRole1) pRole1->Send(ntf);
	if (pRole2) pRole2->Send(ntf);

	MarriageRelationMgr::Instance()->DoTxLog(roleId1, roleId2, TMarriageType6, type, pWed->GetId());
	roRes.set_result(KKSG::ERR_SUCCESS);
}
