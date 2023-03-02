#include "pch.h"
#include "wedding/rpcc2m_startweddingcar.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/team.h"
#include "team/teammgr.h"
#include "team/teammember.h"
#include "marriage/weddingmgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "wedding/rpcm2g_holdweddingcheckm2g.h"
#include "wedding/rpcm2g_weddingoperm2g.h"
#include "network/gslink.h"
#include "wedding/ptcm2c_weddinginvitentf.h"

// generate by ProtoGen at date: 2017/7/31 15:17:24

RPC_SERVER_IMPLEMETION(RpcC2M_StartWeddingCar, StartWeddingCarArg, StartWeddingCarRes)

void RpcC2M_StartWeddingCar::OnCall(const StartWeddingCarArg &roArg, StartWeddingCarRes &roRes)
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
		roRes.set_result(KKSG::ERR_WEDDING_NEED_TWO_MARRIAGE);
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
	ErrorCode code = (ErrorCode)pMarr->CheckTakeWeddingCar();
	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(code);
		return;
	}
	if (WeddingMgr::Instance()->HasWeddingCarRunning())
	{
		roRes.set_result(KKSG::ERR_WEDDINGCAR_OTHER_RUNNING);
		return;
	}
	// 先设置上，失败的话清空
	WeddingMgr::Instance()->SetWeddingCar(roleId1, roleId2, pMarr->GetWeddingType());
	RpcM2G_WeddingOperM2G* rpc = RpcM2G_WeddingOperM2G::CreateRpc();
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
	rpc->m_oArg.set_type(pMarr->GetWeddingType());
	rpc->m_oArg.set_rpcid(DelayRpc());
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}

void RpcC2M_StartWeddingCar::OnDelayReplyRpc(const StartWeddingCarArg &roArg, StartWeddingCarRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		WeddingMgr::Instance()->ResetWeddingCar();
		return;
	}

	WeddingMgr::Instance()->StartWeddingCarTimer();
	//WeddingMgr::Instance()->NotifyWeddingCar();
	roRes.set_result(KKSG::ERR_SUCCESS);
}
