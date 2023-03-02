#include "pch.h"
#include "designation/rpcg2m_gsdesignationeventreq.h"
#include "event/eventmgr.h"

// generate by ProtoGen at date: 2016/11/4 11:36:04

RPC_SERVER_IMPLEMETION(RpcG2M_GSDesignationEventReq, DesignationEventReq, DesignationEventRes)

void RpcG2M_GSDesignationEventReq::OnCall(const DesignationEventReq &roArg, DesignationEventRes &roRes)
{
	for (UINT32 i =  0; i < roArg.event_size(); i++)
	{
		const KKSG::DesignationEvent *pEvent = &roArg.event(i);
		EventMgr::Instance()->AddEventFromGS(roArg.roleid(), pEvent->neventid(), pEvent->nparam1(), pEvent->nparam2());
	}	
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcG2M_GSDesignationEventReq::OnDelayReplyRpc(const DesignationEventReq &roArg, DesignationEventRes &roRes, const CUserData &roUserData)
{
}
