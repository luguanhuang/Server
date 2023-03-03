#ifndef __RPCM2G_MSUSESUMMARYREQ_H__
#define __RPCM2G_MSUSESUMMARYREQ_H__

// generate by ProtoGen at date: 2016/8/22 21:20:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_MSUSESUMMARYREQ_TYPE 20767

class RpcM2G_MSUseSummaryReq : public CRpc
{
private:

	RpcM2G_MSUseSummaryReq():CRpc(RPCM2G_MSUSESUMMARYREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_MSUseSummaryReq *CreateRpc()
	{
		return new RpcM2G_MSUseSummaryReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MSRobotSumamryArg &roArg, const MSRobotSumamryRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MSRobotSumamryArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MSRobotSumamryArg &roArg, MSRobotSumamryRes &roRes);
	void OnDelayReplyRpc(const MSRobotSumamryArg &roArg, MSRobotSumamryRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	MSRobotSumamryArg m_oArg;
	MSRobotSumamryRes m_oRes;
};

#endif
