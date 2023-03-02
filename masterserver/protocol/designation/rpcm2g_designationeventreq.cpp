#include "pch.h"
#include "designation/rpcm2g_designationeventreq.h"
#include "event/eventmgr.h"

// generate by ProtoGen at date: 2016/9/21 15:58:52

RPC_CLIENT_IMPLEMETION(RpcM2G_DesignationEventReq, DesignationEventReq, DesignationEventRes)

void RpcM2G_DesignationEventReq::OnReply(const DesignationEventReq &roArg, const DesignationEventRes &roRes, const CUserData &roUserData)
{
	if (roRes.errorcode() == ERR_SUCCESS)
	{
		/*if (roArg.roleid(),roArg.event().size()==1)
		{
			const KKSG::DesignationEvent *pEvent	= &roArg.event(0);
			EventMgr::Instance()->DelEvent(roArg.roleid(),pEvent->neventid()); 
		}else
		{
			EventMgr::Instance()->Clear(roArg.roleid());
		}*/
		std::vector<UINT64> vecEvent;
		for (int i = 0; i < roArg.event_size(); ++i)
		{
			vecEvent.push_back(roArg.event(i).nuuid());
		}
		EventMgr::Instance()->DelEvent(roArg.roleid(), vecEvent); 
	}
	else
	{

	}
}

void RpcM2G_DesignationEventReq::OnTimeout(const DesignationEventReq &roArg, const CUserData &roUserData)
{
}
