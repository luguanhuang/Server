#ifndef __RPCG2M_ENDAREANMS_H__
#define __RPCG2M_ENDAREANMS_H__

// generate by ProtoGen at date: 2016/9/12 20:26:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_ENDAREANMS_TYPE 23525

class RpcG2M_EndAreanMs : public CRpc
{
private:

	RpcG2M_EndAreanMs():CRpc(RPCG2M_ENDAREANMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_EndAreanMs *CreateRpc()
	{
		return new RpcG2M_EndAreanMs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const EndAreanMsArg &roArg, const EndAreanMsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const EndAreanMsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const EndAreanMsArg &roArg, EndAreanMsRes &roRes);
	void OnDelayReplyRpc(const EndAreanMsArg &roArg, EndAreanMsRes &roRes, const CUserData &roUserData);

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
	EndAreanMsArg m_oArg;
	EndAreanMsRes m_oRes;
};

#endif
