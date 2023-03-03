#ifndef __RPCC2G_MOBASIGNALING_H__
#define __RPCC2G_MOBASIGNALING_H__

// generate by ProtoGen at date: 2017/6/13 19:39:42

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_MOBASIGNALING_TYPE 52475

class RpcC2G_MobaSignaling : public CRpc
{
private:

	RpcC2G_MobaSignaling():CRpc(RPCC2G_MOBASIGNALING_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_MobaSignaling *CreateRpc()
	{
		return new RpcC2G_MobaSignaling;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MobaSignalingArg &roArg, const MobaSignalingRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MobaSignalingArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MobaSignalingArg &roArg, MobaSignalingRes &roRes);
	void OnDelayReplyRpc(const MobaSignalingArg &roArg, MobaSignalingRes &roRes, const CUserData &roUserData);

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
	MobaSignalingArg m_oArg;
	MobaSignalingRes m_oRes;
};

#endif
