#ifndef __RPCC2G_JUSTDANCE_H__
#define __RPCC2G_JUSTDANCE_H__

// generate by ProtoGen at date: 2016/11/11 20:25:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_JUSTDANCE_TYPE 43613

class RpcC2G_JustDance : public CRpc
{
private:

	RpcC2G_JustDance():CRpc(RPCC2G_JUSTDANCE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_JustDance *CreateRpc()
	{
		return new RpcC2G_JustDance;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const JustDanceArg &roArg, const JustDanceRes &roRes, const CUserData &roUserData);
	void OnTimeout(const JustDanceArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const JustDanceArg &roArg, JustDanceRes &roRes);
	void OnDelayReplyRpc(const JustDanceArg &roArg, JustDanceRes &roRes, const CUserData &roUserData);

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
	JustDanceArg m_oArg;
	JustDanceRes m_oRes;
};

#endif
