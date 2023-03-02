#include "pch.h"
#include "designation/rpcg2m_gsdesignationeventreq.h"
#include "event/eventmgr.h"

// generate by ProtoGen at date: 2016/11/4 11:36:04

RPC_CLIENT_IMPLEMETION(RpcG2M_GSDesignationEventReq, DesignationEventReq, DesignationEventRes)

void RpcG2M_GSDesignationEventReq::OnReply(const DesignationEventReq &roArg, const DesignationEventRes &roRes, const CUserData &roUserData)
{
	if (roRes.errorcode() == ERR_SUCCESS)
	{
		if (roArg.roleid(),roArg.event().size()==1)
		{
			const KKSG::DesignationEvent *pEvent	= &roArg.event(0);
			EventMgr::Instance()->DelEvent(roArg.roleid(),pEvent->nuuid()); 
		}else
		{
			EventMgr::Instance()->Clear(roArg.roleid());
		}
	}

}

void RpcG2M_GSDesignationEventReq::OnTimeout(const DesignationEventReq &roArg, const CUserData &roUserData)
{
}
