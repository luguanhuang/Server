#ifndef __RPCM2G_DESIGNATIONEVENTREQ_H__
#define __RPCM2G_DESIGNATIONEVENTREQ_H__

// generate by ProtoGen at date: 2016/9/21 15:58:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_DESIGNATIONEVENTREQ_TYPE 63158

class RpcM2G_DesignationEventReq : public CRpc
{
private:

	RpcM2G_DesignationEventReq():CRpc(RPCM2G_DESIGNATIONEVENTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_DesignationEventReq *CreateRpc()
	{
		return new RpcM2G_DesignationEventReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DesignationEventReq &roArg, const DesignationEventRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DesignationEventReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DesignationEventReq &roArg, DesignationEventRes &roRes);
	void OnDelayReplyRpc(const DesignationEventReq &roArg, DesignationEventRes &roRes, const CUserData &roUserData);

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
	DesignationEventReq m_oArg;
	DesignationEventRes m_oRes;
};

#endif
