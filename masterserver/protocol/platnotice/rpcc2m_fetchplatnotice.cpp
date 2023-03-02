#include "pch.h"
#include "platnotice/rpcc2m_fetchplatnotice.h"
#include "idip/platnotice.h"
#include "misc/rpcm2g_checknotice.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/11/21 19:51:18

RPC_SERVER_IMPLEMETION(RpcC2M_FetchPlatNotice, FetchPlatNoticeArg, FetchPlatNoticeRes)

void RpcC2M_FetchPlatNotice::OnCall(const FetchPlatNoticeArg &roArg, FetchPlatNoticeRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	std::vector<KKSG::PlatNotice> list;
	CPlatNoticeMgr::Instance()->GetNotice(roArg.type(), roArg.platid(), list);
	if (list.size())
	{
		RpcM2G_CheckNotice* rpc = RpcM2G_CheckNotice::CreateRpc();
		rpc->m_oArg.set_roleid(role->GetID());
		rpc->m_oArg.set_rpcid(DelayRpc());
		for (UINT32 i = 0; i < list.size(); i ++)
		{
			KKSG::PlatNotice* pInfo = rpc->m_oArg.add_notice();
			pInfo->CopyFrom(list[i]);
		}
		GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
	}
}

void RpcC2M_FetchPlatNotice::OnDelayReplyRpc(const FetchPlatNoticeArg &roArg, FetchPlatNoticeRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS)
	{
		KKSG::CheckNoticeRes* pData = (KKSG::CheckNoticeRes*)roUserData.m_pUserPtr;
		for (INT32 i = 0; i < pData->notice_size(); i ++)
		{
			KKSG::PlatNotice* pInfo = roRes.add_data();
			pInfo->CopyFrom(pData->notice(i));
			//LogDebug("type=%u,noticeid=%u,areaid=%u,platid=%u,updatetime=%u,isnew=%d,title=%s,content=%s", 
			//	pInfo->type(), pInfo->noticeid(), pInfo->areaid(), pInfo->platid(), pInfo->updatetime(), pInfo->isnew()?1:0, pInfo->title().c_str(), pInfo->content().c_str());
		}
	}
	roRes.set_result(KKSG::ErrorCode(roUserData.m_dwUserData));
}
