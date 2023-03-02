#include "pch.h"
#include "platnotice/rpcc2m_clicknewnotice.h"
#include "idip/platnotice.h"
#include "misc/ptcm2g_clicknoticentf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2017/2/1 15:03:32

RPC_SERVER_IMPLEMETION(RpcC2M_ClickNewNotice, ClickNewNoticeArg, ClickNewNoticeRes)

void RpcC2M_ClickNewNotice::OnCall(const ClickNewNoticeArg &roArg, ClickNewNoticeRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (!CPlatNoticeMgr::Instance()->ClickNoticeCheck(roArg.info()))
	{
		roRes.set_result(KKSG::ERR_FAILED);
		LogWarn("invalid notice type=%u,noticeid=%u,areaid=%u,platid=%u", roArg.info().type(), roArg.info().noticeid(), roArg.info().areaid(), roArg.info().platid());
		return;
	}
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	PtcM2G_ClickNoticeNtf ptc;
	ptc.m_Data.mutable_notice()->CopyFrom(roArg.info());
	ptc.m_Data.set_roleid(role->GetID());
	GSLink::Instance()->SendToLine(role->GetGsLine(), ptc);
}

void RpcC2M_ClickNewNotice::OnDelayReplyRpc(const ClickNewNoticeArg &roArg, ClickNewNoticeRes &roRes, const CUserData &roUserData)
{
}
