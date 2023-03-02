#include "pch.h"
#include "invfight/rpcc2m_invfightreqall.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "invfight/invfightrequest.h"
#include "invfight/invfightmgr.h"
#include "invfight/rpcm2g_invfightm2greq.h"
#include "network/gslink.h"
#include "invfight/invfightcrossrequest.h"
#include "invfight/rpcm2s_invfightcross2csreq.h"
#include "config.h"
#include "network/cslink.h"

// generate by ProtoGen at date: 2016/12/13 23:25:17

RPC_SERVER_IMPLEMETION(RpcC2M_InvFightReqAll, InvFightArg, InvFightRes)

void RpcC2M_InvFightReqAll::OnCall(const InvFightArg &roArg, InvFightRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	if(InvFightMgr::Instance()->IsInDelay(pRole->GetID()))
	{
		roRes.set_errorcode(ERR_INVFIGHT_INV_DELAY);
		return ;
	}
	bool isCross = roArg.iscross();
	if (!isCross)
	{
		UINT64 invID = roArg.invid();
		invID >>= 32;
		if (invID)
		{
			isCross = true;
		}
	}
	if (!isCross)
	{
		InvFightRequest req(pRole);
		InvFightProcessDelayInfo delayinfo;
		bool needDelay = req.ProcessCall(roArg, roRes, delayinfo);
		if (needDelay)
		{
			RpcM2G_InvFightM2GReq* rpc = RpcM2G_InvFightM2GReq::CreateRpc();	
			rpc->m_oArg.set_delayid(DelayRpc());
			rpc->m_oArg.set_roleid(pRole->GetID());
			rpc->m_oArg.mutable_c2marg()->CopyFrom(m_oArg);
			if(roArg.reqtype() == IFRT_ACCEPT_ONE)
			{
				rpc->m_oArg.mutable_c2marg()->set_roleid(delayinfo.fromID);
			}
			GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);//只支持主城和公会(gs1)
		}
	}
	else
	{
		InvFightCrossRequest req(pRole);
		InvFightCrossProcessDelayInfo delayinfo;
		bool needDelay = req.ProcessClientCall(roArg, roRes, delayinfo);
		if (needDelay)
		{
			if (CSLink::Instance()->GetUsingCenterServerID() == INVALID_CONN_ID)
			{
				roRes.set_errorcode(KKSG::ERR_SUCCESS);
				roRes.set_isplatfriendonline(true);
				LogWarn("center server not connected");
				return;
			}
			RpcM2S_InvFightCross2CSReq* rpc = RpcM2S_InvFightCross2CSReq::CreateRpc();	
			rpc->m_oArg.set_delayid(DelayRpc());
			rpc->m_oArg.set_reqtype(m_oArg.reqtype());

			KKSG::InvFightCrossMsg* msg = rpc->m_oArg.mutable_invmsg();

			msg->set_fromid(delayinfo.fromID);
			msg->set_toid(delayinfo.toID);
			msg->set_fromsrverid(delayinfo.fromServer);
			msg->set_toserverid(delayinfo.toServer);
			msg->set_fromaccount(delayinfo.fromAccount);
			msg->set_toaccount(delayinfo.toAccount);
			msg->set_invid(delayinfo.invID);
			msg->mutable_smallinfo()->CopyFrom(delayinfo.smallInfo);

			CSLink::Instance()->SendTo(*rpc);
		}
	}
}

void RpcC2M_InvFightReqAll::OnDelayReplyRpc(const InvFightArg &roArg, InvFightRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	bool isCross = roArg.iscross();
	if (!isCross)
	{
		UINT64 invID = roArg.invid();
		invID >>= 32;
		if (invID)
		{
			isCross = true;
		}
	}
	if (!isCross)
	{
		InvFightRequest req(pRole);	
		req.ProcessDelay(roArg, roRes, roUserData);
	}
	else
	{
		InvFightCrossRequest req(pRole);
		req.ProcessClientCallDelay(roArg, roRes, roUserData);
	}
}
