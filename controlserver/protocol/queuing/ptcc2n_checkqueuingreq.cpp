#include "pch.h"
#include "queuing/ptcc2n_checkqueuingreq.h"
#include "queuing/ptcn2c_checkqueuingntf.h"
#include "network/gatelink.h"
#include "queuing/queuingmgr.h"
#include "account/accountsessionmgr.h"

// generate by ProtoGen at date: 2016/11/16 16:37:20

void PtcC2N_CheckQueuingReq::Process(UINT32 dwConnID)
{
	PtcN2C_CheckQueuingNtf ntf;
	ntf.m_sessionID = m_sessionID;
	if (m_Data.iscancel())
	{
		return;
	}

	CAccountSession* info = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if (NULL == info)
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"not find in queuing, session:"<<m_sessionID<<END;
		GateLink::Instance()->SendToSession(ntf);
		return;
	}
	const std::string& account = info->GetAccountID();
	//if (QueuingMgr::Instance()->IsReady(account))
	//{
	//	ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);
	//	GateLink::Instance()->SendToSession(ntf);
	//	return;
	//}
	const WaitInfo* waitinfo = QueuingMgr::Instance()->GetWaitInfo(account);
	if (NULL != waitinfo)
	{
		ntf.m_Data.set_errorcode(KKSG::ERR_ACCOUNT_QUEUING);

		WaitRes res;
		QueuingMgr::Instance()->GetWaitRes(waitinfo, res);
		ntf.m_Data.set_rolecount(res.rolecount);
		ntf.m_Data.set_timeleft(res.timeleft);
		GateLink::Instance()->SendToSession(ntf);
	}
	else
	{
		SSInfo<<"account:"<<account<<" check queuing succeed"<<END;
		ntf.m_Data.set_errorcode(KKSG::ERR_SUCCESS);
		GateLink::Instance()->SendToSession(ntf);
	}
}
