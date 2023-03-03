#ifndef __RPCG2M_GSDESIGNATIONEVENTREQ_H__
#define __RPCG2M_GSDESIGNATIONEVENTREQ_H__

// generate by ProtoGen at date: 2016/11/4 11:36:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GSDESIGNATIONEVENTREQ_TYPE 13009

class RpcG2M_GSDesignationEventReq : public CRpc
{
private:

	RpcG2M_GSDesignationEventReq():CRpc(RPCG2M_GSDESIGNATIONEVENTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GSDesignationEventReq *CreateRpc()
	{
		return new RpcG2M_GSDesignationEventReq;
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
