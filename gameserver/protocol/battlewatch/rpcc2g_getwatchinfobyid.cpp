#include "pch.h"
#include "battlewatch/rpcc2g_getwatchinfobyid.h"
#include "battlewatch/rpcg2m_getwatchinfobytype.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "live/livemanager.h"
#include <time.h>
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/7/11 11:04:35

RPC_SERVER_IMPLEMETION(RpcC2G_GetWatchInfoByID, GetWatchInfoByIDArg, GetWatchInfoByIDRes)

void RpcC2G_GetWatchInfoByID::OnCall(const GetWatchInfoByIDArg &roArg, GetWatchInfoByIDRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_error(ERR_FAILED);

	if (!GSConfig::Instance()->IsCrossGS())
	{
		if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			return;
		}
	}

	RpcG2M_GetWatchInfoByType* rpc = RpcG2M_GetWatchInfoByType::CreateRpc();

	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_livetype(roArg.type());
	rpc->m_oArg.set_rpcid(DelayRpc());

	role->SendToMS(*rpc);

	//UINT32 error = LiveManager::Instance()->FillLiveInfo(roArg.type(), roRes);

	//roRes.set_error((ErrorCode)error);
}

void RpcC2G_GetWatchInfoByID::OnDelayReplyRpc(const GetWatchInfoByIDArg &roArg, GetWatchInfoByIDRes &roRes, const CUserData &roUserData)
{
	roRes.set_error(KKSG::ErrorCode(roUserData.m_dwUserData));
	if(roRes.error() == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr != NULL)
	{
		GetWatchInfoByTypeRes* poRes = (GetWatchInfoByTypeRes*)roUserData.m_pUserPtr;
		for(size_t i = 0; i <  poRes->liverecords_size(); ++i)
		{
			KKSG::OneLiveRecordInfo* info = roRes.add_liverecords();
			info->CopyFrom(poRes->liverecords(i));
		}
		roRes.set_curtime(time(0));
	}
}
